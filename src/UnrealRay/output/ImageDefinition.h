#pragma once
#ifndef UNREALRAY_IMAGES_IMAGEDEFINITION_H
#define UNREALRAY_IMAGES_IMAGEDEFINITION_H

#include<cstdint>

namespace unreal
{
	template<class T, class U>
	class ImageConvert
	{
	public:
		static U convert(T val)
		{
			return static_cast<U>(val);
		}
	};
	template<>
	class ImageConvert<std::uint8_t, double>
	{
	public:
		static double convert(std::uint8_t val)
		{
			return val / 255.0;
		}
	};
	template<>
	class ImageConvert<std::uint8_t, float>
	{
	public:
		static float convert(std::uint8_t val)
		{
			return val / 255.0f;
		}
	};

	template<>
	class ImageConvert<double, std::uint8_t>
	{
	public:
		static std::uint8_t convert(double val)
		{
			double tmp = val * 255;
			if (tmp > 255)
			{
				return 255;
			}
			else if (tmp < 0)
			{
				return 0;
			}
			else
			{
				return static_cast<std::uint8_t>(tmp);
			}
		}
	};
	template<>
	class ImageConvert<float, std::uint8_t>
	{
	public:
		static std::uint8_t convert(float val)
		{
			float tmp = val * 255;
			if (tmp > 255)
			{
				return 255;
			}
			else if (tmp < 0)
			{
				return 0;
			}
			else
			{
				return static_cast<std::uint8_t>(tmp);
			}
		}
	};
}

#endif