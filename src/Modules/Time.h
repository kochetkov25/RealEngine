#pragma once
#include <chrono>

namespace Core
{

	class Time
	{
	public:
		void start();
		/*return time in mili seconds*/
		float stop();
	private:
		bool _isRunning = false;
		std::chrono::time_point<std::chrono::high_resolution_clock> _startTime{};
	};

}