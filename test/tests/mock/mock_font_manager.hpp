#ifndef UINTA_MOCK_FONT_MANAGER_HPP
#define UINTA_MOCK_FONT_MANAGER_HPP

#include <functional>
#include <uinta/error.hpp>
#include <uinta/font.hpp>

namespace uinta {

class MockFontRenderer : public FontRenderer {
 public:
  std::function<uinta_error_code()> on_init;
  uinta_error_code init(FileManager& fileManager, spdlog::logger* logger) noexcept override {
    if (on_init) return on_init();
    return SUCCESS_EC;
  }

  std::function<void()> on_start;
  void start() noexcept override {
    if (on_start) on_start();
  }

  std::function<RenderState()> on_render;
  RenderState render() noexcept override {
    if (on_render) return on_render();
    return 0;
  }

  std::function<void(std::span<u8>)> on_upload_font_texture;
  void upload_font_texture(std::span<u8> bitmap) override {
    if (on_upload_font_texture) on_upload_font_texture(bitmap);
  }
};

class MockFontManager : public FontManager {
 public:
  MockFontManager(Runner& runner, std::string ttf_path = "test.ttf")
      : FontManager(runner, ttf_path, std::make_unique<MockFontRenderer>()) {
  }
};

}  // namespace uinta

#endif  // UINTA_MOCK_FONT_MANAGER_HPP
