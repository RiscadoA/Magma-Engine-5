#include "GLContext.hpp"

#include <GL/glew.h>
#include <sstream>

Magma::Graphics::GLContext::GLContext()
{
	// Init GLEW
	{
		auto err = glewInit();
		if (err != GLEW_OK)
		{
			std::stringstream ss;
			ss << "Failed to init GLContext, couldn't init GLEW:" << std::endl << glewGetErrorString(err);
			throw std::runtime_error(ss.str());
		}
	}

	m_nextID = 1;
	m_activeProgram = 0;
}

Magma::Graphics::GLContext::~GLContext()
{
	
}

int Magma::Graphics::GLContext::CreateShader(ShaderType type, const char * src)
{
	int shader;

	switch (type)
	{
		case ShaderType::Vertex:
			shader = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderType::Fragment:
			shader = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			throw std::runtime_error("Failed to create shader on GLContext: invalid shader type");
			break;
	}

	glShaderSource(shader, 1, (const GLchar**)&src, 0);
	glCompileShader(shader);

	int isCompiled = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		
		auto infoLog = new char[length];
		glGetShaderInfoLog(shader, length, &length, infoLog);

		std::stringstream ss;
		ss << "Failed to create shader (" << m_nextID << ") on GLContext, compilation failed:" << std::endl << infoLog;
		delete[] infoLog;
		throw std::runtime_error(ss.str());
	}

	m_data[m_nextID] = shader;
	return m_nextID++;
}

int Magma::Graphics::GLContext::CreateProgram()
{
	int program = glCreateProgram();
	m_data[m_nextID] = program;
	return m_nextID++;
}

void Magma::Graphics::GLContext::LinkProgram(int program)
{
	int p = m_data.at(program);
	glLinkProgram(p);

	int isLinked = GL_FALSE;
	glGetProgramiv(p, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		int length;
		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &length);

		auto infoLog = new char[length];
		glGetProgramInfoLog(p, length, &length, infoLog);

		std::stringstream ss;
		ss << "Failed to link program (" << m_nextID << ") on GLContext:" << std::endl << infoLog;
		delete[] infoLog;
		throw std::runtime_error(ss.str());
	}
}

void Magma::Graphics::GLContext::AttachShader(int program, int shader)
{
	glAttachShader(m_data.at(program), m_data.at(shader));
}

void Magma::Graphics::GLContext::DestroyShader(int shader)
{
	glDeleteShader(m_data.at(shader));
	m_data.erase(shader);
}

void Magma::Graphics::GLContext::DestroyProgram(int program)
{
	glDeleteProgram(m_data.at(program));
	m_data.erase(program);
}

void Magma::Graphics::GLContext::DetachShader(int program, int shader)
{
	glDetachShader(m_data.at(program), m_data.at(shader));
}

int Magma::Graphics::GLContext::CreateStaticVertexBuffer(int vao, void * data, size_t size)
{
	GLuint vbo;
	glBindVertexArray(m_data.at(vao));
	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	m_data[m_nextID] = vbo;
	return m_nextID++;
}

int Magma::Graphics::GLContext::CreateVertexArray()
{
	GLuint vao;
	glCreateVertexArrays(1, &vao);
	m_data[m_nextID] = vao;
	return m_nextID++;
}

void Magma::Graphics::GLContext::SetVertexAttributePointer(int vao, int vbo, int index, int size, AttributeType type, bool normalized, size_t stride, const void * offset)
{
	glBindVertexArray(m_data.at(vao));
	glBindBuffer(GL_ARRAY_BUFFER, m_data.at(vbo));
	switch (type)
	{
		case AttributeType::Int:
			glVertexAttribIPointer(index, size, GL_INT, stride, offset);
			break;
		case AttributeType::Float:
			glVertexAttribPointer(index, size, GL_FLOAT, normalized, stride, offset);
			break;
		default:
			throw std::runtime_error("Failed to set vertex attribute pointer on GLContext, invalid attribute type");
			break;
	}
	glEnableVertexAttribArray(index);
}

