/*
 Copyright © CloudBackend AB 2020 - 2023..
 */
#ifndef CBE_SUBSCRIBEEVENTPROTOCOL_H_
#define CBE_SUBSCRIBEEVENTPROTOCOL_H_

#include "CBE/Types.h"

namespace cbe {
/** Protocol for implementing a Subscribe delegate.
 * Abstract base class which is used when you want notification about
 * changes in subscribed folders and documents. */
class SubscribeEventProtocol {
public:
  virtual ~SubscribeEventProtocol();

  /** Gets called after the query for subscribed items has completed successfully. */
  virtual void onListMySubscriptions(cbe::QueryResult&& qResult);

  /** Gets called when an Object/Folder has been subscribed to. */
  virtual void onItemSubscribed(std::vector<cbe::Item>&& items);

  /** Gets called when an Object/Folder has been unsubscribed from. */
  virtual void onItemUnsubscribed(cbe::SubscribeId subscribeId,
                                  cbe::ItemId      itemId);

  /** Errors regarding subscribe and list subscriptions calls onSubscribeError. */
  virtual void onSubscribeError(cbe::item_t   type,
                                std::uint32_t code,
                                std::string   reason,
                                std::string   message);
}; // class SubscribeEventProtocol

} // namespace cbe

#endif  // CBE_SUBSCRIBEEVENTPROTOCOL_H_
