/*
Copyright © CloudBackend AB 2020 - 2023.
*/

#ifndef INCLUDE_CBE_SHARE_MANAGER_H_
#define INCLUDE_CBE_SHARE_MANAGER_H_

#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/ListSharesDelegate.h"
#include "CBEIOS/Types.h"

@protocol ListAvailableSharesDelegatePtr <ListSharesDelegatePtr>
@end
@protocol ListMySharesDelegatePtr <ListSharesDelegatePtr>
@end

@interface CBEShareManager : NSObject
/**
 * @brief Lists the shares exposed by other users to current user. 
 * This will give specific share information.
 * @param delegate Pointer to a delegate::ListSharesDelegate
 *                 instance that is implemented by the user.
 */
-(void) listAvailableShares:(id<ListAvailableSharesDelegatePtr>) delegate;
/**
 * @brief Lists the shares exposed by current user. 
 * This will give specific share information.
 * @param delegate Pointer to a delegate::ListSharesDelegate
 *                 instance that is implemented by the user.
 */
-(void) listMyShares:(id<ListMySharesDelegatePtr>) delegate;

- (id) initWith:(void*) cbiPeerPtr;
- (void*) getCbiPeerVoid;

@end
#endif  // INCLUDE_CBE_SHARE_MANAGER_H_
