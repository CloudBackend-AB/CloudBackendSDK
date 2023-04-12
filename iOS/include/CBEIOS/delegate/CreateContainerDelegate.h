#ifndef CBE__delegate__CreateContainerDelegate_h__
#define CBE__delegate__CreateContainerDelegate_h__

#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
// #include "CBE/QueryResult.h"
// #include "CBE/Types.h"

// #include "CBE/delegate/Error.h"

// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// #include <memory>

@class CBEContainer;

@protocol CreateContainerDelegatePtr <NSObject> 
@required
// namespace cbe {
//   namespace delegate {
 /**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Container::createContainer()
 * </ul>
 */
// class CreateContainerDelegate {
// public:
//   using Success = cbe::Container;
  /**
   * Called upon successful CreateContainer.
   * @param object Instance of object that is being Created.
   */
  -(void) onCreateContainerSuccess:(CBEContainer*) container;

//  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  //virtual void onCreateContainerError(Error&& error, cbe::util::Context&& context) = 0;
   - (void) onCreateContainerError:(CBEError*)error  context:(CBEContext*)context;

  /**
   * Contains all information about a failed CreateContainer.
   */
//   // struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//   //   using Base::Base; // Inherit base class' constructors
//   // }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Container::createContainer()
//    * if the request fails.
//    * 
//    * Will change to cbe::Container::createContainerContainer()
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::CreateContainerException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~CreateContainerDelegate();
// }; // class CreateContainerDelegate

/**
 * Pointer to CreateContainerDelegate that is passed into:
 * <ul>
 *   <li> cbe::Container::createContainer()
 * </ul>
 */
// using CreateContainerDelegatePtr = std::shared_ptr<CreateContainerDelegate>;
//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__CreateContainerDelegate_h__