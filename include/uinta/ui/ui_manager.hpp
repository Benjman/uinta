#ifndef UINTA_UI_MANAGER_HPP
#define UINTA_UI_MANAGER_HPP

#include <spdlog/fwd.h>

#include <memory>
#include <span>
#include <uinta/runner_lifecycle.hpp>
#include <uinta/ui/ui_renderer.hpp>
#include <vector>

namespace uinta {

class UiManager : public RunnerLifecycle {
 public:
  UiManager(Scene& scene, std::unique_ptr<UiRenderer> renderer = nullptr) noexcept;

  ~UiManager();

  uinta_error_code init() override;

  void render(const RunnerState& state) override;

  void tick(const RunnerState& state, const InputState& input) override;

  void add_element(std::unique_ptr<UiElement> element) noexcept;

  std::span<const std::unique_ptr<UiElement>> elements() const noexcept;

 private:
  std::vector<std::unique_ptr<UiElement>> m_elements;
  std::shared_ptr<spdlog::logger> m_logger;
  std::unique_ptr<UiRenderer> m_renderer;
  Scene& m_scene;
};

}  // namespace uinta

#endif  // UINTA_UI_MANAGER_HPP
