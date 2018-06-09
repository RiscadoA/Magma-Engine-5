#pragma once

#include "Context.hpp"

#include <glm/glm.hpp>
#include <string>
#include <map>

namespace Magma
{
	namespace Graphics
	{
		/// <summary>
		///		Holds all state information relevant to a character
		/// </summary>
		struct Character
		{
			/// <summary>
			///		Glyph texture ID
			/// </summary>
			int texture;

			/// <summary>
			///		Glyph size
			/// </summary>
			glm::ivec2 size;

			/// <summary>
			///		Glyph bearing (offset from baseline to left/top of glyph)
			/// </summary>
			glm::ivec2 bearing;

			/// <summary>
			///		Horizontal offset to advance to next glyph
			/// </summary>
			int advance;
		};

		/// <summary>
		///		A renderer class used to render a single font.
		///		In order to use another font, you need to create another TextRenderer object for it.
		/// </summary>
		class TextRenderer
		{
		public:
			TextRenderer(Context& context);
			~TextRenderer();

			inline void SetShaderProgram(int shaderProgram) { m_shaderProgram = shaderProgram; m_destroyShader = false; }
			inline int GetShaderProgram() { return m_shaderProgram; }

			inline Character GetCharacter(char chr) { return m_characters[chr]; }

			void Load(const std::string& fontPath, unsigned int fontSize);
			void Render(const std::string& text, const glm::mat4& transform, const glm::vec3& color);

		private:
			std::map<char, Character> m_characters;

			Context& m_context;

			int m_vao, m_vbo;
			int m_shaderProgram;
			int m_vertShader;
			int m_fragShader;
			bool m_destroyShader;
		};
	}
}