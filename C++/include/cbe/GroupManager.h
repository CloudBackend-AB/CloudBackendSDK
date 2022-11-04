/*
 Copyright © CloudBackend AB 2020 - 2022.
 */

#ifndef INCLUDE_CBE_GROUP_MANAGER_H_
#define INCLUDE_CBE_GROUP_MANAGER_H_

#include "cbe/Types.h"

#include "cbe/GroupFilter.h"

#include "cbe/delegate/Error.h"
#include "cbe/delegate/ListGroupsDelegate.h"
#include "cbe/delegate/SearchGroupsDelegate.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"


namespace CBI {
class GroupManager;
} // namespace CBI

namespace cbe {

/**
 * @brief to manage the users own Group.
 * 
 */
class GroupManager {
public:  
  using ListGroupsDelegatePtr = delegate::ListGroupsDelegatePtr;
  /**
   * List all current joined groups.
   * @param delegate      Pointer to a delegate::ListGroupsDelegate instance
   *                      that is implemented by the user.
   */
  void listGroups(ListGroupsDelegatePtr delegate);
  // void listGroups(cbe::GroupDelegatePtr delegate);

  using SearchGroupsDelegatePtr = delegate::SearchGroupsDelegatePtr;
  /**
   * Search for open public groups.
   * @param filter        is a group filter to set search criteria for open 
   *                      public groups. Look att class GroupFilter for more 
   *                      information.
   * @param delegate      Pointer to a delegate::SearchGroupsDelegate instance
   *                      that is implemented by the user.
   * @param parentGroupId is the id of the group to be searched within. 
   *                      If this is not set the tenent id will be used.
   */
  void searchGroups(cbe::GroupFilter        filter,
                    SearchGroupsDelegatePtr delegate,
                    cbe::GroupId            parentGroupId = 0);

  /**
   * Returns the tenant id of the Tenant user group that the user is in.
   */
  cbe::GroupId    getTenantId() const;

  GroupManager(cbe::DefaultCtor);
  ~GroupManager();

  explicit operator bool() const;
private:
  struct Impl;
  std::shared_ptr<Impl> pImpl{};

  friend class CloudBackend;
  GroupManager(CBI::GroupManager& cbiPeer);
  CBI::GroupManager& getCbiPeer() const;
}; // class GroupManager

} // namespace cbe

#endif  // INCLUDE_CBE_GROUP_MANAGER_H_
