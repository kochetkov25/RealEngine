#include "Time.h"


namespace Core
{

	void Time::start()
	{
		/*check if timer is running*/
		if (_isRunning)
			return;

		_isRunning = true;
		/*start time*/
		_startTime = std::chrono::high_resolution_clock::now();
	}
	
	/*return time in mili seconds*/
	float Time::stop()
	{
		/*if timer isnt running*/
		if (!_isRunning)
			return 0.f;

		_isRunning = false;
		auto stopTime = std::chrono::high_resolution_clock::now();
		/*using microsec to have a higher resolution*/
		float miliSec = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - _startTime).count() / 1000.f;
		return miliSec;
	}

}
