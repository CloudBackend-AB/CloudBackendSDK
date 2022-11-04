#ifndef CBE__delegate__group__JoinDelegate_h__
#define CBE__delegate__group__JoinDelegate_h__

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
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Group::join
 * </ul>
 * \note This is different from the query chain join.
 */
class JoinDelegate {
public:
  using Success = cbe::Group;
  /**
   * Called upon successful Join.
   */
  virtual void onJoinSuccess(cbe::Group&& group) = 0;

  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onJoinError(Error&& error, cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed Join.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::Group::join()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::JoinException
#endif // #ifndef CBE_NO_SYNC

  virtual ~JoinDelegate();
}; // class JoinDelegate

/**
 * Pointer to JoinDelegate that is passed into:
 * cbe::Group::join()
 * \note This is different from the query chain join.
 */
using JoinDelegatePtr = std::shared_ptr<JoinDelegate>;
    } // namespace group
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__group__JoinDelegate_h__