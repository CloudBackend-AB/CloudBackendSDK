#ifndef CBE__delegate__object__RenameDelegate_h__
#define CBE__delegate__object__RenameDelegate_h__

// #include "CBE/QueryResult.h"
// #include "CBE/Types.h"

// #include "CBE/delegate/Error.h"

// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// #include <memory>

// namespace cbe {
//   namespace delegate {
//     namespace object {

// /**
//  * Delegate class for the asynchronous version of method:
//  * <ul>
//  *   <li> cbe::Object::rename
//  * </ul>
//  */
// class RenameDelegate {
// public:
//   using Success = cbe::Object;

@class CBEObject;

@protocol RenameObjectDelegatePtr <NSObject> 
@required
//   /**
//    * Called upon successful Rename.
//    * @param object Instance of object that are being renamed.
//    */
//   virtual void onRenameSuccess(cbe::Object&& object) = 0;
- (void) onRenameObjectSuccess:(CBEObject*)object;
//   using Error = delegate::Error;
//   /**
//    * Called if an error is encountered.
//    */
//   virtual void onRenameError(Error&& error, cbe::util::Context&& context) = 0;
- (void) onRenameObjectError:(CBEError*)error  context:(CBEContext*)context;
//   /**
//    * Contains all information about a failed Rename.
//    */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Object::rename()
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
//  * cbe::Object::rename()
//  */
// using RenameDelegatePtr = std::shared_ptr<RenameDelegate>;
//     } // namespace object
//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__object__RenameDelegate_h__