/*
Copyright © CloudBackend AB 2022.
*/

#ifndef INCLUDE_CBE_PUBLISH_H_
#define INCLUDE_CBE_PUBLISH_H_
#import <Foundation/Foundation.h>
#include "CBEIOS/Types.h"

/**
 * @brief manages a \c published Object.
 * 
 */
@interface CBEPublish : NSObject
/**
 * Invites users
 */
-(void) invite;
/**
 * Blocks users
 */
-(void) block;
/**
 * Sets the title
 * 
 * @param title a name describing this
 */
-(void) setTitle: (NSString*) title;
/**
 * Sets the privacy
 * \see Types.h for cbe::PublishVisibility
 *
 * @param privacy New privacy (cbe::PublishVisibility)
 */
-(void) setPrivacy: (CBEPublishVisibility) privacy;
/**
 * Sets the security
 * \see Types.h for cbe::Access
 *
 * @param security New security (cbe::Access)
 */
-(void) setSecurity: (CBEPublishAccess) security;
/**
 * Sets the password
 */
-(void) setPassword: (const NSString*) password;
/**
 * Sets the description
 */
-(void) setDescription:(const NSString*) description;
/**
 * Gets the title
 */
-(NSString*) getTitle;
/**
 * Gets the privacy (PublishVisibility)
 * \see Types.h for cbe::PublishVisibility
 */
-(CBEPublishVisibility) getPrivacy;
/**
 * Gets the security (cbe::Access)
 * \see Types.h for cbe::Access
 */
-(CBEPublishAccess) getSecurity;
/**
 * Gets the password if applicable
 */
-(NSString*) getPassword;
/**
 * Gets the description
 */
-(NSString*) getDescription;
/**
 * Gets the publish id number
 */
-(CBEPublishId) getPublishId;
/**
 * Gets the subscribers
 */
-(void) getSubscribers;
/**
 * Gets the QR code
 */
-(void) getQRCode;

- (id) initWith:(void*) cbiPeerPtr;

@end
#endif   //INCLUDE_CBE_PUBLISH_H_
