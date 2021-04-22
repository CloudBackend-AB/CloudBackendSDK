/**
 * Copyright © CloudBackend AB 2021.
 */

/** Protocol for implementing a Account delegate.
  * Abstract base class which is used when you want notification about
  * changes on the cloudbackend account. */

/** @Override the functions that you want to use ex: Login needs onLogin. */  

package com.cbe;

public class AccountEventProtocol {

  /** Delete the underlying shared pointer object to AccountEventProtocol */
  public synchronized void delete() {}

  /** Gets called when the account status has changed (required). */
  public void onLogin(long atState, CloudBackend cloudbackend) {}

  /** Gets called when the account status has changed (required). */
  public void onLogout(long atState) {}

  /** Gets called when the account status has changed (required). */
  public void onCreated(long atState) {}

  /** Gets called when the account status has changed (required). */
  public void onError(long failedAtState, long code, String reason, String message) {}

}
