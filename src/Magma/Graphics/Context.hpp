#pragma once

#include <glm/glm.hpp>

namespace Magma
{
	namespace Graphics
	{
		/// <summary>
		///		Shader types
		/// </summary>
		enum class ShaderType
		{
			Invalid = -1,

			Vertex,
			Fragment,

			Count
		};

		/// <summary>
		///		Attribute types
		/// </summary>
		enum class AttributeType
		{
			Invalid = -1,

			Int,
			Float,

			Count
		};

		/// <summary>
		///		Draw modes
		/// </summary>
		enum class DrawMode
		{
			Invalid = -1,

			Points,
			Lines,
			LineStrip,
			LineLoop,
			Triangles,
			TriangleStrip,
			TriangleFan,

			Count
		};

		/// <summary>
		///		Pixel data format
		/// </summary>
		enum class PixelFormat
		{
			Invalid = -1,

			R,
			RG,

			RGB,
			BGR,
			RGBA,
			BGRA,

			DepthComponent,

			Count
		};

		/// <summary>
		///		Pixel data type
		/// </summary>
		enum class PixelType
		{
			Invalid = -1,

			UByte,
			Byte,
			UShort,
			Short,
			UInt,
			Int,
			Float,

			Count,
		};

		/// <summary>
		///		Texture filtering types
		/// </summary>
		enum class Filter
		{
			Invalid = -1,

			Nearest,
			Linear,
			NearestMipmapNearest,
			LinearMipmapNearest,
			NearestMipmapLinear,
			LinearMipmapLinear,

			Count
		};

		/// <summary>
		///		Texture wrap modes
		/// </summary>
		enum class WrapMode
		{
			Invalid = -1,

			ClampToEdge,
			MirroredRepeat,
			Repeat,

			Count
		};

		/// <summary>
		///		Buffer bits
		/// </summary>
		enum class BufferBit
		{
			None    = 0x00,
			Color   = 0x01,
			Depth   = 0x02,
			Stencil = 0x04,
		};
		inline BufferBit operator|(BufferBit l, BufferBit r) { return (BufferBit)((int)l | (int)r); }
		inline BufferBit operator&(BufferBit l, BufferBit r) { return (BufferBit)((int)l & (int)r); }

		/// <summary>
		///		Framebuffer attachments
		/// </summary>
		enum class FramebufferAttachment
		{
			Invalid = -1,

			Color0,
			Color1,
			Color2,
			Color3,
			Color4,
			Color5,
			Color6,
			Color7,

			Depth,
			Stencil,
			DepthStencil,
			
			Count
		};

		/// <summary>
		///		Framebuffer targets
		/// </summary>
		enum class FramebufferTarget
		{
			None = 0x00,
			Draw = 0x01,
			Read = 0x02,
			Both = Draw | Read,
		};
		inline FramebufferTarget operator|(FramebufferTarget l, FramebufferTarget r) { return (FramebufferTarget)((int)l | (int)r); }
		inline FramebufferTarget operator&(FramebufferTarget l, FramebufferTarget r) { return (FramebufferTarget)((int)l & (int)r); }

		/// <summary>
		///		Rendering context (provides a layer of abstraction between the low level rendering calls and the Renderer)
		/// </summary>
		class Context
		{
		public:
			Context() = default;
			virtual ~Context() = default;

			/// <summary>
			///		Creates and compiles a shader of a certain type
			/// </summary>
			/// <param name="type">Shader type</param>
			/// <param name="src">Shader source</param>
			/// <returns>Shader ID</returns>
			virtual int CreateShader(ShaderType type, const char* src) = 0;

			/// <summary>
			///		Creates an empty shader program
			/// </summary>
			/// <returns>Shader program ID</returns>
			virtual int CreateProgram() = 0;

			/// <summary>
			///		Links a shader program (do this after attaching all shaders)
			/// </summary>
			/// <param name="program">Shader program to be linked</param>
			virtual void LinkProgram(int program) = 0;

			/// <summary>
			///		Attaches a shader to a shader program
			/// </summary>
			/// <param name="program">Shader program ID</param>
			/// <param name="shader">Shader ID</param>
			virtual void AttachShader(int program, int shader) = 0;

			/// <summary>
			///		Destroys a shader
			/// </summary>
			/// <param name="shader">Shader ID</param>
			virtual void DestroyShader(int shader) = 0;

			/// <summary>
			///		Destroys a shader program
			/// </summary>
			/// <param name="program">Shader program ID</param>
			virtual void DestroyProgram(int program) = 0;

