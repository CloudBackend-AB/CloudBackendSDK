/*
     Copyright © CloudBackend AB 2020.
*/
#ifndef INCLUDE_CBE_GROUP_H_
#define INCLUDE_CBE_GROUP_H_

#include <string>
#include "CBE/Member.h"
#include "CBE/Types.h"

namespace CBE {

  class Group  {
    public:
      /**
      * Returns the name of the group.
      */
      virtual std::string name() const;
      
      /**
      * Returns the id of the group.
      */
      virtual CBE::group_id_t id() const;

      /**
       * Group Parent id.
      */
      virtual CBE::group_id_t parentId() const;
      
      /**
       * Every group has a drive/container where resources can be shared with the members of the group. Works like a shared container.
      */
      virtual CBE::ContainerPtr groupContainer() const;

      /**
       * Visibility of the Group, Public is searchable and Private is not. In this early version you can create Private groups but the ability to invite has been blocked. (work in progress) 
      */
      virtual CBE::visibility getVisibility() const;

      /**
       * Searched groups is obtained through the GroupQuery response. This list of groups have non-joined and joined groups. 
       * Already joined groups can be found on the groupManager->groups() as well and are cached.
       * The joined bool is used to easily sort out groups in the GroupQuery.
      */
      virtual bool joined() const;

      /**
       * Members can be listed / fetched with listMembers. when this is done users can iterate through this list and see all users whom have access to this group.
      */
      virtual std::vector<CBE::MemberPtr> members() const;

      /**
       * Create group, this is only available for owner/admin roles. In the first version only Open Groups will be available for creation, Groups can be Public or Private though.
      */
      virtual CBE::GroupPtr createGroup(std::string name, std::string memberAlias, CBE::GroupDelegatePtr delegate, CBE::visibility visibility = CBE::Visibility::Public);
 
      /**
       * Request to join a group, in this first version All members will be Public, meaning visable for other member inside the group. All groups will also be open so all join requests should be accepted directly.
      */
      virtual void join(std::string alias, CBE::GroupDelegatePtr delegate);
      
      /**
       * Leave group.
      */
      virtual void leave(CBE::GroupDelegatePtr delegate);

      /**
       * Remove group (exlusive for group owners).
      */
      virtual void remove(CBE::GroupDelegatePtr delegate);

      /**
       * Rename the Group, group id does not change.
      */
      virtual void rename(std::string newName, CBE::GroupDelegatePtr delegate);

      /**
       * List all members in the group. After listing, all members will be available though a call to group->members().
      */
      virtual void listMembers(CBE::GroupDelegatePtr delegate);

      virtual ~Group() {};

  };
}
// namespace CBE

#endif  // INCLUDE_CBE_GROUP_H_
