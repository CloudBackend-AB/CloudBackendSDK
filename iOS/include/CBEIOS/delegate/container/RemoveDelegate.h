#ifndef CBE__delegate__container__RemoveDelegate_h__
#define CBE__delegate__container__RemoveDelegate_h__

#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"
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
// class RemoveSuccess;
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Container::remove
 * </ul>
 */

@protocol RemoveContainerDelegatePtr <NSObject> 
@required

// class RemoveDelegate {
// public:
//   using Success = RemoveSuccess;
  /**
   * Called upon successful Remove.
   * @param containerId Id of the cbe::Container being removed.
   * @param name        Name of container that is being removed.
   */
-(void) onRemoveContainerSuccess:(CBEItemId) containerId name:(NSString*) name;
  
  // using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
//  virtual void onRemoveError(Error&& error, cbe::util::Context&& context) = 0;
- (void) onRemoveContainerError:(CBEError*)error  context:(CBEContext*)context;

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
#endif // !CBE__delegate__container__RemoveDelegate_h__