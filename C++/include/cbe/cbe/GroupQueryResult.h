/*
 Copyright © CloudBackend AB 2020-2023.
 */

#ifndef INCLUDE_CBE_GROUP_QUERY_H_
#define INCLUDE_CBE_GROUP_QUERY_H_

#include <string>
#include <vector>

#include "cbe/Types.h"
#include "cbe/GroupFilter.h"

namespace CBI {
class GroupQueryResult;
using GroupQueryResultPtr = std::shared_ptr<CBI::GroupQueryResult>;
class GroupEventProtocol;
using GroupDelegatePtr = std::shared_ptr<GroupEventProtocol>;
} // namespace CBI

namespace cbe {

/**
 * @brief Resultset of data retrieved in a search for Group.
 * 
 */
class GroupQueryResult {
  // friend class DirectoryCache;
  friend class GroupManager;
  friend class CloudBackend;
public:
  GroupQueryResult(cbe::DefaultCtor);
  ~GroupQueryResult();

  /** 
   * Returns a copy of the filter used for query.
   */
  cbe::GroupFilter filter() const;

  using GroupsSnapshot = std::vector<Group>;
  /**
   * Returns a copy of a vector containing the groups for the queryResult.
   * The queryResult will update when new data comes in but the copy will not.
   * If iterating, make sure to create a variable for a local copy.
   * @return vector<cbe::Group> containing the groups matching the query.
   */
  GroupsSnapshot getGroupsSnapshot() const;

  /** 
   * Number of groups loaded in the queryResult. 
   */
  uint64_t GroupsLoaded() const;

  /** 
   * Total number of Groups in the cloud matching the query result.
   * This may be more than loaded.
   */
  uint64_t totalCount() const;

  /** 
   * asking if the group with groupId was loaded in the query. 
   * @param groupId the group id number in question
  */
  bool containsGroup(uint64_t groupId);

  friend std::ostream& operator<<(std::ostream&       os,
                                  const GroupQueryResult&  groupQuery);

  explicit operator bool() const;
private:
  struct Impl;
  std::shared_ptr<Impl> pImpl{};

  GroupQueryResult(CBI::GroupQueryResultPtr cbiPeerPtr);
  CBI::GroupQueryResult& getCbiPeer() const;
}; // class GroupQueryResult

}
// namespace cbe

#endif // INCLUDE_CBE_GROUPQUERY_H_
