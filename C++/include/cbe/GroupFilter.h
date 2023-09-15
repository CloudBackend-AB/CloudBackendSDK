/*
 Copyright © CloudBackend AB 2020-2023.
*/

#ifndef INCLUDE_CBE_GROUPFILTER_H_
#define INCLUDE_CBE_GROUPFILTER_H_

#include "cbe/Types.h"

#include <iosfwd>
#include <memory>

namespace CBI {
class GroupFilter;
class GroupEventProtocol;
using GroupDelegatePtr = std::shared_ptr<GroupEventProtocol>;
} // namespace CBI

namespace cbe {
  namespace util {
class Context;
  } // namespace util
  namespace delegate {
class SearchGroupDelegate;
  } // namespace delegate

/**
 * @brief To filter when searching a list of Group.
 * 
 */
class GroupFilter {
public:
  GroupFilter();
  GroupFilter(const GroupFilter& rh);
  GroupFilter(GroupFilter&&      rh);
  GroupFilter& operator=(const GroupFilter& rh);
  GroupFilter& operator=(GroupFilter&&      rh);
  ~GroupFilter();
  /**
   * Returns the query string that was set on the filter.
   * E.g., name:Abc*
   */
  std::string getQuery() const;

  /** the filter in question */
  std::string getFilter() const;
  
  /**
   * Returns the settings on how the data was sorted and displayed.
   */
  bool getAscending() const;

  /**
   * If deleted objects in the bin are included.
   */
  bool getDeleted() const;

  /**
   * If public groups should be sorted before private.
   */
  bool getPublicFirst() const;
  
  /** Returns the offset that was used for the filter in the query. */
  uint32_t getOffset() const;
  
  /** Returns the value of the count that was set for the query.*/
  uint32_t getCount() const;
  
  /**
   * get the filterorder as a string.
  */
  std::string getOrder() const;
  
  /** 
   * Returns the order the query was sorted check enum FilterOrder 
   * to see options for sorting. */
  cbe::FilterOrder getGroupOrder() const;
  
  /**
   *  Set the query string, 
   * e.g: Name:* (would search for all objects with the metadata key of Name). 
   * \note If used with rootContainer id as the dataId,
   * search in the whole account will be done.
   */
  cbe::GroupFilter& setQuery(std::string);
  
  /**
   * Not fully tested but should be a regular expression.
   */
  cbe::GroupFilter& setFilter(std::string);

  /** 
   * Sets the Order in which data should be displayed: 
   * Acending meaning alfabetical
   */
  cbe::GroupFilter& setAscending(bool);

  /**
   * If you want to see deleted groups set deleted to true.
  */
  cbe::GroupFilter& setDeleted(bool);
  
  /** 
   * Set the offset for paging, offset is the item offset where 
   * to start your query. i.e:
   * There is already a query of the first 99 items and to get the rest 
   * you have setOffest to 100 to get the next set.
   */
  cbe::GroupFilter& setOffset(uint32_t);
  
  /** 
   * Set the Number of items you want to get from a container. 
   * So if a container has 50 items but you only want the 10 first in 
   * ascending order then set ascending to true and setCount to 10. 
  */
  cbe::GroupFilter& setCount(uint32_t);
  
  /** Set the order of how data will be shown by the enum of FilterOrder ex: Titel first, published e.t.c */
  // cbe::GroupFilter& setGroupOrder(FilterOrder o);

  friend std::ostream& operator<<(std::ostream& os, const GroupFilter& GroupFilter);
private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
  friend class Group;
  friend class GroupQueryResult;
  friend class GroupManager;
  friend class delegate::SearchGroupDelegate;
  GroupFilter(CBI::GroupFilter&& cbiPeer);
  CBI::GroupFilter& getCbiPeer() const;
}; // class GroupFilter

} // namespace cbe


#endif // #ifndef CBE__GroupFilter_h__
