#ifndef CBE__delegate__GetStreamsDelegate_h__
#define CBE__delegate__GetStreamsDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Stream.h"
#include "cbe/Types.h"

#include "cbe/delegate/Error.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Object::getStreams()<br>
 *        See @ref anchorEx__cbe__Object__getStreams_async 
 *                 "Example of retrieving the Stream attached to a cbe::Object with Object::getStreams()"
 * </ul>
 */
class GetStreamsDelegate {
public:
  using Success = cbe::Streams;
  /**
   * Called upon successful Object::GetStreams.
   * @param streams The currently loaded @ref cbe::Stream "stream(s) attached to
   *                the  requested object.
   */
  virtual void onGetStreamsSuccess(cbe::Streams&& streams) = 0;
  
  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onGetStreamsError(Error&&              error, 
                                 cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed GetStreams.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::Object::getStreams()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::GetStreamsException
#endif // #ifndef CBE_NO_SYNC

  virtual ~GetStreamsDelegate();
}; // class GetStreamsDelegate

/**
 * Pointer to GetStreamsDelegate that is passed into: \n 
 * Object::getStreams(GetStreamsDelegatePtr).
 */
using GetStreamsDelegatePtr = std::shared_ptr<GetStreamsDelegate>;
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__GetStreamsDelegate_h__