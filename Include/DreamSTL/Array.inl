#pragma once
#include "Array.h"

namespace DreamSTL
{
	template<typename T, typename Allocator>
	inline Array<T, Allocator>::Array(SizeType _count)
	{
		if (_count == 0) return;

		data = AllocateBuffer(_count);
		capacity = _count;

		for (SizeType i = 0; i < _count; ++i)
		{
			new (&data[i]) T();
		}

		size = _count;
	}

	template<typename T, typename Allocator>
	inline Array<T, Allocator>::Array(SizeType _count, const T& _value)
	{
		if (_count == 0) return;

		data = AllocateBuffer(_count);
		capacity = _count;

		for (SizeType i = 0; i < _count; ++i)
		{
			new (&data[i]) T(_value);
		}

		size = _count;
	}

	template<typename T, typename Allocator>
	inline Array<T, Allocator>::Array(std::initializer_list<T> _list)
	{
		const SizeType count = static_cast<SizeType>(_list.size());
		if (count == 0) return;

		data = AllocateBuffer(count);
		capacity = count;

		for (const T& elem : _list)
		{
			new (&data[size++]) T(elem);
		}
	}

	template<typename T, typename Allocator>
	inline Array<T, Allocator>::Array(const Array& _other)
		: allocator(_other.allocator)
	{
		if (_other.size == 0) return;

		data = AllocateBuffer(_other.size);
		capacity = _other.size;

		if constexpr (std::is_trivially_copyable_v<T>)
		{
			std::memcpy(data, _other.data, sizeof(T) * _other.size);
		}
		else
		{
			for (SizeType i = 0; i < _other.size; ++i)
			{
				new (&data[i]) T(_other.data[i]);
			}
		}
		size = _other.size;
	}

	template<typename T, typename Allocator>
	inline Array<T, Allocator>::Array(Array&& _other) noexcept
		:data(_other.data), size(_other.size), capacity(_other.capacity), allocator(std::move(_other.allocator))
	{
		_other.data = nullptr;
		_other.size = 0;
		_other.capacity = 0;

	}

	template<typename T, typename Allocator>
	inline Array<T, Allocator>::~Array()
	{
		DestroyAll();
		DeallocateBuffer(data, capacity);
	}

	template<typename T, typename Allocator>
	inline Array<T, Allocator>& Array<T, Allocator>::operator=(const Array& _other)
	{
		if (this != &_other)
		{
			Array temp(_other);
			Swap(temp);
		}
		return *this;
	}

	template<typename T, typename Allocator>
	inline Array<T, Allocator>& Array<T, Allocator>::operator=(Array&& _other) noexcept
	{
		if (this != &_other)
		{
			DestroyAll();
			DeallocateBuffer(data, capacity);

			data = _other.data;
			size = _other.size;
			capacity = _other.capacity;
			allocator = std::move(_other.allocator);

			_other.data = nullptr;
			_other.size = 0;
			_other.capacity = 0;
		}
		return *this;
	}

	template<typename T, typename Allocator>
	inline Array<T, Allocator>& Array<T, Allocator>::operator=(std::initializer_list<T> _list)
	{
		DestroyAll();

		const SizeType count = static_cast<SizeType>(_list.size());
		if (count > capacity)
		{
			Reallocate(count);
		}

		for (const T& elem : _list)
		{
			new (&data[size++]) T(elem);
		}

		return *this;
	}

	template<typename T, typename Allocator>
	inline T& Array<T, Allocator>::operator[](SizeType _index)
	{
		DS_ASSERT(_index < size);

		return data[_index];
	}

	template<typename T, typename Allocator>
	inline const T& Array<T, Allocator>::operator[](SizeType _index) const
	{
		DS_ASSERT(_index < size);

		return data[_index];
	}

	template<typename T, typename Allocator>
	inline T& Array<T, Allocator>::First()
	{
		DS_ASSERT(!IsEmpty());

		return data[0];
	}

	template<typename T, typename Allocator>
	inline const T& Array<T, Allocator>::First() const
	{
		DS_ASSERT(!IsEmpty());

		return data[0];
	}

	template<typename T, typename Allocator>
	inline T& Array<T, Allocator>::Last()
	{
		DS_ASSERT(!IsEmpty());

		return data[size - 1];
	}

