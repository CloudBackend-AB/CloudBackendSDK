#ifndef CBE__delegate__DownloadDelegate_h__
#define CBE__delegate__DownloadDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/TransferError.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
class DownloadSuccess;    
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Object::download(const std::string&, DownloadDelegatePtr)
 *   <li> cbe::Object::downloadStream(const std::string&,cbe::Stream,DownloadDelegatePtr)
 * </ul>
 */
class DownloadDelegate {
public:
  using Success = DownloadSuccess;
  /**
   * Called upon successful Download.
   * @param object Instance of object that is being Downloaded.
   * @param path Path of object to be downloaded.
   */
  virtual void onDownloadSuccess(cbe::Object&& object, std::string path) = 0;
  
  using Error = cbe::delegate::TransferError;
  /**
   * Called if an error is encountered.
   */
  virtual void onDownloadError(cbe::delegate::TransferError&& transferError,
                               cbe::util::Context&&           context) = 0;

  /**
   * Called when a chunk of data has been received.
   * @param object    @ref cbe::Object "Object" associated with current
   *                  download.
   * @param received  Size, in number of bytes, of the received chunk.
   * @param total     Total number of bytes received so far.
   */
  virtual void onChunkReceived(cbe::Object&&  object,
                               std::uint64_t  received,
                               std::uint64_t  total);

  /**
   * Contains all information about a failed download.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by:
   * <ul>
   *   <li> cbe::Object::download(const std::string&, DownloadDelegatePtr)
   *   <li> cbe::Object::downloadStream(const std::string&,cbe::Stream,DownloadDelegatePtr)
   * </ul>
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::DownloadException
#endif // #ifndef CBE_NO_SYNC

  virtual ~DownloadDelegate();
}; // class DownloadDelegate

/**
 * Pointer to DownloadDelegate that is passed into:
 * <ul>
 *   <li> cbe::Object::download(const std::string&,DownloadDelegatePtr)
 *   <li> cbe::Object::downloadStream(const std::string&,cbe::Stream,DownloadDelegatePtr)
 * </ul>
 */
using DownloadDelegatePtr = std::shared_ptr<DownloadDelegate>;

  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__DownloadDelegate_h__