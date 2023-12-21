#ifndef CBE__delegate__UnPublishDelegate_h__
#define CBE__delegate__UnPublishDelegate_h__

#include "cbe/Types.h"
#include "cbe/util/Context.h"
#include "cbe/delegate/Error.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

namespace cbe {
  namespace delegate {
class UnPublishSuccess; 
/**
 * Delegate class for the asynchronous version of methods:
 * <ul>
 *   <li> cbe::Container::unPublish
 *   <li> cbe::Object::unPublish
 * </ul>
 */
class UnPublishDelegate {
public:
  using Success = UnPublishSuccess;
  /**
   * Called upon successful unPublish.
   * @param publishId Id of the publictoion being published.
   * @param itemId    Id of the item - cbe::Object or cbe::Container - that
   *                  has been unPublished
   */
  virtual void onUnPublishSuccess(cbe::PublishId  publishId, 
                                  cbe::ItemId     itemId) = 0;
  
  using Error = delegate::Error;
  /**
   * Called if an error was encountered.
   */
  virtual void onUnPublishError(Error&&               error,
                                cbe::util::Context&&  context) = 0;
  
  /**
   * Contains all information about a failed UnPublish.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
      using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo
  
#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * <ul>
   * <li> @ref cbe::Container::unPublish() "Container::unPublish()"
   * <li> @ref cbe::Object::unPublish()    "Object::unPublish()"
   * </ul>
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
      using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::UnPublishException
#endif // #ifndef CBE_NO_SYNC

  virtual ~UnPublishDelegate();
}; // class UnPublishDelegate

/**
 * Pointer to UnPublishDelegate that is passed into:
 * <ul>
 *   <li> Container::unPublish(UnPublishDelegatePtr)
 *   <li> Object::unPublish(UnPublishDelegatePtr)
 * </ul>
 */
using UnPublishDelegatePtr = std::shared_ptr<UnPublishDelegate>;

/**
 * @brief
 * Convenience type that bundles all parameters passed to method
 * cbe::delegate::UnPublishDelegate::onUnPublishError.
 */
class UnPublishSuccess {
public:  
  cbe::PublishId   publishId{}; 
  cbe::ItemId      itemId{};

  UnPublishSuccess();
  UnPublishSuccess(cbe::DefaultCtor);
  UnPublishSuccess(cbe::PublishId  publishId, 
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

#endif // #ifndef CBE__delegate__UnPublishDelegate_h__
