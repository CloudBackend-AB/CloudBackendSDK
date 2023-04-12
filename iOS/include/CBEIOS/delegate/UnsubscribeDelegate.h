#ifndef CBE__delegate__UnsubscribeDelegate_h__
#define CBE__delegate__UnsubscribeDelegate_h__

// #include "CBE/Types.h"
// #include "CBE/util/Context.h"
// #include "CBE/delegate/Error.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// namespace cbe {
//   namespace delegate {
// class UnsubscribeSuccess;
// /**
//  * Delegate class for the asynchronous version of methods:
//  * <ul>
//  *   <li> cbe::Container::unsubscribe
//  *   <li> cbe::Object::unsubscribe
// * </ul>
//  */
// class UnsubscribeDelegate {
// public:
//   using Success = UnsubscribeSuccess;

#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"

@protocol UnsubscribeDelegatePtr <NSObject> 
@required
  /**
   * Called upon successful Unsubscribe.
   * @param object Instance of object that is being Unsubscribed.
   */
  -(void) onUnsubscribeSuccess:(CBEPublishId)publishId itemId:(CBEItemId)itemId;

  // using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  -(void) onUnsubscribeError:(CBEError*)error  context:(CBEContext*)context;

  /**
   * Contains all information about a failed Unsubscribe.
   */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * <ul>
//    * <li> @ref cbe::Container::unsubscribe() "Container::unsubscribe()"
//    * <li> @ref cbe::Object::unsubscribe()    "Object::unsubscribe()"
//    * </ul>
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::UnsubscribeException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~UnsubscribeDelegate();
// }; // class UnsubscribeDelegate

// /**
//  * Pointer to UnsubscribeDelegate that is passed into:
//  * <ul>
//  *   <li> Container::unsubscribe(UnsubscribeDelegatePtr)
//  *   <li> Object::unsubscribe(UnsubscribeDelegatePtr)
//  * </ul>
//  */
// using UnsubscribeDelegatePtr = std::shared_ptr<UnsubscribeDelegate>;

// /**
//  * Convenience type that bundles all parameters passed to method
//  * cbe::delegate::onUnsubscribeSuccess.
//  */
// class UnsubscribeSuccess {
// public:  
//   cbe::PublishId   publishId{}; 
//   cbe::ItemId         itemId{};

//   UnsubscribeSuccess();
//   UnsubscribeSuccess(cbe::PublishId  publishId, 
//                      cbe::ItemId     itemId);
// };
//   } // namespace delegate
// } // namespace cbe
@end
#endif // #ifndef CBE__delegate__UnsubscribeDelegate_h__
