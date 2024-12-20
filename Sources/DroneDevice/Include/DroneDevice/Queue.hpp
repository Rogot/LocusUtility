//
// Queue.hpp
//
//  Created on: Mar 22, 2017
//      Author: Alexander
//

#ifndef DRONEDEVICE_QUEUE_HPP_
#define DRONEDEVICE_QUEUE_HPP_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>

namespace QueueHelpers {

template<size_t capacity>
struct RetType {
	using Type = size_t;
};

template<>
struct RetType<std::numeric_limits<uint8_t>::max() + 1> {
	using Type = uint8_t;
};

template<>
struct RetType<std::numeric_limits<uint16_t>::max() + 1> {
	using Type = uint16_t;
};

template<size_t C>
constexpr bool isPowerOfTwo = !(C & (C - 1));

template<size_t C>
constexpr bool isTypeAligned = ((C - 1) == std::numeric_limits<typename RetType<C>::Type>::max());

} // namespace QueueHelpers

template<typename T, size_t capacity>
class Queue {
public:
	static constexpr size_t kCapacity{capacity};

	Queue()
	{
	}

	void clear()
	{
		head = tail = elements = 0;
	}

	bool empty() const
	{
		return elements == 0;
	}

	T &front()
	{
		assert(elements);
		return data[head];
	}

	bool full() const
	{
		return elements == capacity;
	}

	size_t pop(T *aBuffer, size_t aLength)
	{
		size_t num = 0;

		while (elements > 0 && num < aLength) {
			*aBuffer++ = pop();
			++num;
		}

		return num;
	}

	T pop()
	{
		assert(elements);
		return popImpl<capacity>();
	}

	size_t push(const T *aBuffer, size_t aLength)
	{
		size_t num = 0;

		while (elements < capacity && aLength--) {
			push(*aBuffer++);
			++num;
		}

		return num;
	}

	void push(T const &aValue)
	{
		assert(elements < capacity);
		return pushImpl<capacity>(aValue);
	}

	template<typename... Args>
	T &emplaceBack(Args &&...aArgs)
	{
		assert(elements < capacity);
		return emplaceBackImpl(std::forward<Args>(aArgs)...);
	}

	size_t size() const
	{
		return elements;
	}

	size_t vacant() const
	{
		return capacity - elements;
	}

	//!
	//! \brief deferredAppend Append data to the queue that has been filled asynchronously.
	//! Queue size and tail pointer are changed.
	//! \param aSize Size of the chunk that was filled asynchronously.
	//!
	void deferredAppend(size_t aSize)
	{
		assert(elements + aSize <= capacity);

		tail = static_cast<CapacityType>(tail + aSize);
		if (tail >= capacity) {
			tail = static_cast<CapacityType>(tail - capacity);
		}
		elements += aSize;
	}

	//!
	//! \brief deferredForget Remove data from the queue that has been extracted asynchronously.
	//! Queue size and head pointer are changed.
	//! \param aSize Size of the chunk that was extracted asynchronously.
	//!
	void deferredForget(size_t aSize)
	{
		assert(elements >= aSize);

		head = static_cast<CapacityType>(head + aSize);
		if (head >= capacity) {
			head = static_cast<CapacityType>(head - capacity);
		}
		elements -= aSize;
	}

	//!
	//! \brief deferredPop Calculate pointer and size of the chunk that can be extracted asynchronously.
	//! Offset is added to the calculated position, offset is needed to support multiple asynchronous extractions.
	//! All queue fields are left untouched.
	//! \param aBuffer Reference to a pointer to store a beginning of queue data, that can be extracted.
	//! \param aSize Size of the available chunk.
	//! \param aOffset Offset to the beginning of the calculated chunk.
	//!
	void deferredPop(const T *&aBuffer, size_t &aSize, size_t aOffset) const
	{
		assert(elements >= aOffset);

		auto tempHead = static_cast<CapacityType>(head + aOffset);
		if (tempHead >= capacity) {
			tempHead = static_cast<CapacityType>(tempHead - capacity);
		}

		aBuffer = &data[tempHead];

		if (elements > 0) {
			if (tempHead < tail) {
				aSize = elements;
			} else {
				aSize = capacity - tempHead;
			}
		} else {
			aSize = 0;
		}
	}

