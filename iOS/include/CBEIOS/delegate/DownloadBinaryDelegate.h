#ifndef CBE__delegate__DownloadBinaryDelegate_h__
#define CBE__delegate__DownloadBinaryDelegate_h__


#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"

// #include "CBE/delegate/DownloadBinaryDelegatePtr.h"

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
// /**
//  * Delegate class for the asynchronous version of method:
//  * <ul>
//  *   <li> cbe::Object::download
//  * </ul>
//  */

@class CBEObject;

@protocol DownloadBinaryObjectDelegatePtr <NSObject> 
@required 
// class DownloadBinaryDelegate {
// public:
//   using Success = char*;
//   /**
//    * Called upon successful Download.
//    * @param object  Instance of cbe::Object that is being downloaded.
//    * @param data    The downloaded data of the object.
//    */
//   virtual void onDownloadBinarySuccess(cbe::Object&&            object,  
//                                        std::unique_ptr<char[]>  data) = 0;
-(void) onDownloadBinaryObjectSuccess: (NSString*) path;
//   using Error = TransferError;
//   /**
//    * Called if an error is encountered.
//    */
//   virtual void onDownloadBinaryError(
//                                   cbe::delegate::TransferError&& transferError,
//                                   cbe::util::Context&&           context) = 0;
- (void) onDownloadBinaryObjectError:(CBEError*)error  context:(CBEContext*)context;


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
//    * Contains all information about a failed DownloadBinary.
//    */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Object::download()
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::DownloadException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~DownloadBinaryDelegate();
// }; // class DownloadBinaryDelegate

// /**
//  * Convenience type that bundles all parameters passed to method
//  * cbe::delegate::DownloadBinaryDelegate::onDownloadBinarySuccess.
//  */
// class DownloadBinarySuccess {
// public:
//   cbe::Object             object{cbe::DefaultCtor{}};
//   std::unique_ptr<char[]> data{};
  
//   DownloadBinarySuccess();
//   DownloadBinarySuccess(cbe::DefaultCtor);
//   DownloadBinarySuccess(cbe::Object&&            object,  
//                         std::unique_ptr<char[]>  data);
// };

//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__DownloadBinaryDelegate_h__