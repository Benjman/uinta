#include "uinta/lib/stb_image_api.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

namespace uinta {

void StbImageApiImpl::setFlipVerticallyOnLoad(
    bool flag_true_if_should_flip) const noexcept {
  stbi_set_flip_vertically_on_load(flag_true_if_should_flip);
}

u8 *StbImageApiImpl::load(char const *filename, i32 *x, i32 *y,
                          i32 *channels_in_file,
                          i32 desired_channels) const noexcept {
  return stbi_load(filename, x, y, channels_in_file, desired_channels);
}

void StbImageApiImpl::imageFree(void *retval_from_stbi_load) const noexcept {
  stbi_image_free(retval_from_stbi_load);
}

}  // namespace uinta
