#ifndef CBE__delegate__RemoveListenerDelegate_h__
#define CBE__delegate__RemoveListenerDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/Error.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
class RemoveListenerDelegate {
  public:
    /**
     * Called upon successful log in.
     * @param cloudBackend Instance of a CloudBackend holding the session.
     */
    virtual void onRemoveListenerSuccess(CloudBackend&& cloudBackend) = 0;

    using Error = delegate::Error;
    /**
     * Called upon failed log in.
     * @param error   Error information passed from %CloudBackend SDK.
     * @param context Additional context information about the original service
     *                call that has failed.
     */
    virtual void onRemoveListenerError(Error&& error, 
                                       cbe::util::Context&& context) = 0;

    /**
     * Contains all information about a failed RemoveListener.
     */
    struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
      using Base::Base; // Inherit base class' constructors
    };

    /**
     * Type of object delivered on success. I.e., the object delivered through
     * success callback #onRemoveListenerSuccess(CloudBackend&&).
     */
    using Success = CloudBackend;

    /**
     * Exception thrown by
     * RemoveListener(const std::string&,const std::string&,const std::string&) if the
     * RemoveListener fails.
     */
    struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
      using Base::Base; // Inherit base class' constructors
    }; // class struct CloudBackend::RemoveListenerException

    virtual ~RemoveListenerDelegate();
  }; // struct RemoveListenerDelegate

/**
 * Pointer to RemoveDelegate that is passed into:
 * CloudBackend::Remove(cbe::ContainerId   , RemoveDelegatePtr).
 */
using RemoveListenerDelegatePtr = std::shared_ptr<RemoveListenerDelegate>;
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__RemoveDelegate_h__