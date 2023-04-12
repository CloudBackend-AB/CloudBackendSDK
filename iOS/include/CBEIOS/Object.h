/*
Copyright © CloudBackend AB 2020 - 2023.
*/

#ifndef INCLUDE_CBE_OBJECT_H_
#define INCLUDE_CBE_OBJECT_H_
#include "CBEIOS/delegate/AclDelegate.h"
#include "CBEIOS/delegate/DownloadBinaryDelegate.h"
#include "CBEIOS/delegate/DownloadDelegate.h"
#include "CBEIOS/delegate/DownloadStreamDelegate.h"
#include "CBEIOS/delegate/GetStreamsDelegate.h"
#include "CBEIOS/delegate/object/MoveDelegate.h"
#include "CBEIOS/delegate/object/RemoveDelegate.h"
#include "CBEIOS/delegate/object/RenameDelegate.h"
#include "CBEIOS/delegate/PublishDelegate.h"
#include "CBEIOS/delegate/ShareDelegate.h"
#include "CBEIOS/delegate/UnpublishDelegate.h"
#include "CBEIOS/delegate/UnShareDelegate.h"
#include "CBEIOS/delegate/UnsubscribeDelegate.h"
#include "CBEIOS/delegate/UpdateKeyValuesDelegate.h"
#include "CBEIOS/delegate/UploadStreamDelegate.h"
#include "CBEIOS/Item.h"
#include "CBEIOS/Stream.h"

@protocol GetAclObjectDelegatePtr <AclDelegatePtr>
@end
@protocol SetAclObjectDelegatePtr <AclDelegatePtr>
@end

/**
 * @brief holder of a set of data, can represent a table row.
 * 
 */
@interface CBEObject : CBEItem {
}
/**
 * to relocate an object to a different container.
 * 
 * @param destinationContainerId the id number of the container moving to.
 * @param delegate  Pointer to a delegate::object::MoveDelegate instance
 *                  that is implemented by the user.
 */
-(void) move:(CBEObjectId) destinationContainerId 
        delegate:(id<MoveObjectDelegatePtr>) delegate;

/** 
 * Rename object:
 * @param name string name of the object.
 * @param delegate  Pointer to a delegate::object::RenameDelegate instance
 *                  that is implemented by the user.
 */
-(void) rename:   (NSString*)                   name 
        delegate: (id<RenameObjectDelegatePtr>) delegate;

/**
 * Remove the object from cloud and locally
 * @param delegate  Pointer to a delegate::object::RemoveDelegate instance
 *                  that is implemented by the user.
 */
-(void) remove:(id<RemoveObjectDelegatePtr>) delegate;

/**
 * Adds key/value pair data to the object.
 * \note Existing key will be overwritten, otherwise created.
 * 
 * @param delegate  Pointer to a delegate::UpdateKeyValuesDelegate instance
 *                  that is implemented by the user.
 * @param keyValues Optional map of key/value pairs (metadata).
 */
-(void) updateKeyValues: (id<UpdateKeyValuesObjectDelegatePtr>) delegate 
        keyValues:       (NSDictionary*)                        keyValues;
/**
 * Returns the streams attached to the Object. 
 * Use <code>object.streams()</code> after this
 * call to use the streams e.g. in downloadStream.
 * This method must be re-called if you upload more streams, 
 * see @c uploadStream()
 * @param delegate  Pointer to a delegate::GetStreamsDelegate instance
 *                  that is implemented by the user.
 */
-(void) getStreams: (id<GetStreamsDelegatePtr>) delegate;

/**
 * @brief Returns the currently loaded stream / streams attached to the
 *        object. 
 * If you upload a new stream with @c uploadStream(), 
 * you must call getStreams() again to get the new/updated streams.
 * \note If @c getStreams() has not been called, then @c streams() returns an
 * empty Streams object.
 */
- (NSArray*) streams; 

/**
 * Returns the mime type of the object. \n 
 * E.g.  application/pdf, audio/wav, image/jpg, text/xml, video/mp4 etc.
 */
- (NSString*) getMimeType;
/**
 * Returns the binary length/size of the object in Bytes.
 */
- (uint64_t) length;
/**
 * Returns the Object type currently: Other | GroupInvite | ShareInvite
 */
- (CBEObjectType) getObjectType;
/**
 * Returns all the key/values in a map.
 */
- (NSDictionary*) keyValues;

/**
 * Set the Access control list for the object.
 * @param aclMap  The desired @ref cbe::Permission "permission" for current
 *                object. 
 * @param delegate  Pointer to a delegate::AclDelegate instance that is
 *                  implemented by the user.
 */
- (void) setAcl:   (NSDictionary*)               aclMap 
         delegate: (id<SetAclObjectDelegatePtr>) delegate;

/**
 * get the Access Control List for the Object.
 * @param delegate  Pointer to a delegate::AclDelegate instance
 *                  that is implemented by the user.
 */
-(void) getAcl:(id<GetAclObjectDelegatePtr>) delegate;

/**
 * Share Object to a user. Notifies the user that a share has occured so that
 * the user can check what permissions the have been given. Sharing gives read 
 * permissions as of right now but might change in the future.
 * @param toUserGroup takes a user id or group id (lastly named is for the
 * future) and share to.
 * @param description names the specific share between you and the user/group.
 * @param delegate  Pointer to a delegate::ShareDelegatePtr instance
 *                  that is implemented by the user.
 */
-(void) share:       (CBEUserId)            toUserGroup 
        description: (NSString*)            description 
        delegate:    (id<ShareDelegatePtr>) delegate;

/**
 * unShare the object to a specific shareId created when sharing. Each share 
 * is unique between user/group and the one sharing. This is represented with 
 * a unique share id.
 * @param shareId is as mentioned the unique id for a share between the owner 
 * and other user/group.
 * @param delegate  Pointer to a delegate::UnShareDelegate instance
 *                  that is implemented by the user.
 */
-(void) unShare:  (CBEUserId)              toUserGroup 
        delegate: (id<UnShareDelegatePtr>) delegate;
/**
 * Publishes an object to a user.
 * @param security A cbe::Access enum
 * @param privacy A cbe::WebShareVisibility enum
 * @param description Description
 * @param password Password
 * @param publishDelegate Gets notified when the object has been published 
 * (or if there was an error)
 */

-(void) publish:     (CBEPublishAccess)       security 
        privacy:     (CBEPublishVisibility)   privacy 
        description: (NSString*)              description
        password:    (NSString*)              password 
        delegate:    (id<PublishDelegatePtr>) delegate;

/**
 * Unpublishes this object.
 * @param publishDelegate Gets notified when the object has been unpublished 
 * (or if there was an error)
 */
-(void) unpublish:(id<UnpublishDelegatePtr>) delegate;

/**
 * Unsubscribes from this object.
 * @param subscribeDelegate Gets notified when the object has been 
 * unsubscribed (or if there was an error)
 */
-(void) unsubscribe:(id<UnsubscribeDelegatePtr>) delegate;

- (id) initWith:(void*) cbiPeerPtr;
- (void*) getCBIPeerPtrVoid;

@end
#endif  //INCLUDE_CBE_OBJECT_H_
