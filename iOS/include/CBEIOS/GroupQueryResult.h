/*
Copyright © CloudBackend AB 2020 - 2023.
*/

#ifndef INCLUDE_CBE_GROUP_QUERY_H_
#define INCLUDE_CBE_GROUP_QUERY_H_
#import <Foundation/Foundation.h>

#include "CBEIOS/Types.h"
#include "CBEIOS/GroupFilter.h"

/**
 * @brief Resultset of data retrieved in a search for Group.
 */
@interface CBEGroupQueryResult : NSObject {
}
/** 
 * Returns a copy of the filter used for query.
 */
-(CBEGroupFilter*) filter;

/**
 * Returns a copy of a vector containing the groups for the queryResult.
 * The queryResult will update when new data comes in but the copy will not.
 * If iterating, make sure to create a variable for a local copy.
 * @return vector<cbe::Group> containing the groups matching the query.
 */
-(NSArray*) getGroupsSnapshot;

/** 
 * Number of groups loaded in the queryResult. 
 */
-(uint64_t) GroupsLoaded;

/** 
 * Total number of Groups in the cloud matching the query result.
 * This may be more than loaded.
 */
-(uint64_t) totalCount;

/** 
 * asking if the group with groupId was loaded in the query. 
 * @param groupId the group id number in question
*/
-(bool) containsGroup:(uint64_t) groupId;

- (id) initWith:(void*) cbiPeerPtr;

@end
#endif // INCLUDE_CBE_GROUPQUERY_H_
