#pragma once
#ifndef UNREALRAY_IMAGES_BMP_H
#define UNREALRAY_IMAGES_BMP_H

#include<memory>
#include<type_traits>
#include<utility>
#include<string>
#include<memory>
#include <cstddef>
#include<cstdio>
#include<cstring>
#include<cmath>

#include"../core/Pixel.h"
#include"../Stream/FileInputStream.h"
#include"../Stream/FileOutputStream.h"
#include"ImageDefinition.h"

namespace unreal
{
	struct tag_BitMapFileHeader
	{
		char bfType[2];
		uint32_t bfSize;
		uint16_t bfReserved1;
		uint16_t bfReserved2;
		uint32_t bfOffBits;
	};
	struct tag_BitmapInfoHeader
	{
		uint32_t biSize; //指定此结构体的长度，为40
		int32_t biWidth; //位图宽
		int32_t biHeight; //位图高
		uint16_t biPlanes; //平面数，为1
		uint16_t biBitCount; //采用颜色位数，可以是1，2，4，8，16，24，新的可以是32
		uint32_t biCompression; //压缩方式，可以是0，1，2，其中0表示不压缩
		uint32_t biSizeImage; //实际位图数据占用的字节数
		int32_t biXPelsPerMeter; //X方向分辨率
		int32_t biYPelsPerMeter; //Y方向分辨率
		uint32_t biClrUsed; //使用的颜色数，如果为0，则表示默认值(2^颜色位数)
		uint32_t biClrImportant; //重要颜色数，如果为0，则表示所有颜色都是重要的
	};

	struct tag_RGBQuad
	{
		uint8_t rgbBlue; //该颜色的蓝色分量
		uint8_t rgbGreen; //该颜色的绿色分量
		uint8_t rgbRed; //该颜色的红色分量
		uint8_t rgbReserved; //保留值
	};

	enum class BiCompression :int
	{
		BI_RGB = 0,
		BI_RLE8 = 1,
		BI_BLE4 = 2,
		BI_BITFIELDS = 3,
		BI_JPEG = 4,
		BI_PNG = 5
	};

	class BmpWriter
	{
	private:
		int x, y;
	public:
		using StreamType = stream::OutputStream<char>;
		StreamType& data_source;

		BmpWriter(StreamType& stream, int x, int y)
			:data_source(stream), x(x), y(y) {}

		void write(const std::vector<Pixel> &pixels)
		{
			//file header
			data_source.write("BM", 2);
			stream::OutputWrapper<uint32_t>::write(data_source, x*y * 3 + 14 + 40);//file size
			stream::OutputWrapper<uint16_t>::write(data_source, 0);//reserved
			stream::OutputWrapper<uint16_t>::write(data_source, 0);//reserved
			stream::OutputWrapper<uint32_t>::write(data_source, 14 + 40);//bytes before pic info

			//bitmap header
			stream::OutputWrapper<uint32_t>::write(data_source, 40);//bitmap info header size
			stream::OutputWrapper<int32_t>::write(data_source, x);//width
			stream::OutputWrapper<int32_t>::write(data_source, -y);//height
			stream::OutputWrapper<uint16_t>::write(data_source, 1);//biplanes
			stream::OutputWrapper<uint16_t>::write(data_source, 24);//bitcount
			stream::OutputWrapper<uint32_t>::write(data_source, (uint32_t)BiCompression::BI_RGB);//bicompression
			stream::OutputWrapper<uint32_t>::write(data_source, y*x * 3);//DWORD biSizeImage
			stream::OutputWrapper<int32_t>::write(data_source, 0);//x
			stream::OutputWrapper<int32_t>::write(data_source, 0);//y
			stream::OutputWrapper<uint32_t>::write(data_source, 0);//index number
			stream::OutputWrapper<uint32_t>::write(data_source, 0);//index number
			//data

			for (int i = 0; i < y; ++i)
			{
				for (int j = 0; j < x; ++j)
				{
					Float rgb[3];
					Spectrum cur_spectrum = pixels[j*x + i].L;
					cur_spectrum /= (Float)pixels[j*x + i].weightSum;
					cur_spectrum.ToRGB(rgb);
					stream::OutputWrapper<uint8_t>::write(data_source, ImageConvert<Float, std::uint8_t>::convert(rgb[0]));
					stream::OutputWrapper<uint8_t>::write(data_source, ImageConvert<Float, std::uint8_t>::convert(rgb[1]));
					stream::OutputWrapper<uint8_t>::write(data_source, ImageConvert<Float, std::uint8_t>::convert(rgb[2]));
				}
				//padding
				int padding_size = 4 * (int)std::ceil(24 * x / 32.0);
				for (int j = 0; j < padding_size - x * 3; ++j)
				{
					stream::OutputWrapper<uint8_t>::write(data_source, 0);
				}
			}
		}
	};
}

#endif //UNREALRAY_IMAGES_BMP_H