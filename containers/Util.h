#pragma once

#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <limits>
#include <type_traits>
#include <utility>
#include <initializer_list>



using std::size_t;





#define LENGTH(array) (sizeof(array) / sizeof(array[0]))

//template <typename T, size_t N>
//constexpr inline size_t LENGTH(const T (&)[N])
//{
//	return N;
//}



template<typename T>
T* array_calloc(size_t size) noexcept
{
//    cout << "EDW_calloc for " << size <<endl;
	return static_cast<T*>(std::calloc(size, sizeof(T)));
}

#include <iostream>
#include <iomanip>
/*void printPointerAlignment(void* pointer)
{
	uintptr_t alignment = 2;
	while (reinterpret_cast<uintptr_t>(pointer) % alignment == 0)
		alignment *= 2;
	alignment /= 2;
	std::cout << "Pointer " << std::hex << reinterpret_cast<uintptr_t>(pointer) << std::dec <<
			" alignment " << alignment << " bytes" << std::endl;

}*/

template<typename T>
T* array_malloc(size_t size) noexcept
{
//    cout << "EDW_malloc for " << size <<endl;
	return static_cast<T*>(std::malloc(size * sizeof(T)));
}





template <typename T>
T next_power_of_two(T x)
{
	x--;
	                   x |= x >>  1;
	                   x |= x >>  2;
	                   x |= x >>  4;
	if (sizeof(T) > 1) x |= x >>  8;
	if (sizeof(T) > 2) x |= x >> 16;
	if (sizeof(T) > 4) x |= x >> 32;
	x++;
	return x;
}


