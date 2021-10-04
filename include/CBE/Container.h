/*
     Copyright © CloudBackend AB 2020.
*/

#ifndef INCLUDE_CBE_CONTAINER_H_
#define INCLUDE_CBE_CONTAINER_H_

#include <string>
#include "CBE/Item.h"
#include "CBE/Object.h"
#include "CBE/Protocols/ItemEventProtocol.h"
#include "CBE/Types.h"

namespace CBE
{

class Container : virtual public CBE::Item
{
    class ObjectUpdater;

public:
    /**
   * Create's a container Inside this containerto be used for adding objects.
        * @param name for the container.
        * @param delegate: Delegate is a shared pointer to an ItemEventProtocol that the user has implemented.  The pointer is defined in Types.h.
    */
    virtual ContainerPtr create(const std::string &name, CBE::ItemDelegatePtr delegate);

    /** 
   * Move is used to move container whith its content to user specified location ex other container or to root container.
        * @param destinationId: id of the container to which it should be moved to.
        * @param delegate: Delegate is a shared pointer to an ItemEventProtocol that the user has implemented.  The pointer is defined in Types.h.
    */
    virtual void move(CBE::container_id_t destinationId, CBE::ItemDelegatePtr delegate);

    /** 
   * Rename the container.
        * @param name (new name).
        * @param delegate: Delegate is a shared pointer to an ItemEventProtocol that the user has implemented.  The pointer is defined in Types.h.
    */
    virtual void rename(const std::string &name, CBE::ItemDelegatePtr delegate);

    /** 
   * Removes/deletes the container and all its content.
        * @param delegate: Delegate is a shared pointer to an ItemEventProtocol that the user has implemented.  The pointer is defined in Types.h.
    */
    virtual void remove(CBE::ItemDelegatePtr delegate);

    /**
   * Creates an object with indexed tags or indexed tags + non indexed tags.
        * @param name        Name of the object.
        * @param delegate    Delegate is a shared pointer to an ItemEventProtocol that the user has implemented.  The pointer is defined in Types.h.
        * @param metadata    (Optional) Metadata is a map with the tag (key), the value of that tag and if it is indexed or not
    */
    virtual CBE::ObjectPtr createObject(std::string name, CBE::ItemDelegatePtr delegate, std::map<std::string, SDK_tuple<std::string, bool>> metadata = metadata_type());

    /** 
   * Upload object to container with path, the object is instantly returned with a temp. id. Once the respons from the server is called back the object gets updated with the correct unique object id:
       * @param name.
       * @param path.
       * @param delegate: Delegate is a shared pointer to an TransferEventProtocol that the user has implemented.  The pointer is defined in Types.h.
    */
    virtual CBE::ObjectPtr upload(const std::string &name, const std::string &path, CBE::TransferUploadDelegatePtr delegate);

    /** 
   * Upload object to container with binary data:
       * @param name.
       * @param length (size of file).
       * @param byteData (char pointer to an array containing the data).
       * @param delegate: Delegate is a shared pointer to an TransferEventProtocol that the user has implemented.  The pointer is defined in Types.h.
    */
    virtual CBE::ObjectPtr upload(const std::string &name, uint64_t length, char *byteData, CBE::TransferUploadDelegatePtr delegate);

    /**
    *  Call to get a list of items in the container. Implement onQueryLoaded to from CBE::ItemDelegatePtrto recieve the callback.
       * @param delegate is a shared pointer to the class in which you implement CBE::ItemDelegate. CBE::ItemDelegate is defined in Types.h and is a shared pointer to an ItemEventProtocol.       
    */
    virtual void query(CBE::ItemDelegatePtr delegate);

    /**
       *  Call to get a list of items in the contianer. Implement onQueryLoaded to from CBE::ItemDelegatePtrto recieve the callback.
       * @param filter
       * @param delegate is a shared pointer to the class in which you implement CBE::ItemDelegate. CBE::ItemDelegate is defined in Types.h and is a shared pointer to an ItemEventProtocol.       
        */
    virtual void query(CBE::Filter filter, CBE::ItemDelegatePtr delegate);

    /**
     * set the Access control list for the container. For containers set does set the whole container tree, so all its' sub items as well. Remeber this is set and not update so everytime you set all ids' that should be there should be added.
     * @param toUserPermissions is a std::map of uint64_t and signed int (userId and permission), simply setting the permission you want to a valid user id that you want. CBE::permission_status_t is defined in Types.h.
     * @param Delegate. Delegate is a shared pointer to the class in wich you implement CBE::ShareDelegate. CBE::ShareDelegate is defined in Types.h and is a shared pointer to an ShareEventProtocol.
    */
    virtual void setACL(std::map<CBE::user_id_t, CBE::permission_status_t> toUserPermissions, CBE::ShareDelegatePtr delegate);

    /**
     * get the Access Control List for the Container.
     * @param Delegate. Delegate is a shared pointer to the class in wich you implement CBE::ShareDelegate. CBE::ShareDelegate is defined in Types.h and is a shared pointer to an ShareEventProtocol.
    */
    virtual void getACL(CBE::ShareDelegatePtr delegate);

    /**
    * Shares a container and its content to a user.  This provides the user the ability to access what has been shared to them via the listAvailableShares command.  To allow users to view and change shared information see ACLs. 
    * Note: At present Shareing the container gives the user read permissions for the container and all its sub-items, this might change in the future.
    * @param toUserGroup takes a user id or group id to share to.
    * @param description names the specific share between you and the user/group.
    * @param Delegate. Delegate is a shared pointer to the class in wich you implement
CBE::ShareDelegate. CBE::ShareDelegate is defined in Types.h and is a shared pointer to an
ShareEventProtocol.
    */
    virtual void share(user_id_t toUserGroup, std::string description, CBE::ShareDelegatePtr delegate);

    /**
    * unShare the container to a specific shareId created when sharing. Each share is unique between user/group and the one sharing. This is represented with a unique share id.
    * @param shareId is as mentioned the unique id for a share between the owner and other user/group.
    * @param Delegate. Delegate is a shared pointer to the class in wich you implement CBE::ShareDelegate. CBE::ShareDelegate is defined in Types.h and is a shared pointer to an ShareEventProtocol.
    */
    virtual void unShare(uint64_t shareId, CBE::ShareDelegatePtr delegate);

   protected:
    typedef std::map<std::string, SDK_tuple<std::string, bool>> metadata_type;
    /// This function should not be called directly.
    Container() : CBE::Item() {}
};
} // namespace CBE

#endif // INCLUDE_CBE_CONTAINER_H_
