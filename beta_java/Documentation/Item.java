/**
 * Copyright © CloudBackend AB 2021.
 */
package com.cbe;

public class Item {
  
  public synchronized void delete() {}
  
  /** Get the shareId from userId 
   * @apiNote this has no real implementation in the java wrapper version of 0.2 since ACL:s and shares have not been fully wrapped.
  */
  public java.math.BigInteger getShareFromUserId(java.math.BigInteger userId) {}

  /** Get the shareId from userId 
   * @apiNote this has no real implementation in the java wrapper version of 0.2 since ACL:s and shares have not been fully wrapped.
  */
  public java.math.BigInteger getUserFromShareId(java.math.BigInteger shareId) {}

  /** Setting the Access Control List for an object or container involves reading the tag first. the tag works as a hash key for the ACL settings.
   * @apiNote this has no real implementation in the java wrapper version of 0.2 since ACL:s and shares have not been fully wrapped.
   */
  public String aclTag() {}

  /** description is part of the sharing module and is added to all shares between accounts.
   * @apiNote this has no real implementation in the java wrapper version of 0.2 since ACL:s and shares have not been fully wrapped.
   * @return string with a declarative description of what is being shared.
   */
  public String description() {}

  /** Returns an Items id. */
  public java.math.BigInteger id() {}

  /** Returns the id of the Items parent. */
  public java.math.BigInteger parentId() {}
  public java.math.BigInteger oldParentId() {}

  /** Returns the name. */
  public String name() {}

  /** Returns the path */
  public String path() {}

  /** Returns the owner id. */
  public java.math.BigInteger ownerId() {}

  /** Returns which drive the container resides on. */
  public java.math.BigInteger driveId() {}

  /** Returns the username of the Containers owner. */
  public String username() {}

  /** Returns the creation date in Unix time. */
  public java.math.BigInteger created() {}

  /**Returns the updated date/time in Unix time*/
  public java.math.BigInteger updated() {}

  /** Returns the deleted date in Unix time*/
  public java.math.BigInteger deleted() {}

  /** Returns Container (8) or Object (4). */
  public int type() {}

}
