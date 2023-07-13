#ifndef CBE__delegate__LogInDelegate_h__
#define CBE__delegate__LogInDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/Error.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::CloudBackend::logIn(const std::string&,const std::string&,const std::string&,cbe::delegate::LogInDelegatePtr)<br>
 *        See @ref anchorEx__cbe__CloudBackend__logIn_async
 *                 "Example of asynchronous login"
 * </ul>
 */
class LogInDelegate {
public:
  /**
   * Called upon successful log in.
   * @param cloudBackend Instance of a CloudBackend holding the session.
   */
  virtual void onLogInSuccess(CloudBackend&& cloudBackend) = 0;

  using Error = cbe::delegate::Error;
  /**
   * Called upon failed log in.
   * @param error   Error information passed from %CloudBackend SDK.
   * @param context Additional context information about the original service
   *                call that has failed.
   */
  virtual void onLogInError(Error&& error, cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed login.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

  /**
   * Type of object delivered on success. I.e., the object delivered through
   * success callback #onLogInSuccess(CloudBackend&&).
   */
  using Success = CloudBackend;

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::CloudBackend::logIn(const std::string&,const std::string&,
                              const std::string&)
    * if the request fails.
    */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::LogInException
#endif // #ifndef CBE_NO_SYNC

  virtual ~LogInDelegate();
}; // class LogInDelegate

/**
 * Pointer to LogInDelegate that is passed into:
 * <ul>
 *   <li> cbe::CloudBackend::logIn(const std::string&,const std::string&,const std::string&,LogInDelegatePtr)
 * </ul>
 */
using LogInDelegatePtr = std::shared_ptr<LogInDelegate>;
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__LogInDelegate_h__