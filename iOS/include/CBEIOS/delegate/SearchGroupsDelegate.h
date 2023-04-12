#ifndef CBE__delegate__SearchGroupsDelegate_h__
#define CBE__delegate__SearchGroupsDelegate_h__

 #include "CBEIOS/Types.h"
// #include "CBE/delegate/Error.h"
// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// namespace cbe {
//   namespace delegate {

#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"

@class CBEGroupQueryResult;


@protocol SearchGroupsDelegatePtr <NSObject> 
@required
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::GroupManager::searchGroups()
 * </ul>
 */
// class SearchGroupsDelegate {
// public:
  /**
   * Called upon successful search.
   * @param queryResult Ref. instance of cbe::GroupQueryResult holding the result of
   * the search.
   */
   -(void) onSearchGroupsSuccess:(CBEGroupQueryResult*) queryResult;
  
//  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
   -(void) onSearchGroupsError:(CBEError*)error  context:(CBEContext*)context;

  /**
   * Contains all information about a failed SearchGroups.
   */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::GroupManager::searchGroups()
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::SearchGroupsException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~SearchGroupsDelegate();
// }; // class SearchGroupsDelegate

// /**
//  * Pointer to SearchGroupsDelegate that is passed into:
//  * <ul>
//  *   <li> cbe::GroupManager::searchGroups()
//  * </ul>
//  */
// using SearchGroupsDelegatePtr = std::shared_ptr<SearchGroupsDelegate>;
//   } // namespace delegate
// } // namespace cbe
@end
#endif