void Magma::Graphics::GLContext::DestroyVertexBuffer(int vbo)
{
	glDeleteBuffers(1, &m_data.at(vbo));
	m_data.erase(vbo);
}

void Magma::Graphics::GLContext::DestroyVertexArray(int vao)
{
	glDeleteVertexArrays(1, &m_data.at(vao));
	m_data.erase(vao);
}

void Magma::Graphics::GLContext::DrawVertexArray(int vao, DrawMode mode, int first, size_t count)
{
	glBindVertexArray(m_data.at(vao));
	switch (mode)
	{
		case DrawMode::Points: glDrawArrays(GL_POINTS, first, count); break;
		case DrawMode::Lines: glDrawArrays(GL_LINES, first, count); break;
		case DrawMode::LineStrip: glDrawArrays(GL_LINE_STRIP, first, count); break;
		case DrawMode::LineLoop: glDrawArrays(GL_LINE_LOOP, first, count); break;
		case DrawMode::Triangles: glDrawArrays(GL_TRIANGLES, first, count); break;
		case DrawMode::TriangleStrip: glDrawArrays(GL_TRIANGLE_STRIP, first, count); break;
		case DrawMode::TriangleFan: glDrawArrays(GL_TRIANGLE_FAN, first, count); break;
		default: throw std::runtime_error("Failed to draw vertex array: invalid mode"); break;
	}
	glBindVertexArray(0);
}

void Magma::Graphics::GLContext::ActivateProgram(int program)
{
	m_activeProgram = program;
	glUseProgram(m_data.at(program));
}

void Magma::Graphics::GLContext::DeactivateProgram(int program)
{
	if (m_activeProgram != program)
		throw std::runtime_error("Failed to deactivate program, program is not active");
	glUseProgram(0);
}

void Magma::Graphics::GLContext::SetUniform1i(int index, int value)
{
	glUniform1i(index, value);
}

void Magma::Graphics::GLContext::SetUniform1f(int index, float value)
{
	glUniform1f(index, value);
}

void Magma::Graphics::GLContext::SetUniform2i(int index, const glm::ivec2 & vec)
{
	glUniform2i(index, vec.x, vec.y);
}

void Magma::Graphics::GLContext::SetUniform2f(int index, const glm::vec2 & vec)
{
	glUniform2f(index, vec.x, vec.y);
}

void Magma::Graphics::GLContext::SetUniform3i(int index, const glm::ivec3 & vec)
{
	glUniform3i(index, vec.x, vec.y, vec.z);
}

void Magma::Graphics::GLContext::SetUniform3f(int index, const glm::vec3 & vec)
{
	glUniform3f(index, vec.x, vec.y, vec.z);
}

void Magma::Graphics::GLContext::SetUniform4i(int index, const glm::ivec4 & vec)
{
	glUniform4i(index, vec.x, vec.y, vec.z, vec.w);
}

void Magma::Graphics::GLContext::SetUniform4f(int index, const glm::vec4 & vec)
{
	glUniform4f(index, vec.x, vec.y, vec.z, vec.w);
}

void Magma::Graphics::GLContext::SetUniform3x3f(int index, const glm::mat3 & mat)
{
	glUniformMatrix3fv(index, 1, GL_FALSE, &mat[0][0]);
}

void Magma::Graphics::GLContext::SetUniform4x4f(int index, const glm::mat4 & mat)
{
	glUniformMatrix4fv(index, 1, GL_FALSE, &mat[0][0]);
}

void Magma::Graphics::GLContext::SetUniform1iv(int index, size_t count, int * value)
{
	glUniform1iv(index, count, value);
}

void Magma::Graphics::GLContext::SetUniform1fv(int index, size_t count, float * value)
{
	glUniform1fv(index, count, value);
}

void Magma::Graphics::GLContext::SetUniform2iv(int index, size_t count, const glm::ivec2 * vec)
{
	glUniform2iv(index, count, &vec[0][0]);
}

void Magma::Graphics::GLContext::SetUniform2fv(int index, size_t count, const glm::vec2 * vec)
{

	glUniform2fv(index, count, &vec[0][0]);
}

