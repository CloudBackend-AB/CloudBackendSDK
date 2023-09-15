/*
     Copyright © CloudBackend AB 2020-2023.
*/

#ifndef INCLUDE_CBE_ITEM_H_
#define INCLUDE_CBE_ITEM_H_

#include <thread>
#include <mutex>
#include <map>
#include <string>
#include <vector>
#include "cbe/Types.h"
#include "cbe/Utility.h"

namespace CBI {
class Item;
using ItemPtr = std::shared_ptr<CBI::Item>;
class ItemEventProtocol;
using ItemDelegatePtr = std::shared_ptr<ItemEventProtocol>;
class PublishEventProtocol;
using PublishDelegatePtr = std::shared_ptr<PublishEventProtocol>;
class SubscribeEventProtocol;
using SubscribeDelegatePtr = std::shared_ptr<SubscribeEventProtocol>;
} // namespace CBI

namespace cbe
{

/** 
 * @brief A set made up of Container and Object.
 * 
 * Class for an Item.
 * This is class is the base class of Objects and Containers.
 */
class Item {
 public:

  /** a map of share ids for this item */
  cbe::ShareIds getShareIds() const;

  /** Get the shareId by reference of userId */
  cbe::ShareId getShareFromUserId(cbe::UserId userId);

  /** Get the userId reference of shareId */
  cbe::UserId getUserFromShareId(cbe::UserId shareId);

  /** the ACL of the item */
  std::string aclTag() const;

  /** A item text description where applicable */
  std::string description() const; 

  /** Returns an Items id. */
  cbe::ItemId id() const;

  /** Returns the id of the Items parent container id. */
  cbe::ContainerId    parentId() const;

  /** Returns the id of the Items old parent container id
   * in cases where the item has just been moved. */
  cbe::ContainerId    oldParentId() const;

  /** Returns the name of the item. */
  std::string name() const;

  /** Returns the path if applicable. */
  std::string path() const;

  /** Returns the owner id number. */
  cbe::UserId ownerId() const;

  /** Returns which drive number the container resides on. */
  cbe::ContainerId    driveId() const;

  /** Returns the username of the owner if applicable. */
  std::string username() const;

  /** if the query did get a resultset. */
  bool idLoaded() const;

  /** if data was loaded */
  bool dataLoaded() const;

  /** Returns the creation date in Unix epoch time. */
  cbe::Date created() const;

  /**Returns the updated date/time in Unix epoch time*/
  cbe::Date updated() const;

  /** Returns the deleted date, if applicable (i.e. is in the bin), in Unix epoch time*/
  cbe::Date deleted() const;

  /** Container or Object, see enum in Types.h */
  cbe::ItemType type() const;

  /**
   * Checks if it has published info and if @c getPublished() returns a valid object.
   */
  bool hasPublished() const;

  /**
   * Gets the publish information
   * @sa hasPublished()
   */
  Publish getPublished() const;

  /**
   * Checks if it has subscribe info and if @c getSubscribe() returns a valid object.
   */
  bool hasSubscribe() const;

  /**
   * Gets the subscribe information
   * @sa hasSubscribe()
   */
  Subscribe getSubscribe() const;

  bool operator<(const cbe::Item& rh) const;

  /** 
   * Retrieve the map of the permissions the users and groups for this item.
   */
  AclMap aclMap() const; 

  //! @cond Doxygen_Suppress
  Item(cbe::DefaultCtor);
  /// This function should not be called as it will not continue to be public
  /// and any call made on the item will fail.
  ~Item();
  //! @endcond

  /**
   * @brief Checks if the inherited instance is real.
   * 
   * An "unreal" instance implies typically a failed event.
   * 
   * Relies on the \c Default \c constructor
   * cbe::DefaultCtor
   * 
   * @return \c true  : is real
   * @return \c false : unreal; got nullptr; if current instance is unbound/undefined. I.e., if it is
   *         only default constructed.
   */
  explicit operator bool() const;

  friend std::ostream& operator<<(std::ostream& os, const Item& item);
private:
  struct Impl;
  std::shared_ptr<Impl> pImpl{};

  friend class CloudBackend;
  friend class Container;
  friend class Object;
  friend class PublishManager;
  friend class QueryResult;
  friend class SubscribeManager;
 Item(CBI::ItemPtr cbiPeerPtr);
 CBI::Item&  getCbiPeer() const;
protected:
  Item(std::shared_ptr<Impl> pImpl);

  template <class ImplT>
  ImplT& castImpl() const;
private:
  CBI::ItemPtr&       getCbiPeerPtr();
  const CBI::ItemPtr& getCbiPeerPtr() const;
}; // class Item

cbe::ItemType operator|(cbe::ItemType lh, cbe::ItemType rh);

std::ostream& operator<<(std::ostream& os, ItemType itemType);


} // namespace cbe
// namespace XAPI

#endif // INCLUDE_XAPI_ITEM_H_
