//
// Main.cpp
//
//  Created on: Jul 18, 2023
//      Author: Alexander
//

#include <DroneDevice/Queue.hpp>
#include "gtest/gtest.h"
#include <cstdlib>

struct TestStruct
{
	int64_t x;
	int32_t y;
	int8_t z;
} __attribute__((packed));

static bool compareElements(const TestStruct *a, const TestStruct *b)
{
	return memcmp(a, b, sizeof(TestStruct)) == 0;
}

static TestStruct createElement(int number)
{
	return {
		(int64_t)((number & 1) ? number : -number),
		(int32_t)((number & 1) ? -number : number),
		(int8_t)number
	};
}

template<class Container>
static void runPushPop(Container *aQueue, int aCount)
{
	// Fill the queue with push back function

	for (int i = 0; i < aCount; ++i) {
		const TestStruct element = createElement(i);
		aQueue->push(element);
	}

	ASSERT_EQ(aQueue->size(), aCount);
	ASSERT_EQ(aQueue->empty(), (aCount == 0));
	ASSERT_EQ(aQueue->full(), (aCount == Container::kCapacity));
	ASSERT_EQ(aQueue->vacant(), (Container::kCapacity - aCount));

	// Pop elements from the queue while checking their values

	for (int i = 0; i < aCount; ++i) {
		const TestStruct refElement = createElement(i);
		const TestStruct element = aQueue->front();
		aQueue->pop();

		ASSERT_EQ(compareElements(&element, &refElement), true);
	}

	ASSERT_EQ(aQueue->size(), 0);
	ASSERT_EQ(aQueue->empty(), true);
	ASSERT_EQ(aQueue->full(), false);
	ASSERT_EQ(aQueue->vacant(), Container::kCapacity);
}

template<class Container>
static void runEmplaceBackPop(Container *aQueue, int aCount)
{
	// Fill the queue with emplace back function
	for (int i = 0; i < aCount; ++i) {
		const TestStruct element = createElement(i);
		auto &inserted = aQueue->emplaceBack(element.x, element.y, element.z);
		ASSERT_EQ(compareElements(&element, &inserted), true);
	}

	ASSERT_EQ(aQueue->size(), aCount);
	ASSERT_EQ(aQueue->empty(), (aCount == 0));
	ASSERT_EQ(aQueue->full(), (aCount == Container::kCapacity));
	ASSERT_EQ(aQueue->vacant(), (Container::kCapacity - aCount));

	// Pop elements from the queue while checking their values
	for (int i = 0; i < aCount; ++i) {
		const TestStruct refElement = createElement(i);
		const TestStruct element = aQueue->front();
		aQueue->pop();

		ASSERT_EQ(compareElements(&element, &refElement), true);
	}

	ASSERT_EQ(aQueue->size(), 0);
	ASSERT_EQ(aQueue->empty(), true);
	ASSERT_EQ(aQueue->full(), false);
	ASSERT_EQ(aQueue->vacant(), Container::kCapacity);
}

TEST(Queue, AlignedSize)
{
	Queue<TestStruct, 256> queue;

	for (int iter = 1; iter < static_cast<int>(queue.kCapacity * 2); ++iter)
	{
		const int count = abs(iter - static_cast<int>(queue.kCapacity));
		runPushPop(&queue, count);
		runEmplaceBackPop(&queue, count);
	}
}

TEST(Queue, PowerOfTwoSize)
{
	Queue<TestStruct, 64> queue;

	for (int iter = 1; iter < static_cast<int>(queue.kCapacity * 2); ++iter)
	{
		const int count = abs(iter - static_cast<int>(queue.kCapacity));
		runPushPop(&queue, count);
		runEmplaceBackPop(&queue, count);
	}
}

TEST(Queue, UnalignedSize)
{
	Queue<TestStruct, 17> queue;

	for (int iter = 1; iter < static_cast<int>(queue.kCapacity * 2); ++iter)
	{
		const int count = abs(iter - static_cast<int>(queue.kCapacity));
		runPushPop(&queue, count);
		runEmplaceBackPop(&queue, count);
	}
}
