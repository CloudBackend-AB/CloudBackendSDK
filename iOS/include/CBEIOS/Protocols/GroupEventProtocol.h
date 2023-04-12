/*
 Copyright © CloudBackend AB 2020 - 2023..
 */

#ifndef CBE_GROUPEVENTPROTOCOL_H_
#define CBE_GROUPEVENTPROTOCOL_H_

#include <vector>
#include "CBE/Group.h"
#include "CBE/Types.h"

namespace cbe {
class State;
class Group;
class Member;

/** Protocol for implementing a Item delegate.
 * Abstract base class which is used when you want notification about
 * events from group calls. */

class GroupEventProtocol {
public:
  virtual ~GroupEventProtocol();

  /* Gets called on a search groups response. */
  virtual void onGroupSearchLoaded(cbe::GroupQueryResult&& queryResult);

  /**
   * Gets called when a group has been created.
   */
  virtual void onGroupAdded(cbe::Group&& group);

  /**
   * Lists the groups that the user is included in or owner/admin of.
   */
  virtual void onListGroups(std::vector<cbe::Group>&& groups);

  /**
   * Gets called when a group has been removed.
   */
  virtual void onGroupRemoved(cbe::GroupId groupId, std::string&& name);

  /**
   * Gets called when the user leaves a group.
   */
  virtual void onMemberLeaveGroup(std::string&& memberName, cbe::MemberId memberId);

  /**
   * Gets called when a user have become a member of a group.
   */
  virtual void onMemberJoinGroup(cbe::Group&& group);

  /**
   * Gets called when a member has been kicked.
   */
  virtual void onMemberKicked(std::string&& memberName, cbe::MemberId memberId);

  /**
   * Gets called when a member has been banned.
   */
  virtual void onMemberBanned(std::string&& memberName, cbe::MemberId memberId);

  /**
   * Gets called when a member has been unbanned.
   */
  virtual void onMemberUnBanned(std::string&& memberName, cbe::MemberId memberId);

  /**
   * Gets called when listBannedMembers has been called.
   */
  virtual void onListBannedMembers(std::vector<cbe::Member>&& members);

  /**
   * Gets called when a list call of members of a group have happened.
   */
  virtual void onListMembers(std::vector<cbe::Member>&& members);

  /**
   * Gets called when a group have been renamed.
   */
  virtual void onGroupRenamed(cbe::Group&& group, std::string&& newName);

  /**
   * Gets called when an error have occured.
   */
  virtual void onGroupError(uint32_t    code,
                            std::string reason,
                            std::string message);
}; // class GroupEventProtocol

}

#endif  //CBE_GROUPEVENTPROTOCOL_H_
