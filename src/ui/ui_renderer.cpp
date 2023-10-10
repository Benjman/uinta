#include <uinta/logging.hpp>
#include <uinta/ui/ui_renderer.hpp>

namespace uinta {

UiRenderer::~UiRenderer() {
  if (m_logger) spdlog::drop(m_logger->name());
}

}  // namespace uinta
