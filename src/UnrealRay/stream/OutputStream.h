#pragma once
#ifndef UNREALRAY_OUTPUTSTREAM_H
#define UNREALRAY_OUTPUTSTREAM_H

#include<cstdint>
#include"StreamBase.h"
#include"Endian.h"

using namespace unreal::data;

namespace unreal
{
	namespace stream
	{
		template <typename T = char, typename Traits = StreamTraits>
		class OutputStream :public StreamBase<T, Traits>
		{
		public:
			virtual ~OutputStream() = default;

			virtual bool isWriteable() const override
			{
				return true;
			}
			virtual bool isReadable() const override
			{
				return false;
			}
			virtual T read() override
			{
				throw std::runtime_error("stream is not readable.");
			}
			virtual std::size_t read(T* /*buffer*/, std::size_t /*count*/) override
			{
				throw std::runtime_error("stream is not readable.");
			}
			virtual void skip() override
			{
				throw std::runtime_error("stream is not readable.");
			}
			virtual void skip(std::size_t /*count*/) override
			{
				throw std::runtime_error("stream is not readable.");
			}
		};

		template<class T, class CharType = char>
		class OutputWrapper
		{
		public:
			static void write(OutputStream<CharType>& stream_, T value)
			{
				if (isLE())
				{
					stream_.write(reinterpret_cast<const char *>(&value), sizeof(T));
				}
				else
				{
					T swap_value(SwapEndian<T>::swap(value));
					stream_.write(reinterpret_cast<const char *>(&swap_value), sizeof(T));
				}
			}
		};
	}
}

#endif // !UNREALRAY_OUTPUTSTREAM_H
