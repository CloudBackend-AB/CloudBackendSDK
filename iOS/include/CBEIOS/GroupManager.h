/*
Copyright © CloudBackend AB 2020 - 2023.
*/

#ifndef INCLUDE_CBE_GROUP_MANAGER_H_
#define INCLUDE_CBE_GROUP_MANAGER_H_

#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/ListGroupsDelegate.h"
#include "CBEIOS/delegate/SearchGroupsDelegate.h"
#include "CBEIOS/Types.h"

@class CBEGroupFilter;

@interface CBEGroupManager : NSObject {
}

/**
 * @brief to manage the users own Group.
 * 
 */

/**
 * List all current joined groups.
 */
-(void) listGroups:(id<ListGroupsDelegatePtr>)delegate;

/**
 * Search for open public groups.
 * @param filter        is a group filter to set search criteria for open 
 *                      public groups. Look att class GroupFilter for more 
 *                      information.
 * @param delegate      is the callback protocol template for group actions.
 * @param parentGroupId is the id of the group to be searched within. 
 *                      If this is not set the tenent id will be used.
 */
-(void) searchGroups:(CBEGroupFilter*)filter
                  delegate:(id<SearchGroupsDelegatePtr>) delegate
                  parentGroupId:(CBEGroupId)            parentGroupId;

/**
 * Returns the tenant id of the Tenant user group that the user is in.
 */
-(CBEGroupId) getTenantId;

- (id) initWith:(void*)cbiPeerPtr;
@end
#endif  // INCLUDE_CBE_GROUP_MANAGER_H_
