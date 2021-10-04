/*
     Copyright © CloudBackend AB 2020.
*/

#ifndef INCLUDE_CBE_GROUPFILTER_H_
#define INCLUDE_CBE_GROUPFILTER_H_

#include <string>
#include <map>

#include "CBE/Types.h"
#include <vector>


namespace CBE {

  class GroupFilter {

    public:
      
      /** Returns the query string that was set on the filter. I.E key:value (name:*) would be an example. */
      std::string getQuery() const;
      std::string getFilter() const;
      
      /** Returns the settings on how the data was sorted and displayed. */
      bool getAscending() const;
      bool getDeleted() const;
      bool getPublicFirst() const;
      
      /** Returns the offset that was used for the filter in the query. */
      uint32_t getOffset() const;
      
      /** Returns the value of the count that was set for the query.*/
      uint32_t getCount() const;
      
      /**
       * get the filterorder as a string.
      */
      std::string getOrder() const;
      
      /** Returns the order the query was sorted check enum FilterOrder to see options for sorting. */
      CBE::FilterOrder getGroupOrder() const;
      
      /** Set the query string, e.x: Name:* (would search for all objects with the metadata key of Name). 
       * Note* if used with rootContainer id as the dataId to search in the whole account will be searched. 
      */
      void setQuery(std::string);
      
      /**
       * Not fully tested but should be a regular expression.
      */
      void setFilter(std::string);

      /** Sets the Order in which data should be displayed: Acending meaning alfabetical  */
      void setAscending(bool);

      /**
       * If you want to see deleted groups set deleted to true.
      */
      void setDeleted(bool);
      
      /** Set the offset for paging, offset is the item offset where to start your query. i.e:
       *  There is already a query of the first 99 items and to get the rest you've setOffest to 100 to get the next set.
       */
      void setOffset(uint32_t);
      
      /** Set the Number of items you want to get from a container. 
       * So if a container has 50 items but you only want the 10 first in ascending order then set ascending to true and setCount to 10. 
      */
      void setCount(uint32_t);
      
      /** Set the order of how data will be shown by the enum of FilterOrder ex: Titel first, published e.t.c */
      void setGroupOrder(FilterOrder o);


      
      // Used Internally, do not use or change.
      bool matchesGroupId(uint64_t id);
      bool matchesQuery(uint64_t parentId, CBE::application_t type, bool isDeleted);
      bool matchesQuery(CBE::GroupPtr group);
      bool equals(GroupFilter filter);
      void updateCompareValue();
      std::string order() const;
      void order(std::string o);

    protected:

      /**
       * Internal do not use this. 
      */
      static std::vector<std::string> _approvedOrder;
      std::map<FilterOrder, std::string> orderStrings = {
                              { FilterOrder::Title, "title"},
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
      bool publicFirst;
      std::string query;
      std::string _order;
      std::string _compareValue{""};
      std::string filter{""};
      bool bypassCache{false};
      void parseQueryString();
      CBE::application_t applicationType{CBE::ApplicationType::Open};

      struct Term {
      std::string modifier{""};
      std::string prefix{""};
      std::string str{""};
      };
      
      std::vector<Term> _parsedQueryString = {};
      std::vector<CBE::group_id_t> _groupIds = {};
    
    
  };
}

#endif // INCLUDE_CBE_GROUPFILTER_H_