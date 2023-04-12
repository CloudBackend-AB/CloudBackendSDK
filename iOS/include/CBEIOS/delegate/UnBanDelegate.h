#ifndef CBE__delegate__UnBanDelegate_h__
#define CBE__delegate__UnBanDelegate_h__

// #include "CBEIOS/QueryResult.h"
// #include "CBEIOS/Types.h"

// // #include "CBE/delegate/Error.h"

// // #include "CBE/util/Context.h"
// // #include "CBE/util/ErrorInfo.h"
// // #include "CBE/util/Exception.h"

// #include <memory>

// namespace cbe {
//   namespace delegate {
// class UnBanSuccess;

#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Member::unBan
 * </ul>
 */

@protocol UnBanDelegatePtr <NSObject> 
@required
// class UnBanDelegate {
// public:
//   using Success = UnBanSuccess;
  /**
   * Called upon successful unBan.
   * @param memberName  Name of the member being unbanned.
   * @param memberId    Id of the member being unbanned.
   */
   -(void) onUnBanSuccess:(NSString*) memberName
                              memberId:(CBEMemberId)memberId;

  /**
   * Called if an error is encountered.
   */
   -(void) onUnBanError:(CBEError*)error  context:(CBEContext*)context;

  /**
   * Contains all information about a failed UnBan.
   */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//       using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo
  
// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Member::unBan()
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//       using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::UnBanException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~UnBanDelegate();
// }; // class UnBanDelegate
// /**
//  * Pointer to UnBanDelegate that is passed into:
//  * <ul>
//  *   <li> cbe::Member::unBan(UnBanDelegatePtr)
//  * </ul>
//  */
// using UnBanDelegatePtr = std::shared_ptr<UnBanDelegate>;

// /**
//  * Convenience type that bundles all parameters passed to method
//  * cbe::delegate::UnBanDelegate::onUnBanSuccess.
//  */
// class UnBanSuccess {
// public:
//   std::string   memberName{};
//   cbe::MemberId memberId{};

//   UnBanSuccess();
//   UnBanSuccess(cbe::DefaultCtor);
//   UnBanSuccess(std::string&& memberName,
//                cbe::MemberId memberId);
// };

//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__UnBanDelegate_h__