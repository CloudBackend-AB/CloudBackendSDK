/*
     Copyright © CloudBackend AB 2020 - 2023..
*/

#ifndef CBE_ITEMEVENTPROTOCOL_H_
#define CBE_ITEMEVENTPROTOCOL_H_

#include "CBE/Types.h"
//#include "CBE/Item.h"
//#include "CBE/Container.h"
//#include "CBE/QueryResult.h"
//#include "CBE/Filter.h"

namespace cbe {
  class Filter;

/** Protocol for implementing a Item delegate.
 * Abstract base class which is used when you want notification about
 * changes in folder and on documents. */
class ItemEventProtocol {
public:
  virtual ~ItemEventProtocol();

  /** Gets called when a Object has been added. */
  virtual void onObjectAdded(cbe::Object&& object);

  /** Gets called when Metadata has been saved/Added. */
  virtual void onMetadataAdded(cbe::Object&& object);

  /** Gets called when a Object has been moved. */
  virtual void onObjectMoved(cbe::Object&& object);

  /** Gets called when a Object has been removed. */
  //virtual void onObjectRemoved(cbe::Object&& object);
  virtual void onObjectRemoved(cbe::ItemId objectId, std::string name);

  /** Gets called when a Object has been renamed. */
  virtual void onObjectRenamed(cbe::Object&& object);

  /** Gets called when a Object has been Updated. */
  virtual void onObjectUpdated(cbe::Object&& object);

  /** Gets called when a Object has been Updated. */
  virtual void onStreamsLoaded(cbe::Object&& object);

  /** Gets called when a Container has been added. */
  virtual void onContainerAdded(cbe::Container&& container);

  /** Gets called when a Container has been moved. */
  virtual void onContainerMoved(cbe::Container&& container);

  /** Gets called when a Container has been removed. */
  virtual void onContainerRemoved(cbe::ItemId containerId, std::string name);

  /** Gets called when a Container has been renamed. */
  virtual void onContainerRenamed(cbe::Container&& container);

  /** Gets called when a Container has been restored. */
  virtual void onContainerRestored(cbe::Container&& container);

  /** Gets called when a Query has been loaded either from cache or edge / cloud */
  virtual void onQueryLoaded(cbe::QueryResult&& dir);

  /** If a query fail e.g a filter requesting a container or object that does not exists, a loadError will happen.
   * "In the future when there will be micro clouds with edge nodes including central clouds you will be able to ask for containers and objects
   * from different peering points in the hierarchy and might then fail if the object or container is not loacated in that point (due to settings in where you want data replicated).
   */
  virtual void onLoadError(cbe::Filter&& filter, uint32_t code, std::string reason, std::string message);

  /** Gets called when an join error has happened, ex: Bad Request in form of key word searching for table does not exist. */
  virtual void onJoinError(uint32_t code, std::string reason, std::string message);

  /** Gets called when an error regarding an item occurred. e.g create rename, move, remove */  //this could be typesafe instead
  virtual void onItemError(cbe::Item&& container, cbe::item_t type, uint32_t code, std::string reason, std::string message);
}; // class ItemEventProtocol

} // namespace cbe

#endif //CBE_ITEMEVENTPROTOCOL_H_
