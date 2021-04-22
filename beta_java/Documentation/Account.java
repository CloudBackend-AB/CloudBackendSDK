/**
 * Copyright © CloudBackend AB 2021.
 */
package com.cbe;

public class Account {
  
  /** Used to remove references of an account object. */ 
  public synchronized void delete() {}

  /** Returns the account id of the user. */
  public java.math.BigInteger userId() {}

  /// Returns the username of the account.
  public String username() {}

  /// Returns the password of the account.
  public String password() {}

  /// Returns the source of the account.
  public String source() {}

  /// Returns the name of the user.
  public String firstName() {}

  /// Returns the surname of the user.
  public String lastName() {}

  /// Returns the rootContainer for the account
  public Container rootContainer() {}

  /// Returns the libContainerId for the account.  For more information about libContainers see documentation.
  public java.math.BigInteger containerId() {}

  /// Returns the rootContainerId for the account
  public java.math.BigInteger rootDriveId() {}

}
