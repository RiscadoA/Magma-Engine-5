#include "Engine.hpp"

#include "../Input/Window.hpp"
#include "../Graphics/GLContext.hpp"
#include "../Graphics/TerminalRenderer.hpp"
#include "../Graphics/TextRenderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <fstream>

using namespace Magma;

int main(int argc, char** argv)
{
	Engine engine;

	auto window = Input::Window(1400, 800, "Window");
	auto running = true;

	window.OnClose.AddListener([&]() { running = false; });

	Graphics::Context* context = new Graphics::GLContext();
	//Graphics::TerminalRenderer* renderer = new Graphics::TerminalRenderer(*context);

	/*window.OnKeyDown.AddListener([&](auto key, auto mods)
	{
		renderer->RenderOutput("test");
	});

	renderer->RenderInput("test");*/

	Graphics::TextRenderer* consolasTextRenderer = new Graphics::TextRenderer(*context);
	Graphics::TextRenderer* otherTextRenderer = new Graphics::TextRenderer(*context);
	try
	{
		consolasTextRenderer->Load("../../../../resources/Consolas.ttf", 80);
		otherTextRenderer->Load("../../../../resources/Pacific Again.ttf", 60);
	}
	catch (std::runtime_error& err)
	{
		fprintf(stderr, "Caught runtime error exception while trying to load font:\n%s", err.what());
		getchar();
		return -1;
	}

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
			800.0f, 0.0f, 0.0f,
			800.0f, 800.0f, 0.0f,
		};

		auto vbo = context->CreateStaticVertexBuffer(vao, data, sizeof(data));
		context->SetVertexAttributePointer(vao, vbo, 0, 3, Graphics::AttributeType::Float, false, 0, nullptr);
	}

	while (running)
	{
		window.PollEvents();

		context->Clear(Graphics::BufferBit::Color);

		glm::mat4 proj = glm::ortho(0.0f, 1400.0f, 0.0f, 800.0f);
		

		context->ActivateProgram(program);
		context->SetUniform4x4f(0, proj);
		context->DrawVertexArray(vao, Graphics::DrawMode::Triangles, 0, 3);
		context->DeactivateProgram(program);

		glm::mat4 transform = glm::mat4(1.0f);
		transform *= proj;

		consolasTextRenderer->Render("/test -f test.txt", glm::translate(proj, glm::vec3(0.0f, 400.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));

		otherTextRenderer->Render("Sample Text", glm::translate(proj, glm::vec3(900.0f, 200.0f, 0.0f)), glm::vec3(0.0f, 1.0f, 1.0f));

		window.SwapBuffers();
	}

	Init(engine);

	delete consolasTextRenderer;
	delete otherTextRenderer;
	//delete renderer;
	delete context;
}

Magma::Engine::Engine()
{

}

Magma::Engine::~Engine()
{

}
