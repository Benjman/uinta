#ifndef UINTA_ERROR_HPP
#define UINTA_ERROR_HPP

#include <uinta/types.h>

#include <map>
#include <string>
#include <system_error>

namespace uinta {

using uinta_error_code = std::error_code;  // Alias in case of future plans to change this to a bitwise handle. If this is here
                                           // for a while without being changed, delete this comment. Comment made 2023-AUG-31
using uinta_error_code_t = i32;

class uinta_error_category_t : public std::error_category {
 public:
  static constexpr uinta_error_code_t SUCCESS = 0;

  uinta_error_category_t(const std::string& name, const std::map<uinta_error_code_t, std::string>& error_messages = {})
      : m_messages(error_messages), m_name(name.c_str()) {
  }

  virtual const char* name() const noexcept override {
    return m_name.c_str();
  }

  virtual std::string message(uinta_error_code_t ev) const noexcept override {
    // TODO: Formattable error messages
    const auto it = m_messages.find(ev);
    return it == m_messages.end() ? "Unknown error" : it->second;
  }

 protected:
  const std::map<uinta_error_code_t, std::string> m_messages;
  const std::string m_name;
} static const uinta_error_category("uinta");

static uinta_error_code make_error(uinta_error_code_t e, const uinta_error_category_t& c = uinta_error_category) {
  return {e, c};
}

static const uinta_error_code SUCCESS_EC = make_error(uinta_error_category_t::SUCCESS);

#define UINTA_ERROR_FRAMEWORK(NAME, MESSAGES)                                         \
  class error_category_##NAME##_t : public uinta_error_category_t {                   \
   public:                                                                            \
    error_category_##NAME##_t() : uinta_error_category_t(#NAME, MESSAGES) {           \
    }                                                                                 \
  } static const error_##NAME##_category;                                             \
  namespace error_##NAME##_ns {                                                       \
    uinta_error_code make_error(error e) {                                            \
      return make_error(static_cast<uinta_error_code_t>(e), error_##NAME##_category); \
    }                                                                                 \
  }                                                                                   \
  using namespace error_##NAME##_ns;

}  // namespace uinta

#endif  // UINTA_ERROR_HPP
