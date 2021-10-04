/*
     Copyright © CloudBackend AB 2020.
*/

#ifndef CBE_GROUPEVENTPROTOCOL_H_
#define CBE_GROUPEVENTPROTOCOL_H_

#include <vector>
#include "CBE/Group.h"
#include "CBE/Types.h"

namespace CBE {
    class State;
    class Group;
    class Member;

    /** Protocol for implementing a Item delegate.
     * Abstract base class which is used when you want notification about
     * events from group calls. */

    class GroupEventProtocol {
    public:
      virtual ~GroupEventProtocol() {}

      /* Gets called on a search groups response. */
      virtual void onGroupSearchLoaded(CBE::persistence_t newPersistenceState, CBE::GroupQueryPtr queryResult) {}

      /**
       * Gets called when a group has been created.
      */
      virtual void onGroupAdded(CBE::persistence_t newPersistenceState, CBE::GroupPtr group) {}

      /**
      * Lists the groups that the user is included in or owner/admin of.
      */
      virtual void onListGroups(std::vector<CBE::GroupPtr> groups) {}

      /**
       * Gets called when a group has been removed.
      */
      virtual void onGroupRemoved(CBE::persistence_t newPersistenceState) {}

      /**
       * Gets called when the user leaves a group.
      */
      virtual void onMemberLeaveGroup(CBE::persistence_t newPersistenceState) {}

      /**
       * Gets called when a user have become a member of a group.
      */
      virtual void onMemberJoinGroup(CBE::persistence_t newPersistenceState) {}

      /**
       * Gets called when a list call of members of a group have happened.
      */
      virtual void onListMembers(CBE::persistence_t newPersistenceState, std::vector<CBE::MemberPtr> members) {}

      /**
       * Gets called when a group have been renamed.
      */
      virtual void onGroupRenamed(CBE::persistence_t newPersistenceState) {}

      /**
       * Gets called when an error have occured.
      */
      virtual void onGroupError(uint64_t operationId, CBE::persistence_t operation, CBE::persistence_t failedAtState, uint32_t code, std::string reason, std::string message) {}
    };
}

#endif  //CBE_GROUPEVENTPROTOCOL_H_
