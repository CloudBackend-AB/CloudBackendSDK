#ifndef CBE__delegate__UnSubscribeDelegate_h__
#define CBE__delegate__UnSubscribeDelegate_h__

#include "cbe/Types.h"
#include "cbe/util/Context.h"
#include "cbe/delegate/Error.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

namespace cbe {
  namespace delegate {
class UnSubscribeSuccess;
/**
 * Delegate class for the asynchronous version of methods:
 * <ul>
 *   <li> cbe::Container::unSubscribe
 *   <li> cbe::Object::unSubscribe
* </ul>
 */
class UnSubscribeDelegate {
public:
  using Success = UnSubscribeSuccess;
  /**
   * Called upon successful UnSubscribe.
   * @param publishId The PublishId of the UnSubscribe.
   * @param itemId Instance of ItemId that is being UnSubscribed.
   */
  virtual void onUnSubscribeSuccess(cbe::PublishId  publishId, 
                                    cbe::ItemId     itemId) = 0;

  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onUnSubscribeError(Error&&               error,
                                  cbe::util::Context&&  context) = 0;

  /**
   * Contains all information about a failed UnSubscribe.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * <ul>
   * <li> @ref cbe::Container::unSubscribe() "Container::unSubscribe()"
   * <li> @ref cbe::Object::unSubscribe()    "Object::unSubscribe()"
   * </ul>
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::UnSubscribeException
#endif // #ifndef CBE_NO_SYNC

  virtual ~UnSubscribeDelegate();
}; // class UnSubscribeDelegate

/**
 * Pointer to UnSubscribeDelegate that is passed into:
 * <ul>
 *   <li> Container::unSubscribe(UnSubscribeDelegatePtr)
 *   <li> Object::unSubscribe(UnSubscribeDelegatePtr)
 * </ul>
 */
using UnSubscribeDelegatePtr = std::shared_ptr<UnSubscribeDelegate>;

/**
 * @brief
 * Convenience type that bundles all parameters passed to method
 * cbe::delegate::UnSubscribeDelegate::onUnSubscribeSuccess.
 */
class UnSubscribeSuccess {
public:  
  cbe::PublishId   publishId{}; 
  cbe::ItemId         itemId{};

  UnSubscribeSuccess();
  UnSubscribeSuccess(cbe::PublishId  publishId, 
                     cbe::ItemId     itemId);
  /**
   * @brief Checks if current instance is valid.
   * 
   * @return \c true: is valid
   */
  explicit operator bool() const;
};
  } // namespace delegate
} // namespace cbe

#endif // #ifndef CBE__delegate__UnSubscribeDelegate_h__
