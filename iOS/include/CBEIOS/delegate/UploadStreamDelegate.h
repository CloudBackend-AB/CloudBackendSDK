#ifndef CBE__delegate__UploadStreamDelegate_h__
#define CBE__delegate__UploadStreamDelegate_h__

// #include "CBE/Types.h"

// #include "CBE/delegate/TransferError.h"

// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// #include <memory>

// namespace cbe {
//   namespace delegate {
// /**
//  * Delegate class for the asynchronous version of method:
//  * <ul>
//  *   <li> cbe::Object::uploadStream
//  * </ul>
//  */

@class CBEObject;

@protocol UploadStreamObjectDelegatePtr <NSObject> 
@required 
// class UploadStreamDelegate {
// public:
//   using Success = cbe::Object;
//   /**
//    * Called upon successful Upload.
//    * @param object Instance of object that is being Uploaded.
//    */
//   virtual void onUploadStreamSuccess(cbe::Object&& object) = 0;
-(void) onUploadStreamObjectSuccess:(CBEObject*)object;  

//   using Error = TransferError;
//   /**
//    * Called if an error is encountered.
//    */
//   virtual void onUploadStreamError(cbe::delegate::TransferError&& transferError, 
//                                    cbe::util::Context&&           context) = 0;
-(void) onUploadStreamObjectError:(CBEError*)error  context:(CBEContext*)context;

//   /**
//    * Called when a chunk of data has been sent.
//    * @param object  @ref cbe::Object "Object" associated with current
//    *                upload.
//    * @param sent    Size, in number of bytes, of the sent chunk.
//    * @param total   Total number of bytes sent so far.
//    */
//   virtual void onChunkSent(cbe::Object&&  object,
//                            std::uint64_t  sent,
//                            std::uint64_t  total);
-(void) onChunkSent: (CBEObject*) object 
        sent:        (uint64_t)   sent 
        total:       (uint64_t)   total;

//   /**
//    * Contains all information about a failed UploadStream.
//    */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Object::uploadStream(cbe::Container::upload(const std::string&,cbe::StreamId)
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::UploadException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~UploadStreamDelegate();
// }; // class UploadStreamDelegate
// /**
//  * Pointer to UploadStreamDelegate that is passed into:
//  * <ul>
//  *   <li> cbe::Object::uploadStream(const std::string&,cbe::StreamId,UploadStreamDelegate).
//  * </ul>
//  */
// using UploadStreamDelegatePtr = std::shared_ptr<UploadStreamDelegate>;
//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__UploadStreamDelegate_h__
