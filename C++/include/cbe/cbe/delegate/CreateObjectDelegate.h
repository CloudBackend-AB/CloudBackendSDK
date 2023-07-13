#ifndef CBE__delegate__CreateObjectDelegate_h__
#define CBE__delegate__CreateObjectDelegate_h__

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
 *   <li> cbe::Container::createObject
 * </ul>
 */
class CreateObjectDelegate {
public:
  using Success = cbe::Object;
  /**
   * Called upon successful CreateObject.
   * @param object Instance of Object that is being created.
   */
  virtual void onCreateObjectSuccess(cbe::Object&& object) = 0;
  
  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onCreateObjectError(Error&& error, cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed CreateObject.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::Container::createObject()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::CreateObjectException
#endif // #ifndef CBE_NO_SYNC

  virtual ~CreateObjectDelegate();
}; // class CreateObjectDelegate

/**
 * Pointer to CreateObjectDelegate that is passed into: \n 
 * Container::createObject(std::string,CreateObjectDelegatePtr,cbe::KeyValues).
 */
using CreateObjectDelegatePtr = std::shared_ptr<CreateObjectDelegate>;
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__CreateObjectDelegate_h__