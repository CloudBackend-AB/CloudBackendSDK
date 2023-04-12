#ifndef CBE__delegate__CreateGroupDelegate_h__
#define CBE__delegate__CreateGroupDelegate_h__
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
 *   <li> cbe::Group::createGroup
 * </ul>
 */

@class CBEGroup;


@protocol CreateGroupDelegatePtr <NSObject> 
@required
// class CreateGroupDelegate {
// public:
//   using Success = cbe::Group;
  /**
   * Called upon successful create group.
   * @param group Instance of a cbe::Group.
   */
  -(void) onCreateGroupSuccess:(CBEGroup*) group;

//  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  -(void) onCreateGroupError:(CBEError*)error  context:(CBEContext*)context;

  /**
   * Contains all information about a failed CreateGroup.
   */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Group::createGroup()
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::CreateGroupException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~CreateGroupDelegate();
// }; // class CreateGroupDelegate
// /**
//  * Pointer to CreateGroupDelegate that is passed into: \n 
//  * Group::createGroup(std::string,std::string,CreateGroupDelegatePtr,cbe::Visibility).
//  */
// using CreateGroupDelegatePtr = std::shared_ptr<CreateGroupDelegate>;
//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__CreateGroupDelegate_h__