#ifndef CBE__delegate__group__RenameDelegate_h__
#define CBE__delegate__group__RenameDelegate_h__
#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/Error.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
    namespace group {
class RenameSuccess;
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Group::rename
 * </ul>
 */
class RenameDelegate {
public:
  using Success = RenameSuccess;
  /**
   * Called upon successful rename.
   */
  virtual void onRenameSuccess(cbe::Group&& group, std::string&& newName) = 0;

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
   * cbe::Group::rename()
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
 * cbe::Group::rename()
 */
using RenameDelegatePtr = std::shared_ptr<RenameDelegate>;
    } // namespace group
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__group__RenameDelegate_h__