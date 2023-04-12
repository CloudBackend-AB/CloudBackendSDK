/*
     Copyright © CloudBackend AB 2022.
*/

#ifndef INCLUDE_CBE_PUBLISH_MANAGER_H_
#define INCLUDE_CBE_PUBLISH_MANAGER_H_
#import <Foundation/Foundation.h>
#include "CBEIOS/Types.h"

/** 
 * @brief list of web shares.
 */ 
@interface CBEPublishManager : NSObject

/**
 * @brief Lists web shares that are shared with other @ref cbe::UserId
 *        "userids".
 * 
 * Listing is done independently of where in the actual
 * @ref cbe::Container "container/subcontainer" tree the
 * @ref cbe::Item "items" are located.
 *
 * @return The web shares in terms of @ref cbe::Item "items".
 */
-(NSArray*) getPublished;

- (id) initWith:(void*) cbiPeerPtr;

@end
#endif  // INCLUDE_CBE_PUBLISH_MANAGER_H_
