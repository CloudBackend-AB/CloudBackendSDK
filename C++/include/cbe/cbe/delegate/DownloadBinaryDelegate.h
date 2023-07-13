#ifndef CBE__delegate__DownloadBinaryDelegate_h__
#define CBE__delegate__DownloadBinaryDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/TransferError.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
class DownloadBinarySuccess;
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Object::download(DownloadBinaryDelegatePtr)
 * </ul>
 */
class DownloadBinaryDelegate {
public:
  using Success = DownloadBinarySuccess;
  /**
   * Called upon successful Download.
   * @param object  Instance of cbe::Object that is being downloaded.
   * @param data    The downloaded data of the object.<br>
   *                The provided memory is an array of bytes (<code>char</code>)
   *                dynamically allocated by the SDK, and managed by the 
   *                [<b><c>std::unique_ptr</c></b>]
   *                  (https://en.cppreference.com/w/cpp/memory/unique_ptr). So,
   *                no further precautions needed concerning the the disposal of
   *                this memory.
   */
  virtual void onDownloadBinarySuccess(cbe::Object&&            object,  
                                       std::unique_ptr<char[]>  data) = 0;
  
  using Error = TransferError;
  /**
   * Called if an error is encountered.
   */
  virtual void onDownloadBinaryError(
                                  cbe::delegate::TransferError&& transferError,
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
   * Contains all information about a failed DownloadBinary.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::Object::download()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::DownloadException
#endif // #ifndef CBE_NO_SYNC

  virtual ~DownloadBinaryDelegate();
}; // class DownloadBinaryDelegate

/**
 * Pointer to DownloadBinaryDelegate that is passed into:
 * <ul>
 *   <li> cbe::Object::download(DownloadBinaryDelegatePtr)
 * </ul>
 */
using DownloadBinaryDelegatePtr = std::shared_ptr<DownloadBinaryDelegate>;

  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__DownloadBinaryDelegate_h__