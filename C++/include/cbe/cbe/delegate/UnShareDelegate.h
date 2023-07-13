#ifndef CBE__delegate__UnShareDelegate_h__
#define CBE__delegate__UnShareDelegate_h__

#include "cbe/Types.h"
#include "cbe/delegate/Error.h"
#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

namespace cbe {
  namespace delegate {  
class UnShareSuccess;
/**
 * Delegate class for the asynchronous version of methods:
 * <ul>
 *   <li> cbe::Container::unShare()
 *   <li> cbe::Object::unShare()
 * </ul>
 */
class UnShareDelegate {
public:
  using Success = UnShareSuccess;
  /**
   * Called upon successful UnShare.
   * @param message Message of the Unshare.
   */
  virtual void onUnShareSuccess(std::string&& message) = 0;

  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onUnShareError(Error&& error, cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed UnShare.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * <ul>
   * <li> @ref cbe::Container::unShare() "Container::unShare()"
   * <li> @ref cbe::Object::unShare()    "Object::unShare()"
   * </ul>
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::UnShareException
#endif // #ifndef CBE_NO_SYNC
  
  virtual ~UnShareDelegate();
}; // class UnShareDelegate

/**
 * Pointer to UnShareDelegate that is passed into:
 * <ul>
 *   <li> cbe::Container::unShare()
 *   <li> cbe::Object::unShare()
 * </ul>
 */
using UnShareDelegatePtr = std::shared_ptr<UnShareDelegate>;

/**
 * @brief
 * Convenience type that bundles the parameter passed to method
 * cbe::delegate::ShareDelegate::onShareSuccess.
 */
class UnShareSuccess {
  bool ok = false;
public:
  std::string   message{};

  UnShareSuccess();
  UnShareSuccess(cbe::DefaultCtor);
  UnShareSuccess(std::string&&   message);
/**
   * @brief Checks if current instance is valid.
   * 
   * @return \c true: is valid
   */
  explicit operator bool() const;
}; // class UnShareSuccess
  } // namespace delegate
} // namespace cbe
#endif