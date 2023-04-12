#ifndef CBE__delegate__CreateAccountDelegate_h__
#define CBE__delegate__CreateAccountDelegate_h__

#include "CBE/QueryResult.h"
#include "CBE/Types.h"

#include "CBE/delegate/Error.h"

#include "CBE/util/Context.h"
#include "CBE/util/ErrorInfo.h"
#include "CBE/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> CloudBackend::CreateAccount
 * </ul>
 */
class CreateAccountDelegate {
public:
  using Success = void;
  /**
   * Called upon successful log in.
   * @param cloudBackend Instance of a CloudBackend holding the session.
   */
  virtual void onCreateAccountSuccess() = 0;

  using Error = delegate::Error;
  /**
   * Called upon failed log in.
   * @param error   Error information passed from %CloudBackend SDK.
   * @param context Additional context information about the original service
   *                call that has failed.
   */
  virtual void onCreateAccountError(Error&& error, 
                                    cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed CreateAccount.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::CloudBackend::createAccount()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::CreateAccountException
#endif // #ifndef CBE_NO_SYNC

  virtual ~CreateAccountDelegate();
}; // class CreateAccountDelegate

/**
 * Pointer to CreateAccountDelegate that is passed into: \n 
 * CloudBackend::createAccount(const std::string&,const std::string&,const std::string&,const std::string&,const std::string&,const std::string&,CreateAccountDelegatePtr).
 */
using CreateAccountDelegatePtr = std::shared_ptr<CreateAccountDelegate>;
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__CreateAccountDelegate_h__