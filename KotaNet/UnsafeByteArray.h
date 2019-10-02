#pragma once


namespace Kota
{	
	template<size_t N, size_t BlockSize>
	class UnsafeByteArray
	{
	public:
		static const int32_t DEFAULT_MAKE_SIZE = 1024;

	public:
		UnsafeByteArray()		
		{
			static_assert(N > 1024, "Must be greater than 1024");
			static_assert(BlockSize < (N / 2), "BlockSize must be greater than N");
		}

		~UnsafeByteArray()
		{
		}

		int8_t* Alloc()
		{	
		}

	private:
		std::array<int8_t, N> _byteArray;
		int32_t _arraySize = N;
		int32_t _blockSize = BlockSize;
		int32_t _cursor;
	};
}