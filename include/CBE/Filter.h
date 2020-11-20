/*
     Copyright © CloudBackend AB 2020.
*/

#ifndef INCLUDE_CBE_FILTER_H_
#define INCLUDE_CBE_FILTER_H_

#include <string>
#include <map>

#include "CBE/Types.h"
#include <vector>

namespace CBE {

// This class is currently being examined
    struct Term {
      std::string modifier;
      std::string prefix;
      std::string str;
    };

    class Filter {

    public:
      Filter();

      CBE::user_id_t getUserId();

      //std::string getDataType;
      CBE::item_t getDataType();
      std::vector<uint64_t> getDataId();
      std::map<std::string, std::string> getTagLabels();
      std::string getPath();
      std::string getUrl();
      std::string getQuery();
      std::string getGroup(); //Group - NYI
      std::string getOrder();
      int getLoopBack();

      bool getAscending();
      bool getDeleted();
      bool getFolderFirst();
      bool getItem();
      uint32_t getOffset();
      uint32_t getCount();

      void    setUserId(CBE::user_id_t);
      void    setDataType(CBE::item_t);
      void    setDataId(std::vector<uint64_t>);
      void    setTagLabels(std::map<std::string, std::string>);
      void    setPath(std::string);
      void    setUrl(std::string);
      void    setQuery(std::string);
      void    setGroup(std::string); //Group - NYI
      void    setOrder(std::string);
      void    setAscending(bool);
      void    setDeleted(bool);
      void    setFolderFirst(bool);
      void    setItem(bool);
      void    setOffset(uint32_t);
      void    setCount(uint32_t);
      void    setItemOrder(std::string o);
      void    setLoopBack(int n);

     int loopBack;
     CBE::user_id_t userId;
     CBE::item_t dataType;
     std::vector<uint64_t> dataId;
     std::map<std::string, std::string> tagLabels;
     std::string path;
     std::string fullPath;
     std::string url;
     std::string query;
     std::string group; //Group - NYI
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
     std::string order();
     void order(std::string o);
     void order(FilterOrder o);
     bool ascending;
     bool deleted;
     bool folderFirst;
     bool item;
     uint32_t offset;
     uint32_t count;

     bool matchesDataId(uint64_t id);
     bool matchesQuery(uint64_t parentId, CBE::item_t type, bool isDeleted);
     bool matchesQuery(CBE::Item& item);
     bool matchesQueryString(CBE::Item item);
     void parseQueryString();

     bool equals(Filter filter);

     std::vector<CBE::item_t> getDataTypes();

     bool bypassCache;

    protected:
     void updateCompareValue();
     std::string _compareValue;

     std::vector<Term> _parsedQueryString;

     std::string _order;
     static std::vector<std::string> _approvedOrder;


  };
}
// namespace CBE

#endif // INCLUDE_CBE_FILTER_H_
