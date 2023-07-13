#ifndef CBE__delegate__QueryDelegate_h__
#define CBE__delegate__QueryDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Types.h"
#include "cbe/delegate/QueryError.h"


#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

namespace cbe {
  namespace delegate {

/**
  * Delegate class for the asynchronous version of methods:
  * <ul>
  *   <li> @ref cbe::CloudBackend::queryWithPath() "CloudBackend::queryWithPath()",
  *        and its overloads
  *   <li> @ref cbe::CloudBackend::query() "CloudBackend::query()",
  *        and its overloads
  *   <li> @ref cbe::CloudBackend::search() "CloudBackend::search()",
  *        and its overloads
  *   <li> @ref cbe::Container::queryWithPath() "Container::queryWithPath()"
  *   <li> @ref cbe::Container::query() "Container::query()",
  *        and its overloads
  *   <li> @ref cbe::Container::search() "Container::search()",
  *        and its overloads
  * </ul>
  * 
  * @anchor anchorEx__cbe__delegate_QueryDelegate 
  * @par Example
  * @include example/query_delegate.cpp
  * Usage of the class above, see
  * @ref anchorEx__cbe__CloudBackend__query_async 
   *     "Example of asynchronous query"
 */
class QueryDelegate {
public:
  using Success = cbe::QueryResult;
  /**
   * Called upon successful query.
   * @param queryResult Instance of a QueryResult containing the result set.
   */
  virtual void onQuerySuccess(cbe::QueryResult&& queryResult) = 0;

  using Error = QueryError;
  /**
   * Called upon a failed query() or join() call.
   * @param error   Error information passed from %CloudBackend SDK.
   * @param context Additional context information about the original service
   *                call that has failed.
   */
  virtual void onQueryError(cbe::delegate::QueryError&&         error,
                            cbe::util::Context&&                context) = 0;

  /**
   * Contains all information about a failed query.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
    * @brief exception thrown by
    * <ul>
    *   <li> @ref cbe::CloudBackend::queryWithPath() "CloudBackend::queryWithPath()",
    *        and its overloads
    *   <li> @ref cbe::CloudBackend::query() "CloudBackend::query()",
    *        and its overloads
    *   <li> @ref cbe::CloudBackend::search() "CloudBackend::search()",
    *        and its overloads
    *   <li> @ref cbe::Container::queryWithPath() "Container::queryWithPath()"
    *   <li> @ref cbe::Container::query() "Container::query()",
    *        and its overloads
    *   <li> @ref cbe::Container::search() "Container::search()",
    *        and its overloads
    * </ul>
    * </ul>
    * if the request fails.
    */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::LogInException
#endif // #ifndef CBE_NO_SYNC

  virtual ~QueryDelegate();
}; // class QueryDelegate

/**
 * Pointer to QueryDelegate that is passed into:
 * <ul>
 * <li> CloudBackend::query(ContainerId,delegate::QueryDelegatePtr) and
 *      overloads.
 * <li> Container::query(delegate::QueryDelegatePtr) and overloads.
 * </ul>
 */
using QueryDelegatePtr = std::shared_ptr<QueryDelegate>;


  } // namespace delegate
} // namespace cbe


#endif // #ifndef CBE__delegate__QueryDelegate_h__
