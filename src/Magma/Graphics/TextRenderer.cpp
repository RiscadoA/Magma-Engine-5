#include "TextRenderer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

Magma::Graphics::TextRenderer::TextRenderer(Context & context)
	: m_context(context)
{
	// Compile default text shader
	{
		m_vertShader = m_context.CreateShader(ShaderType::Vertex,
											  R"glsl(
	#version 430 core
		
	layout (location = 0) in vec4 vertex; // xy - Position; zw - UVs
	out vec2 fragUVs;

	layout (location = 0) uniform mat4 transform;

	void main()
	{
		gl_Position = transform * vec4(vertex.xy, 0.0, 1.0);
		fragUVs = vertex.zw;
	}
	)glsl");

		m_fragShader = m_context.CreateShader(ShaderType::Fragment,
											  R"glsl(
	#version 430 core
	
	in vec2 fragUVs;
	out vec4 fragColor;

	layout (location = 1) uniform sampler2D font;
	layout (location = 2) uniform vec3 textColor;

	void main()
	{
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(font, fragUVs).r);
		fragColor = vec4(textColor, 1.0) * sampled;
		if (fragColor.a == 0.0)
			discard;
	}
	)glsl");

		m_shaderProgram = m_context.CreateProgram();
		m_context.AttachShader(m_shaderProgram, m_vertShader);
		m_context.AttachShader(m_shaderProgram, m_fragShader);
		m_context.LinkProgram(m_shaderProgram);

		m_destroyShader = true;
	}

	// Create vertex array for texture quads
	m_vao = m_context.CreateVertexArray();
	m_vbo = m_context.CreateDynamicVertexBuffer(m_vao, NULL, sizeof(float) * 6 * 4);
	m_context.SetVertexAttributePointer(m_vao, m_vbo, 0, 4, AttributeType::Float, false, 0, 0);
}

Magma::Graphics::TextRenderer::~TextRenderer()
{
	// Destroy shaders
	if (m_destroyShader)
	{
		m_context.DetachShader(m_shaderProgram, m_vertShader);
		m_context.DetachShader(m_shaderProgram, m_fragShader);
		m_context.DestroyProgram(m_shaderProgram);
	}

	m_context.DestroyShader(m_vertShader);
	m_context.DestroyShader(m_fragShader);

	// Destroy vertex array
	m_context.DestroyVertexBuffer(m_vbo);
	m_context.DestroyVertexArray(m_vao);
}

void Magma::Graphics::TextRenderer::Load(const std::string & fontPath, unsigned int fontSize)
{
	for (auto& c : m_characters)
		m_context.DestroyTexture2D(c.second.texture);
	m_characters.clear();

	FT_Error error;
	FT_Library ft;
	if (error = FT_Init_FreeType(&ft))
		throw std::runtime_error("Failed to load font into TextRenderer: couldn't init FreeType library");

	FT_Face face;
	error = FT_New_Face(ft, fontPath.c_str(), 0, &face);
	if (error == FT_Err_Unknown_File_Format)
		throw std::runtime_error("Failed to load font into TextRenderer: FreeType couldn't load font, unknown file format");
	else if (error == FT_Err_Cannot_Open_Resource)
		throw std::runtime_error("Failed to load font into TextRenderer: FreeType couldn't load font, failed to open file");
	else if (error)
		throw std::runtime_error("Failed to load font into TextRenderer: FreeType couldn't load font, unknown error");
	
	FT_Set_Pixel_Sizes(face, 0, fontSize);

	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			fprintf(stderr, "FreeType error, failed to load Glyph");
			continue;
		}

		m_context.SetUnpackAlignment(1);
		int texture = m_context.CreateTexture2D();
		m_context.ActivateTexture2D(texture, 0);

		
		m_context.TextureData2D(
			0,
			PixelFormat::R,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			PixelFormat::R,
			PixelType::UByte,
			face->glyph->bitmap.buffer
		);
		m_context.SetTextureWrapSMode(WrapMode::ClampToEdge);
		m_context.SetTextureWrapTMode(WrapMode::ClampToEdge);
		m_context.SetTextureMinFilter(Filter::Linear);
		m_context.SetTextureMagFilter(Filter::Linear);
		m_context.DeactivateTexture2D(texture, 0);

		m_characters.insert(std::make_pair(c, Character {
										   texture,
										   glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
										   glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
										   face->glyph->advance.x }));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Magma::Graphics::TextRenderer::Render(const std::string & text, const glm::mat4 & transform, const glm::vec3 & color)
{
	m_context.ActivateProgram(m_shaderProgram);
	m_context.SetUniform4x4f(0, transform);
	m_context.SetUniform1i(1, 0); // Set texture slot to be used
	m_context.SetUniform3f(2, color); // Set text color

	float x = 0.0f;
	float y = 0.0f;

	for (auto& c : text)
	{
		Character ch;
		if (m_characters.find(c) == m_characters.end())
			ch = m_characters['?'];
		else
			ch = m_characters[c];

		float w = ch.size.x;
		float h = ch.size.y;

		float xpos = x + ch.bearing.x;
		float ypos = y - (h - ch.bearing.y);

		float vertices[6][4] =
		{
			{ xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 } 
		};

		m_context.ActivateTexture2D(ch.texture, 0);
		m_context.SetDynamicVertexBufferData(m_vao, m_vbo, vertices, sizeof(vertices));
		m_context.DrawVertexArray(m_vao, DrawMode::Triangles, 0, 6);
		m_context.DeactivateTexture2D(ch.texture, 0);

		x += (ch.advance >> 6);
	}

	m_context.DeactivateProgram(m_shaderProgram);
}
