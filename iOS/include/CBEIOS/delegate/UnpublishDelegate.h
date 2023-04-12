#ifndef CBE__delegate__UnpublishDelegate_h__
#define CBE__delegate__UnpublishDelegate_h__

// #include "CBE/Types.h"
// #include "CBE/util/Context.h"
// #include "CBE/delegate/Error.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// namespace cbe {
//   namespace delegate {
// class UnpublishSuccess; 

#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"

@protocol UnpublishDelegatePtr <NSObject> 
@required
/**
 * Delegate class for the asynchronous version of methods:
 * <ul>
 *   <li> cbe::Container::unpublish
 *   <li> cbe::Object::unpublish
 * </ul>
 */
// class UnpublishDelegate {
// public:
//   using Success = UnpublishSuccess;
  /**
   * Called upon successful unpublish.
   * @param publishId Id of the publictoion being published.
   * @param itemId    Id of the item - cbe::Object or cbe::Container - that
   *                  has been unpublished
   */
   - (void) onUnpublishSuccess:(CBEPublishId)  publishId 
                                  itemId:(CBEItemId)itemId;
  
  /**
   * Called if an error was encountered.
   */
  - (void) onUnpublishError:(CBEError*)error  context:(CBEContext*)context;
  
  /**
   * Contains all information about a failed Unpublish.
   */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//       using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo
  
// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * <ul>
//    * <li> @ref cbe::Container::unpublish() "Container::unpublish()"
//    * <li> @ref cbe::Object::unpublish()    "Object::unpublish()"
//    * </ul>
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//       using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::UnpublishException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~UnpublishDelegate();
// }; // class UnpublishDelegate

// /**
//  * Pointer to UnpublishDelegate that is passed into:
//  * <ul>
//  *   <li> Container::unpublish(UnpublishDelegatePtr)
//  *   <li> Object::unpublish(UnpublishDelegatePtr)
//  * </ul>
//  */
// using UnpublishDelegatePtr = std::shared_ptr<UnpublishDelegate>;

// /**
//  * Convenience type that bundles all parameters passed to method
//  * cbe::delegate::onUnpublishError.
//  */
// class UnpublishSuccess {
// public:  
//   cbe::PublishId   publishId{}; 
//   cbe::ItemId      itemId{};

//   UnpublishSuccess();
//   UnpublishSuccess(cbe::DefaultCtor);
//   UnpublishSuccess(cbe::PublishId  publishId, 
//                    cbe::ItemId     itemId);
// };

//       } // namespace delegate
// } // namespace cbe
@end
#endif // #ifndef CBE__delegate__UnpublishDelegate_h__
