
#include "ItemEventProtocol.h"

#include <stdio.h>
#include <iostream>

/** Gets called when a Container has been added. */
void ItemEventProtocol::onContainerAdded(CBE::ContainerPtr container) {
  std::cout << "Container Added: " << container->name() << std::endl;
  cbeTL->logic();
}

/** Gets called when an error occurred. */
void ItemEventProtocol::onItemError(CBE::ItemPtr container, CBE::item_t type, CBE::persistence_t operation, CBE::persistence_t failedAtState, uint32_t code, std::string reason, std::string message) {
  std::cout << "Item Event Error on: " << container->name() << std::endl;
  cbeTL->programFinished();
}

/** Gets called when an Object has been added. */
void ItemEventProtocol::onObjectAdded(CBE::ObjectPtr object) {
  std::cout << "Object Added: " << object->name() << std::endl;
  cbeTL->logic();
}

/** Gets called with a query result.*/
void ItemEventProtocol::onQueryLoaded(CBE::QueryResultPtr q) {
  std::cout << "Query loaded" << std::endl;
  cbeTL->saveQueryResultContinue(q);
}

/** Gets called when loading a query fails. */
void ItemEventProtocol::onLoadError(CBE::Filter filter, uint32_t operation, uint32_t code, std::string reason, std::string message) {
  std::cout << "Item Event Load Error" << std::endl;
  cbeTL->programFinished();
}

ItemEventProtocol::ItemEventProtocol(Logic* ptr) {
  cbeTL = ptr;
}
