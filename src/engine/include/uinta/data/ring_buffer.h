#ifndef SRC_ENGINE_INCLUDE_UINTA_DATA_RING_BUFFER_H_
#define SRC_ENGINE_INCLUDE_UINTA_DATA_RING_BUFFER_H_

#include <array>
#include <cstddef>
#include <iterator>

namespace uinta {

template <typename T, size_t N>
class RingBuffer {
 public:
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;

  void push(const value_type& value) noexcept {
    buffer_[head_] = value;
    head_ = (head_ + 1) % N;
    if (full_) {
      tail_ = (tail_ + 1) % N;
    }
    full_ = head_ == tail_;
  }

  [[nodiscard]] size_t size() const noexcept {
    if (full_) {
      return N;
    }
    if (head_ >= tail_) {
      return head_ - tail_;
    }
    return N + head_ - tail_;
  }

  [[nodiscard]] constexpr size_t capacity() const noexcept { return N; }

  [[nodiscard]] bool isEmpty() const noexcept {
    return (!full_ && (head_ == tail_));
  }

  [[nodiscard]] bool isFull() const noexcept { return full_; }

  struct iterator final {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = const value_type*;
    using reference = const value_type&;

    iterator() noexcept : buf_(nullptr), pos_(0), done_(true) {}

    iterator(const RingBuffer& buf, size_t pos, bool done = false)
        : buf_(&buf), pos_(pos), done_(done) {}

    reference operator*() const { return buf_->buffer_[pos_]; }
    pointer operator->() const { return &buf_->buffer_[pos_]; }

    iterator& operator++() noexcept {
      pos_ = (pos_ + 1) % N;
      if (pos_ == buf_->head_) {
        done_ = true;
      }
      return *this;
    }

    iterator operator++(int) noexcept {
      iterator temp = *this;
      ++(*this);
      return temp;
    }

    friend bool operator==(const iterator& a, const iterator& b) noexcept {
      return a.pos_ == b.pos_ && a.done_ == b.done_;
    }

    friend bool operator!=(const iterator& a, const iterator& b) noexcept {
      return !(a == b);
    }

   private:
    const RingBuffer* buf_;
    size_t pos_;
    bool done_;
  };

  [[nodiscard]] iterator begin() const noexcept {
    if (isEmpty()) {
      return end();
    }
    return {*this, tail_};
  }

  [[nodiscard]] iterator end() const noexcept { return {*this, head_, true}; }

 private:
  std::array<value_type, N> buffer_;
  size_t head_ = 0;
  size_t tail_ = 0;
  bool full_ = false;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DATA_RING_BUFFER_H_
