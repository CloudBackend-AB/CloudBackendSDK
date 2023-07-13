#ifndef CBE__delegate__CreateContainerDelegate_h__
#define CBE__delegate__CreateContainerDelegate_h__

#include "cbe/QueryResult.h"
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
 *   <li> cbe::Container::createContainer()
 * </ul>
 */
class CreateContainerDelegate {
public:
  using Success = cbe::Container;
  /**
   * Called upon successful CreateContainer.
   * @param container Instance of Container that is being created.
   */
  virtual void onCreateContainerSuccess(cbe::Container&& container) = 0;

  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onCreateContainerError(Error&& error, cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed CreateContainer.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::Container::createContainer()
   * if the request fails.
   * 
   * Will change to cbe::Container::createContainerContainer()
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::CreateContainerException
#endif // #ifndef CBE_NO_SYNC

  virtual ~CreateContainerDelegate();
}; // class CreateContainerDelegate

/**
 * Pointer to CreateContainerDelegate that is passed into:
 * <ul>
 *   <li> cbe::Container::createContainer()
 * </ul>
 */
using CreateContainerDelegatePtr = std::shared_ptr<CreateContainerDelegate>;
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__CreateContainerDelegate_h__