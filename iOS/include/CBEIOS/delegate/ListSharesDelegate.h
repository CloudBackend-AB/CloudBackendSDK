#ifndef CBE__delegate__ListSharesDelegate_h__
#define CBE__delegate__ListSharesDelegate_h__

#import <Foundation/Foundation.h>
// #include "CBEIOS/QueryResult.h"
#include "CBEIOS/delegate/Error.h"
#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"
// #include "CBEIOS/QueryChain.h"

// #include "CBE/QueryResult.h"

// #include "CBE/delegate/Error.h"

// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// #include <memory>

@class CBEItem;

@protocol ListSharesDelegatePtr <NSObject> 
@required

// namespace cbe {
//   namespace delegate {
// /**
//  * Delegate class for the asynchronous version of method:
//  * <ul>
//  *   <li> cbe::ShareManager::listMyShares
//  * </ul>
//  */
// class ListSharesDelegate {
// public:
//   using Success = cbe::QueryResult;
//   /**
//    * Called upon successful Share.
//    * @param qResult Instance of cbe::QueryResult containing list of shares.
//    */
//   virtual void onListSharesSuccess(cbe::QueryResult&& qResult) = 0;
- (void) onListSharesSuccess:(CBEQueryResult*)qResult;
//   using Error = delegate::Error;
//   /**
//    * Called if an error is encountered.
//    */
//   virtual void onListSharesError(Error&&              error, 
//                                  cbe::util::Context&& context) = 0;
- (void) onListSharesError:(CBEError*)error  context:(CBEContext*)context;

//   /**
//    * Contains all information about a failed Share.
//    */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//   using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::ShareManager::listAvailableShares()
//    * or
//    * cbe::ShareManager::listMyShares() 
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//   using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::ShareException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~ListSharesDelegate();
// }; // class ShareDelegate

// /**
//  * Pointer to ListSharesDelegate that is passed into:
//  * <ul>
//  *   <li> ShareManager::listAvailableShares(ListSharesDelegatePtr)
//  *   <li> ShareManager::listMyShares(ListSharesDelegatePtr)
//  * </ul>
//  */
// using ListSharesDelegatePtr = std::shared_ptr<ListSharesDelegate>;
//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__ListSharesDelegate_h__