#include "windows/stdafx.h"
#include "soundHelpers.h"


beep::beep()
{
}

beep::~beep()
{
}

void beep::error()
{
	MessageBeep(MB_ICONERROR);
}

void beep::allgood()
{
	MessageBeep(MB_ICONINFORMATION);
}
