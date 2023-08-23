#ifndef UINTA_GLFW_UI_H
#define UINTA_GLFW_UI_H
#include <uinta/types.h>
namespace uinta {
constexpr struct {
  f32 min = -INFINITY;
  f32 zero = 0;
  f32 one = 1.0;
  f32 one_tenth = 0.1;
  f32 twenty = 20;
  f32 threeSixty = 360;
  f32 max = INFINITY;
} limits;
}  // namespace uinta
#endif /* ifndef UINTA_GLFW_UI_H */