			/// <summary>
			///		Detaches a shader from a shader program
			/// </summary>
			/// <param name="program">Shader program ID</param>
			/// <param name="shader">Shader ID</param>
			virtual void DetachShader(int program, int shader) = 0;

			/// <summary>
			///		Creates a static vertex buffer
			/// </summary>
			/// <param name="vao">Vertex array object ID where the buffer will be attached</param>
			/// <param name="data">Vertex buffer data</param>
			/// <param name="size">Vertex buffer data size</param>
			/// <returns>Vertex buffer object ID</returns>
			virtual int CreateStaticVertexBuffer(int vao, void* data, size_t size) = 0;

			/// <summary>
			///		Creates a dynamic vertex buffer
			/// </summary>
			/// <param name="vao">Vertex array object ID where the buffer will be attached</param>
			/// <param name="data">Vertex buffer data</param>
			/// <param name="size">Vertex buffer data size</param>
			/// <returns>Vertex buffer object ID</returns>
			virtual int CreateDynamicVertexBuffer(int vao, void* data, size_t size) = 0;

			/// <summary>
			///		Sets a dynamic vertex buffer data
			/// </summary>
			/// <param name="vao">Vertex array object ID where the buffer is attached</param>
			/// <param name="vbo">Vertex buffer object</param>
			/// <param name="data">New vertex buffer data</param>
			/// <param name="size">New vertex buffer data size</param>
			virtual void SetDynamicVertexBufferData(int vao, int vbo, void* data, size_t size) = 0;

			/// <summary>
			///		Creates an empty vertex array
			/// </summary>
			/// <returns>Vertex array object ID</returns>
			virtual int CreateVertexArray() = 0;

			/// <summary>
			///		Sets a vertex buffer attribute pointer in a vertex array
			/// </summary>
			/// <param name="vao">Vertex array object</param>
			/// <param name="vbo">Vertex buffer object</param>
			/// <param name="index">Vertex attribute index</param>
			/// <param name="size">Vertex attribute size</param>
			/// <param name="type">Vertex attribute type</param>
			/// <param name="normalized">Is the data in the buffer normalized?</param>
			/// <param name="stride">Vertex attribute data stride</param>
			/// <param name="offset">Vertex attribute data offset</param>
			virtual void SetVertexAttributePointer(int vao, int vbo, int index, int size, AttributeType type, bool normalized, size_t stride, const void* offset) = 0;

			/// <summary>
			///		Destroys a vertex buffer object
			/// </summary>
			/// <param name="vbo">Vertex buffer object ID</param>
			virtual void DestroyVertexBuffer(int vbo) = 0;

			/// <summary>
			///		Destroys a vertex array
			/// </summary>
			/// <param name="vao">Vertex array object ID</param>
			virtual void DestroyVertexArray(int vao) = 0;

			/// <summary>
			///		Draws a vertex array
			/// </summary>
			/// <param name="vao">Vertex array object ID</param>
			/// <param name="mode">Draw mode</param>
			/// <param name="first">First vertex index</param>
			/// <param name="count">Vertex count</param>
			virtual void DrawVertexArray(int vao, DrawMode mode, int first, size_t count) = 0;

			/// <summary>
			///		Activates a shader program
			/// </summary>
			/// <param name="program">Shader program ID</param>
			virtual void ActivateProgram(int program) = 0;

			/// <summary>
			///		Deactivates a shader program
			/// </summary>
			/// <param name="program">Shader program ID</param>
			virtual void DeactivateProgram(int program) = 0;

			/// <summary>
			///		Sets an uniform integer in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="value">Uniform value</param>
			virtual void SetUniform1i(int index, int value) = 0;

			/// <summary>
			///		Sets an uniform float in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="value">Uniform value</param>
			virtual void SetUniform1f(int index, float value) = 0;

			/// <summary>
			///		Sets an uniform 2D integer vector in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="vec">Uniform value</param>
			virtual void SetUniform2i(int index, const glm::ivec2& vec) = 0;

			/// <summary>
			///		Sets an uniform 2D floating point vector in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="vec">Uniform value</param>
			virtual void SetUniform2f(int index, const glm::vec2& vec) = 0;

			/// <summary>
			///		Sets an uniform 3D integer vector in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="vec">Uniform value</param>
			virtual void SetUniform3i(int index, const glm::ivec3& vec) = 0;

