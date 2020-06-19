#pragma once

#include "public/containers/allocator.h"

/* Standard includes */
#undef min
#undef max
#include <list>

template<class T>
class List : public std::list<T>
{
public:

	bool contains(const T& item)
	{
		for(auto x : *this)
			if(item == x) return true;
		return false;
	}
};