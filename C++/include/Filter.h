/*
     Copyright © CloudBackend AB 2020 - 2022..
*/

#ifndef INCLUDE_CBE_FILTER_H_
#define INCLUDE_CBE_FILTER_H_

#include <string>
#include <map>

#include "CBE/Types.h"
#include <vector>


namespace CBE {

  // enum FilterOrder : uint32_t {
  //   Title = 1,
  //   Relevance = 2,
  //   Published = 3,
  //   Updated = 4,
  //   Length = 5,
  //   S1 = 6,
  //   S2 = 7,
  //   S3 = 8,
  //   S4 = 9
  // };
  class Filter {

    public:
      Filter();


      
      /** Returns the requested data type I.E CBE::ItemType::Container, Object, Tag, ... */
      CBE::item_t getDataType() const;
      
      /** Returns the query string that was set on the filter. I.E key:value (name:*) would be an example. */
      std::string getQuery() const;
      
      /** Returns the settings on how the data was sorted and displayed. */
      bool getAscending() const;
      bool getDeleted() const;
      bool getContainerFirst() const;
      bool isJoinedResult() const;      
      /** Returns the offset that was used for the filter in the query. */
      uint32_t getOffset() const;
      
      /** Returns the value of the count that was set for the query.*/
      uint32_t getCount() const;
      
      /** Returns if skipping the cache was used or not. */
      bool getByPassCache() const;
      
      /** Returns the order the query was sorted check enum FilterOrder to see options for sorting. */
      CBE::FilterOrder getItemOrder() const;

      /** Returns the parent container / container that was queried if it is in the cache, if not nullptr will be returned. (this is after a query was done not before.) */
      CBE::ContainerPtr container();
      
      /** Set which data types to query for. */
      void setDataType(CBE::item_t);
      

      
      /** Set the query string, e.x: Name:* (would search for all objects with the metadata key of Name). 
       * Note* if used with rootContainer id as the dataId to search in the whole account will be searched. 
      */
      void setQuery(std::string);
      
      /** Sets the Order in which data should be displayed: Ascending, show deleted items and if Containers should be displayed top down first.  */
      void setAscending(bool);
      void setDeleted(bool);
      void setContainerFirst(bool);

      /** Set the offset for paging, offset is the item offset where to start your query. i.e:
       *  There is already a query of the first 99 items and to get the rest you've setOffest to 100 to get the next set.
       */
      void setOffset(uint32_t);
      
      /** Set the Number of items you want to get from a container. 
       * So if a container has 50 items but you only want the 10 first in ascending order then set ascending to true and setCount to 10. */
      void setCount(uint32_t);
      
      /** Set the order of how data will be shown by the enum of FilterOrder ex: Titel first, Relevance, published e.t.c */
      void setItemOrder(CBE::FilterOrder order);
      
      /** Set bool to true, to skip the cache. Force update from Server. */
      void setByPassCache(bool);

    protected:

      /**
       * Internal do not use this. 
      */
      static std::vector<std::string> _approvedOrder;
      std::map<FilterOrder, std::string> orderStrings = {
                              { FilterOrder::Title, "title"},
                              { FilterOrder::Relevance, "relevance"},
                              { FilterOrder::Published, "published"},
                              { FilterOrder::Updated, "updated"},
                              { FilterOrder::Length, "length"},
                              { FilterOrder::S1, "s1"},
                              { FilterOrder::S2, "s2"},
                              { FilterOrder::S3, "s3"},
                              { FilterOrder::S4, "s4"}
                              };
      uint32_t offset;
      uint32_t count;
      bool ascending;
      bool deleted;
      bool containerFirst;
      bool isJoined;
      std::string query;
      CBE::item_t dataType;
      std::string _order;
      bool bypassCache;
      void parseQueryString();
      CBE::ContainerPtr _container;
      
      struct Term {
      std::string modifier;
      std::string prefix;
      std::string str;
      };
      
      std::vector<Term> _parsedQueryString;
    
    
  };
}

#endif // INCLUDE_CBE_FILTER_H_
