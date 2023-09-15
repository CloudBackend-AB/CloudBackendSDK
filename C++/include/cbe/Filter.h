/*
     Copyright © CloudBackend AB 2020 - 2023.
*/

#ifndef CBE__Filter_h__
#define CBE__Filter_h__

#include "cbe/Types.h"

#include <iosfwd>
#include <memory>

namespace CBI {
class Filter;
class ItemEventProtocol;
using ItemDelegatePtr = std::shared_ptr<ItemEventProtocol>;
} // namespace CBI

namespace cbe {
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
  } // namespace delegate

/**
 * @brief Use to select Item that meets specific criterias when doing a query.
 * 
 * Filtering on metadata KeyValues can be done with setQuery()
 * and only to query for Object, not Container.
 */
class Filter {
public:
  Filter();
  Filter(const Filter& rh);
  Filter(Filter&&      rh);
  Filter& operator=(const Filter& rh);
  Filter& operator=(Filter&&      rh);
  ~Filter();

  /**
   * @brief Returns the requested data type.
   * 
   * Such as ItemType e.g.,
   * <ul>
   *   <li>cbe::ItemType::Container </li>
   *   <li>cbe::ItemType::Object </li>
   *   <li>cbe::ItemType::Group </li>
   * </ul>
   */
  /**  */
  cbe::ItemType getDataType() const;

  /**
   * @brief Returns the query string that was set on the filter.
   * 
   * Query string e.g.,
   * <ul>
   *   <li>name:Abc* </li>
   *   <li>age:100 </li>
   *   <li>price<200 </li>
   *   <li>size>40 </li>
   * </ul>
   */
  std::string getQuery() const;

  /**
   * @brief Returns the settings on how the data was sorted and displayed.
   */
  bool getAscending() const;

  /**
   * @brief Determines if deleted objects in the bin are included.
   */
  bool getDeleted() const;

  /**
   * @brief Determines if containers should be sorted before objects.
   */
  bool getContainerFirst() const;

  /**
   * @brief Determines if the resultset comes from a joined query.
   */
  bool isJoinedResult() const;

  /**
   * @brief Determines the offset of items to be skiped.
   * 
   * Returns the offset of items in the beginning of total resultset
   * to be skiped,
   * that was used for the filter in the query.
   */
  std::uint32_t getOffset() const;

  /**
   * @brief Returns the size of the resultset of the query.
   */
  std::uint32_t getCount() const;

  /**
   * @brief Returns if skipping the cache was used or not.
   */
  bool getByPassCache() const;

  /**
   * @brief Returns the order the query was sorted.
   * 
   * Check enum \c FilterOrder to see
   * options for sorting.
   */
  cbe::FilterOrder getItemOrder() const;

  /**
   * @brief Returns the parent container that was queried, if it is in the cache. 
   * This is after a query was done, not before.
   */
  cbe::Container container();

  /**
   * @brief Set which \c datatypes to query for.
   * 
   * @param itemType as defined in Types.h
   */
  cbe::Filter& setDataType(cbe::ItemType itemType);

  /**
   * @brief Set the query string.
   * 
   * E.g., firstname:*
   * (would search for all objects with the metadata key labeled \c firstname ).
   * 
   * Search string e.g.,
   * <ul>
   *   <li>name:Abc* </li>
   *   <li>age:100 </li>
   *   <li>price<200 </li>
   *   <li>size>40 </li>
   * </ul>
   * 
   * \note If used with rootContainer id as the dataId to search in, 
   * the whole account will be searched.
   * 
   * @param query is a string of key tags or key:value pairs. \n 
   * E.g. Name:*|Country:Sweden|Country:Norway \n
   * This will search for objects 
   * with key Name but any value and
   * where key Country is either Sweden or Norway. \n
   */
  cbe::Filter& setQuery(std::string query);

  /**
   * @brief Sets the order, \p ascending , in which data should be displayed.
   * 
   * @param ascending true = sorted in increasing order \n 
   * false = sorted in decreasing order
   */
  cbe::Filter& setAscending(bool ascending);

  /**
   * @brief Whether to include deleted items.
   * 
   * @param deleted true = include items in the bin \n 
   * default is false
   */
  cbe::Filter& setDeleted(bool deleted);

  /**
   * @brief If \c Container should be displayed before \c Object
   * 
   * @param containerFirst true = sort containers at the top of the resultset. \n 
   * default is to not sort for this.
   */
  cbe::Filter& setContainerFirst(bool containerFirst);

  /**
   * @brief Set the offset for paging.
   * 
   * Offset is the item offset where to start your resultset. \n 
   * E.g.:
   * There is already a query resultset of the first 100 items and
   * to get the rest you put setOffset() to 100 to get the next resultset
   * from 101-.
   * 
   * @param offset first item start number of the complete resultset to return
   */
  cbe::Filter& setOffset(std::uint32_t offset);

  /**
   * @brief Set the maximum resultset.
   * 
   * Set the maximum number of items you want to get from a container. \n 
   * E.g.: a container has 50 items but you only want the 10 first in ascending
   * order then set ascending to true and setCount to 10.
   * 
   * @param count maximum number of items to return 
   */
  cbe::Filter& setCount(std::uint32_t count);

  /**
   * @brief Set the order of how data will be shown by the enum of FilterOrder.
   * 
   * E.g.: Title first, Relevance, published e.t.c.
   * 
   * @param order see the enum \c FilterOrder in Types.h
   */
  cbe::Filter& setItemOrder(cbe::FilterOrder order);

  /**
   * @brief Set to ignore the local SDK cache.
   * 
   * Instead allways fetch the resultset from the cloud.
   * 
   * @param byPassCache true = skip the SDK cache and ask the cloud. \n 
   * default is to use the cache when available.
   */
  cbe::Filter& setByPassCache(bool byPassCache);

  friend std::ostream& operator<<(std::ostream& os, const Filter& filter);
private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
  friend class CloudBackend;
  friend class Container;
  friend class QueryChain;
  friend class QueryResult;
  friend CBI::ItemDelegatePtr delegate::createCbiQueryDelegate(
                                                         delegate::QueryDelegatePtr,
                                                         cbe::util::Context&&);
  friend CBI::ItemDelegatePtr delegate::createCbiQueryDelegate(
                                                         delegate::QueryJoinDelegatePtr,
                                                         cbe::util::Context&&);
  Filter(CBI::Filter&& cbiPeer);
  CBI::Filter& getCbiPeer() const;
}; // class Filter

} // namespace cbe


#endif // #ifndef CBE__Filter_h__
