#pragma once
#include <compare>
#include <cstddef>
#include <iterator>
#include <type_traits>

#include "Core.h"

namespace Dream
{
	template<typename T>
	class ContiguousIterator
	{
	public:
		using iterator_concept = std::contiguous_iterator_tag;
		using iterator_category = std::random_access_iterator_tag;
		using value_type = std::remove_cv_t<T>;
		using element_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

	public:
		ContiguousIterator() noexcept = default;
		explicit ContiguousIterator(pointer _ptr) noexcept;

		// Iterator -> ConstIterator 변환만 허용
		template<typename U>
			requires std::is_same_v<const U, T>
		ContiguousIterator(const ContiguousIterator<U>& _other) noexcept;

		ContiguousIterator(const ContiguousIterator&) noexcept = default;
		ContiguousIterator& operator=(const ContiguousIterator&) noexcept = default;

		// 역참조
		[[nodiscard]] reference operator*() const noexcept;
		[[nodiscard]] pointer   operator->() const noexcept;
		[[nodiscard]] reference operator[](difference_type _offset) const noexcept;

		// 증감 연산자
		ContiguousIterator& operator++() noexcept;
		ContiguousIterator  operator++(int) noexcept;
		ContiguousIterator& operator--() noexcept;
		ContiguousIterator  operator--(int) noexcept;

		// 산술 연산자
		ContiguousIterator& operator+=(difference_type _offset) noexcept;
		ContiguousIterator& operator-=(difference_type _offset) noexcept;

		[[nodiscard]] ContiguousIterator operator+(difference_type _offset) const noexcept;
		[[nodiscard]] ContiguousIterator operator-(difference_type _offset) const noexcept;

		// n + it (클래스 템플릿의 friend는 안에서 정의해야 해서 예외적으로 본문 포함)
		[[nodiscard]] friend ContiguousIterator operator+(difference_type _offset, const ContiguousIterator& _it) noexcept
		{
			return _it + _offset;
		}

		// it1 - it2 (Iterator와 ConstIterator 사이도 가능)
		template<typename U>
		[[nodiscard]] difference_type operator-(const ContiguousIterator<U>& _other) const noexcept;

		// 비교 (!=, <, <=, >, >=는 자동 생성)
		template<typename U>
		[[nodiscard]] bool operator==(const ContiguousIterator<U>& _other) const noexcept;

		template<typename U>
		[[nodiscard]] std::strong_ordering operator<=>(const ContiguousIterator<U>& _other) const noexcept;

		// 내부 포인터
		[[nodiscard]] pointer Get() const noexcept;

	private:
		pointer m_ptr = nullptr;
	};

	static_assert(std::contiguous_iterator<ContiguousIterator<int>>);
	static_assert(std::contiguous_iterator<ContiguousIterator<const int>>);
}