			/// <summary>
			///		Sets an uniform 3D floating point vector in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="vec">Uniform value</param>
			virtual void SetUniform3f(int index, const glm::vec3& vec) = 0;

			/// <summary>
			///		Sets an uniform 4D integer point vector in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="vec">Uniform value</param>
			virtual void SetUniform4i(int index, const glm::ivec4& vec) = 0;

			/// <summary>
			///		Sets an uniform 4D floating point vector in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="vec">Uniform value</param>
			virtual void SetUniform4f(int index, const glm::vec4& vec) = 0;

			/// <summary>
			///		Sets an uniform 3x3 matrix in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="mat">Uniform value</param>
			virtual void SetUniform3x3f(int index, const glm::mat3& mat) = 0;

			/// <summary>
			///		Sets an uniform 4x4 matrix in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="mat">Uniform value</param>
			virtual void SetUniform4x4f(int index, const glm::mat4& mat) = 0;

			/// <summary>
			///		Sets an uniform integer array in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="count">Uniform array element count</param>
			/// <param name="value">Uniform array pointer</param>
			virtual void SetUniform1iv(int index, size_t count, int* value) = 0;

			/// <summary>
			///		Sets an uniform floating point array in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="count">Uniform array element count</param>
			/// <param name="value">Uniform array pointer</param>
			virtual void SetUniform1fv(int index, size_t count, float* value) = 0;

			/// <summary>
			///		Sets an uniform 2D integer vector array in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="count">Uniform array element count</param>
			/// <param name="vec">Uniform array pointer</param>
			virtual void SetUniform2iv(int index, size_t count, const glm::ivec2* vec) = 0;

			/// <summary>
			///		Sets an uniform 2D floating point vector array in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="count">Uniform array element count</param>
			/// <param name="vec">Uniform array pointer</param>
			virtual void SetUniform2fv(int index, size_t count, const glm::vec2* vec) = 0;

			/// <summary>
			///		Sets an uniform 3D integer vector array in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="count">Uniform array element count</param>
			/// <param name="vec">Uniform array pointer</param>
			virtual void SetUniform3iv(int index, size_t count, const glm::ivec3* vec) = 0;

			/// <summary>
			///		Sets an uniform 3D floating point vector array in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="count">Uniform array element count</param>
			/// <param name="vec">Uniform array pointer</param>
			virtual void SetUniform3fv(int index, size_t count, const glm::vec3* vec) = 0;

			/// <summary>
			///		Sets an uniform 4D integer vector array in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="count">Uniform array element count</param>
			/// <param name="vec">Uniform array pointer</param>
			virtual void SetUniform4iv(int index, size_t count, const glm::ivec4* vec) = 0;

			/// <summary>
			///		Sets an uniform 4D floating point vector array in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="count">Uniform array element count</param>
			/// <param name="vec">Uniform array pointer</param>
			virtual void SetUniform4fv(int index, size_t count, const glm::vec4* vec) = 0;

			/// <summary>
			///		Sets an uniform 3x3 matrix array in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="count">Uniform array element count</param>
			/// <param name="mat">Uniform array pointer</param>
			virtual void SetUniform3x3fv(int index, size_t count, const glm::mat3* mat) = 0;

			/// <summary>
			///		Sets an uniform 4x4 matrix array in a shader program
			/// </summary>
			/// <param name="index">Uniform index</param>
			/// <param name="count">Uniform array element count</param>
			/// <param name="mat">Uniform array pointer</param>
			virtual void SetUniform4x4fv(int index, size_t count, const glm::mat4* mat) = 0;

			/// <summary>
			///		Sets the viewport of the context
			/// </summary>
			/// <param name="x">Viewport left X coordinate</param>
			/// <param name="y">Viewport lower Y coordinate</param>
			/// <param name="width">Viewport width</param>
			/// <param name="height">Viewport height</param>
			virtual void SetViewport(float x, float y, float width, float height) = 0;

			/// <summary>
			///		Creates an empty framebuffer
			/// </summary>
			/// <returns>Framebuffer ID</returns>
			virtual int CreateFramebuffer() = 0;

			/// <summary>
			///		Binds a framebuffer (sets it as active)
			/// </summary>
			/// <param name="target">Framebuffer target (read, draw or both)</param>
			/// <param name="framebuffer">Framebuffer ID</param>
			virtual void BindFramebuffer(FramebufferTarget target, int framebuffer) = 0;

