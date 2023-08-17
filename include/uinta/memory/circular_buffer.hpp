#ifndef UINTA_CIRCULAR_BUFFER_HPP
#define UINTA_CIRCULAR_BUFFER_HPP

#include <uinta/types.h>

#include <array>
#include <stdexcept>

namespace uinta {

template <std::size_t _size>
class CircularBuffer {
 public:
  class iterator {
   public:
    iterator(CircularBuffer& buf, std::size_t index) : buf(buf), index(index % _size) {
    }

    f32& operator*() {
      return buf.buffer[index];
    }

    iterator& operator++() {
      index = (index + 1) % _size;
      return *this;
    }

    bool operator!=(const iterator& other) {
      return index != other.index;
    }

   private:
    CircularBuffer& buf;
    std::size_t index;
  };

  bool isFull() const {
    return size == _size;
  }

  bool isEmpty() const {
    return size == 0;
  }

  void push(f32 value) {
    if (isFull())
      head = (head + 1) % _size;
    else
      ++size;
    buffer[tail] = value;
    tail = (tail + 1) % _size;
  }

  f32 pop() {
    if (isEmpty()) throw std::out_of_range("Buffer is empty");
    f32 value = buffer[head];
    head = (head + 1) % _size;
    --size;
    return value;
  }

  std::size_t getSize() const {
    return size;
  }

  iterator begin() {
    return iterator(*this, head);
  }
  iterator end() {
    return iterator(*this, tail);
  }

 private:
  std::array<f32, _size> buffer;
  std::size_t head = 0;
  std::size_t tail = 0;
  std::size_t size = 0;
};

}  // namespace uinta

#endif /* ifndef UINTA_CIRCULAR_BUFFER_HPP */
