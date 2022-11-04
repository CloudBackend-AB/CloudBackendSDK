#ifndef CBE__delegate__container__MoveDelegate_h__
#define CBE__delegate__container__MoveDelegate_h__

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
 *   <li> cbe::Container::move
 * </ul>
 */
class MoveDelegate {
public:
  using Success = cbe::Container;
  /**
   * Called upon successful Move.
   * @param container Instance of container that is being moved.
   */
  virtual void onMoveSuccess(cbe::Container&& container) = 0;

  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onMoveError(Error&& error, cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed Move.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::Container::move()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::MoveException
#endif // #ifndef CBE_NO_SYNC

  virtual ~MoveDelegate();
}; // class MoveDelegate
/**
 * Pointer to MoveDelegate that is passed into:
 * Container::move(  void move(cbe::ContainerId,MoveDelegatePtr).
 */
using MoveDelegatePtr = std::shared_ptr<MoveDelegate>;
    } // namespace container
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__container__MoveDelegate_h__