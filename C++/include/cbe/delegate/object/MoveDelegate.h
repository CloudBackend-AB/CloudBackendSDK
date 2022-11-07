#ifndef CBE__delegate__object__MoveDelegate_h__
#define CBE__delegate__object__MoveDelegate_h__

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
 *   <li> cbe::Object::move
 * </ul>
 */
class MoveDelegate {
public:
  using Success = cbe::Object;
  /**
   * Called upon successful Move.
   * @param object Instance of object that is being moved.
   */
  virtual void onMoveSuccess(cbe::Object&& object) = 0;
  
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
   * cbe::Object::move()
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
 * cbe::Object::move()
 */
using MoveDelegatePtr = std::shared_ptr<MoveDelegate>;
    } // namespace object
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__object__MoveDelegate_h__