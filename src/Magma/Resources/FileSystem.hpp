#pragma once

#include <string>

namespace Magma
{
	namespace Resources
	{
		class FileSystem
		{
		public:
			FileSystem(const std::string& root);
			~FileSystem();

			virtual void Read(const std::string& path);

		private:

		};
	}
}