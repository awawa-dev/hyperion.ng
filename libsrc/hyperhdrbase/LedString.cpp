// STL includes
#include <cstring>
#include <iostream>

#include <hyperhdrbase/LedString.h>

std::vector<Led>& LedString::leds()
{
	return mLeds;
}

const std::vector<Led>& LedString::leds() const
{
	return mLeds;
}
