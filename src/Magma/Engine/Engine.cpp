#include "Engine.hpp"

#include "../Input/Window.hpp"
#include "../Graphics/GLContext.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace Magma;

int main(int argc, char** argv)
{
	Engine engine;

	auto window = Input::Window(1400, 800, "Window");
	auto running = true;

	window.OnClose.AddListener([&]() { running = false; });

	Graphics::Context* context = new Graphics::GLContext();

	auto program = context->CreateProgram();

	{
		auto shader = context->CreateShader(Graphics::ShaderType::Vertex, R"glsl(
		#version 430 core	

		layout (location = 0) in vec3 vertexPosition;

		layout (location = 0) uniform mat4 mvp;

		out vec4 gl_Position;

		void main()
		{
			gl_Position = mvp * vec4(vertexPosition, 1.0);
		}
		)glsl");

		context->AttachShader(program, shader);
	}

	{
		auto shader = context->CreateShader(Graphics::ShaderType::Fragment, R"glsl(
		#version 430 core	

		out vec4 gl_FragColor;

		void main()
		{
			gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
		)glsl");

		context->AttachShader(program, shader);
	}

	context->LinkProgram(program);

	auto vao = context->CreateVertexArray();

	{
		float data[] =
		{
			0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
		};

		auto vbo = context->CreateStaticVertexBuffer(vao, data, sizeof(data));
		context->SetVertexAttributePointer(vao, vbo, 0, 3, Graphics::AttributeType::Float, false, 0, nullptr);
	}

	// Create framebuffer
	auto framebuffer = context->CreateFramebuffer();
	context->BindFramebuffer(Graphics::FramebufferTarget::Both, framebuffer);

	{
		// Create color texture
		auto color = context->CreateTexture2D();
		context->ActivateTexture2D(color, 0);
		context->TextureData2D(0, Graphics::PixelFormat::RGB, 1400, 800, Graphics::PixelFormat::RGB, Graphics::PixelType::UByte, nullptr);
		context->SetTextureMinFilter(Graphics::Filter::Linear);
		context->SetTextureMagFilter(Graphics::Filter::Linear);

		Graphics::FramebufferAttachment attachments[] = { Graphics::FramebufferAttachment::Color0 };
		context->SetDrawBuffers(1, attachments);
		context->FramebufferTexture2D(Graphics::FramebufferTarget::Both, Graphics::FramebufferAttachment::Color0, color, 0);
	}

	context->BindFramebuffer(Graphics::FramebufferTarget::Both, 0);

	while (running)
	{
		window.PollEvents();

		// Set frame buffer
		context->BindFramebuffer(Graphics::FramebufferTarget::Both, framebuffer);
		context->SetViewport(0.0f, 0.0f, 1400.0f, 800.0f);
		context->Clear(Graphics::BufferBit::Color);

		// Get projection matrix
		glm::mat4 ortho = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f);

		// Render triangle
		context->ActivateProgram(program);
		context->SetUniform4x4f(0, ortho);
		context->DrawVertexArray(vao, Graphics::DrawMode::Triangles, 0, 3);
		context->DeactivateProgram(program);

		context->BindFramebuffer(Graphics::FramebufferTarget::Read, framebuffer);
		context->BindFramebuffer(Graphics::FramebufferTarget::Draw, 0);
		context->BlitFramebuffer(0, 0, 1400, 800, 0, 0, 200, 800, Graphics::BufferBit::Color, Graphics::Filter::Linear);

		window.SwapBuffers();
	}

	Init(engine);

	delete context;
}

Magma::Engine::Engine()
{

}

Magma::Engine::~Engine()
{

}
