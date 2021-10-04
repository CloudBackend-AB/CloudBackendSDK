/*
     Copyright © CloudBackend AB 2020.
*/

#ifndef CBE_ITEMEVENTPROTOCOL_H_
#define CBE_ITEMEVENTPROTOCOL_H_

#include "CBE/Types.h"
#include "CBE/Item.h"
#include "CBE/QueryResult.h"
#include "CBE/Filter.h"

namespace CBE {
  /** Protocol for implementing a Item delegate.
   * Abstract base class which is used when you want notification about
   * changes in folder and on documents. */
    class QueryResult;

  class ItemEventProtocol {
    public:
      virtual ~ItemEventProtocol() {
      }

      /** Gets called when a Object has been added. */
      virtual void onObjectAdded(CBE::ObjectPtr object){};

      /** Gets called when Metadata has been saved/Added. */
      virtual void onMetadataAdded(CBE::ObjectPtr object){};

      /** Gets called when a Object has been moved. */
      virtual void onObjectMoved(CBE::ObjectPtr object){};

      /** Gets called when a Object has been removed. */
      //virtual void onObjectRemoved(CBE::ObjectPtr object){};
      virtual void onObjectRemoved(CBE::item_id_t objectId, std::string name){};

      /** Gets called when a Object has been renamed. */
      virtual void onObjectRenamed(CBE::ObjectPtr object){};

      /** Gets called when a Object has been Updated. */
      virtual void onObjectUpdated(CBE::ObjectPtr object){};

      /** Gets called when a Object has been Updated. */
      virtual void onStreamsLoaded(CBE::ObjectPtr object){};

      /** Gets called when a Container has been added. */
      virtual void onContainerAdded(CBE::ContainerPtr container){};

      /** Gets called when a Container has been moved. */
      virtual void onContainerMoved(CBE::ContainerPtr container){};

      /** Gets called when a Container has been removed. */
      virtual void onContainerRemoved(CBE::item_id_t containerId, std::string name){};

      /** Gets called when a Container has been renamed. */
      virtual void onContainerRenamed(CBE::ContainerPtr container){};

      /** Gets called when a Container has been restored. */
      virtual void onContainerRestored(CBE::ContainerPtr container){};

      /** Gets called when a Query has been loaded either from cache or edge / cloud */
      virtual void onQueryLoaded(CBE::QueryResultPtr dir){};

      /** If a query fail e.g a filter requesting a container or object that does not exists, a loadError will happen.
       * "In the future when there will be micro clouds with edge nodes including central clouds you will be able to ask for containers and objects 
       * from different peering points in the hierarchy and might then fail if the object or container is not loacated in that point (due to settings in where you want data replicated).
       */
      virtual void onLoadError(CBE::Filter filter, uint32_t operation, uint32_t code, std::string reason, std::string message){};

      /** Gets called when an error regarding an item occurred. e.g create rename, move, remove */
      virtual void onItemError(CBE::ItemPtr container, CBE::item_t type, uint32_t operation, uint32_t failedAtState, uint32_t code, std::string reason, std::string message){};


  };
}

#endif //CBE_ITEMEVENTPROTOCOL_H_
