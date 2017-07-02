#pragma once
#ifndef UNREALRAY_MATRIX
#define UNREALRAY_MATRIX

class Transform
{
public:
	Transform()
	{
		for (int i = 0; i < 16; ++i)
			data[i] = 0.0;
	}
	double data[16];
};

#endif // !UNREALRAY_MATRIX
