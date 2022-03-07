/*
     Copyright © CloudBackend AB 2020 - 2022..
*/

#ifndef INCLUDE_CBE_GROUP_MANAGER_H_
#define INCLUDE_CBE_GROUP_MANAGER_H_

#include <string>
#include <vector>
#include "CBE/Types.h"
#include "CBE/GroupFilter.h"
#include "CBE/GroupQuery.h"

namespace CBE {
  class GroupManager {
    public:
      /**
       * List all current joined groups. 
      */  
      virtual void listGroups(CBE::GroupDelegatePtr delegate);
      
      /**
       * Search for open public groups.
       * @param filter is a group filter to set search criteria for open public groups. Look att class GroupFilter for more information.
       * @param delegate is the callback protocol template for group actions.
       * @param parentGroupId is the id of the group to be searched within. if this is not set the tenent id will be used.
      */
      virtual void searchGroups(CBE::GroupFilter filter, CBE::GroupDelegatePtr delegate, CBE::group_id_t parentGroupId = 0);

      /**
       * Returns the tenant id of the Tenant user group that the user is in.
      */
      virtual CBE::group_id_t getTenantId() const;
      
      /**
       * Destructor.
      */
      virtual ~GroupManager(){};
    
    protected:
    
     GroupManager(){}
  };
}
// namespace CBE

#endif  // INCLUDE_CBE_GROUP_MANAGER_H_
