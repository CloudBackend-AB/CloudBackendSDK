#ifndef CBE__delegate__CreateDelegate_h__
#define CBE__delegate__CreateDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/Error.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
 class CreateDelegate {
  public:
    using Success = cbe::Container;
    /**
     * Called upon successful Create.
     * @param object Instance of object that is being Created.
     */
    virtual void onCreateSuccess(cbe::Container&& container) = 0;

    using Error = delegate::Error;
    /**
     * Called if an error was encountered.
     */
    virtual void onCreateError(Error&& error, cbe::util::Context&& context) = 0;

    /**
     * Contains all information about a failed Create.
     */
    struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
      using Base::Base; // Inherit base class' constructors
    };

    /**
     * Exception thrown by
     * Create(const std::string &name, CreateDelegatePtr delegate) if the
     * Create fails.
     */
    struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
      using Base::Base; // Inherit base class' constructors
    }; // class struct CloudBackend::CreateException
    virtual ~CreateDelegate();
  }; // class CreateDelegate

/**
 * Pointer to CreateDelegate that is passed into:
 * CloudBackend::Create(cbe::ContainerId   , CreateDelegatePtr).
 */
using CreateDelegatePtr = std::shared_ptr<CreateDelegate>;
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__CreateDelegate_h__