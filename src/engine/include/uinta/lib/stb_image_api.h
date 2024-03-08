#ifndef SRC_ENGINE_INCLUDE_UINTA_LIB_STB_IMAGE_API_H_
#define SRC_ENGINE_INCLUDE_UINTA_LIB_STB_IMAGE_API_H_

#include "uinta/types.h"

namespace uinta {

class StbImageApi {
 public:
  virtual void setFlipVerticallyOnLoad(bool) const noexcept = 0;
  virtual u8 *load(char const *, i32 *, i32 *, i32 *, i32) const noexcept = 0;
  virtual void imageFree(void *) const noexcept = 0;
};

class StbImageApiImpl : public StbImageApi {
 public:
  static const StbImageApi *Instance() noexcept {
    static StbImageApiImpl instance;
    return &instance;
  }

  ~StbImageApiImpl() noexcept = default;

  StbImageApiImpl(const StbImageApiImpl &) noexcept = delete;
  StbImageApiImpl &operator=(const StbImageApiImpl &) noexcept = delete;
  StbImageApiImpl(StbImageApiImpl &&) noexcept = delete;
  StbImageApiImpl &operator=(StbImageApiImpl &&) noexcept = delete;

  void setFlipVerticallyOnLoad(bool) const noexcept override;
  u8 *load(char const *, i32 *, i32 *, i32 *, i32) const noexcept override;
  void imageFree(void *) const noexcept override;

 private:
  StbImageApiImpl() noexcept = default;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LIB_STB_IMAGE_API_H_