void Magma::Graphics::GLContext::SetUniform3iv(int index, size_t count, const glm::ivec3 * vec)
{
	glUniform3iv(index, count, &vec[0][0]);
}

void Magma::Graphics::GLContext::SetUniform3fv(int index, size_t count, const glm::vec3 * vec)
{
	glUniform3fv(index, count, &vec[0][0]);
}

void Magma::Graphics::GLContext::SetUniform4iv(int index, size_t count, const glm::ivec4 * vec)
{
	glUniform4iv(index, count, &vec[0][0]);
}

void Magma::Graphics::GLContext::SetUniform4fv(int index, size_t count, const glm::vec4 * vec)
{
	glUniform4fv(index, count, &vec[0][0]);
}

void Magma::Graphics::GLContext::SetUniform3x3fv(int index, size_t count, const glm::mat3 * mat)
{
	glUniformMatrix3fv(index, count, GL_FALSE, &mat[0][0][0]);
}

void Magma::Graphics::GLContext::SetUniform4x4fv(int index, size_t count, const glm::mat4 * mat)
{
	glUniformMatrix4fv(index, count, GL_FALSE, &mat[0][0][0]);
}

void Magma::Graphics::GLContext::SetViewport(float x, float y, float width, float height)
{
	glViewport(x, y, width, height);
}

int Magma::Graphics::GLContext::CreateFramebuffer()
{
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	m_data[m_nextID] = framebuffer;
	return m_nextID++;
}

void Magma::Graphics::GLContext::BindFramebuffer(FramebufferTarget target, int framebuffer)
{
	int fb = (framebuffer == 0) ? 0 : m_data.at(framebuffer);
	if (target == FramebufferTarget::Draw)
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb);
	else if (target == FramebufferTarget::Read)
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fb);
	else if (target == FramebufferTarget::Both)
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
	else
	{
		std::stringstream ss;
		ss << "Failed to activate framebuffer (" << framebuffer << "), invalid target";
		throw std::runtime_error(ss.str());
	}
}

void Magma::Graphics::GLContext::FramebufferTexture2D(FramebufferTarget target, FramebufferAttachment attachment, int texture, int level)
{
	GLenum glTarget;
	GLenum glAttachment;

	switch (target)
	{
		case FramebufferTarget::Draw: glTarget = GL_DRAW_FRAMEBUFFER; break;
		case FramebufferTarget::Read: glTarget = GL_READ_FRAMEBUFFER; break;
		case FramebufferTarget::Both: glTarget = GL_FRAMEBUFFER; break;
		default:
			throw std::runtime_error("Failed to add 2D texture to framebuffer: invalid framebuffer target");
			break;
	}

	switch (attachment)
	{
		case FramebufferAttachment::Color0: glAttachment = GL_COLOR_ATTACHMENT0; break;
		case FramebufferAttachment::Color1: glAttachment = GL_COLOR_ATTACHMENT1; break;
		case FramebufferAttachment::Color2: glAttachment = GL_COLOR_ATTACHMENT2; break;
		case FramebufferAttachment::Color3: glAttachment = GL_COLOR_ATTACHMENT3; break;
		case FramebufferAttachment::Color4: glAttachment = GL_COLOR_ATTACHMENT4; break;
		case FramebufferAttachment::Color5: glAttachment = GL_COLOR_ATTACHMENT5; break;
		case FramebufferAttachment::Color6: glAttachment = GL_COLOR_ATTACHMENT6; break;
		case FramebufferAttachment::Color7: glAttachment = GL_COLOR_ATTACHMENT7; break;
		case FramebufferAttachment::Depth: glAttachment = GL_DEPTH_ATTACHMENT; break;
		case FramebufferAttachment::Stencil: glAttachment = GL_STENCIL_ATTACHMENT; break;
		case FramebufferAttachment::DepthStencil: glAttachment = GL_DEPTH_STENCIL_ATTACHMENT; break;
		default:
			throw std::runtime_error("Failed to add 2D texture to framebuffer: invalid framebuffer attachment");
			break;
	}

	glFramebufferTexture2D(glTarget, glAttachment, GL_TEXTURE_2D, m_data.at(texture), level);
}

