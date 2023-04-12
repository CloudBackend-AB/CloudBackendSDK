#ifndef CBE__delegate__LeaveDelegate_h__
#define CBE__delegate__LeaveDelegate_h__

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
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Group::leave
 * </ul>
 */

@protocol LeaveDelegatePtr <NSObject> 
@required
// class LeaveDelegate {
// public:
  /**
   * Called upon successful leave.
   */
  -(void) onLeaveSuccess:(NSString*) memberName
                              memberId:(CBEMemberId) memberId;

//  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  -(void) onLeaveError:(CBEError*)error  context:(CBEContext*)context;

  /**
   * Contains all information about a failed Leave.
   */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Group::leave() if the
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::LeaveException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~LeaveDelegate();
// }; // class LeaveDelegate
// /**
//  * Pointer to LeaveDelegate that is passed into: \n 
//  * Group::leave(LeaveDelegatePtr).
//  */
// using LeaveDelegatePtr = std::shared_ptr<LeaveDelegate>;
//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__LeaveDelegate_h__