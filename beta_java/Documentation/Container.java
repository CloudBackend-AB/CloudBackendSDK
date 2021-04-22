/**
 * Copyright © CloudBackend AB 2021.
 */
package com.cbe;

public class Container extends Item {

  public synchronized void delete() {}

  /**
   * Create's a container Inside this containerto be used for adding objects.
        * @param name for the container.
        * @param delegate: Delegate is a shared pointer to an ItemEventProtocol that the user has implemented.  The pointer is defined in Types.h.
  */
  public Container create(String name, ItemEventProtocol delegate) {}

  /** 
   * Move is used to move container whith its content to user specified location ex other container or to root container.
        * @param destinationId: id of the container to which it should be moved to.
        * @param delegate: Delegate is a shared pointer to an ItemEventProtocol that the user has implemented.  The pointer is defined in Types.h.
  */
  public void move(java.math.BigInteger destinationId, ItemEventProtocol delegate) {}

  /** 
   * Rename the container.
        * @param name (new name).
        * @param delegate: Delegate is a shared pointer to an ItemEventProtocol that the user has implemented.  The pointer is defined in Types.h.
  */
  public void rename(String name, ItemEventProtocol delegate) {}

  /** 
   * Removes/deletes the container and all its content.
        * @param delegate: Delegate is a shared pointer to an ItemEventProtocol that the user has implemented.  The pointer is defined in Types.h.
  */
  public void remove(ItemEventProtocol delegate) {}

  /** 
   * Upload object to container with path, the object is instantly returned with a temp. id. Once the respons from the server is called back the object gets updated with the correct unique object id:
       * @param name.
       * @param path.
       * @param delegate: Delegate is a shared pointer to an TransferEventProtocol that the user has implemented.  The pointer is defined in Types.h.
  */
  public Object upload(String name, String path, TransferEventProtocol delegate) {}

  /** 
   * Upload object to container with binary data:
       * @param name.
       * @param data (byte array containing the data).
       * @param delegate: Delegate is a shared pointer to an TransferEventProtocol that the user has implemented.  The pointer is defined in Types.h.
  */
  public Object uploadBinary(String name, byte[] data, TransferEventProtocol delegate) {}

  /**
    *  Call to get a list of items in the container. Implement onQueryLoaded to from CBE::ItemDelegatePtrto recieve the callback.
       * @param delegate is a shared pointer to the class in which you implement CBE::ItemDelegate. CBE::ItemDelegate is defined in Types.h and is a shared pointer to an ItemEventProtocol.       
  */
  public void query(ItemEventProtocol delegate) {}

  /**
       *  Call to get a list of items in the contianer. Implement onQueryLoaded to from CBE::ItemDelegatePtrto recieve the callback.
       * @param filter
       * @param delegate is a shared pointer to the class in which you implement CBE::ItemDelegate. CBE::ItemDelegate is defined in Types.h and is a shared pointer to an ItemEventProtocol.       
  */
  public void query(Filter filter, ItemEventProtocol delegate) {}

}