void Magma::Graphics::GLContext::SetDrawBuffers(size_t count, FramebufferAttachment * attachments)
{
	if (count > (size_t)FramebufferAttachment::Count)
		throw std::runtime_error("Failed to set framebuffer draw buffers: too many attachments");

	GLenum drawBuffers[(size_t)FramebufferAttachment::Count];

	for (size_t i = 0; i < count; ++i)
		switch (attachments[i])
		{
			case FramebufferAttachment::Color0: drawBuffers[i] = GL_COLOR_ATTACHMENT0; break;
			case FramebufferAttachment::Color1: drawBuffers[i] = GL_COLOR_ATTACHMENT1; break;
			case FramebufferAttachment::Color2: drawBuffers[i] = GL_COLOR_ATTACHMENT2; break;
			case FramebufferAttachment::Color3: drawBuffers[i] = GL_COLOR_ATTACHMENT3; break;
			case FramebufferAttachment::Color4: drawBuffers[i] = GL_COLOR_ATTACHMENT4; break;
			case FramebufferAttachment::Color5: drawBuffers[i] = GL_COLOR_ATTACHMENT5; break;
			case FramebufferAttachment::Color6: drawBuffers[i] = GL_COLOR_ATTACHMENT6; break;
			case FramebufferAttachment::Color7: drawBuffers[i] = GL_COLOR_ATTACHMENT7; break;
			case FramebufferAttachment::Depth: drawBuffers[i] = GL_DEPTH_ATTACHMENT; break;
			case FramebufferAttachment::Stencil: drawBuffers[i] = GL_STENCIL_ATTACHMENT; break;
			case FramebufferAttachment::DepthStencil: drawBuffers[i] = GL_DEPTH_STENCIL_ATTACHMENT; break;
			default:
				throw std::runtime_error("Failed to set framebuffer draw buffers: invalid framebuffer attachment");
				break;
		}

	glDrawBuffers(count, drawBuffers);
}

void Magma::Graphics::GLContext::BlitFramebuffer(int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, BufferBit mask, Filter filter)
{
	GLenum glMask = 0, glFilter = 0;

	if ((mask & BufferBit::Color) != BufferBit::None)
		glMask |= GL_COLOR_BUFFER_BIT;
	if ((mask & BufferBit::Depth) != BufferBit::None)
		glMask |= GL_DEPTH_BUFFER_BIT;
	if ((mask & BufferBit::Stencil) != BufferBit::None)
		glMask |= GL_STENCIL_BUFFER_BIT;

	switch (filter)
	{
		case Filter::Nearest: glFilter = GL_NEAREST; break;
		case Filter::Linear: glFilter = GL_LINEAR; break;
		case Filter::NearestMipmapNearest: glFilter = GL_NEAREST_MIPMAP_NEAREST; break;
		case Filter::NearestMipmapLinear: glFilter = GL_NEAREST_MIPMAP_LINEAR; break;
		case Filter::LinearMipmapNearest: glFilter = GL_LINEAR_MIPMAP_NEAREST; break;
		case Filter::LinearMipmapLinear: glFilter = GL_LINEAR_MIPMAP_LINEAR; break;
		default: throw std::runtime_error("Failed to blit framebuffer: invalid filter"); break;
	}

	glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, glMask, glFilter);
}

int Magma::Graphics::GLContext::CreateTexture2D()
{
	GLuint texture;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	m_data[m_nextID] = texture;
	return m_nextID++;
}

void Magma::Graphics::GLContext::ActivateTexture2D(int texture, int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_data.at(texture));
}

