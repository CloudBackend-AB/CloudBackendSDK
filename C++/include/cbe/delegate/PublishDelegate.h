#ifndef CBE__delegate__PublishDelegate_h__
#define CBE__delegate__PublishDelegate_h__

#include "cbe/Types.h"
#include "cbe/util/Context.h"
#include "cbe/delegate/Error.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

namespace cbe {
  namespace delegate {
class PublishSuccess;
/**
 * Delegate class for the asynchronous version of methods:
 * <ul>
 *   <li> cbe::Container::publish
 *   <li> cbe::Object::publish
 * </ul>
 */
class PublishDelegate {
public:
  using Success = PublishSuccess;
  /**
   * Called upon successful publish.
   * @param items Instance of items that are being published.
   */
  virtual void onPublishSuccess(cbe::Items&& items) = 0;

  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onPublishError(Error&& error, cbe::util::Context&& context) = 0;
  
  /**
   * Contains all information about a failed Publish.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
      using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo
  
#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by:
   * <ul>
   *   <li> cbe::Container::publish()
   *   <li> cbe::Object::publish()
   * </ul>
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
      using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::PublishException
#endif // #ifndef CBE_NO_SYNC

  virtual ~PublishDelegate();
}; // class PublishDelegate

/**
 * Pointer to PublishDelegate that is passed into:
 * <ul>
 *   <li> cbe::Container::publish()
 *   <li> cbe::Object::publish()
 * </ul>
 */
using PublishDelegatePtr = std::shared_ptr<PublishDelegate>;

  } // namespace delegate
} // namespace cbe

#endif // #ifndef CBE__delegate__PublishDelegate_h__
