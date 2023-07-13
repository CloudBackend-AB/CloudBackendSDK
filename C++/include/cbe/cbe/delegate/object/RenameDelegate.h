#ifndef CBE__delegate__object__RenameDelegate_h__
#define CBE__delegate__object__RenameDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/Error.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
    namespace object {

/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Object::rename
 * </ul>
 */
class RenameDelegate {
public:
  using Success = cbe::Object;
  /**
   * Called upon successful Rename.
   * @param object Instance of object that are being renamed.
   */
  virtual void onRenameSuccess(cbe::Object&& object) = 0;
  
  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onRenameError(Error&& error, cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed Rename.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::Object::rename()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::RenameException
#endif // #ifndef CBE_NO_SYNC

  virtual ~RenameDelegate();
}; // class RenameDelegate
/**
 * Pointer to RenameDelegate that is passed into:
 * cbe::Object::rename()
 */
using RenameDelegatePtr = std::shared_ptr<RenameDelegate>;
    } // namespace object
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__object__RenameDelegate_h__