#ifndef CBE__delegate__DownloadDelegate_h__
#define CBE__delegate__DownloadDelegate_h__

#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"

// #include "CBE/delegate/DownloadDelegatePtr.h"

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
// class DownloadSuccess;    
// /**
//  * Delegate class for the asynchronous version of method:
//  * <ul>
//  *   <li> cbe::Object::download
//  * </ul>
//  */
@class CBEObject;

@protocol DownloadObjectDelegatePtr <NSObject> 
@required 

// class DownloadDelegate {
// public:
//   using Success = DownloadSuccess;
//   /**
//    * Called upon successful Download.
//    * @param object Instance of object that is being Downloaded.
//    * @param path Path of object to be downloaded.
//    */
//   virtual void onDownloadSuccess(cbe::Object&& object, std::string path) = 0;
-(void) onDownloadObjectSuccess:(CBEObject*) objectPtr path:(NSString*) path;

//   using Error = cbe::delegate::TransferError;
//   /**
//    * Called if an error is encountered.
//    */
//   virtual void onDownloadError(cbe::delegate::TransferError&& transferError,
//                                cbe::util::Context&&           context) = 0;
- (void) onDownloadObjectError:(CBEError*)error  context:(CBEContext*)context;
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

//   /**
//    * Contains all information about a failed download.
//    */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Object::download(const std::string& path)
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::DownloadException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~DownloadDelegate();
// }; // class DownloadDelegate

// /**
//  * Convenience type that bundles all parameters passed to method
//  * cbe::delegate::DownloadDelegate::onDownloadSuccess.
//  */
// class DownloadSuccess {
// public:
//   cbe::Object object{cbe::DefaultCtor{}};
//   std::string path{};
  
//   DownloadSuccess();
//   DownloadSuccess(cbe::DefaultCtor);
//   DownloadSuccess(cbe::Object&& object, 
//                   std::string   path);
// };

//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__DownloadDelegate_h__