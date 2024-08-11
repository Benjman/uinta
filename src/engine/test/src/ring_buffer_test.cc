#include "uinta/data/ring_buffer.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <numeric>

#include "uinta/types.h"

namespace uinta {

TEST(RingBuffer, EmptyBufferIncrementFailure) {
  RingBuffer<i32, 1> buffer;
  auto begin = buffer.begin();
  auto end = buffer.end();
  ASSERT_EQ(begin, end) << "Begin and End should be equal for an empty buffer.";
  std::for_each(begin, end, [&](auto) {
    FAIL() << "Iterator shouldn't iterate on an empty buffer.";
  });
}

TEST(RingBuffer, SingleElement) {
  bool iteratorCalled = false;
  std::vector<i32> vec;
  vec.push_back(1);
  std::for_each(vec.begin(), vec.end(), [&](auto) { iteratorCalled = true; });
  RingBuffer<i32, 1> buffer;
  buffer.push(2);
  auto begin = buffer.begin();
  auto end = buffer.end();
  std::for_each(begin, end, [&](auto) { iteratorCalled = true; });
  ASSERT_TRUE(iteratorCalled);
}

TEST(RingBuffer, MultipleElement) {
  constexpr size_t elementCount = 3;
  std::array<i32, elementCount> elements = {1, 3, 7};
  RingBuffer<i32, elementCount> buffer;
  buffer.push(elements.at(0));
  buffer.push(elements.at(1));
  buffer.push(elements.at(2));
  i32 iteratorCallCount = 0;
  auto begin = buffer.begin();
  auto end = buffer.end();
  std::for_each(begin, end, [&](auto v) {
    ASSERT_EQ(elements.at(iteratorCallCount), v);
    iteratorCallCount++;
  });
  ASSERT_EQ(elementCount, iteratorCallCount);
}

TEST(RingBuffer, MultipleElementOverflow) {
  std::array<i32, 4> elements = {1, 3, 7, 11};
  RingBuffer<i32, 3> buffer;

  ASSERT_EQ(0, buffer.size()) << "Initial buffer size should be 0.";

  buffer.push(elements.at(0));
  ASSERT_EQ(1, buffer.size()) << "Buffer size should be 1 after first push.";
  ASSERT_EQ(elements.at(0), *std::next(buffer.begin(), 0))
      << "First element should be " << elements.at(0);

  buffer.push(elements.at(1));
  ASSERT_EQ(2, buffer.size()) << "Buffer size should be 2 after second push.";
  ASSERT_EQ(elements.at(0), *std::next(buffer.begin(), 0))
      << "First element should still be " << elements.at(0);
  ASSERT_EQ(elements.at(1), *std::next(buffer.begin(), 1))
      << "Second element should be " << elements.at(1);

  buffer.push(elements.at(2));
  ASSERT_EQ(3, buffer.size()) << "Buffer size should be 3 after third push.";
  ASSERT_EQ(elements.at(0), *std::next(buffer.begin(), 0))
      << "First element should still be " << elements.at(0);
  ASSERT_EQ(elements.at(1), *std::next(buffer.begin(), 1))
      << "Second element should still be " << elements.at(1);
  ASSERT_EQ(elements.at(2), *std::next(buffer.begin(), 2))
      << "Third element should be " << elements.at(2);
  ASSERT_EQ(elements.at(0) + elements.at(1) + elements.at(2),
            std::accumulate(buffer.begin(), buffer.end(), 0))
      << "Sum of buffer elements should be "
      << (elements.at(0) + elements.at(1) + elements.at(2));

  buffer.push(elements.at(3));
  ASSERT_EQ(3, buffer.size())
      << "Buffer size should still be 3 after overflow push.";
  ASSERT_EQ(elements.at(1), *std::next(buffer.begin(), 0))
      << "First element should now be " << elements.at(1) << " after overflow.";
  ASSERT_EQ(elements.at(2), *std::next(buffer.begin(), 1))
      << "Second element should now be " << elements.at(2)
      << " after overflow.";
  ASSERT_EQ(elements.at(3), *std::next(buffer.begin(), 2))
      << "Third element should now be " << elements.at(3) << " after overflow.";
  ASSERT_NE(buffer.begin(), buffer.end()) << "Buffer should not be empty.";
  ASSERT_EQ(elements.at(1) + elements.at(2) + elements.at(3),
            std::accumulate(buffer.begin(), buffer.end(), 0))
      << "Sum of buffer elements after overflow should be "
      << (elements.at(1) + elements.at(2) + elements.at(3));

  i32 iteratorCallCount = 0;
  auto begin = buffer.begin();
  auto end = buffer.end();
  std::for_each(begin, end, [&](auto) { iteratorCallCount++; });
  ASSERT_GT(iteratorCallCount, 0) << "Iterator should iterate over elements.";
}

}  // namespace uinta
