#ifndef CBE__delegate__DownloadStreamDelegate_h__
#define CBE__delegate__DownloadStreamDelegate_h__

// #include "CBE/delegate/DownloadStreamDelegatePtr.h"

// #include "CBE/Object.h"
// #include "CBE/QueryResult.h"
// #include "CBE/Types.h"

// #include "CBE/delegate/TransferError.h"

// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// #include <memory>

// namespace cbe {
//   namespace delegate {
// class DownloadStreamSuccess;

// /**
//  * Delegate class for the asynchronous version of method:
//  * <ul>
//  *   <li> cbe::Object::downloadStream
//  * </ul>
//  */
// class DownloadStreamDelegate {
// public:
//   using Success = DownloadStreamSuccess;
@class CBEObject;

@protocol DownloadStreamObjectDelegatePtr <NSObject> 
@required
//   /**
//    * Called upon successful Download.
//    * @param object Instance of stream that is being Downloaded.
//    */
//   virtual void onDownloadStreamSuccess(cbe::Object&&  object, 
//                                        std::string    path) = 0;
-(void) onDownloadStreamObjectSuccess:(CBEObject*)object path:(NSString*)path;  
//   using Error = TransferError;
//   /**
//    * Called if an error is encountered.
//    */
//   virtual void onDownloadStreamError(
//                                   cbe::delegate::TransferError&&  transferError,
//                                   cbe::util::Context&&            context) = 0;
-(void) onDownloadStreamObjectError:(CBEError*)error  context:(CBEContext*)context;
//   /**
//    * Called when a chunk of data has been received.
//    * @param object    @ref cbe::Object "Object" associated with current
//    *                  download.
//    * @param received  Size, in number of bytes, of the received chunk.
//    * @param total     Total number of bytes received so far.
//    */
//   virtual void onChunkReceived(cbe::Object&&  object,
//                                std::uint64_t  received,
//                                std::uint64_t  total);
-(void) onChunkReceived:(CBEObject*)object received:(uint64_t)received total:(uint64_t)total;

//   /**
//    * Contains all information about a failed DownloadStream.
//    */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Object::downloadStream(const std::string&,cbe::Stream)
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::DownloadException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~DownloadStreamDelegate();
// }; // class DownloadStreamDelegate

// /**
//  * Convenience type that bundles all parameters passed to method
//  * cbe::delegate::DownloadStreamDelegate::onDownloadStreamSuccess.
//  */
// class DownloadStreamSuccess {
// public:
//   cbe::Object object{cbe::DefaultCtor{}};
//   std::string path{};
  
//   DownloadStreamSuccess();
//   DownloadStreamSuccess(cbe::DefaultCtor);
//   DownloadStreamSuccess(cbe::Object&& object, 
//                         std::string   path);
// };

//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__DownloadStreamDelegate_h__