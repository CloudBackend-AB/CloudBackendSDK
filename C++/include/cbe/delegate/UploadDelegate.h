#ifndef CBE__delegate__UploadDelegate_h__
#define CBE__delegate__UploadDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/TransferError.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {

/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Container::upload(const std::string&,UploadDelegatePtr)<br>
 *        See @ref anchorEx__cbe__Container__upload_async
 *                 "Async example of creation of a cbe::Object by using Container::upload()"
 *   <li> cbe::Container::upload(const std::string&,const std::string&,UploadDelegatePtr)
 *   <li> cbe::Object::uploadStream(const std::string&,cbe::StreamId,UploadDelegatePtr)
 * </ul>
 */
  class UploadDelegate {
public:
  using Success = cbe::Object;
  /**
   * Called upon successful Upload.
   * @param object Instance of object that is being uploaded.
   */
  virtual void onUploadSuccess(cbe::Object&& object) = 0;

  using Error = TransferError;
  /**
   * Called if an error is encountered.
   */
  virtual void onUploadError(cbe::delegate::TransferError&& transferError, 
                             cbe::util::Context&&           context) = 0;

  /**
   * Called when a chunk of data has been sent.
   * @param object  @ref cbe::Object "Object" associated with current
   *                upload.
   * @param sent    Size, in number of bytes, of the sent chunk.
   * @param total   Total number of bytes sent so far.
   */
  virtual void onChunkSent(cbe::Object&&  object,
                           std::uint64_t  sent,
                           std::uint64_t  total);
  /**
   * Contains all information about a failed Upload.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::Container::upload(const std::string&,const std::string&)
   * and its overloads
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::UploadException
#endif // #ifndef CBE_NO_SYNC
  virtual ~UploadDelegate();
}; // class UploadDelegate

/**
 * Pointer to UploadDelegate that is passed into:
 * <ul>
 *   <li> cbe::Container::upload(const std::string&,UploadDelegatePtr)
 *   <li> cbe::Container::upload(const std::string&,const std::string&,UploadDelegatePtr)
 *   <li> cbe::Object::uploadStream(const std::string&,cbe::StreamId,UploadDelegatePtr)
 * </ul>
 */
using UploadDelegatePtr = std::shared_ptr<UploadDelegate>;

  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__UploadDelegate_h__