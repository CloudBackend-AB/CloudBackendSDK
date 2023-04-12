#ifndef CBE__delegate__container__RenameDelegate_h__
#define CBE__delegate__container__RenameDelegate_h__
#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"
//// #include "CBE/QueryResult.h"
// #include "CBE/Types.h"

// #include "CBE/delegate/Error.h"

// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// #include <memory>

// namespace cbe {
//   namespace delegate {
//     namespace container {
// class RemoveSuccess;
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Container::remove
 * </ul>
 */
@class CBEContainer;

@protocol RenameContainerDelegatePtr <NSObject> 
@required

// class RemoveDelegate {
// public:
//   using Success = RemoveSuccess;
//   /**
//    * Called upon successful Rename.
//    * @param container Instance of container that is being Renamed.
//    */

-(void) onRenameContainerSuccess:(CBEContainer*) container;
  
  // using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
//  virtual void onRemoveError(Error&& error, cbe::util::Context&& context) = 0;
- (void) onRenameContainerError:(CBEError*)error  context:(CBEContext*)context;

  /**
   * Contains all information about a failed Remove.
   */
  // struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
  //   using Base::Base; // Inherit base class' constructors
  // }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
// /**
//  * @brief exception thrown by
//  * cbe::Container::remove()
//  * if the request fails.
//  */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct RemoveDelegate::RemoveException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~RemoveDelegate();
// }; // class RemoveDelegate
/**
 * Pointer to RemoveDelegate that is passed into:
 * <ul>
 *   <li> cbe::Container::remove(RemoveDelegatePtr)
 * </ul>
 */
// using RemoveDelegatePtr = std::shared_ptr<RemoveDelegate>;

/**
 * Convenience type that bundles all parameters passed to method
 * cbe::delegate::container::RemoveDelegate::onRemoveSuccess.
//  */
// class RemoveSuccess {
// public:  
//   cbe::ItemId containerId{};
//   std::string name{};

//   RemoveSuccess();
//   RemoveSuccess(cbe::DefaultCtor);
//   RemoveSuccess(cbe::ItemId containerId, std::string name);
// }; // struct RemoveSuccess

//     } // namespace container
//   } // namespace delegate
// } // namespace cbe
@end
// #include "CBE/QueryResult.h"
// #include "CBE/Types.h"

// #include "CBE/delegate/Error.h"

// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// #include <memory>

// namespace cbe {
//   namespace delegate {
//     namespace container {

// /**
//  * Delegate class for the asynchronous version of method:
//  * <ul>
//  *   <li> cbe::Container::rename
//  * </ul>
//  */
// class RenameDelegate {
// public:
//   using Success = cbe::Container;
//   /**
//    * Called upon successful Rename.
//    * @param container Instance of container that is being Renamed.
//    */
//   virtual void onRenameSuccess(cbe::Container&& container) = 0;

//   using Error = delegate::Error;
//   /**
//    * Called if an error is encountered.
//    */
//   virtual void onRenameError(Error&& error, cbe::util::Context&& context) = 0;

//   /**
//    * Contains all information about a failed Rename.
//    */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Container::rename()
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::RenameException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~RenameDelegate();
// }; // class RenameDelegate
// /**
//  * Pointer to RenameDelegate that is passed into:
//  * Container::rename(const std::string&,RenameDelegatePtr).
//  */
// using RenameDelegatePtr = std::shared_ptr<RenameDelegate>;
//     } // namespace container
//   } // namespace delegate
// } // namespace cbe

#endif // !CBE__delegate__container__RenameDelegate_h__