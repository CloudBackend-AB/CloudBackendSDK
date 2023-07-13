#ifndef CBE__delegate__container__RenameDelegate_h__
#define CBE__delegate__container__RenameDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/Error.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
    namespace container {

/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Container::rename
 * </ul>
 */
class RenameDelegate {
public:
  using Success = cbe::Container;
  /**
   * Called upon successful Rename.
   * @param container Instance of container that is being Renamed.
   */
  virtual void onRenameSuccess(cbe::Container&& container) = 0;

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
   * cbe::Container::rename()
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
 * Container::rename(const std::string&,RenameDelegatePtr).
 */
using RenameDelegatePtr = std::shared_ptr<RenameDelegate>;
    } // namespace container
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__container__RenameDelegate_h__