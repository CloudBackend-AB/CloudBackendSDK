#ifndef CBE__delegate__group__RemoveDelegate_h__
#define CBE__delegate__group__RemoveDelegate_h__
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
// class RemoveSuccess;
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Group::remove
 * </ul>
 */

@protocol RemoveGroupDelegatePtr <NSObject> 
@required
// class RemoveDelegate {
// public:
//   using Success = RemoveSuccess;
  /**
   * Called upon successful remove.
   * @param groupId   Id of the cbe::Group being removed.
   * @param groupName Name of the group being removed.
   */
   -(void) onRemoveSuccess:(CBEGroupId)groupId
                               groupName:(NSString*)groupName;

//  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  -(void) onRemoveError:(CBEError*)error  context:(CBEContext*)context;

  /**
   * Contains all information about a failed Remove.
   */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Group::remove()
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::RemoveException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~RemoveDelegate();
// }; // class RemoveDelegate

// /**
//  * Pointer to RemoveDelegate that is passed into:
//  * <ul>
//  *   <li> cbe::Group::remove(RemoveDelegatePtr)
//  * </ul>
//  */
// using RemoveDelegatePtr = std::shared_ptr<RemoveDelegate>;

// /**
//  * Convenience type that bundles all parameters passed to method
//  * cbe::delegate::group::RemoveDelegate::onRemoveSuccess.
//  */
// class RemoveSuccess {
// public:
//    cbe::GroupId groupId{};
//    std::string  groupName{};

//    RemoveSuccess();
//    RemoveSuccess(cbe::DefaultCtor);
//    RemoveSuccess(cbe::GroupId   groupId,
//                  std::string&&  groupName);
// };

//     } // namespace group
//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__group__RemoveDelegate_h__