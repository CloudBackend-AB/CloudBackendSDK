/*
     Copyright © CloudBackend AB 2020 - 2022..
*/

#ifndef INCLUDE_CBE_OBJECT_H_
#define INCLUDE_CBE_OBJECT_H_

#include <string>

#include "CBE/Item.h"
#include "CBE/Stream.h"
#include "CBE/Types.h"
#include "CBE/Protocols/TransferEventProtocol.h"

namespace CBE
{

class Object : virtual public CBE::Item
{
public:
    /** 
   * Move object to either a container or the root container:
        * @param destinationContainerId: destionation container Id.
        * @param delegate: Delegate is a shared pointer to the an ItemEventProtocol that the user has implemented. The pointer is defined in Types.h.
  */
    virtual void move(CBE::container_id_t destinationContainerId, CBE::ItemDelegatePtr delegate);

    /** 
   * Rename object:
        * @param name string name of the object.
        * @param delegate: Delegate is a shared pointer to an ItemEventProtocol that the user has implemented. The pointer is defined in Types.h.
  */

    virtual void rename(const std::string &name, CBE::ItemDelegatePtr delegate);

    /** 
   * Remove the object from cloud and locally
        * @param delegate: Delegate is a shared pointer to an ItemEventProtocol that the user has implemented. The pointer is defined in Types.h.
  */
    virtual void remove(CBE::ItemDelegatePtr delegate);

    /** 
   * Download object with path from a container to local:
        * @param path to where it will be downloaded.
        * @param delegate: Delegate is a shared pointer to a TransferEventProtocol that the user has implemented.  The pointer is defined in Types.h.
      */
    virtual void download(const std::string &path, CBE::TransferDownloadDelegatePtr delegate);

    /**
   *  Download object with binary data from contair and passes it to the delegate.  This is data is on the heap and you are responsible for calling delete on it, though we may change it to a shared pointer in the future.
       * @param delegate: Delegate is a shared pointer to a TransferEventProtocol that the user has implemented. The pointer is defined in Types.h.
  */
    virtual void download(CBE::TransferDownloadDelegatePtr delegate);

    /**
   * Downloads the stream # with stream id, to a path of the your choice. To get which streams are added on a object use getStream.   
        * @param path: Select which path you want to download the stream to.
        * @param streamId: Get which stream you want by first calling getStream and then choose which one to download.
        * @param delegate: Delegate is a shared pointer to a TransferEventProtocol that the user has implemented. The pointer is defined in Types.h.
   */
    virtual void downloadStream(const std::string &path, CBE::Stream stream, CBE::TransferDownloadDelegatePtr delegate);

    /**
   * Adds keyValue data to the existing object, if data has the same name it will be overwritten else itt will add to the existing keyValue on the object.
        * @param delegate: Delegate is a shared pointer to an ItemEventProtocol that the user has implemented. The pointer is defined in Types.h. 
        * @param metadata: (Optional) Metadata is a map with the keyValue (key), the associated value of that key and if it is indexed or not
 */
    virtual void updateKeyValues(CBE::ItemDelegatePtr delegate, std::map<std::string, std::pair<std::string, bool>> metadata = metadata_type());

    /**
   * Returns the streams attached to the Object, use object->_streams after this call to use the streams e.g in downloadStream.   
        * @param delegate: Delegate is a shared pointer to a ItemEventProtocol that the user has implemented. The pointer is defined in Types.h.
   */
    virtual void getStreams(CBE::ItemDelegatePtr delegate);

    /**
   * Returns the currently loaded stream / streams attached to the object. (NB if getStreams has not been called then streams() will return NULL).
   */
    virtual std::vector<CBE::Stream> streams() const;

    /**
   * Returns the mime type of the object e.g xml/text or jpg e.t.c 
   */
    virtual std::string getMimeType() const;

    /**
     * Returns the binary length/size of the object.
    */
    virtual uint64_t length() const;

     /**
      * Returns the Object type currently Other | GroupInvite | ShareInvite
     */
    virtual CBE::object_t getObjectType() const;

    /**
   * Returns all the keyValues
   */
    virtual std::map<std::string, std::pair<std::string, bool>> keyValues();

    /**
     * set the Access control list for the object.
     * @param toUserPermissions is a std::map of uint64_t and signed int (userId and permission), simply setting the permission you want to a valid user id that you want. CBE::permission_status_t is defined in Types.h.
     * @param Delegate. Delegate is a shared pointer to the class in wich you implement CBE::ShareDelegate. CBE::ShareDelegate is defined in Types.h and is a shared pointer to an ShareEventProtocol.
    */
    virtual void setACL(std::map<CBE::user_id_t, CBE::permission_status_t> ACL, CBE::ShareDelegatePtr delegate);

    /**
     * get the Access Control List for the Object.
     * @param Delegate. Delegate is a shared pointer to the class in wich you implement CBE::ShareDelegate. CBE::ShareDelegate is defined in Types.h and is a shared pointer to an ShareEventProtocol.
    */
    virtual void getACL(CBE::ShareDelegatePtr delegate);

    /**
    * Share Object to a user. Notifies the user that a share has occured so that the user can check what permissions the've been given. Shareing gives read permissions as of right now but might change in the future.
    * @param toUserGroup takes a user id or group id (lastly named is for the future) and share to.
    * @param description names the specific share between you and the user/group.
    * @param Delegate. Delegate is a shared pointer to the class in wich you implement CBE::ShareDelegate. CBE::ShareDelegate is defined in Types.h and is a shared pointer to an ShareEventProtocol.
    */
    virtual void share(user_id_t toUserGroup, std::string description, CBE::ShareDelegatePtr delegate);

    /**
    * unShare the object to a specific shareId created when sharing. Each share is unique between user/group and the one sharing. This is represented with a unique share id.
    * @param shareId is as mentioned the unique id for a share between the owner and other user/group.
    * @param Delegate. Delegate is a shared pointer to the class in wich you implement CBE::ShareDelegate. CBE::ShareDelegate is defined in Types.h and is a shared pointer to an ShareEventProtocol.
    */
    virtual void unShare(uint64_t shareId, CBE::ShareDelegatePtr delegate);

    //url to item
    std::string url();

private:
};
} // namespace CBE
// namespace CBE

#endif // INCLUDE_CBE_OBJECT_H_
