#pragma once

#include <functional>
#include <map>

namespace Magma
{
	namespace Input
	{
		template <typename ... TArgs>
		class Event
		{
		public:
			inline Event() { m_nextID = 1; }
			inline ~Event() { }

			inline size_t AddListener(std::function<void(TArgs...)> listener) { m_listeners.try_emplace(m_nextID, listener); return m_nextID++; }
			inline void RemoveListener(size_t listener) { m_listeners.erase(listener); }

			inline void Fire(TArgs ... args) { for (auto& l : m_listeners) l.second(args...); }

		private:
			size_t m_nextID;
			std::map<size_t, std::function<void(TArgs...)>> m_listeners;
		};
	}
}