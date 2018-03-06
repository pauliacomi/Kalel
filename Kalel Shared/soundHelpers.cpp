#include "soundHelpers.h"
#include <iostream>

namespace soundh {

	beep::beep()
	{
	}

	beep::~beep()
	{
	}

	void beep::error()
	{
		std::cout << '\a';
	}

	void beep::allgood()
	{
		std::cout << '\a' << '\a';
	}

}