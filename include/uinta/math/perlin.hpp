#ifndef UINTA_MATH_PERLIN_HPP
#define UINTA_MATH_PERLIN_HPP

#include <uinta/types.h>

#include <PerlinNoise/PerlinNoise.hpp>
#include <glm/vec2.hpp>
#include <uinta/utils/buffer2d.hpp>

namespace uinta {

/**
 * @brief Generates a 2D buffer of coherent noise values ranging from [0, 1].
 *
 * @param buffer The buffer to fill with coherent noise values.
 * @param perlin The Perlin noise library to query for data.
 * @param frequency The period at which data is sampled.
 * @param octaves The successive layers of noise functions that are combined to create more complex, finer-grained patterns
 * @param offset The offset to apply to the Perlin noise sampling.
 */
inline void perlinNoise(Buffer2d<f32>& buffer, const siv::PerlinNoise& perlin, f32 frequency, i32 octaves,
                        const glm::ivec2& offset = {}) {
  const auto fx = frequency / buffer.size.x;
  const auto fy = frequency / buffer.size.y;
  for (auto y = 0; y < buffer.size.y; ++y)
    for (auto x = 0; x < buffer.size.x; ++x)
      buffer[y * buffer.size.x + x] = perlin.octave2D_01(((x + offset.x) * fx), ((y + offset.y) * fy), octaves);
}
}  // namespace uinta

#endif /* ifndef UINTA_MATH_PERLIN_HPP */
