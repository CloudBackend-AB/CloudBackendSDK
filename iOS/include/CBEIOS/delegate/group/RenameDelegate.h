#ifndef CBE__delegate__group__RenameDelegate_h__
#define CBE__delegate__group__RenameDelegate_h__
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
 *   <li> cbe::Group::rename
 * </ul>
 */

@protocol RenameGroupDelegatePtr <NSObject> 
@required
// class RenameDelegate {
// public:
  /**
   * Called upon successful rename.
   */
   -(void) onRenameSuccess:(CBEGroup*) group newName:(NSString*)newName;

//  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  -(void) onRenameError:(CBEError*)error  context:(CBEContext*)context;

  /**
   * Contains all information about a failed Rename.
   */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Group::rename()
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::RenameException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~RenameDelegate();
// }; // class RenameDelegate

// /**
//  * Pointer to RenameDelegate that is passed into:
//  * cbe::Group::rename()
//  */
// using RenameDelegatePtr = std::shared_ptr<RenameDelegate>;
//     } // namespace group
//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__group__RenameDelegate_h__