	template<typename T, typename Allocator>
	inline const T& Array<T, Allocator>::Last() const
	{
		DS_ASSERT(!IsEmpty());

		return data[size - 1];
	}

	template<typename T, typename Allocator>
	inline void Array<T, Allocator>::Reserve(SizeType _newCapacity)
	{
		if (_newCapacity <= capacity)
			return;

		Reallocate(_newCapacity);
	}

	template<typename T, typename Allocator>
	inline void Array<T, Allocator>::Resize(SizeType _newSize)
	{
		if (_newSize > size)
		{
			if (_newSize > capacity)
			{
				Reserve(CalculateGrowth(_newSize));
			}
			for (SizeType i = size; i < _newSize; ++i)
			{
				new (&data[i]) T();
			}
			size = _newSize;
		}
		else if (_newSize < size)
		{
			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				for (SizeType i = _newSize; i < size; ++i)
				{
					data[i].~T();
				}
			}
			size = _newSize;
		}
	}

	template<typename T, typename Allocator>
	inline void Array<T, Allocator>::Resize(SizeType _newSize, const T& _value)
	{
		if (_newSize > size)
		{
			if (_newSize > capacity)
			{
				T copy(_value);
				Reserve(CalculateGrowth(_newSize));
				for (SizeType i = size; i < _newSize; ++i)
				{
					new (&data[i]) T(copy);
				}
			}
			else
			{
				for (SizeType i = size; i < _newSize; ++i)
				{
					new (&data[i]) T(_value);
				}
			}
		}
		else if (_newSize < size)
		{
			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				for (SizeType i = _newSize; i < size; ++i)
				{
					data[i].~T();
				}
			}
			size = _newSize;
		}
		size = _newSize;
	}

	template<typename T, typename Allocator>
	inline void Array<T, Allocator>::Shrink()
	{
		if (size < capacity)
			Reallocate(size);
	}

	template<typename T, typename Allocator>
	inline void Array<T, Allocator>::Clear() noexcept
	{
		DestroyAll();
	}

	template<typename T, typename Allocator>
	inline T& Array<T, Allocator>::Add(const T& _value)
	{
		return Emplace(_value);
	}

	template<typename T, typename Allocator>
	inline T& Array<T, Allocator>::Add(T&& _value)
	{
		return Emplace(std::move(_value));
	}

	template<typename T, typename Allocator>
	template<typename ...Args>
	inline T& Array<T, Allocator>::Emplace(Args && ..._args)
	{
		if (size < capacity)
		{
			new (&data[size]) T(std::forward<Args>(_args)...);
			T& ret = data[size];
			size++;
			return ret;
		}
		else
		{
			return EmplaceSlow(std::forward<Args>(_args)...);
		}
	}



	template<typename T, typename Allocator>
	inline T& Array<T, Allocator>::Insert(SizeType _index, const T& _value)
	{
		DS_ASSERT(_index <= size);
		if (_index == size)
			return Emplace(_value);

		T temp(_value);
		return Insert(_index, std::move(temp));
	}

	template<typename T, typename Allocator>
	inline T& Array<T, Allocator>::Insert(SizeType _index, T&& _value)
	{
		DS_ASSERT(_index <= size);
		if (_index == size)
			return Emplace(std::move(_value));

		T moved(std::move(_value));

		if (size >= capacity)
		{
			Reserve(CalculateGrowth(size + 1));
		}

		new (&data[size]) T(std::move(data[size - 1]));

		for (SizeType i = size - 1; i >= _index + 1; --i)
		{
			data[i] = std::move(data[i - 1]);
		}

		data[_index] = std::move(moved);
		++size;
		return data[_index];
	}

	template<typename T, typename Allocator>
	inline void Array<T, Allocator>::Pop()
	{
		DS_ASSERT(!IsEmpty());
		--size;
		if constexpr (!std::is_trivially_destructible_v<T>)
		{
			data[size].~T();
		}
	}

	template<typename T, typename Allocator>
	inline void Array<T, Allocator>::RemoveAt(SizeType _index)
	{
		DS_ASSERT(_index < size);
		for (SizeType i = _index; i < size - 1; i++)
		{
			data[i] = std::move(data[i + 1]);
		}
		Pop();
	}

	template<typename T, typename Allocator>
	inline void Array<T, Allocator>::RemoveAtSwap(SizeType _index)
	{
		DS_ASSERT(_index < size);
		if (_index != size - 1)
		{
			data[_index] = std::move(data[size - 1]);
		}
		Pop();
	}

	template<typename T, typename Allocator>
	inline Array<T, Allocator>::SizeType Array<T, Allocator>::Find(const T& _value) const
	{
		for (SizeType i = 0; i < size; ++i)
		{
			if (data[i] == _value)
			{
				return i;
			}
		}
		return InvalidIndex;
	}

	template<typename T, typename Allocator>
	inline bool Array<T, Allocator>::Contains(const T& _value) const
	{
		return Find(_value) != InvalidIndex;
	}


	template<typename T, typename Allocator>
	template<typename ...Args>
	inline T& Array<T, Allocator>::EmplaceSlow(Args && ..._args)
	{
		SizeType newCapacity = CalculateGrowth(size + 1);
		T* newData = AllocateBuffer(newCapacity);

		new (&newData[size]) T(std::forward<Args>(_args)...);

		if (size > 0)
		{
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				std::memcpy(newData, data, sizeof(T) * size);
			}
			else
			{
				for (SizeType i = 0; i < size; ++i)
				{
					new (&newData[i]) T(std::move(data[i]));
					data[i].~T();
				}
			}
		}

		// 메모리 반납
		DeallocateBuffer(data, capacity);

		// 멤버 갱신
		data = newData;
		capacity = newCapacity;
		T& ret = data[size];
		size++;
		return ret;
	}

	template<typename T, typename Allocator>
	inline void Array<T, Allocator>::Swap(Array& _other) noexcept
	{
		std::swap(data, _other.data);
		std::swap(size, _other.size);
		std::swap(capacity, _other.capacity);
		std::swap(allocator, _other.allocator);
	}

	template<typename T, typename Allocator>
	inline bool Array<T, Allocator>::operator==(const Array& _other) const
	{
		if (size != _other.size) return false;

		for (SizeType i = 0; i < size; ++i)
		{
			if (!(data[i] == _other.data[i])) return false;
		}
		return true;
	}

	template<typename T, typename Allocator>
	inline Array<T, Allocator>::SizeType Array<T, Allocator>::CalculateGrowth(SizeType _required) const
	{
		constexpr SizeType maxCap = InvalidIndex - 1;
		DS_ASSERT(_required <= maxCap);

		if (capacity > maxCap - capacity / 2)
		{
			return maxCap;
		}

		SizeType geometric = capacity + capacity / 2;
		if (geometric < 4)
		{
			geometric = 4;
		}

		return (geometric < _required) ? _required : geometric;
	}

	template<typename T, typename Allocator>
	inline void Array<T, Allocator>::Reallocate(SizeType _newCapacity)
	{
		DS_ASSERT(_newCapacity >= size);

		if (_newCapacity == 0)
		{
			DeallocateBuffer(data, capacity);
			data = nullptr;
			capacity = 0;
			return;
		}

		T* newData = AllocateBuffer(_newCapacity);

		if (size > 0)
		{
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				std::memcpy(newData, data, sizeof(T) * size);
			}
			else
			{
				for (SizeType i = 0; i < size; ++i)
				{
					new (&newData[i]) T(std::move(data[i]));
					data[i].~T();
				}
			}
		}

		DeallocateBuffer(data, capacity);
		data = newData;
		capacity = _newCapacity;
	}

	template<typename T, typename Allocator>
	inline T* Array<T, Allocator>::AllocateBuffer(SizeType _capacity)
	{
		return static_cast<T*>(allocator.Allocate(sizeof(T) * _capacity, alignof(T)));
	}
	template<typename T, typename Allocator>
	inline void Array<T, Allocator>::DeallocateBuffer(T* _ptr, SizeType _capacity) noexcept
	{
		if (_ptr != nullptr)
		{
			allocator.Deallocate(_ptr, sizeof(T) * _capacity, alignof(T));
		}
	}

	template<typename T, typename Allocator>
	inline void Array<T, Allocator>::DestroyAll() noexcept
	{
		if constexpr (!std::is_trivially_destructible_v<T>)
		{
			for (SizeType i = 0; i < size; ++i)
			{
				data[i].~T();
			}
		}
		size = 0;
	}


}