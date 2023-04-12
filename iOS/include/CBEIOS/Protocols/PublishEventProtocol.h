/*
     Copyright © CloudBackend AB 2020 - 2023..
*/
#ifndef CBE_PUBLISHEVENTPROTOCOL_H_
#define CBE_PUBLISHEVENTPROTOCOL_H_

#include <vector>

#include "CBE/Item.h"
#include "CBE/Types.h"

namespace cbe {
/** Protocol for implementing a Publish delegate.
 * Abstract base class which is used when you want notification about
 * changes in published folders and documents. */

class PublishEventProtocol {
public:
  virtual ~PublishEventProtocol();

  /** Gets called after the query for published items has completed successfully. */
  virtual void onListMyPublications(cbe::QueryResult&& qResult);

  /** Gets called when Objects/Folders have been published. */
  virtual void onItemsPublished(std::vector<cbe::Item> items);

  /**
   * Gets called when a Objects/Folders have been unpublished.
   * @param publishId Publish id that got unpublished
   * @param itemId Id of item (cbe::Container or cbe::Object) that got unpublished
   */
  virtual void onItemsUnpublished(cbe::PublishId publishId,
                                  cbe::ItemId    itemId);

  /** Errors regarding publish and list publications calls onPublishError. */
  virtual void onPublishError(cbe::item_t   type,
                              std::uint32_t code,
                              std::string   reason,
                              std::string   message);
}; // class PublishEventProtocol

} // namespace cbe

#endif  // CBE_PUBLISHEVENTPROTOCOL_H_
