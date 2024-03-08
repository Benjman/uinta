#ifndef SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_STB_IMAGE_API_H_
#define SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_STB_IMAGE_API_H_

#include <functional>

#include "uinta/lib/stb_image_api.h"

namespace uinta {

struct MockImageApi : StbImageApi {
  std::function<void(bool)> onSetFlipVerticallyOnLoad = [](bool) {};
  void setFlipVerticallyOnLoad(bool flag_true_if_should_flip) const noexcept {
    return onSetFlipVerticallyOnLoad(flag_true_if_should_flip);
  }

  std::function<u8 *(char const *, i32 *, i32 *, i32 *, i32)> onLoad =
      [](char const *, i32 *, i32 *, i32 *, i32) -> u8 * { return nullptr; };
  u8 *load(char const *file, i32 *x, i32 *y, i32 *channels_in_file,
           i32 desired_channels) const noexcept {
    return onLoad(file, x, y, channels_in_file, desired_channels);
  }

  std::function<void(bool)> onImageFree = [](bool) {};
  void imageFree(void *retval_fron_stbi_load) const noexcept {
    return onImageFree(retval_fron_stbi_load);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_STB_IMAGE_API_H_
