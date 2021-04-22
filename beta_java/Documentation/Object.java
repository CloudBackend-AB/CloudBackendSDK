/**
 * Copyright © CloudBackend AB 2021.
 */
package com.cbe;

public class Object extends Item {
  
  public synchronized void delete() {}

  /** 
   * Move object to either a container or the root container:
        * @param destinationContainerId: destionation container Id.
        * @param delegate: Delegate is a shared pointer to the an ItemEventProtocol that the user has implemented. The pointer is defined in Types.h.
  */
  public void move(java.math.BigInteger destinationContainerId, ItemEventProtocol delegate) {}

  /** 
   * Rename object:
        * @param name string name of the object.
        * @param delegate: Delegate is a shared pointer to an ItemEventProtocol that the user has implemented. The pointer is defined in Types.h.
  */
  public void rename(String name, ItemEventProtocol delegate) {}

  /** 
   * Remove the object from cloud.
        * @param delegate: Delegate is a shared pointer to an ItemEventProtocol that the user has implemented. The pointer is defined in Types.h.
  */
  public void remove(ItemEventProtocol delegate) {}

  /** 
   * Download object with path from a container to local path:
        * @param path to where it will be downloaded.
        * @param delegate: Delegate is a shared pointer to a TransferEventProtocol that the user has implemented.  The pointer is defined in Types.h.
      */
  public void download(String path, TransferEventProtocol delegate) {}

  /**
   *  Download object with binary data from the object.  This data is on the heap and you are responsible for calling delete on it, in java use delete function.
       * @param delegate: Delegate is a shared pointer to a TransferEventProtocol that the user has implemented. The pointer is defined in Types.h.
       * Object will be downloaded to the home folder of the device.
  */
  public void download(TransferDownloadEventProtocolSwigImpl delegate) {}

  /**
   * Downloads the stream # with stream id, to a path of the your choice. To get which streams are added on a object use getStream.   
        * @param path: Select which path you want to download the stream to.
        * @param stream: Get which stream you want by first calling getStream and then choose which one to download.
        * @param delegate: Delegate is a shared pointer to a TransferEventProtocol that the user has implemented. The pointer is defined in Types.h.
  */
  public void downloadStream(String path, Stream stream, TransferEventProtocol delegate) {}

  /**
   * Returns the streams attached to the Object, use object->_streams after this call to use the streams e.g in downloadStream.   
        * @param delegate: Delegate is a shared pointer to a ItemEventProtocol that the user has implemented. The pointer is defined in Types.h.
  */
  public void getStreams(ItemEventProtocol delegate) {}

  /**
   * Returns the currently loaded stream / streams attached to the object. (NB if getStreams has not been called then streams() will return NULL).
  */
  public Streams_vec streams() {}

  /**
   * Returns the mime type of the object e.g xml/text or jpg e.t.c 
  */
  public String getMimeType() {}
  
  /**
     * Returns the binary length/size of the object.
  */
  public long length() {}

  /**
    * Share Object to a user. Notifies the user that a share has occured so that the user can check what permissions the've been given. Shareing gives read permissions as of right now but might change in the future.
    * @param toUserGroup takes a user id or group id (lastly named is for the future) and share to.
    * @param description names the specific share between you and the user/group.
    * @param Delegate. Delegate is a shared pointer to the class in wich you implement CBE::ShareDelegate. CBE::ShareDelegate is defined in Types.h and is a shared pointer to an ShareEventProtocol.
  */
  public void share(java.math.BigInteger toUserGroup, String description, ShareEventProtocol delegate) {}

  /**
    * unShare the object to a specific shareId created when sharing. Each share is unique between user/group and the one sharing. This is represented with a unique share id.
    * @param shareId is as mentioned the unique id for a share between the owner and other user/group.
    * @param Delegate. Delegate is a shared pointer to the class in wich you implement CBE::ShareDelegate. CBE::ShareDelegate is defined in Types.h and is a shared pointer to an ShareEventProtocol.
  */
  public void unShare(java.math.BigInteger shareId, ShareEventProtocol delegate) {}

  public Object() {}

}
