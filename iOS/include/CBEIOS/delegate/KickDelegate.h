#ifndef CBE__delegate__KickDelegate_h__
#define CBE__delegate__KickDelegate_h__
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
// class KickSuccess;
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Member::kick
 * </ul>
 */

@protocol KickDelegatePtr <NSObject> 
@required
// class KickDelegate {
// public:
//   using Success = KickSuccess;
  /**
   * Called upon successful ban.
   * @param memberName  Name of the member being kicked.
   * @param memberId    Id of the member being kicked.
   */
   -(void) onKickSuccess:(NSString*)  memberName
                             memberId:(CBEMemberId)memberId;

  // using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  -(void) onKickError:(CBEError*)error  context:(CBEContext*)context;
  
//   /**
//    * Contains all information about a failed Kick.
//    */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//       using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Member::kick(std::string)
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//       using Base::Base; // Inherit base class' constructors
//   }; // struct CloudBackend::KickException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~KickDelegate();
// }; // class KickDelegate

// /**
//  * Pointer to KickDelegate that is passed into:
//  * <ul>
//  *   <li> cbe::Member::kick(std::string,KickDelegatePtr)
//  * </ul>
//  */
// using KickDelegatePtr = std::shared_ptr<KickDelegate>;

// /**
//  * Convenience type that bundles all parameters passed to method
//  * cbe::delegate::KickDelegate::onKickSuccess.
//  */
// class KickSuccess {
// public:
//   std::string   memberName{};
//   cbe::MemberId memberId{};

//   KickSuccess();
//   KickSuccess(cbe::DefaultCtor);
//   KickSuccess(std::string&& memberName,
//               cbe::MemberId memberId);
// };
//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__KickDelegate_h__
