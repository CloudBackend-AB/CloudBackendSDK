/*
     Copyright © CloudBackend AB 2020 - 2023.
*/

#ifndef INCLUDE_CBE_CONTAINER_H_
#define INCLUDE_CBE_CONTAINER_H_

#include "CBEIOS/delegate/AclDelegate.h"
#include "CBEIOS/delegate/container/MoveDelegate.h"
#include "CBEIOS/delegate/container/RemoveDelegate.h"
#include "CBEIOS/delegate/container/RenameDelegate.h"
#include "CBEIOS/delegate/CreateContainerDelegate.h"
#include "CBEIOS/delegate/CreateObjectDelegate.h"
#include "CBEIOS/delegate/PublishDelegate.h"
#include "CBEIOS/delegate/QueryDelegate.h"
#include "CBEIOS/delegate/QueryJoinDelegate.h"
#include "CBEIOS/delegate/ShareDelegate.h"
#include "CBEIOS/delegate/UnpublishDelegate.h"
#include "CBEIOS/delegate/UnShareDelegate.h"
#include "CBEIOS/delegate/UnsubscribeDelegate.h"
#include "CBEIOS/delegate/UploadDelegate.h"
#include "CBEIOS/Item.h"
#include "CBEIOS/QueryChain.h"

@protocol GetAclDelegatePtr <AclDelegatePtr>
@end
@protocol SetAclDelegatePtr <AclDelegatePtr>
@end

/**
 * @brief collection of Item, can represent a table.
 *
 */
@interface CBEContainer : CBEItem {
}

/**
 * Creates a container inside this container to be used for adding objects.
 * @param name Name of the container to be created.
 * @param delegate Pointer to a delegate::CreateContainerDelegate instance that is
 *                 implemented by the user.
 */
-(CBEContainer*) createContainer: (NSString*)                      name
                 delegate:        (id<CreateContainerDelegatePtr>) delegate;

/**
 * Move is used to move container with its content to user specified location
 * e.g. other container or to root container.
 * @param destinationId: id of the container to which it should be moved to.
 * @param delegate Pointer to a delegate::container::MoveDelegate instance
 *                 that is implemented by the user.
 */
-(void) move:     (CBEContainerId)               destinationId
        delegate: (id<MoveContainerDelegatePtr>) delegate;

/**
 * Rename the container.
 * @param name New name of the container.
 * @param delegate Pointer to a delegate::container::RenameDelegate instance
 *                 that is implemented by the user.
 */
-(void) rename: (NSString*)                      name
      delegate: (id<RenameContainerDelegatePtr>) delegate;

/**
 * Removes/deletes the container and all its content.
 * @param delegate Pointer to a delegate::container::RemoveDelegate instance
 *                 that is implemented by the user.
 */
-(void) remove: (id<RemoveContainerDelegatePtr>) delegate;

/**
 * Creates an object with indexed tags or indexed tags + non indexed tags.
 * @param name      Name of the object.
 * @param delegate  Pointer to a delegate::CreateObjectDelegate instance
 *                  that is implemented by the user.
 * @param keyValues Optional map of key/value pairs (metadata).
 */
-(CBEObject*) createObject: (NSString*)                   name 
              delegate:     (id<CreateObjectDelegatePtr>) delegate 
              keyValues:    (NSDictionary*)               keyValues;

/**
 * @param name   name that the uploaded object will get
 * @param length size of file in Bytes
 * @param byteData (char pointer to an array containing the data).
 * @param delegate  Pointer to a delegate::UploadDelegate instance
 *                  that is implemented by the user.
 */
-(CBEObject*) upload:   (NSString*)              name
              length:   (uint64_t)               length
              data:     (NSData*)                data
              delegate: (id<UploadDelegatePtr>)  delegate;

/**
 * In line with function
 * CloudBackend::query(ContainerId,QueryDelegatePtr), but with
 * its \p ContainerId parameter excluded.
 * \see CloudBackend::query(ContainerId,QueryDelegatePtr)
 */