void Magma::Graphics::GLContext::DeactivateTexture2D(int texture, int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Magma::Graphics::GLContext::TextureData2D(int level, PixelFormat internalFormat, size_t width, size_t height, PixelFormat format, PixelType type, void * data)
{
	// TO DO
	GLenum glInternalFormat;
	GLenum glFormat;
	GLenum glType;

	switch (internalFormat)
	{
		case PixelFormat::R: glInternalFormat = GL_R; break;
		case PixelFormat::RG: glInternalFormat = GL_RG; break;
		case PixelFormat::RGB: glInternalFormat = GL_RGB; break;
		case PixelFormat::BGR: glInternalFormat = GL_BGR; break;
		case PixelFormat::RGBA: glInternalFormat = GL_RGBA; break;
		case PixelFormat::BGRA: glInternalFormat = GL_BGRA; break;
		case PixelFormat::DepthComponent: glInternalFormat = GL_DEPTH_COMPONENT; break;
		default: throw std::runtime_error("Failed to set texture data: invalid pixel data internal format"); break;
	}

	switch (format)
	{
		case PixelFormat::R: glFormat = GL_R; break;
		case PixelFormat::RG: glFormat = GL_RG; break;
		case PixelFormat::RGB: glFormat = GL_RGB; break;
		case PixelFormat::BGR: glFormat = GL_BGR; break;
		case PixelFormat::RGBA: glFormat = GL_RGBA; break;
		case PixelFormat::BGRA: glFormat = GL_BGRA; break;
		case PixelFormat::DepthComponent: glFormat = GL_DEPTH_COMPONENT; break;
		default: throw std::runtime_error("Failed to set texture data: invalid pixel data format"); break;
	}

	switch (type)
	{
		case PixelType::UByte: glType = GL_UNSIGNED_BYTE; break;
		case PixelType::UShort: glType = GL_UNSIGNED_SHORT; break;
		case PixelType::UInt: glType = GL_UNSIGNED_INT; break;
		case PixelType::Byte: glType = GL_BYTE; break;
		case PixelType::Short: glType = GL_SHORT; break;
		case PixelType::Int: glType = GL_INT; break;
		case PixelType::Float: glType = GL_FLOAT; break;
		default: throw std::runtime_error("Failed to set texture data: invalid pixel data type"); break;
	}

	glTexImage2D(GL_TEXTURE_2D, level, glInternalFormat, width, height, 0, glFormat, glType, data);
}

void Magma::Graphics::GLContext::SetTextureMinFilter(Filter filter)
{
	GLenum glFilter;
	switch (filter)
	{
		case Filter::Nearest: glFilter = GL_NEAREST; break;
		case Filter::Linear: glFilter = GL_LINEAR; break;
		case Filter::NearestMipmapNearest: glFilter = GL_NEAREST_MIPMAP_NEAREST; break;
		case Filter::NearestMipmapLinear: glFilter = GL_NEAREST_MIPMAP_LINEAR; break;
		case Filter::LinearMipmapNearest: glFilter = GL_LINEAR_MIPMAP_NEAREST; break;
		case Filter::LinearMipmapLinear: glFilter = GL_LINEAR_MIPMAP_LINEAR; break;
		default: throw std::runtime_error("Failed to set texture min filter: invalid filter"); break;
	}
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
}

void Magma::Graphics::GLContext::SetTextureMagFilter(Filter filter)
{
	GLenum glFilter;
	switch (filter)
	{
		case Filter::Nearest: glFilter = GL_NEAREST; break;
		case Filter::Linear: glFilter = GL_LINEAR; break;
		case Filter::NearestMipmapNearest: glFilter = GL_NEAREST_MIPMAP_NEAREST; break;
		case Filter::NearestMipmapLinear: glFilter = GL_NEAREST_MIPMAP_LINEAR; break;
		case Filter::LinearMipmapNearest: glFilter = GL_LINEAR_MIPMAP_NEAREST; break;
		case Filter::LinearMipmapLinear: glFilter = GL_LINEAR_MIPMAP_LINEAR; break;
		default: throw std::runtime_error("Failed to set texture mag filter: invalid filter"); break;
	}
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);
}

void Magma::Graphics::GLContext::Clear(BufferBit mask)
{
	GLenum glMask = 0, glFilter = 0;

	if ((mask & BufferBit::Color) != BufferBit::None)
		glMask |= GL_COLOR_BUFFER_BIT;
	if ((mask & BufferBit::Depth) != BufferBit::None)
		glMask |= GL_DEPTH_BUFFER_BIT;
	if ((mask & BufferBit::Stencil) != BufferBit::None)
		glMask |= GL_STENCIL_BUFFER_BIT;

	glClear(glMask);
}
