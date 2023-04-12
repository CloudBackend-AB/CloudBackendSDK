#ifndef CBE__delegate__SubscribeDelegate_h__
#define CBE__delegate__SubscribeDelegate_h__

// #include "CBE/Types.h"
// #include "CBE/util/Context.h"
// #include "CBE/delegate/Error.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"
#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"
// namespace cbe {
//   namespace delegate {
// /**
//  * Delegate class for the asynchronous version of method:
//  * <ul>
//  *   <li> cbe::SubscribeManager::subscribe
//  * </ul>
//  */
// class SubscribeDelegate {
// public:
//   using Success = cbe::Items;
@protocol SubscribeDelegatePtr <NSObject> 
@required
//   /**
//    * Called upon successful subscribe.
//    * @param object Instance of object that is being subscribed.
//    */
//   virtual void onSubscribeSuccess(cbe::Items&& success) = 0;

  -(void) onSubscribeSuccess:(NSArray*) subscribe;

//   using Error = delegate::Error;
//   /**
//    * Called if an error is encountered.
//    */
//   virtual void onSubscribeError(Error&& error, cbe::util::Context&& context) = 0;
  -(void) onSubscribeError:(CBEError*)error  context:(CBEContext*)context;

//   /**
//    * Contains all information about a failed subscribe.
//    */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::SubscribeManager::subscribe()
//    * and its overloads
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct SubscribeException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~SubscribeDelegate();
// }; // class SubscribeDelegate
// using SubscribeDelegatePtr = std::shared_ptr<SubscribeDelegate>;
//   } // namespace delegate
// } // namespace cbe

@end
#endif // #ifndef CBE__delegate__SubscribeDelegate_h__
