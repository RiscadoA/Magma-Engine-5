#pragma once

#include <cstdint>


/*

Main thread:
	Load settings;
	Load base resources;
	Start console thread;
	Open window;
	While running:
		Handle events;

Console thread:
	Open console window;
	While running:
		Get input;
		If input is a command:
			Handle command;
		Render text;
	Close console window;

*/

namespace Magma
{
	class Engine final
	{
	public:
		Engine();
		~Engine();

		inline bool IsRunning() const { return m_running; }

	private:
		bool m_running;
	};
}

extern void Init(Magma::Engine& engine);
