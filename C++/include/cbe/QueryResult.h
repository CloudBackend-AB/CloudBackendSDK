/*
     Copyright © CloudBackend AB 2020-2023.
*/


#ifndef INCLUDE_CBE_QUERY_RESULT_H_
#define INCLUDE_CBE_QUERY_RESULT_H_

#include "cbe/Types.h"

#include <vector>

namespace CBI {
class QueryResult;
using QueryResultPtr = std::shared_ptr<CBI::QueryResult>;
class ItemEventProtocol;
using ItemDelegatePtr = std::shared_ptr<ItemEventProtocol>;
class PublishEventProtocol;
using PublishDelegatePtr = std::shared_ptr<PublishEventProtocol>;
class ShareEventProtocol;
using ShareDelegatePtr = std::shared_ptr<ShareEventProtocol>;
class SubscribeEventProtocol;
using SubscribeDelegatePtr = std::shared_ptr<SubscribeEventProtocol>;
} // namespace CBI

namespace cbe {

class Filter;
  namespace util {
class Context;
  } // namespace util
  namespace delegate {
class QueryDelegate;
using QueryDelegatePtr = std::shared_ptr<QueryDelegate>;
CBI::ItemDelegatePtr createCbiQueryDelegate(QueryDelegatePtr,
                                            cbe::util::Context&&);
class QueryJoinDelegate;
using QueryJoinDelegatePtr = std::shared_ptr<QueryJoinDelegate>;
CBI::ItemDelegatePtr createCbiQueryDelegate(QueryJoinDelegatePtr,
                                            cbe::util::Context&&);
class JoinDelegate;
using JoinDelegatePtr = std::shared_ptr<JoinDelegate>;
CBI::ItemDelegatePtr createCbiJoinDelegate(JoinDelegatePtr,
                                           cbe::util::Context&&);
  } // namespace delegate

/**
 * @brief resultset of data retrieved.
 * 
 */
class QueryResult {
public:
  /**
   * Returns a copy of the filter used for query.
   */
  Filter filter() const;

  /**
   * @brief A vector of items to hold a snapshot.
   */
  using ItemsSnapshot = std::vector<Item>;
  /**
   * @brief Returns a copy of a vector containing the items for this QueryResult.
   * 
   * The QueryResult will be updated when new data comes in, but the returned
   * copy will not.
   * \note If iterating, make sure to create a variable for a local copy.
   *
   * @return The items matching the query.
   */
  ItemsSnapshot getItemsSnapshot() const;

  /**
   * Number of \c items loaded in current QueryResult.
   */
  std::uint64_t itemsLoaded() const;

  /**
   * total number of items in the cloud matching the query result.
   * This may be more than loaded.
   */
  std::uint64_t totalCount() const;

  /**
   * Returns number of \c objects loaded in to the query result.
   */
  std::uint64_t objectsLoaded() const;

  /**
   * Returns number of \c containers loaded in to the query result.
   */
  std::uint64_t containersLoaded() const;

  /**
   * Checks if the Item with id is in the result-set.
   * 
   * @param itemId id number of the item asked for
   */
  bool containsItem(ItemId itemId) const;

  friend std::ostream& operator<<(std::ostream&       os,
                                  const QueryResult&  queryResult);

  /**
   * @brief Default constructor.
   * 
   * Construct a new object with 
   * the \c DefaultCtor to enable the
   * @ref operator bool()
   * test
   */
  QueryResult(cbe::DefaultCtor);
  ~QueryResult();

  /**
   * @brief Checks if the current instance is real.
   * 
   * An "unreal" instance implies typically a failed event.
   * 
   * Relies on the \c Default \c constructor
   * QueryResult(cbe::DefaultCtor)
   * 
   * @return \c true  : is real
   * @return \c false : unreal; got nullptr; if current instance is unbound/undefined. I.e., if it is
   *         only default constructed.
   */
  explicit operator bool() const;
private:
  struct Impl;
  std::shared_ptr<Impl> pImpl{};

  friend class CloudBackend;
  friend class Container;
  friend class QueryChain;
  friend class QueryChainSync;
  friend class ShareManager;
  friend class SubscribeManager;
  friend CBI::ItemDelegatePtr delegate::createCbiQueryDelegate(
                                                         delegate::QueryDelegatePtr,
                                                         cbe::util::Context&&);
  friend CBI::ItemDelegatePtr delegate::createCbiQueryDelegate(
                                                         delegate::QueryJoinDelegatePtr,
                                                         cbe::util::Context&&);
  friend CBI::ItemDelegatePtr delegate::createCbiJoinDelegate(
                                                         delegate::JoinDelegatePtr,
                                                         cbe::util::Context&&);
  QueryResult(CBI::QueryResultPtr cbiPeerPtr);
  CBI::QueryResult& getCbiPeer() const;
}; // class QueryResult

}
// namespace cbe

#endif // INCLUDE_CBE_DIRECTORY_H_
