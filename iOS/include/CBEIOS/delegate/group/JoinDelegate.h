#ifndef CBE__delegate__group__JoinDelegate_h__
#define CBE__delegate__group__JoinDelegate_h__
#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"
// #include "CBE/QueryResult.h"
// #include "CBE/Types.h"

// #include "CBE/delegate/Error.h"

// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// #include <memory>

// namespace cbe {
//   namespace delegate {
//     namespace group {
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Group::join
 * </ul>
 * \note This is different from the query chain join.
 */

@protocol JoinGroupDelegatePtr <NSObject> 
@required
// class JoinDelegate {
// public:
//   using Success = cbe::Group;
  /**
   * Called upon successful Join.
   */
   - (void) onJoinSuccess:(CBEGroup*) group;

//  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  -(void) onJoinError:(CBEError*)error  context:(CBEContext*)context;

  /**
   * Contains all information about a failed Join.
   */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Group::join()
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::JoinException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~JoinDelegate();
// }; // class JoinDelegate

// /**
//  * Pointer to JoinDelegate that is passed into:
//  * cbe::Group::join()
//  * \note This is different from the query chain join.
//  */
// using JoinDelegatePtr = std::shared_ptr<JoinDelegate>;
//     } // namespace group
//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__group__JoinDelegate_h__