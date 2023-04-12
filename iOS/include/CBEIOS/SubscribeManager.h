/*
Copyright © CloudBackend AB 2022.
*/

#ifndef INCLUDE_CBE_SUBSCRIBE_MANAGER_H_
#define INCLUDE_CBE_SUBSCRIBE_MANAGER_H_
#import <Foundation/Foundation.h>

#include "CBEIOS/Subscribe.h"
#include "CBEIOS/delegate/SubscribeDelegate.h"
#include "CBEIOS/delegate/GetSubscriptionsDelegate.h"

/**
 * @brief Class for managing Publish, represented as a list.
 */
@interface CBESubscribeManager : NSObject {

}
/**
 * Lists of publish that you are sharing with others. Listing is
 * done independently of where in the actual directory tree the files
 * are located.
 *
 * @param subscribeDelegate Pointer to a delegate::GetSubscriptionsDelegate
 *                          instance that is implemented by the user.
 */
-(void) getSubscriptions: (id<GetSubscriptionsDelegatePtr>) delegate ;

/**
 * Subscribes to a publish shared by some other user (or yourself).
 * @param sharingUserId User id of the owner of the publish 
 * or 0 if sharingUserName should be used instead
 * @param sharingUserName User name of the owner of the publish 
 * or empty if sharingUserId should be used instead
 * @param publishId Publish id or 0 if publishName should be used instead
 * @param publishName Publish name or empty if publishId should be used instead
 * @param password Required password or an empty string if no password
 * @param subscribeName Name of created favorite (usually same as the publish)
 * @param subscribeDelegate Gets notified of the result
 */
-(void) subscribe:        (CBEUserId)                 userId 
        sharingUserName:  (NSString*)                 sharingUserName
        publishId:        (CBEPublishId)              publishId
        publishName:      (NSString*)                 publishName
        password:         (NSString*)                 password
        subscribeName:    (NSString*)                 subscribeName
        delegate:         (id<SubscribeDelegatePtr>)  delegate; 
- (id) initWith:(void*) cbiPeerPtr;
@end
#endif // INCLUDE_CBE_SUBSCRIBE_MANAGER_H_
