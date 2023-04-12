/*
Copyright © CloudBackend AB 2020 - 2023.
*/

#ifndef INCLUDE_CBE_GROUPFILTER_H_
#define INCLUDE_CBE_GROUPFILTER_H_
#import <Foundation/Foundation.h>

#include "CBEIOS/Types.h"

/**
 * @brief to filter when searching a list of Group.
 */
@interface CBEGroupFilter : NSObject {
}
/**
 * Returns the query string that was set on the filter.
 * E.g., name:Abc*
 */
-(NSString*) getQuery;

/**
 * Returns the settings on how the data was sorted and displayed.
 */
-(bool) getAscending;

/**
 * If deleted objects in the bin are included.
 */
-(bool) getDeleted;

/** Returns the offset that was used for the filter in the query. */
-(uint32_t) getOffset;

/** Returns the value of the count that was set for the query.*/
-(uint32_t) getCount;

/**
 *  Set the query string, 
 * e.g: Name:* (would search for all objects with the metadata key of Name). 
 * \note If used with rootContainer id as the dataId,
 * search in the whole account will be done.
 */
-(void) setQuery:(NSString*)query;

/**
 * Not fully tested but should be a regular expression.
 */
-(void) setFilter:(NSString*)filter;

/** 
 * Sets the Order in which data should be displayed: 
 * Acending meaning alfabetical
 */
-(void) setAscending:(bool)ascending;

/**
 * If you want to see deleted groups set deleted to true.
*/
-(void) setDeleted:(bool)deleted;

/** 
 * Set the offset for paging, offset is the item offset where 
 * to start your query. i.e:
 * There is already a query of the first 99 items and to get the rest 
 * you have setOffest to 100 to get the next set.
 */
-(void) setOffset:(uint32_t)offset;

/** 
 * Set the Number of items you want to get from a container. 
 * So if a container has 50 items but you only want the 10 first in 
 * ascending order then set ascending to true and setCount to 10. 
*/
-(void) setCount:(uint32_t)count;

- (id) initWith:(void*) cbiPeerPtr;
- (void*) getCbiPeerVoid;

@end
#endif // #ifndef CBE__GroupFilter_h__
