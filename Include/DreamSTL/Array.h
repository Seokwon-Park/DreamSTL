#pragma once
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <new>
#include <type_traits>
#include <utility>

#include "Core.h"
#include "DefaultAllocator.h"
#include "ContiguousIterator.h"

namespace DreamSTL
{
	template<typename T, typename Allocator = DefaultAllocator>
	class Array
	{
	public:
		// 타입
		using ElementType = T;
		using AllocatorType = Allocator;
		using SizeType = typename Allocator::SizeType;

		using Iterator = ContiguousIterator<T>;          
		using ConstIterator = ContiguousIterator<const T>;

	public:
		// 생성 / 소멸
		Array() = default;
		explicit Array(const AllocatorType& _allocator) :allocator(_allocator) {}

		explicit Array(SizeType _count);                      // 기본 생성된 원소 _count개
		Array(SizeType _count, const T& _value);              // _value 복사본 _count개
		Array(std::initializer_list<T> _list);                // Array<int> a = { 1, 2, 3 };

		Array(const Array& _other);                           // 깊은 복사
		Array(Array&& _other) noexcept;                       // 포인터만 가져오고 _other는 빈 상태로

		~Array();                                             // 원소 소멸 + 메모리 해제

		Array& operator=(const Array& _other);
		Array& operator=(Array&& _other) noexcept;
		Array& operator=(std::initializer_list<T> _list);

		// 원소 접근
		[[nodiscard]] T& operator[](SizeType _index);           // DS_ASSERT(_index < m_size)
		[[nodiscard]] const T& operator[](SizeType _index) const;

		[[nodiscard]] T& First();                               // DS_ASSERT(!IsEmpty())
		[[nodiscard]] const T& First() const;
		[[nodiscard]] T& Last();
		[[nodiscard]] const T& Last() const;

		[[nodiscard]] T* Data() noexcept { return data; }
		[[nodiscard]] const T* Data() const noexcept { return data; }

		// 반복자 (범위 기반 for 지원)
		// 이름이 소문자 begin/end여야 for (auto& x : arr)가 동작함
		[[nodiscard]] Iterator      begin() noexcept { return Iterator(data); }
		[[nodiscard]] ConstIterator begin() const noexcept { return ConstIterator(data); }
		[[nodiscard]] Iterator      end() noexcept { return Iterator(data + size); }
		[[nodiscard]] ConstIterator end() const noexcept { return ConstIterator(data + size); }

		// 크기 / 용량
		[[nodiscard]] SizeType Size() const noexcept { return size; }
		[[nodiscard]] SizeType Capacity() const noexcept { return capacity; }
		[[nodiscard]] bool     IsEmpty() const noexcept { return size == 0; }

		void Reserve(SizeType _newCapacity);      
		void Resize(SizeType _newSize);           
		void Resize(SizeType _newSize, const T& _value);
		void Shrink();                            
		void Clear() noexcept;                    

		// 추가
		T& Add(const T& _value);                  
		T& Add(T&& _value);

		template<typename... Args> 
		T& Emplace(Args&&... _args);              

		T& Insert(SizeType _index, const T& _value); 
		T& Insert(SizeType _index, T&& _value);

		// 제거
		void Pop();                               
		void RemoveAt(SizeType _index);           
		void RemoveAtSwap(SizeType _index);       

		// 검색
		[[nodiscard]] SizeType Find(const T& _value) const;      
		[[nodiscard]] bool     Contains(const T& _value) const;

		static constexpr SizeType InvalidIndex = std::numeric_limits<SizeType>::max();

		// 기타
		void Swap(Array& _other) noexcept;
		[[nodiscard]] const AllocatorType& GetAllocator() const noexcept { return allocator; }

		[[nodiscard]] bool operator==(const Array& _other) const;   

	private:
		// 내부 헬퍼
		template<typename... Args>
		T& EmplaceSlow(Args&&... _args);          // 재할당 경로. 새 원소를 먼저 생성

		[[nodiscard]] SizeType CalculateGrowth(SizeType _required) const;
		void Reallocate(SizeType _newCapacity);   // 새 메모리 할당 -> 원소 이동 -> 옛 메모리 해제

		[[nodiscard]] T* AllocateBuffer(SizeType _capacity);

		void DeallocateBuffer(T* _ptr, SizeType _capacity) noexcept;

		void DestroyAll() noexcept;               // 소멸자, Clear, 대입 연산자에서 공용

	private:
		T* data = nullptr;
		SizeType size = 0;
		SizeType capacity = 0;
		DS_NO_UNIQUE_ADDRESS AllocatorType allocator;
	};
}

#include "Array.inl"