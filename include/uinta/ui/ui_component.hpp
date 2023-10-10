#ifndef UINTA_UI_COMPONENT_HPP
#define UINTA_UI_COMPONENT_HPP

#include <uinta/fwd.hpp>

namespace uinta {

struct UiComponent {
  enum class Status : u8 {
    Running,
    Paused,
    Completed,
  };

  UiComponent(UiElement& element) : m_element(element), m_status(Status::Running) {
  }

  virtual void tick(f32 delta) = 0;

  const UiElement& element() const noexcept {
    return m_element;
  }

  Status status() const noexcept {
    return m_status;
  }

 protected:
  UiElement& m_element;

  void status(Status status) noexcept {
    m_status = status;
  }

 private:
  Status m_status;
};

inline std::string to_string(UiComponent::Status status) {
  switch (status) {
    case (UiComponent::Status::Running):
      return "Running";
    case (UiComponent::Status::Paused):
      return "Paused";
    case (UiComponent::Status::Completed):
      return "Completed";
    default:
      return "Unknown";
  };
}

}  // namespace uinta

#endif  // UINTA_UI_COMPONENT_HPP
