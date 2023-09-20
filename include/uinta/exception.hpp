#ifndef UINTA_EXCEPTION_HPP
#define UINTA_EXCEPTION_HPP

#include <exception>
#include <string>
#include <uinta/fwd.hpp>

namespace uinta {

/**
 * @class UintaException
 * @brief A custom exception class that extends the std::exception class.
 *
 * UintaException class is used to handle exceptions in a custom manner, by incorporating error codes and their respective
 * categories in the exception messages.
 */
class UintaException : public std::exception {
 public:
  /**
   * @brief Formats the error message string.
   *
   * This static member function takes an error message and an error code, and formats them into a single string with the format
   * "[error_category] error_message".
   *
   * @param message The error message as a std::string.
   * @param ec The error code of type uinta_error_code.
   * @return A formatted error message string.
   */
  static std::string format_message(const std::string& message, const uinta_error_code& ec) {
    return std::string("[") + ec.category().name() + ("] " + ec.message());
  }

  /**
   * @brief Constructor that takes an error code as parameter.
   *
   * This constructor takes a uinta_error_code object, uses it to format the error message, and initializes the UintaException
   * object with it.
   *
   * @param ec The error code of type uinta_error_code used to initialize the UintaException object.
   */
  UintaException(const uinta_error_code& ec) : m_error_code(ec), m_what(format_message(ec.message(), ec)) {
  }

  /**
   * @brief Returns the formatted error message.
   *
   * Overrides the what() function of the std::exception class to return the formatted error message stored in the UintaException
   * object.
   *
   * @return A pointer to the formatted error message string.
   */
  const char* what() const noexcept override {
    return m_what.c_str();
  }

  /**
   * @brief Retrieves the error code object.
   *
   * Provides access to the error code object stored in the UintaException object.
   *
   * @return A constant reference to the error code object.
   */
  const uinta_error_code& error_code() const {
    return m_error_code;
  }

 private:
  /**
   * @brief Holds the error code object passed to the constructor.
   */
  const uinta_error_code m_error_code;

  /**
   * @brief Stores the formatted error message string created in the constructor.
   */
  const std::string m_what;
};

}  // namespace uinta

#endif  // UINTA_EXCEPTION_HPP
