#include <spdlog/sinks/stdout_color_sinks.h>
#include <uinta/types.h>

#include <uinta/error.hpp>
#include <uinta/exception.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/font.hpp>
#include <uinta/logging.hpp>

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb/stb_rect_pack.h>

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

namespace uinta {

enum class error : error_t {
  FontNotLoaded = 10,
  InvalidChar = 11,
  InvalidFontPath = 12,
  InvalidLineSize = 13,
  PackBegin = 14,
  PackFontRange = 15,
};
static const std::map<uinta_error_code_t, std::string> errorMessages = {
    {static_cast<uinta_error_code_t>(error::FontNotLoaded), "Font not loaded."},
    {static_cast<uinta_error_code_t>(error::InvalidChar), "Invalid char."},
    {static_cast<uinta_error_code_t>(error::InvalidFontPath), "Invalid font path."},
    {static_cast<uinta_error_code_t>(error::InvalidLineSize), "Line size must have a value."},
    {static_cast<uinta_error_code_t>(error::PackBegin), "Failed `stbtt_PackBegin` call."},
    {static_cast<uinta_error_code_t>(error::PackFontRange), "Failed `stbtt_PackFontRange` call."},
};

UINTA_ERROR_FRAMEWORK(Grid, errorMessages);

inline u32 renderable_char_count(const std::string& input);

uinta_error_code Font::load(FileManager& file_manager, spdlog::logger* logger) noexcept {
  if (m_path.empty() || !m_path.ends_with(".ttf")) {
    if (logger) SPDLOG_LOGGER_WARN(logger, "{}: '{}'", errorMessages.at(static_cast<error_t>(error::InvalidFontPath)), m_path);
    return make_error(error::InvalidFontPath);
  }

  if (!m_line_size) return make_error(error::InvalidLineSize);

  const auto ttf_file = file_manager.registerFile(m_path);
  file_manager.loadFile(ttf_file);
  const auto ttf_buffer = file_manager.getDataString(ttf_file);
  u8 u8_buffer[file_manager.getSize(ttf_file)];
  std::copy(std::begin(ttf_buffer), std::end(ttf_buffer), u8_buffer);
  file_manager.releaseFile(ttf_file);

  constexpr void* AllocContext = nullptr;
  constexpr i32 FontIndex = 0;
  constexpr i32 Padding = 1;
  constexpr i32 Stride = 0;

  stbtt_pack_context ctx;
  if (!stbtt_PackBegin(&ctx, m_bitmap.data(), sqrt(m_bitmap.size()), sqrt(m_bitmap.size()), Stride, Padding, AllocContext))
    return make_error(error::PackBegin);
  if (!stbtt_PackFontRange(&ctx, u8_buffer, FontIndex, m_line_size, FirstChar, CharCount, m_chars.data()))
    return make_error(error::PackFontRange);
  stbtt_PackEnd(&ctx);

  // get font metrics
  stbtt_fontinfo info;
  stbtt_InitFont(&info, u8_buffer, 0);
  i32 ascent, descent, line_gap;
  stbtt_GetFontVMetrics(&info, &ascent, &descent, &line_gap);
  m_scale = stbtt_ScaleForPixelHeight(&info, m_line_size);
  m_ascent = ascent * m_scale;
  m_descent = descent * m_scale;
  m_line_gap = line_gap * m_scale;

  if (logger) SPDLOG_LOGGER_INFO(logger, "Loaded '{}' into {}x{} bitmap.", m_path, sqrt(m_bitmap.size()), sqrt(m_bitmap.size()));

  if (true) stbi_write_png("/tmp/test.png", sqrt(m_bitmap.size()), sqrt(m_bitmap.size()), 1, m_bitmap.data(), 0);

  return SUCCESS_EC;
}

stbtt_aligned_quad Font::quad_info(char c, f32* x_pos, f32* y_pos) const {
  if (!loaded()) {
    SPDLOG_ERROR("Font '{}' not loaded.", m_path);
    throw UintaException(make_error(error::FontNotLoaded));
  }
  if ((c - Font::FirstChar) >= m_chars.size() || (c - Font::FirstChar) < 0) {
    SPDLOG_ERROR("Prevented out of bounds read attempt for quads collection for char '{}' in font '{}'.", c, m_path);
    throw UintaException(make_error(error::InvalidChar));
  }
  constexpr i32 AlignToInt = 0;
  stbtt_aligned_quad quad;
  stbtt_GetPackedQuad(m_chars.data(), sqrt(m_bitmap.size()), sqrt(m_bitmap.size()), c - Font::FirstChar, x_pos, y_pos, &quad,
                      AlignToInt);
  return quad;
}

}  // namespace uinta
