#include <uinta/logging.hpp>
#include <uinta/math/perlin.hpp>

namespace uinta {

void perlinNoise(Buffer2d<f32>& buffer, const siv::PerlinNoise& perlin, f32 frequency, i32 octaves, const glm::ivec2& offset) {
  const auto fx = frequency / buffer.size.x;
  const auto fy = frequency / buffer.size.y;
  for (auto y = 0; y < buffer.size.y; ++y)
    for (auto x = 0; x < buffer.size.x; ++x)
      buffer[y * buffer.size.x + x] = perlin.octave2D_01(((x + offset.x) * fx), ((y + offset.y) * fy), octaves);
  SPDLOG_INFO("Noise generated with freq {}\toct {}\tbuffer.size ({} {})\toffset ({} {})", frequency, octaves, buffer.size.x,
              buffer.size.y, offset.x, offset.y);
}

}  // namespace uinta
