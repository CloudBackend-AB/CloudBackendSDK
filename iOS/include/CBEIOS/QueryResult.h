/*
    Copyright © CloudBackend AB 2020 - 2023.
*/


#ifndef INCLUDE_CBE_QUERY_RESULT_H_
#define INCLUDE_CBE_QUERY_RESULT_H_

#include "CBEIOS/Filter.h"

/**
 * @brief resultset of data retrieved.
 * 
 */

@interface CBEQueryResult : NSObject {
}
/**
 * Returns a copy of the filter used for query.
 */
-(CBEFilter*) filter ;

/**
 * Returns a copy of a vector containing the items for this QueryResult.
 * The QueryResult will be updated when new data comes in, but the returned
 * copy will not.
 * If iterating make sure to create a variable for a local copy.
 *
 * @return The items matching the query.
 */
-(NSArray*) getItemsSnapshot ;

/**
 * Number of \c items loaded in current QueryResult.
 */
-(uint64_t) itemsLoaded ;

/**
 * total number of items in the cloud matching the query result.
 * This may be more than loaded.
 */
-(uint64_t) totalCount ;

/**
 * Returns number of \c objects loaded in to the query result.
 */
-(uint64_t) objectsLoaded ;

/**
 * Returns number of \c containers loaded in to the query result.
 */
-(uint64_t) containersLoaded ;

/**
 * Checks if the Item with id is in the resultset.
 * 
 * @param itemId id number of the item asked for
 */
-(bool) containsItem:(CBEItemId) itemId ;


- (id) initWith:(void*) cbiPeerPtr;

@end
#endif // INCLUDE_CBE_DIRECTORY_H_
