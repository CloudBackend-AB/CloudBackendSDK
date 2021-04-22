/**
 * Copyright © CloudBackend AB 2021.
 */

/** Protocol for implementing a Share delegate.
  * Abstract base class which is used when you want notification about
  * changes in shares regarding containers and objects. */

/** @Override the functions that you want to use ex: share (found in Container and Object class) needs respective onContainer- and onObjectShared(..). */

package com.cbe;

public class ShareEventProtocol {
 
  /** Delete the underlying shared pointer object to ShareEventProtocol */
  public synchronized void delete() {}

  /** Gets called when a query for available shares that have been shared to you. */
  public void onListAvailableShares(QueryResult result) {}

  /** Gets called when a query for shares that you have shared to other accounts or groups. */
  public void onListMyShares(QueryResult qResult) {}

  /** Gets called when a Container has been shared
   * @param shareId is the id of the share and is needed when unSharing and can be used to locate which user is tied to that share. 
  */
  public void onContainerShared(java.math.BigInteger shareId) {}

  /** Gets called when a Container has been unShared, message will be OK else an shareError will be thrown.  */
  public void onContainerUnShared(String message) {}

  /** Gets called when an Object has been shared. */
  public void onObjectShared(java.math.BigInteger shareId) {}

  /** Gets called when an Object has been unshared, massage will be OK, if not ok there will be a shareError thrown. */
  public void onObjectUnShared(String message) {}

  /** Errors regarding share and list share calls onShareError. */
  public void onShareError(int type, long operation, long code, String reason, String message) {}

}
