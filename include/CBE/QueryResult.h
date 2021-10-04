/*
     Copyright © CloudBackend AB 2020.
*/


#ifndef INCLUDE_CBE_QUERY_RESULT_H_
#define INCLUDE_CBE_QUERY_RESULT_H_

#include <string>
#include <vector>
#include <iostream>

#include "CBE/Types.h"
#include "CBE/Item.h"
#include "CBE/Filter.h"

namespace CBE {

  class QueryResult {
    friend class DirectoryCache;
    friend class CloudBackend;
    public:
       QueryResult();
      //  QueryResult(CBE::Filter f);
      virtual ~QueryResult();

      ///Filter can be used to see what has been loaded.
      // CBE::Filter filter;

      /** Returns a copy of the filter used for query.*/
      virtual CBE::Filter filter() const;

      /**
       * 
        * Returns a copy of a vector containing the items for the queryResult.    
        * The queryResult will update when new data comes in but the copy will not.
        * If iterating make sure to create a variable for a local copy.
       * @return vector<CBE::ItemPtr> contains the items matching the query.
      */
      virtual std::vector<CBE::ItemPtr> getItemsSnapshot();

      /// items loaded in the queryResult.
      virtual uint64_t itemsLoaded();

      /// total items in the cloud matching the query result.  This may be more than loaded.
      virtual uint64_t totalCount();
      
      /** Returns # of objects loaded in to the query result. */
      virtual uint64_t objectsLoaded();
      
      /** Returns " of containers loaded in to the query result.*/
      virtual uint64_t containersLoaded();

      /** Checks if the Item with id @param itemId, is in the query result. */
      bool containsItem(uint64_t itemId);

     protected:

      /** Vector with all Items loaded from the query.*/
      std::vector<CBE::ItemPtr> items;
    
      /** Resizes the items vector to fit the total count of items.*/  
      void initItems();

      /**The number of items on the cloud. */
      uint64_t _totalCount;

      std::vector< std::pair<uint64_t, uint64_t> > _loadedPages;
      

  };
}
// namespace CBE

#endif // INCLUDE_CBE_DIRECTORY_H_
