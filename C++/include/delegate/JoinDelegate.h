#ifndef CBE__delegate__JoinDelegate_h__
#define CBE__delegate__JoinDelegate_h__

#include "cbe/Types.h"

#include "cbe/delegate/QueryError.h"
#include "cbe/delegate/QueryJoinDelegate.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

namespace cbe {
  namespace delegate {

/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::QueryChain::join()
 * </ul>
 * \note This is different from the group join.
 */
class JoinDelegate {
public:
  using Success = cbe::QueryResult;
  /**
   * Called upon successful query.
   * @param queryResult Instance of a QueryResult containing the result set.
   */
  virtual void onJoinSuccess(cbe::QueryResult&& queryResult) = 0;

  using JoinError = QueryError;
  using Error = JoinError;
  /**
   * Called upon a failed join() call.
   * @param error   Error information passed from %CloudBackend SDK.
   * @param context Additional context information about the original service
   *                call that has failed.
   */
  virtual void onJoinError(JoinError&&          error,
                           cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed join.
   */
  using ErrorInfo = QueryJoinDelegate::ErrorInfo;

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::QueryChain::join()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::LogInException
#endif // #ifndef CBE_NO_SYNC

  virtual ~JoinDelegate();
}; // class JoinDelegate

/**
 * Pointer to JoinDelegate that is passed into
 * cbe::QueryChain::join().
 * \note This is different from the group join.
 */
using JoinDelegatePtr = std::shared_ptr<JoinDelegate>;

  } // namespace delegate
} // namespace cbe


#endif // #ifndef CBE__delegate__JoinDelegate_h__
