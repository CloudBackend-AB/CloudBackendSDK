#ifndef CBE__util__Exception_h__
#define CBE__util__Exception_h__

#include <stdexcept>
#include <string>
#include <type_traits>
namespace cbe {
  namespace util {

/**
 * X
 */
struct Exception : std::runtime_error {
  template <typename... Ts>
  Exception(Ts&&... args);
  std::string typeAsString() const;

  /**
   * @brief Examines whether current exception is of type \p CbeExceptionT 
   *         
   * @tparam CbeExceptionT Subtype of cbe::util::Exception to be examined.
   * @return Pointer to the requested exception, nullptr implies no such 
   *         subclass type.
   */
  template <class CbeExceptionT>
  const CbeExceptionT* derivedCbeException() const {
    static_assert(std::is_base_of<cbe::util::Exception, CbeExceptionT>::value, 
                  "The provided exception type, CbeExceptionT, "
                  "is not a subclass of cbe::util::Exception");
    return dynamic_cast<const CbeExceptionT*>(this);
  }
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
