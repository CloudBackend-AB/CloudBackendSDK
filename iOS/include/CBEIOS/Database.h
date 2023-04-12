/*
     Copyright © CloudBackend AB 2022.
*/

#ifndef _CBE_DATABASE_H_
#define _CBE_DATABASE_H_
#import <Foundation/Foundation.h>

#include "CBEIOS/Types.h"
#include "CBEIOS/Container.h"
@interface CBEDatabase : NSObject {

}

/**
 * the userId number of the owner
 * 
 * @return cbe::UserId 
 */
-(CBEUserId) ownerId;
/**
 * the driveId number
 * 
 * @return cbe::DriveId 
 */
-(CBEDriveId) driveId;
/**
 * the containerId number
 * 
 * @return cbe::ContainerId    
 */
-(CBEContainerId) folderId;
/**
 * the database name
 * 
 * @return std::string 
 */
-(NSString*) name;
/**
 * if it has a read lock
 * 
 * @return true 
 * @return false 
 */
-(BOOL) readLocked;
/**
 * if it has a write lock
 * 
 * @return true 
 * @return false 
 */
-() writeLocked;
/**
 *  when it was created, as unix epoch number
 * 
 * @return cbe::Date 
 */
-(CBEDate) created;
/**
 * the top container of the database
 * 
 * @return cbe::Container 
 */
-(CBEContainer*) rootContainer;

#endif   _CBE_DATABASE_H_
@end