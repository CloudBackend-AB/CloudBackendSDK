/**
 * Copyright © CloudBackend AB 2021.
 */
package com.cbe;

public class ShareManager {
  
  public synchronized void delete() {}

  /**
   * lists shares that have been shared to you. this will give you information similar to a query but with specific share information.
   * @param delegate ShareDelegatePtr is a shared smart pointer pointing back to shareManager() and the callbacks defined in ShareEventProtocol.
  */
  public void listAvailableShares(ShareEventProtocol delegate) {}

  /**
   * lists shares that have been shared by you. this will give you information similar to a query but with specific share information.
   * @param delegate ShareDelegatePtr is a shared smart pointer pointing back to shareManager() and the callbacks defined in ShareEventProtocol.
  */
  public void listMyShares(ShareEventProtocol delegate) {}

}
