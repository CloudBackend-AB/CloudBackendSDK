/*
Copyright © CloudBackend AB 2022.
*/

#ifndef INCLUDE_CBE_SUBSCRIBE_H_
#define INCLUDE_CBE_SUBSCRIBE_H_
#import <Foundation/Foundation.h>
#include "CBEIOS/Types.h"

/**
* @brief to subscribe to a \c published Item.
* 
*/
@interface CBESubscribe : NSObject {

}
/**
 * Gets the date as unix epoch number
 */
-(CBEDate) getDate;
/**
 * Gets the title
 */
-(NSString*) getTitle;
/**
 * Gets the description
 */
-(NSString*) getDescription;
/**
 * Gets the password
 */
-(NSString*) getPassword;
/**
 * Gets the security 
 * \see Types.h for cbe::PublishAccess enum
 */
-(CBEPublishAccess) getSecurity;
/**
 * Gets the privacy 
 * \see Types.h for cbe::PublishVisibility
 */
-(CBEPublishVisibility) getPrivacy;
/**
 * Gets the subscribe id number
 */
-(CBESubscribeId) getSubscribeId;
/**
 * Gets the publish id number
 */
-(CBEPublishId) getPublishId;
/**
 * Gets the owner id number
 */
-(CBEUserId) getOwner;
/**
 * Unsubscribe
 */
-(void) unsubscribe;

- (id) initWith:(void*) cbiPeerPtr;
@end
#endif  // INCLUDE_CBE_SUBSCRIBE_H_
