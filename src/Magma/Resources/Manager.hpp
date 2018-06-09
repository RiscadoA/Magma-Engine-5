#pragma once

namespace Magma
{
	namespace Resources
	{
		class ResourceInfo
		{
		public:

		private:
			const char* m_path;
			const char* m_relative;
		};

		class Resource
		{
		public:

			virtual size_t GetSize() const = 0;

		private:

		};

		class Manager final
		{
		public:
			Manager();
			~Manager();

		private:

		};
	}
}