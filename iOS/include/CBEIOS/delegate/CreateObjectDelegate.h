#ifndef CBE__delegate__CreateObjectDelegate_h__
#define CBE__delegate__CreateObjectDelegate_h__

// #include "CBE/QueryResult.h"
// #include "CBE/Types.h"

// #include "CBE/delegate/Error.h"

// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// #include <memory>

#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"

@class CBEObject;

@protocol CreateObjectDelegatePtr <NSObject> 
@required

// namespace cbe {
//   namespace delegate {
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Container::createObject
 * </ul>
 */
// class CreateObjectDelegate {
// public:
//   using Success = cbe::Object;
  /**
   * Called upon successful CreateObject.
   * @param object Instance of object that is being CreateObjectd.
   */
  -(void) onCreateObjectSuccess:(CBEObject*) object;
  
  // using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  - (void) onCreateObjectError:(CBEError*)error  context:(CBEContext*)context;

//  virtual void onCreateObjectError(Error&& error, cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed CreateObject.
   */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Container::createObject()
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::CreateObjectException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~CreateObjectDelegate();
// }; // class CreateObjectDelegate

// /**
//  * Pointer to CreateObjectDelegate that is passed into: \n 
//  * Container::createObject(std::string,CreateObjectDelegatePtr,cbe::KeyValues).
//  */
// using CreateObjectDelegatePtr = std::shared_ptr<CreateObjectDelegate>;
//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__CreateObjectDelegate_h__