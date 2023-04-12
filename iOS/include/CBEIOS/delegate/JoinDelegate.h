#ifndef CBE__delegate__JoinDelegate_h__
#define CBE__delegate__JoinDelegate_h__

// #include "CBE/Types.h"

// #include "CBE/delegate/QueryError.h"
// #include "CBE/delegate/QueryJoinDelegate.h"

// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// namespace cbe {
//   namespace delegate {
#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
//#include "CBE/util/ErrorInfo.h"
//#include "CBE/util/Exception.h"

//#include <memory>

/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::CloudBackend::logIn()
 * </ul>
 */

@class CBEQueryResult;

@protocol JoinDelegatePtr <NSObject> 
@required
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::QueryChain::join()
 * </ul>
 * \note This is different from the group join.
 */
//class JoinDelegate {
//public:
 // using Success = cbe::QueryResult;
  /**
   * Called upon successful query.
   * @param queryResult Instance of a QueryResult containing the result set.
   */
   -(void) onJoinSuccess:(CBEQueryResult*) queryResult;

  //using JoinError = QueryError;
  //using Error = JoinError;
  /**
   * Called upon a failed join() call.
   * @param error   Error information passed from %CloudBackend SDK.
   * @param context Additional context information about the original service
   *                call that has failed.
   */
   - (void) onJoinError:(CBEError*)error  context:(CBEContext*)context;

//  virtual void onJoinError(JoinError&&          error,
  //                         cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed join.
   */
  //using ErrorInfo = QueryJoinDelegate::ErrorInfo;

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::QueryChain::join()
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::LogInException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~JoinDelegate();
// }; // class JoinDelegate

// /**
//  * Pointer to JoinDelegate that is passed into
//  * cbe::QueryChain::join().
//  * \note This is different from the group join.
//  */
// using JoinDelegatePtr = std::shared_ptr<JoinDelegate>;

//   } // namespace delegate
// } // namespace cbe
@end  


#endif // #ifndef CBE__delegate__JoinDelegate_h__
