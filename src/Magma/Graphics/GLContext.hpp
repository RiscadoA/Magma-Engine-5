#pragma once

#include "Context.hpp"

#include <map>

namespace Magma
{
	namespace Graphics
	{
		/// <summary>
		///		OpenGL implementation of Context using GLEW
		/// </summary>
		class GLContext : public Context
		{
		public:
			GLContext();
			virtual ~GLContext();

		private:
			std::map<int, unsigned int> m_data;
			int m_nextID;

			// Inherited via Context
			virtual int CreateShader(ShaderType type, const char * src) override;
			virtual int CreateProgram() override;
			virtual void LinkProgram(int program) override;
			virtual void AttachShader(int program, int shader) override;
			virtual void DestroyShader(int shader) override;
			virtual void DestroyProgram(int program) override;
			virtual void DetachShader(int program, int shader) override;
			virtual int CreateStaticVertexBuffer(int vao, void * data, size_t size) override;
			virtual int CreateDynamicVertexBuffer(int vao, void* data, size_t size) override;
			virtual void SetDynamicVertexBufferData(int vao, int vbo, void* data, size_t size) override;
			virtual int CreateVertexArray() override;
			virtual void SetVertexAttributePointer(int vao, int vbo, int index, int size, AttributeType type, bool normalized, size_t stride, const void * offset) override;
			virtual void DestroyVertexBuffer(int vbo) override;
			virtual void DestroyVertexArray(int vao) override;
			virtual void DrawVertexArray(int vao, DrawMode mode, int first, size_t count) override;
			virtual void ActivateProgram(int program) override;
			virtual void DeactivateProgram(int program) override;
			virtual void SetUniform1i(int index, int value) override;
			virtual void SetUniform1f(int index, float value) override;
			virtual void SetUniform2i(int index, const glm::ivec2 & vec) override;
			virtual void SetUniform2f(int index, const glm::vec2 & vec) override;
			virtual void SetUniform3i(int index, const glm::ivec3 & vec) override;
			virtual void SetUniform3f(int index, const glm::vec3 & vec) override;
			virtual void SetUniform4i(int index, const glm::ivec4 & vec) override;
			virtual void SetUniform4f(int index, const glm::vec4 & vec) override;
			virtual void SetUniform3x3f(int index, const glm::mat3 & mat) override;
			virtual void SetUniform4x4f(int index, const glm::mat4 & mat) override;
			virtual void SetUniform1iv(int index, size_t count, int * value) override;
			virtual void SetUniform1fv(int index, size_t count, float * value) override;
			virtual void SetUniform2iv(int index, size_t count, const glm::ivec2 * vec) override;
			virtual void SetUniform2fv(int index, size_t count, const glm::vec2 * vec) override;
			virtual void SetUniform3iv(int index, size_t count, const glm::ivec3 * vec) override;
			virtual void SetUniform3fv(int index, size_t count, const glm::vec3 * vec) override;
			virtual void SetUniform4iv(int index, size_t count, const glm::ivec4 * vec) override;
			virtual void SetUniform4fv(int index, size_t count, const glm::vec4 * vec) override;
			virtual void SetUniform3x3fv(int index, size_t count, const glm::mat3 * mat) override;
			virtual void SetUniform4x4fv(int index, size_t count, const glm::mat4 * mat) override;
			virtual void SetViewport(float x, float y, float width, float height) override;
			virtual int CreateFramebuffer() override;
			virtual void BindFramebuffer(FramebufferTarget target, int framebuffer) override;
			virtual void FramebufferTexture2D(FramebufferTarget target, FramebufferAttachment attachment, int texture, int level) override;
			virtual void SetDrawBuffers(size_t count, FramebufferAttachment * attachments) override;
			virtual void BlitFramebuffer(int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, BufferBit mask, Filter filter) override;
			virtual int CreateTexture2D() override;
			virtual void DestroyTexture2D(int texture) override;
			virtual void ActivateTexture2D(int texture, int slot) override;
			virtual void DeactivateTexture2D(int texture, int slot) override;
			virtual void TextureData2D(int level, PixelFormat internalFormat, size_t width, size_t height, PixelFormat format, PixelType type, void * data) override;
			virtual void SetTextureMinFilter(Filter filter) override;
			virtual void SetTextureMagFilter(Filter filter) override;
			virtual void SetTextureWrapSMode(WrapMode mode) override;
			virtual void SetTextureWrapTMode(WrapMode mode) override;
			virtual void Clear(BufferBit mask) override;
			virtual void SetUnpackAlignment(int alignment) override;

			int m_activeProgram;	
		};
	}
}