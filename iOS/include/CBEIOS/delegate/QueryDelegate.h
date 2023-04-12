#ifndef CBE__delegate__QueryDelegate_h__
#define CBE__delegate__QueryDelegate_h__


#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"

// #include "CBE/QueryResult.h"
// #include "CBE/Types.h"
// #include "CBE/delegate/QueryError.h"


// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

//namespace cbe {
  //namespace delegate {

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
 */

@class CBEQueryResult;

@protocol QueryDelegatePtr <NSObject> 
@required
//class QueryDelegate {
//public:
//  using Success = cbe::QueryResult;
  /**
   * Called upon successful query.
   * @param queryResult Instance of a QueryResult containing the result set.
   */
 // virtual void onQuerySuccess(cbe::QueryResult&& queryResult) = 0;
  -(void) onQuerySuccess:(CBEQueryResult*) queryResult;

//  using Error = QueryError;
  /**
   * Called upon a failed query() or join() call.
   * @param error   Error information passed from %CloudBackend SDK.
   * @param context Additional context information about the original service
   *                call that has failed.
   */
  - (void) onQueryError:(CBEError*)error  context:(CBEContext*)context;

  // virtual void onQueryError(cbe::delegate::QueryError&&         error,
  //                           cbe::util::Context&&                context) = 0;

  /**
   * Contains all information about a failed query.
   */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
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
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::LogInException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~QueryDelegate();
// }; // class QueryDelegate

/**
 * Pointer to QueryDelegate that is passed into:
 * <ul>
 * <li> CloudBackend::query(ContainerId,delegate::QueryDelegatePtr) and
 *      overloads.
 * <li> Container::query(delegate::QueryDelegatePtr) and overloads.
 * </ul>
 */
//using QueryDelegatePtr = std::shared_ptr<QueryDelegate>;


//  } // namespace delegate
//} // namespace cbe

@end  

#endif // #ifndef CBE__delegate__QueryDelegate_h__
