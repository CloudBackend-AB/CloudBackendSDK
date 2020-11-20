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
      virtual void onGroupSearchLoaded(uint64_t operationId, CBE::persistence_t newPersistenceState, std::vector<CBE::Group> groups) {}

      virtual void onGroupAdded(uint64_t operationId, CBE::persistence_t newPersistenceState, CBE::group_id_t newGroupId) {}

      virtual void onGroupRemoved(uint64_t operationId, CBE::persistence_t newPersistenceState) {}

      virtual void onMemberLeaveGroup(uint64_t operationId, CBE::persistence_t newPersistenceState) {}

      virtual void onMemberJoinGroup(uint64_t operationId, CBE::persistence_t newPersistenceState) {}

      virtual void onMemberInvited(uint64_t operationId, CBE::persistence_t newPersistenceState) {}

      virtual void onMemberKicked(uint64_t operationId, CBE::persistence_t newPersistenceState) {}

      virtual void onListInvites(uint64_t operationId, CBE::persistence_t newPersistenceState) {}

      virtual void onListMembers(uint64_t operationId, CBE::persistence_t newPersistenceState, std::vector<CBE::Member> members) {}

      virtual void onGroupError(uint64_t operationId, CBE::persistence_t operation, CBE::persistence_t failedAtState, uint32_t code, std::string reason, std::string message) {}
    };
}

#endif  //CBE_GROUPEVENTPROTOCOL_H_
