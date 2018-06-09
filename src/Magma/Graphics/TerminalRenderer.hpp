#pragma once

#include "Context.hpp"
#include "TextRenderer.hpp"

#include <string>

namespace Magma
{
	namespace Graphics
	{
		class TerminalRenderer final
		{
		public:
			TerminalRenderer(Context& context);
			virtual ~TerminalRenderer() = default;

			void ClearOutput();
			void RenderOutput(const std::string& outputString);
			void RenderInput(const std::string& inputString);

		protected:
			Context & m_context;
			TextRenderer* m_consoleTextRenderer;
		};
	}
}