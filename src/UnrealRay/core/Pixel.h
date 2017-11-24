#pragma once

#include"Type.h"
#include"Spectrum.h"

namespace unreal
{
	struct Pixel
	{
		Pixel() : L(0.0f)
		{
			alpha = 0.0f;
			weightSum = 0;
		}
		Spectrum L;
		Float alpha;
		int weightSum;
	};
}
