#ifndef CBE__delegate__LogInDelegate_h__
#define CBE__delegate__LogInDelegate_h__

//#include "CBE/QueryResult.h"
//#include "CBEIOS/Types.h"
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

@class CloudBackend;

@protocol LogInDelegatePtr <NSObject> 
@required
  /**
   * Called upon successful log in.
   * @param cloudBackend Instance of a CloudBackend holding the session.
   */
  - (void) onLogInSuccess:(CloudBackend*) cloudBackend;

  // using Error = cbedelegateError;
  // /**
  //  * Called upon failed log in.
  //  * @param error   Error information passed from %CloudBackend SDK.
  //  * @param context Additional context information about the original service
  //  *                call that has failed.
  //  */
   - (void) onLogInError:(CBEError*)error  context:(CBEContext*)context;

  // /**
  //  * Contains all information about a failed login.
  //  */
  // struct ErrorInfo : cbeutilErrorInfoImpl<Error> {
  //   using Base::Base; // Inherit base class' constructors
  // }; // struct ErrorInfo

  /**
   * Type of object delivered on success. I.e., the object delivered through
   * success callback #onLogInSuccess(CloudBackend&&).
   */
//  using Success = CloudBackend;



//   ~LogInDelegate();
// class LogInDelegate

/**
 * Pointer to LogInDelegate that is passed into:
 * <ul>
 *   <li> cbe::CloudBackend::logIn(const std::string&,const std::string&,const std::string&,LogInDelegatePtr)
 * </ul>
 */

@end  
//using LogInDelegatePtr = cbedelegateLogInDelegate*;
#endif // !CBE__delegate__LogInDelegate_h__