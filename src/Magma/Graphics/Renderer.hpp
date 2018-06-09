#pragma once

#include "Context.hpp"

namespace Magma
{
	namespace Graphics
	{
		enum class RenderOrder
		{

		};

		struct RenderFrame
		{

		};

		class Renderer
		{
		public:
			Renderer(Context& context);
			virtual ~Renderer() = default;

			virtual void Render(RenderFrame* frame) = 0;

		protected:
			Context& m_context;
		};
	}
}