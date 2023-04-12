#ifndef CBE__delegate__BanDelegate_h__
#define CBE__delegate__BanDelegate_h__
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
// class BanSuccess;
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Member::ban
 * </ul>
 */

@protocol BanDelegatePtr <NSObject> 
@required
// class BanDelegate {
// public:
//   using Success = BanSuccess;
  /**
   * Called upon successful ban.
   * @param memberName  Name of the member being banned.
   * @param memberId    Id of the member being banned.
   */
  -(void) onBanSuccess:(NSString*) memberName
                              memberId:(CBEMemberId)memberId;

//  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  -(void) onBanError:(CBEError*)error  context:(CBEContext*)context;
  
  /**
   * Contains all information about a failed Ban.
   */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//       using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo
  
// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Member::ban(std::string)
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//       using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::BanException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~BanDelegate();
// }; // class BanDelegate

// /**
//  * Pointer to BanDelegate that is passed into:
//  * <ul>
//  *   <li> cbe::Member::ban(std::string,BanDelegatePtr)
//  * </ul>
//  */
// using BanDelegatePtr = std::shared_ptr<BanDelegate>;

// /**
//  * Convenience type that bundles all parameters passed to method
//  * cbe::delegate::BanDelegate::onBanSuccess.
//  */
// class BanSuccess {
// public:
//   std::string   memberName{};
//   cbe::MemberId memberId{};

//   BanSuccess();
//   BanSuccess(cbe::DefaultCtor);
//   BanSuccess(std::string&& memberName,
//              cbe::MemberId memberId);
// };
//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__BanDelegate_h__