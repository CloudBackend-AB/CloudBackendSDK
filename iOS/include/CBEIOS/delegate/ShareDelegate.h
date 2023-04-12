#ifndef CBE__delegate__ShareDelegate_h__
#define CBE__delegate__ShareDelegate_h__

#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"

// #include "CBE/Types.h"
// #include "CBE/delegate/Error.h"
// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// namespace cbe {
//   namespace delegate {
/**
 * Delegate class for the asynchronous version of methods:
 * <ul>
 *   <li> cbe::Container::share
 *   <li> cbe::Object::share
 * </ul>
 */
@protocol ShareDelegatePtr <NSObject> 
@required

// class ShareDelegate {
// public:
//   using Success = cbe::ShareId;
  /**
   * Called upon successful share.
   * @param shareId Id of the share.
   */
  //virtual void onShareSuccess(cbe::ShareId shareId) = 0;
  - (void) onShareSuccess:(CBEShareId) shareId;
//  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  - (void) onShareError:(CBEError*)error  context:(CBEContext*)context;

  // virtual void onShareError(Error&&               error,
  //                           cbe::util::Context&&  context) = 0;

  /**
   * Contains all information about a failed Share.
   */
  // struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
  //   using Base::Base; // Inherit base class' constructors
  // }; // struct ErrorInfo

//#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * <ul>
   *   <li> cbe::Container::share(cbe::UserId,std::string)
   *   <li> cbe::Object::share(cbe::UserId,std::string)
   * </ul>
   * if the request fails.
   */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::ShareException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~ShareDelegate();
// }; // class ShareDelegate

// /**
//  * Pointer to ShareDelegate that is passed into:
//  * <ul>
//  *   <li> cbe::Container::share(cbe::UserId,std::string,ShareDelegatePtr)
//  *   <li> cbe::Object::share(cbe::UserId,std::string,ShareDelegatePtr)
//  * </ul>
//  */
// using ShareDelegatePtr = std::shared_ptr<ShareDelegate>;

//   } // namespace delegate
// } // namespace cbe
@end
#endif