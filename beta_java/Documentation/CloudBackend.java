/**
 * Copyright © CloudBackend AB 2021.
 */
package com.cbe;

public class CloudBackend {

  /** Call delete on the cloudbackend object to remove any library refrences to the object. */
  public synchronized void delete() {}

  /** Used for login in on the CloudBackend account. Returns the cloudbackend object containing the account and rootContainer. */
  public static CloudBackend logIn(String username, String password, String source, AccountEventProtocol delegate) {}

  /** Add event listener on the account (only include events regarding Item event protocol and transfer event protocol (upload/download)) */
  public void addListener(ItemEventProtocol delegate) {}

  /** Removes the Event listener for the account. */
  public void removeListener(ItemEventProtocol delegate) {}

  /** Call query on container with container id, containerId, with a preset Filter for ItemTypes COntainer and Objects in ascending order. */
  public void query(java.math.BigInteger containerId, ItemEventProtocol delegate) {}

  /** Call query on container with container id, containerId, and with Filter filter. */
  public void query(java.math.BigInteger containerId, Filter filter, ItemEventProtocol delegate) {}

  /** Casts items in to containers (if ItemType is a Container) */
  public static Container castContainer(Item item) {}

  /** Casts Items in to Objects (if ItemType is a Object) */
  public static Object castObject(Item item) {}

  /** Returns the cloudbackend account, callable API:s /methods for the account found in the account class. */
  public Account account() {}

  /** Returns version of the sdk that these wrappers where built on. For wrapper version check the .jar file for wrapper version. */ 
  public String version() {}

  /** Returns the shareManager which is exposing calls from the shareManager class. */
  public ShareManager shareManager() {}

}
