#ifndef CBE__delegate__GetSubscriptionsDelegate_h__
#define CBE__delegate__GetSubscriptionsDelegate_h__

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
 *   <li> cbe::SubscribeManager::getSubscriptions
 * </ul>
 */
class GetSubscriptionsDelegate {
public:
  using Success = cbe::QueryResult;
  /**
   * Called upon successful GetSubscriptions.
   * @param object Instance of object that is being streamed.
   */
  virtual void onGetSubscriptionsSuccess(cbe::QueryResult&& object) = 0;
  
  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onGetSubscriptionsError(Error&&                error, 
                                        cbe::util::Context&&   context) = 0;

  /**
   * Contains all information about a failed GetSubscriptions.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::SubscribeManager::getSubscriptions()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::GetSubscriptionsException
#endif // #ifndef CBE_NO_SYNC

  virtual ~GetSubscriptionsDelegate();
}; // class GetSubscriptionsDelegate

/**
 * Pointer to GetSubscriptionsDelegate that is passed into:
 * SubscribeManager::getSubscriptions(GetSubscriptionsDelegatePtr).
 */
using GetSubscriptionsDelegatePtr = std::shared_ptr<GetSubscriptionsDelegate>;
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__GetSubscriptionsDelegate_h__
