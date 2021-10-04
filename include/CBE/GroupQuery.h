/*
     Copyright © CloudBackend AB 2020.
*/


#ifndef INCLUDE_CBE_GROUP_QUERY_H_
#define INCLUDE_CBE_GROUP_QUERY_H_

#include <string>
#include <vector>
#include <iostream>

#include "CBE/Types.h"
#include "CBE/GroupFilter.h"

namespace CBE {

  class GroupQuery {
    // friend class DirectoryCache;
    friend class GroupManager;
    friend class CloudBackend;
    public:

      GroupQuery();
      virtual ~GroupQuery();

      /** Returns a copy of the filter used for query.*/
      virtual CBE::GroupFilter filter() const;

      /** 
       * Returns a copy of a vector containing the groups for the queryResult.    
       * The queryResult will update when new data comes in but the copy will not.
       * If iterating make sure to create a variable for a local copy.
       * @return vector<CBE::GroupPtr> contains the groups matching the query.
      */
      virtual std::vector<CBE::GroupPtr> getGroupsSnapshot();

      /// groups loaded in the queryResult.
      virtual uint64_t GroupsLoaded();

      /// total Groups in the cloud matching the query result.  This may be more than loaded.
      virtual uint64_t totalCount();
      
      /** Returns if the group with groupId was loaded in the query. */
      bool containsGroup(uint64_t groupId);

     protected:

      /** Vector with all Groups loaded from the query.*/      
      std::vector<CBE::GroupPtr> groups;
      
      /** Resizes the groups vector to fit the total count of groups.*/
      void initGroups();

      //The number of Groups in the parent Group on the cloud.
      uint64_t _totalCount;
      
      std::vector< std::pair<uint64_t, uint64_t> > _loadedPages;   

  };
}
// namespace CBE

#endif // INCLUDE_CBE_GROUPQUERY_H_
