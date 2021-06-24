
#ifndef ITEM_EVENT_PROTOCOL_H_
#define ITEM_EVENT_PROTOCOL_H_

#include "CBE/Protocols/ItemEventProtocol.h"
#include "Logic.h"

class ItemEventProtocol : public CBE::ItemEventProtocol {
  public:
    /** Pointer to the Logic class. */
    Logic* cbeTL;
    
    /** Returns the container with the id that have been set on the backend. */
    void onContainerAdded(CBE::ContainerPtr container);
    
    /** Gets called when an error occurred.*/
    void onItemError(CBE::ItemPtr container, CBE::item_t type, CBE::persistence_t operation, CBE::persistence_t failedAtState, uint32_t code, std::string reason, std::string message);
    
    /** Sets a pointer to the Logic class so that we can continue with API calls to the server. */
    ItemEventProtocol(Logic* ptr);
    
    /** Gets called when an Object has been added. */
    void onObjectAdded(CBE::ObjectPtr object);

    /** Gets called with the result of a query that has been done. 
     * The result contains both the filter used for the query and the list with items as the result. */
    void onQueryLoaded(CBE::QueryResultPtr dir);

    /** Gets called if the server returns an error for the query. */
    void onLoadError(CBE::Filter filter, uint32_t operation, uint32_t code, std::string reason, std::string message);
};

#endif
