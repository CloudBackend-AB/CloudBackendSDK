/*
     Copyright © CloudBackend AB 2020 - 2023.
*/

#ifndef INCLUDE_CBE_ITEM_H_
#define INCLUDE_CBE_ITEM_H_
#import <Foundation/Foundation.h>

#include "CBEIOS/Types.h"
/** 
 * @brief set made up of Container and Object
 * 
 * Class for an Item.
 * This is class is the base class of Objects and Containers.
 */
@interface CBEItem : NSObject {
}
/** the ACL of the item */
- (NSString*) aclTag;

/** Returns an Items id. */
- (CBEItemId) id  ;

/** Returns the id of the Items parent container id. */
- (CBEContainerId)    parentId  ;

/** Returns the id of the Items old parent container id
 * in cases where the item has just been moved. */
- (CBEContainerId)    oldParentId  ;

/** Returns the name of the item. */
- (NSString*) name  ;

/** Returns the path if applicable. */
- (NSString*) path  ;

/** Returns the owner id number. */
- (CBEUserId) ownerId  ;

/** Returns which drive number the container resides on. */
- (CBEContainerId)    driveId  ;

/** Returns the username of the owner if applicable. */
- (NSString*) username  ;

/** if the query did get a resultset. */
- (bool) idLoaded  ;

/** if data was loaded */
- (bool) dataLoaded  ;

/** Returns the creation date in Unix epoch time. */
- (CBEDate) created  ;

/**Returns the updated date/time in Unix epoch time*/
- (CBEDate) updated  ;

/** Returns the deleted date, if applicable (i.e. is in the bin), in Unix epoch time*/
- (CBEDate) deleted  ;

- (id) initWith:(void*) cbiPeerPtr;

@end
#endif // INCLUDE_XAPI_ITEM_H_
