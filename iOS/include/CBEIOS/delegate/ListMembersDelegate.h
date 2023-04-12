#ifndef CBE__delegate__ListMembersDelegate_h__
#define CBE__delegate__ListMembersDelegate_h__
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
 *   <li> cbe::Group::listMembers
 * </ul>
 */

@protocol ListMembersDelegatePtr <NSObject> 
@required
// class ListMembersDelegate {
// public:
//   using Members = std::vector<cbe::Member>;
//   using Success = Members; 
   /**
   * Called upon successful ListMembers.
   * @param members Vector of cbe::Member holding the members
   * of a group.
   */
  -(void) onListMembersSuccess:(NSArray*) members;

//  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  -(void) onListMembersError:(CBEError*)error  context:(CBEContext*)context;

  /**
   * Contains all information about a failed ListMembers.
   */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    *  cbe::Group::listMembers()
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::ListMembersException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~ListMembersDelegate();
// }; // class ListMembersDelegate

// /**
//  * Pointer to ListMembersDelegate that is passed into:
//  * <ul>
//  *   <li> cbe::Group::listMembers(ListMembersDelegatePtr).
//  * </ul>
//  */
// using ListMembersDelegatePtr = std::shared_ptr<ListMembersDelegate>;
//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__ListMembersDelegate_h__