/**
 * Copyright © CloudBackend AB 2021.
 */

/** Protocol for implementing a Item delegate.
  * Abstract base class which is used when you want notification about
  * changes in Containers and objects. */

/** @Override the functions that you want to use ex: createObject (found in class Container) needs onObjectAdded(Object object). */
  
package com.cbe;

public class ItemEventProtocol {

  /** Delete the underlying shared pointer object to ItemEventProtocol */
  public synchronized void delete() {}

  /** Gets called when a Object has been added. */
  public void onObjectAdded(Object object) {}

  /** Gets called when Metadata has been saved/Added. */
  public void onMetadataAdded(Object object) {}

  /** Gets called when a Object has been moved. */
  public void onObjectMoved(Object object) {}

  /** Gets called when a Object has been removed. */
  public void onObjectRemoved(java.math.BigInteger objectId, String name) {}

  /** Gets called when a Object has been renamed. */
  public void onObjectRenamed(Object object) {}

  /** Gets called when a Object has been Updated. */
  public void onObjectUpdated(Object object) {}

  /** Gets called when a stream has been loaded to an object. */
  public void onStreamsLoaded(Object object) {}

  /** Gets called when a Container has been added. */
  public void onContainerAdded(Container container) {}

  /** Gets called when a Container has been moved. */
  public void onContainerMoved(Container container) {}

  /** Gets called when a Container has been removed. */
  public void onContainerRemoved(java.math.BigInteger containerId, String name) {}

  /** Gets called when a Container has been renamed. */
  public void onContainerRenamed(Container container) {}

  /** Gets called when a Container has been restored. */
  public void onContainerRestored(Container container) {}

  /** Gets called when a Query has been loaded either from cache or edge / cloud */
  public void onQueryLoaded(QueryResult dir) {}

  /** If a query fail e.g a filter requesting a container or object that does not exists, a loadError will happen.
   * "In the future when there will be micro clouds with edge nodes including central clouds you will be able to ask for containers and objects 
   * from different peering points in the hierarchy and might then fail if the object or container is not loacated in that point (due to settings in where you want data replicated).
  */
  public void onLoadError(Filter filter, long operation, long code, String reason, String message) {}

  /** Gets called when an error regarding an item occurred. e.g create rename, move, remove */
  public void onItemError(Item container, int type, long operation, long failedAtState, long code, String reason, String message) {}

}