			/// <summary>
			///		Attaches a 2D texture to a framebuffer
			/// </summary>
			/// <param name="target">Framebuffer target (read, draw or both)</param>
			/// <param name="attachment">Framebutter attachment point</param>
			/// <param name="texture">Texture ID to attach</param>
			/// <param name="level">Texture LOD level</param>
			virtual void FramebufferTexture2D(FramebufferTarget target, FramebufferAttachment attachment, int texture, int level) = 0;

			/// <summary>
			///		Sets the framebuffer attachments which will be drawn
			/// </summary>
			/// <param name="count">Attachment point count</param>
			/// <param name="attachments">Attachment points</param>
			virtual void SetDrawBuffers(size_t count, FramebufferAttachment* attachments) = 0;

			/// <summary>
			///		Draws a framebuffer (FramebufferTarget::Read) into another (FramebufferTarget::Draw)
			/// </summary>
			/// <param name="srcX0">Read framebuffer left X coordinate</param>
			/// <param name="srcY0">Read framebuffer lower Y coordinate</param>
			/// <param name="srcX1">Read framebuffer right X coordinate</param>
			/// <param name="srcY1">Read framebuffer upper Y coordinate</param>
			/// <param name="dstX0">Draw framebuffer left X coordinate</param>
			/// <param name="dstY0">Draw framebuffer lower Y coordinate</param>
			/// <param name="dstX1">Draw framebuffer right X coordinate</param>
			/// <param name="dstY1">Draw framebuffer upper Y coordinate</param>
			/// <param name="mask">Buffer bit mask</param>
			/// <param name="filter">Filter to use</param>
			virtual void BlitFramebuffer(int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, BufferBit mask, Filter filter) = 0;

			/// <summary>
			///		Creates an empty 2D texture
			/// </summary>
			/// <returns>Texture ID</returns>
			virtual int CreateTexture2D() = 0;

			/// <summary>
			///		Destroys a 2D texture
			/// </summary>
			/// <param name="texture">Texture ID</param>
			virtual void DestroyTexture2D(int texture) = 0;

			/// <summary>
			///		Activates a 2D texture on a certain slot
			/// </summary>
			/// <param name="texture">Texture ID</param>
			/// <param name="slot">Texture slot</param>
			virtual void ActivateTexture2D(int texture, int slot) = 0;

			/// <summary>
			///		Deactivates a 2D texture on a certain slot
			/// </summary>
			/// <param name="texture">Texture ID</param>
			/// <param name="slot">Texture slot</param>
			virtual void DeactivateTexture2D(int texture, int slot) = 0;

			/// <summary>
			///		Sets a 2D texture data
			/// </summary>
			/// <param name="level">Texture LOD level</param>
			/// <param name="internalFormat">Texture internal data pixel format (the format used by the context)</param>
			/// <param name="width">Texture width</param>
			/// <param name="height">Texture height</param>
			/// <param name="format">Texture data pixel format (the format used in the data sent to the function)</param>
			/// <param name="type">Pixel data type used</param>
			/// <param name="data">Data pointer</param>
			virtual void TextureData2D(int level, PixelFormat internalFormat, size_t width, size_t height, PixelFormat format, PixelType type, void* data) = 0;

			/// <summary>
			///		Sets the minifying filter used by the current active texture
			/// </summary>
			/// <param name="filter">Filter to be used</param>
			virtual void SetTextureMinFilter(Filter filter) = 0;

			/// <summary>
			///		Sets the magnifying filter used by the current active texture
			/// </summary>
			/// <param name="filter">Filter to be used</param>
			virtual void SetTextureMagFilter(Filter filter) = 0;

			/// <summary>
			///		Sets the wrap mode for texture coordinate S used by the current active texture
			/// </summary>
			/// <param name="mode">Wrap mode to be used</param>
			virtual void SetTextureWrapSMode(WrapMode mode) = 0;

			/// <summary>
			///		Sets the wrap mode for texture coordinate T used by the current active texture
			/// </summary>
			/// <param name="mode">Wrap mode to be used</param>
			virtual void SetTextureWrapTMode(WrapMode mode) = 0;

			/// <summary>
			///		Clears the viewport
			/// </summary>
			/// <param name="mask">Buffer bit mask</param>
			virtual void Clear(BufferBit mask) = 0;

			/// <summary>
			///		Sets the texture unpack alignment
			/// </summary>
			/// <param name="alignment">Unpack alignment</param>
			virtual void SetUnpackAlignment(int alignment) = 0;
		};
	}
}