-(CBEQueryChain*) query: (id<QueryDelegatePtr>)  queryDelegate;

/**
 * In line with function
 * CloudBackend::query(ContainerId,Filter,QueryDelegatePtr), but
 * with its \p ContainerId parameter excluded.
 * \see CloudBackend::query(ContainerId,Filter,QueryDelegatePtr)
 */
-(CBEQueryChain*) query:    (CBEFilter*)           filter
                  delegate: (id<QueryDelegatePtr>) queryDelegate;

/**
 * Queries the container with a given relative path, returns container with
 * objects. \n
 * E.g. /Documents/Pictures will return objects and subContainers for
 * Pictures. \n
 * N.B.: .. or . path options are not available,
 * top down Paths in the container tree are available.
 * @param path container path, e.g. /Documents/Pictures
 * @param delegate  Pointer to a delegate::QueryDelegate instance
 *                  that is implemented by the user.
 */

/**
 * Set the Access Control List \c ACL for the container.
 * For containers set does set the whole container tree,
 * so all its sub items as well.
 * Remember this is  set and not update so every time you set
 * all ids that should be there should be added.
 * @param aclMap    The desired @ref cbe::Permission "permission" for current
 *                  container.
 * @param delegate  Pointer to a delegate::AclDelegate instance
 *                  that is implemented by the user.
 */
- (void) setAcl:   (NSDictionary*)         aclMap 
         delegate: (id<SetAclDelegatePtr>) delegate;

/**
 * get the Access Control List \c ACL of the Container.
 * @param delegate  Pointer to a delegate::AclDelegate instance
 *                  that is implemented by the user.
 */
  -(void) getAcl:(id<GetAclDelegatePtr>) delegate;

/**
 * Shares a container and its content to a user.  This provides the user the
 * ability to access what has been shared to them via the listAvailableShares
 * command.  To allow users to view and change shared information see \c ACL .
 * \note At present Sharing the container gives the user read permissions for
 * the container and all its sub-items, this might change in the future.
 * @param toUserGroup takes a user id or group id to share to.
 * @param description names the specific share between you and the user/group.
 * @param delegate  Pointer to a delegate::ShareDelegate instance
 *                  that is implemented by the user.
 */
-(void) share:        (CBEUserId)            toUserGroup 
        description:  (NSString*)            description 
        delegate:     (id<ShareDelegatePtr>) delegate;

/**
 * unShare the container to a specific shareId created when sharing. Each
 * share is unique between user/group and the one sharing. This is represented
 * with a unique share id.
 * @param shareId is as mentioned the unique id for a share between the owner
 * and other user/group.
 * @param delegate  Pointer to a delegate::UnShareDelegate instance
 *                  that is implemented by the user.
 */
-(void) unShare:  (CBEShareId)             shareId
        delegate: (id<UnShareDelegatePtr>) delegate;

/**
 * Publishes a container and its content to a user.
 * @param security A cbe::Access enum
 * @param privacy A cbe::PublishVisibility enum
 * @param description Description
 * @param password Password
 * @param delegate  Pointer to a delegate::PublishDelegate instance
 *                  that is implemented by the user.
 */
-(void) publish:      (CBEPublishAccess)       security 
        privacy:      (CBEPublishVisibility)   privacy 
        description:  (NSString*)              description 
        password:     (NSString*)              password 
        delegate:     (id<PublishDelegatePtr>) delegate;

/**
 * Unpublishes this container.
 * @param delegate Gets notified when the container has been
 * unpublished (or if there was an error)
 */
-(void) unpublish:(id<UnpublishDelegatePtr>) delegate;

/**
 * Unsubscribes from this container.
 * @param delegate Gets notified when the container has been
 * unsubscribed (or if there was an error)
 */
-(void) unsubscribe:(id<UnsubscribeDelegatePtr>) delegate;

- (id) initWith:(void*) cbiPeerPtr;
- (void*) getCBIPeerPtrVoid;

@end

#endif // INCLUDE_CBE_CONTAINER_H_
