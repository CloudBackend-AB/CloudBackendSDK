#ifndef CBE__delegate__SubscribeDelegate_h__
#define CBE__delegate__SubscribeDelegate_h__

#include "cbe/Types.h"
#include "cbe/util/Context.h"
#include "cbe/delegate/Error.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

namespace cbe {
  namespace delegate {
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::SubscribeManager::subscribe
 * </ul>
 */
class SubscribeDelegate {
public:
  using Success = cbe::Items;
  /**
   * Called upon successful subscribe.
   * @param object Instance of object that is being subscribed.
   */
  virtual void onSubscribeSuccess(cbe::Items&& success) = 0;

  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onSubscribeError(Error&& error, cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed subscribe.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::SubscribeManager::subscribe()
   * and its overloads
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct SubscribeException
#endif // #ifndef CBE_NO_SYNC

  virtual ~SubscribeDelegate();
}; // class SubscribeDelegate
using SubscribeDelegatePtr = std::shared_ptr<SubscribeDelegate>;
  } // namespace delegate
} // namespace cbe

#endif // #ifndef CBE__delegate__SubscribeDelegate_h__
