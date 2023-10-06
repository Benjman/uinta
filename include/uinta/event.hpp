#ifndef UINTA_EVENT_HPP
#define UINTA_EVENT_HPP

#include <uinta/types.h>

#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <uinta/fwd.hpp>
#include <unordered_map>

namespace uinta {

struct Event {
  Event(f64 at) : at(at) {
  }

  virtual ~Event() = default;

  const f64 at;
};

class EventManager {
 public:
  template <typename EventType>
  const subscription_t subscribe(EventType event_type, std::function<void(const Event*)> callback) noexcept {
    return subscribe(static_cast<event_t>(event_type), std::move(callback));
  }

  const subscription_t subscribe(event_t event_type, std::function<void(const Event*)> callback) noexcept {
    std::lock_guard<std::mutex> lock(m_mtx);
    m_event_subscribers[event_type][++m_subscriber_index] = callback;
    return m_subscriber_index;
  }

  template <typename EventType>
  void unsubscribe(EventType event_type, subscription_t subscription) noexcept {
    unsubscribe(static_cast<event_t>(event_type), subscription);
  }

  void unsubscribe(event_t event_type, subscription_t subscription) noexcept {
    std::lock_guard<std::mutex> lock(m_mtx);
    if (m_event_subscribers.find(static_cast<event_t>(event_type)) != m_event_subscribers.end()) {
      m_event_subscribers[static_cast<event_t>(event_type)].erase(subscription);
    }
  }

  virtual void add_event(event_t event_type, std::unique_ptr<const Event> event) noexcept = 0;

 protected:
  template <typename EventType>
  void queue_event(EventType event_type, std::unique_ptr<const Event> event) noexcept {
    queue_event(static_cast<event_t>(event_type), std::move(event));
  }

  void queue_event(event_t event_type, std::unique_ptr<const Event> event) noexcept {
    std::lock_guard<std::mutex> lock(m_mtx);
    m_queue[event_type].push(std::move(event));
  }

  template <typename EventType>
  void publish(EventType event_type, const Event& event) noexcept {
    publish(static_cast<event_t>(event_type), std::move(event));
  }

  void publish(event_t event_type, const Event& event) noexcept {
    if (m_event_subscribers.find(static_cast<event_t>(event_type)) != m_event_subscribers.end()) {
      for (const auto& subscriber : m_event_subscribers[static_cast<event_t>(event_type)]) {
        if (subscriber.second) subscriber.second(&event);
      }
    }
  }

  void publish_queued_events() noexcept {
    for (auto& queue : m_queue) {
      while (!queue.second.empty()) {
        publish(queue.first, *queue.second.front());
        queue.second.pop();
      }
    }
  }

 private:
  std::unordered_map<event_t, std::unordered_map<subscription_t, std::function<void(const Event*)>>> m_event_subscribers;
  std::unordered_map<event_t, std::queue<std::unique_ptr<const Event>>> m_queue;
  std::mutex m_mtx;
  u64 m_subscriber_index = 0;
};

}  // namespace uinta

#endif  // UINTA_EVENT_HPP
