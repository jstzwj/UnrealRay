#pragma once
#ifndef UNREALRAY_STREAMBASE_H
#define UNREALRAY_STREAMBASE_H

#include<exception>
#include<cstdint>

namespace unreal
{
	namespace stream
	{
		class StreamTraits
		{
		public:
			enum class PosType { Begin, Current, End };
		};

		template <typename T, typename Traits>
		class StreamBase
		{
		public:
			using CharType = T;
			using PosType = typename Traits::PosType;
			virtual ~StreamBase() = default;
			//read virtual function
			virtual bool isReadable() const = 0;
			virtual T read() = 0;
			virtual std::size_t read(T* buffer, std::size_t count) = 0;

			virtual void skip() = 0;
			virtual void skip(std::size_t count) = 0;

			//write virtual function
			virtual bool isWriteable() const = 0;
			virtual void write(T value) = 0;
			virtual void write(const T* buffer, std::size_t count) = 0;

			//buffer virtual function
			virtual void flush() = 0;

			//seek virtual function
			virtual bool isSeekable() const = 0;
			virtual std::intmax_t seek(std::intmax_t offset, PosType method) = 0;

			virtual std::intmax_t getPosition() = 0;
		};
	}
}

#endif //UNREALRAY_STREAMBASE_H
