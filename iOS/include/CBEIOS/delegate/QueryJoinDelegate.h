#ifndef CBE__delegate__QueryJoinDelegate_h__
#define CBE__delegate__QueryJoinDelegate_h__

// #include "CBE/Types.h"
// #include "CBE/delegate/QueryDelegate.h"
// #include "CBE/delegate/QueryError.h"


// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// namespace cbe {
//   namespace delegate {

@class CBEQueryResult;

@protocol QueryJoinDelegatePtr <NSObject> 
@required
/**
 * Delegate callback interface for asynchronous methods:
 * <ul>
 * <li> CloudBackend::query(ContainerId,delegate::QueryJoinDelegatePtr) and
 *      overloads.
 * <li> Container::query(delegate::QueryJoinDelegatePtr) and overloads.
 * </ul>
 */
// class QueryJoinDelegate {
// public:
//   using Success = cbe::QueryResult;
  /**
   * Called upon successful query.
   * @param queryResult Instance of a QueryResult containing the result set.
   */
//  virtual void onQueryJoinSuccess(cbe::QueryResult&& queryResult) = 0;
  -(void) onQueryJoinSuccess:(CBEQueryResult*) queryResult;


  // using QueryJoinError = QueryError;
  // using Error = QueryJoinError;
  /**
   * Called upon a failed query() or join() call.
   * @param error   Error information passed from %CloudBackend SDK.
   * @param context Additional context information about the original service
   *                call that has failed.
   */
  // virtual void onQueryJoinError(QueryJoinError&&     error,
  //                               cbe::util::Context&& context) = 0;
  - (void) onQueryJoinError:(CBEError*)error  context:(CBEContext*)context;

  /**
   * Contains all information about a failed query.
   */
 // using ErrorInfo = QueryDelegate::ErrorInfo;

//#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * <ul>
   * <li> @ref cbe::CloudBackend::query() "CloudBackend::query()",
   *      and its overloads
   * <li> @ref cbe::Container::query() "Container::query()",
   *      and its overloads
   * </ul>
   * if a query fails.
   */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::LogInException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~QueryJoinDelegate();
// }; // struct QueryJoinDelegate

// /**
//  * Pointer to QueryJoinDelegate that is passed into:
//  * <ul>
//  * <li> CloudBackend::query(ContainerId,delegate::QueryJoinDelegatePtr) and
//  *      overloads.
//  * <li> Container::query(delegate::QueryJoinDelegatePtr) and overloads.
//  * </ul>
//  */
// using QueryJoinDelegatePtr = std::shared_ptr<QueryJoinDelegate>;


//   } // namespace delegate
// } // namespace cbe

@end  

#endif // #ifndef CBE__delegate__QueryJoinDelegate_h__
