#ifndef CBE__delegate__UploadDelegate_h__
#define CBE__delegate__UploadDelegate_h__

#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"

// #include "CBE/QueryResult.h"

// #include "CBE/delegate/TransferError.h"

// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

//#include <memory>

// namespace cbe {
//   namespace delegate {

/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Container::upload
 * </ul>
 */
@protocol UploadDelegatePtr <NSObject> 
@required

// class UploadDelegate {
// public:
  // using Success = cbe::Object;
  /**
   * Called upon successful Upload.
   * @param object Instance of object that is being Uploadd.
   */
//  virtual void onUploadSuccess(cbe::Object&& object) = 0;
- (void) onUploadSuccess:(CBEObject*)object;
//  using Error = TransferError;
  /**
   * Called if an error is encountered.
   */
  - (void) onUploadError:(CBEError*)error  context:(CBEContext*)context;

  // virtual void onUploadError(cbe::delegate::TransferError&& transferError, 
  //                            cbe::util::Context&&           context) = 0;

  /**
   * Called when a chunk of data has been sent.
   * @param object  @ref cbe::Object "Object" associated with current
   *                upload.
   * @param sent    Size, in number of bytes, of the sent chunk.
   * @param total   Total number of bytes sent so far.
   */
  // virtual void onChunkSent(cbe::Object&&  object,
  //                          std::uint64_t  sent,
  //                          std::uint64_t  total);
- (void) onChunkSent:(CBEObject*)object sent:(uint64_t)sent total:(uint64_t)total;

  /**
   * Contains all information about a failed Upload.
   */
  // struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
  //   using Base::Base; // Inherit base class' constructors
  // }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Container::upload(const std::string&,const std::string&)
//    * and its overloads
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::UploadException
// #endif // #ifndef CBE_NO_SYNC
//   virtual ~UploadDelegate();
// }; // class UploadDelegate

/**
 * Pointer to UploadDelegate that is passed into:
 * <ul>
 *   <li> cbe::Container::upload(const std::string&,const std::string&,UploadDelegatePtr).
 * </ul>
 */
// using UploadDelegatePtr = std::shared_ptr<UploadDelegate>;

//   } // namespace delegate
// } // namespace cbe
@end

#endif // !CBE__delegate__UploadDelegate_h__