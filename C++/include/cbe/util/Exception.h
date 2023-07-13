#ifndef CBE__util__Exception_h__
#define CBE__util__Exception_h__

#include <stdexcept>
#include <string>
namespace cbe {
  namespace util {


/**
 * X
 */
struct Exception : std::runtime_error {
  template <typename... Ts>
  Exception(Ts&&... args);
  std::string typeAsString() const;
};

/**
 * XX
 */
template <class ErrorInfoT>
struct ExceptionImpl : Exception {
  ExceptionImpl(ErrorInfoT&& errorInfo);

  using Base = ExceptionImpl<ErrorInfoT>;

  const ErrorInfoT       errorInfo;
};

  } // namespace util
} // namespace cbe


#endif // #ifndef CBE__util__Exception_h__
