#pragma once

#include "ContiguousIterator.h"

namespace DreamSTL
{

	template<typename T>
	inline ContiguousIterator<T>::reference ContiguousIterator<T>::operator*() const noexcept
	{
		DS_ASSERT(ptr != nullptr);

		return *ptr;
	}

	template<typename T>
	inline ContiguousIterator<T>::pointer ContiguousIterator<T>::operator->() const noexcept
	{
		return ptr;
	}
	template<typename T>
	inline ContiguousIterator<T>::reference ContiguousIterator<T>::operator[](difference_type _offset) const noexcept
	{
		DS_ASSERT(ptr != nullptr);

		return ptr[_offset];
	}
	template<typename T>
	inline ContiguousIterator<T>& ContiguousIterator<T>::operator++() noexcept
	{
		++ptr;
		return *this;
	}
	template<typename T>
	inline ContiguousIterator<T> ContiguousIterator<T>::operator++(int) noexcept
	{
		ContiguousIterator tmp = *this;
		++ptr;
		return tmp;
	}
	template<typename T>
	inline ContiguousIterator<T>& ContiguousIterator<T>::operator--() noexcept
	{
		--ptr;
		return *this;
	}
	template<typename T>
	inline ContiguousIterator<T> ContiguousIterator<T>::operator--(int) noexcept
	{
		ContiguousIterator tmp = *this;
		--ptr;
		return tmp;
	}
	template<typename T>
	inline ContiguousIterator<T>& ContiguousIterator<T>::operator+=(difference_type _offset) noexcept
	{
		ptr += _offset;
		return *this;
	}
	template<typename T>
	inline ContiguousIterator<T>& ContiguousIterator<T>::operator-=(difference_type _offset) noexcept
	{
		ptr -= _offset;
		return *this;
	}

	template<typename T>
	inline ContiguousIterator<T> ContiguousIterator<T>::operator+(difference_type _offset) const noexcept
	{
		return ContiguousIterator(ptr + _offset);
	}

	template<typename T>
	inline ContiguousIterator<T> ContiguousIterator<T>::operator-(difference_type _offset) const noexcept
	{
		return ContiguousIterator(ptr -_offset);
	}

	template<typename T>
	template<typename U>
	inline ContiguousIterator<T>::difference_type ContiguousIterator<T>::operator-(const ContiguousIterator<U>& _other) const noexcept
	{
		return ptr - _other.Get();
	}

	template<typename T>
	template<typename U>
	inline bool ContiguousIterator<T>::operator==(const ContiguousIterator<U>& _other) const noexcept
	{
		return ptr == _other.Get();
	}

	template<typename T>
	template<typename U>
	inline std::strong_ordering ContiguousIterator<T>::operator<=>(const ContiguousIterator<U>& _other) const noexcept
	{
		return ptr <=> _other.Get();
	}

	
}