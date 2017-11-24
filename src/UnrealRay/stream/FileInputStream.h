#pragma once
#ifndef UNREALRAY_FILEINPUTSTREAM_H
#define UNREALRAY_FILEINPUTSTREAM_H

#include<cstdio>
#include<exception>

#include"InputStream.h"
namespace unreal
{
	namespace stream
	{
		class FileInputStream :public InputStream<char>
		{
		protected:
			std::FILE * file;
		public:
			FileInputStream(std::FILE* file_) : file(file_) {}
			FileInputStream(const FileInputStream& src) = delete;
			FileInputStream(FileInputStream&& src) : file(src.file)
			{
				src.file = nullptr;
			}
			virtual ~FileInputStream() = default;

			FileInputStream& operator =(const FileInputStream& src) = delete;
			FileInputStream& operator =(FileInputStream&& src)
			{
				file = src.file;
				src.file = nullptr;
				return *this;
			}
			//read virtual function
			virtual CharType read() override
			{
				CharType ret;
				if (read(&ret, 1) != 0)
					return ret;
				else
					throw std::runtime_error("End of stream");
			}
			virtual std::size_t read(CharType * buffer, std::size_t count)override
			{
				std::size_t ret = std::fread(buffer, sizeof(CharType), count, file);
				return ret;
			}

			virtual void skip()override
			{
				skip(1);
			}
			virtual void skip(std::size_t count)override
			{
				if (std::fseek(file, count, SEEK_CUR))
					throw std::runtime_error("std::fseek failed");
			}

			//buffer virtual function
			virtual void flush() {}

			//seek virtual function
			virtual bool isSeekable() const
			{
				return true;
			}
			virtual std::intmax_t seek(std::intmax_t offset, PosType method)
			{
				int seek_pos;
				switch (method)
				{
				case PosType::Begin:
					seek_pos = SEEK_SET;
					break;
				case PosType::End:
					seek_pos = SEEK_END;
					break;
				case PosType::Current:
					seek_pos = SEEK_CUR;
					break;
				default:
					throw std::runtime_error("Unknown postype.");
				}
				return std::fseek(file, static_cast<long>(offset), seek_pos);
			}

			virtual std::intmax_t getPosition()
			{
				return std::ftell(file);
			}
		};
	}
}

#endif // !UNREALRAY_FILEINPUTSTREAM_H