	//!
	//! \brief deferredPush Calculate pointer and size of the chunk that can be filled asynchronously.
	//! Offset is added to the calculated position, offset is needed to support multiple asynchronous pushes.
	//! All queue fields are left untouched.
	//! \param aBuffer Reference to a pointer to store a beginning of queue data, that can be filled.
	//! \param aSize Size of the available chunk.
	//! \param aOffset Offset from the beginning of the calculated chunk.
	//!
	void deferredPush(T *&aBuffer, size_t &aSize, size_t aOffset) const
	{
		assert(elements + aOffset <= capacity);

		auto tempTail = static_cast<CapacityType>(tail + aOffset);
		if (tempTail >= capacity) {
			tempTail = static_cast<CapacityType>(tempTail - capacity);
		}

		aBuffer = const_cast<T *>(&data[tempTail]);

		if (elements < capacity) {
			if (head > tempTail) {
				aSize = capacity - (elements + aOffset);
			} else {
				aSize = capacity - tempTail;
			}
		} else {
			aSize = 0;
		}
	}

protected:
	using CapacityType = typename QueueHelpers::RetType<capacity>::Type;

	T data[capacity];

	size_t elements{0}; //!< Current number of elements in the queue
	CapacityType head{0}; //!< Index of the first element
	CapacityType tail{0}; //!< Index of the last element

private:
	template<size_t C>
	std::enable_if_t<QueueHelpers::isTypeAligned<C>, void>
	pushImpl(const T &aValue)
	{
		data[tail++] = aValue;
		++elements;
	}

	template<size_t C>
	std::enable_if_t<!QueueHelpers::isTypeAligned<C> && QueueHelpers::isPowerOfTwo<C>, void>
	pushImpl(const T &aValue)
	{
		data[tail++] = aValue;
		tail &= C - 1;
		++elements;
	}

	template<size_t C>
	std::enable_if_t<!QueueHelpers::isTypeAligned<C> && !QueueHelpers::isPowerOfTwo<C>, void>
	pushImpl(const T &aValue)
	{
		data[tail++] = aValue;
		if (tail == capacity) {
			tail = 0;
		}
		++elements;
	}


	template<typename... Args, size_t C = capacity>
	std::enable_if_t<QueueHelpers::isTypeAligned<C>, T &>
	emplaceBackImpl(Args &&...aArgs)
	{
		++elements;
		new (&data[tail]) T{std::forward<Args>(aArgs)...};
		return data[tail++];
	}

	template<typename... Args, size_t C = capacity>
	std::enable_if_t<!QueueHelpers::isTypeAligned<C> && QueueHelpers::isPowerOfTwo<C>, T &>
	emplaceBackImpl(Args &&...aArgs)
	{
		new (&data[tail]) T{std::forward<Args>(aArgs)...};
		T &inserted = data[tail++];
		tail &= capacity - 1;
		++elements;

		return inserted;
	}

	template<typename... Args, size_t C = capacity>
	std::enable_if_t<!QueueHelpers::isTypeAligned<C> && !QueueHelpers::isPowerOfTwo<C>, T &>
	emplaceBackImpl(Args &&...aArgs)
	{
		new (&data[tail]) T{std::forward<Args>(aArgs)...};
		T &inserted = data[tail++];

		if (tail == capacity) {
			tail = 0;
		}
		++elements;

		return inserted;
	}

	template<size_t C>
	std::enable_if_t<QueueHelpers::isTypeAligned<C>, T>
	popImpl()
	{
		const T tmp = data[head++];
		--elements;
		return tmp;
	}

	template<size_t C>
	std::enable_if_t<!QueueHelpers::isTypeAligned<C> && QueueHelpers::isPowerOfTwo<C>, T>
	popImpl()
	{
		const T tmp = data[head++];
		head &= C - 1;
		--elements;
		return tmp;
	}

	template<size_t C>
	std::enable_if_t<!QueueHelpers::isTypeAligned<C> && !QueueHelpers::isPowerOfTwo<C>, T>
	popImpl()
	{
		const T tmp = data[head++];
		if (head == capacity) {
			head = 0;
		}
		--elements;
		return tmp;
	}
};

template<typename T, size_t capacity> constexpr size_t Queue<T, capacity>::kCapacity;

#endif // DRONEDEVICE_QUEUE_HPP_
