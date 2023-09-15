/*
 Copyright © CloudBackend AB 2020-2023.
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
#include "cbe/util/Optional.h"

namespace CBI {
class GroupManager;
} // namespace CBI

namespace cbe {

/**
 * @brief For managing the groups.
 */
class GroupManager {
public:
  /**
   * Pointer to cbe::delegate::ListGroupsDelegate that is passed into
   * asynchronous version of method
   * @ref listGroups(ListGroupsDelegatePtr) "listGroups()"
   */
  using ListGroupsDelegatePtr = delegate::ListGroupsDelegatePtr;
  /**
   * List all current joined groups.
   * @param delegate Pointer to a delegate::ListGroupsDelegate instance
   *                 that is implemented by the user.
   */
  void listGroups(ListGroupsDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::ListGroupsDelegate::Exception
  */
  using ListGroupsException = delegate::ListGroupsDelegate::Exception;
  /**
   * @brief Synchronous [exception]
   * <b>Synchronous</b> version of
   * listGroups(ListGroupsDelegatePtr)
   * , and <b>throws an exception</b>, #ListGroupsException, in case of a failed
   * call.
   * <br>See listGroups(ListGroupsDelegatePtr)
   *
   * @return Information about the listGroups object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::ListGroupsDelegate::Groups " "
   * @throws #ListGroupsException
   */
  std::vector<cbe::Group> listGroups();
  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref listGroups(ListGroupsError&) "listGroups()"
   * <br>See delegate::ListGroupsDelegate::ErrorInfo
   */
  using ListGroupsError = delegate::ListGroupsDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing]
   * <b>Synchronous</b> version of
   * listGroups(ListGroupsDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See listGroups(ListGroupsDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #ListGroupsError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<std::vector<cbe::Group>> listGroups(ListGroupsError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to SearchGroupsDelegate that is passed into:
   * <ul>
   *   <li> cbe::GroupManager::searchGroups()
   * </ul>
   */
  using SearchGroupsDelegatePtr = delegate::SearchGroupsDelegatePtr;
  /**
   * Search for open public groups.
   * @param filter        is a group filter to set search criteria for open
   *                      public groups. Look att class GroupFilter for more
   *                      information.
   * @param delegate      Pointer to a delegate::SearchGroupsDelegate instance
   *                      that is implemented by the user.
   * @param parentGroupId is the id of the group to be searched within.
   *                      If this is not set the tenant id will be used.
   */
  void searchGroups(cbe::GroupFilter        filter,
                    SearchGroupsDelegatePtr delegate,
                    cbe::GroupId            parentGroupId = 0);
  /**
   * Pointer to cbe::delegate::SearchGroupsDelegate that is passed into
   * asynchronous version of method
   * @ref searchGroups(SearchGroupsDelegatePtr) "searchGroups()"
   */

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::SearchGroupsDelegate::Exception
  */
  using SearchGroupsException = delegate::SearchGroupsDelegate::Exception;
  /**
   * @brief Synchronous [exception]
   * <b>Synchronous</b> version of
   * searchGroups(filter, SearchGroupsDelegatePtr, parentGroupId = 0)
   * , and <b>throws an exception</b>, #SearchGroupsException, in case of a 
   * failed call.
   * <br>See searchGroups(SearchGroupsDelegatePtr)
   *
   * @return Information about the searchGroups object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::SearchGroupsDelegate::Success " "
   * @throws #SearchGroupsException
   */
  delegate::SearchGroupsDelegate::Success searchGroups(
                    cbe::GroupFilter        filter,
                    cbe::GroupId            parentGroupId = 0);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref searchGroups(filter, parentGroupId, SearchGroupsError&) "searchGroups()"
   * <br>See delegate::SearchGroupsDelegate::ErrorInfo
   */
  using SearchGroupsError = delegate::SearchGroupsDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing]
   * <b>Synchronous</b> version of
   * searchGroups(filter, parentGroupId, SearchGroupsDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See searchGroups(filter, parentGroupId,  SearchGroupsDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #SearchGroupsError object passed in will we be populated with 
   *              the error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::SearchGroupsDelegate::Success> searchGroups(
                                              cbe::GroupFilter   filter,
                                              cbe::GroupId       parentGroupId, 
                                              SearchGroupsError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /**
   * Returns the tenant id of the Tenant user group that the user is in.
   */
  cbe::GroupId    getTenantId() const;

  /**
   * @brief Default constructor.
   * 
   * Construct a new object with 
   * the \c DefaultCtor to enable the
   * @ref operator bool()
   * test
   */
  GroupManager(cbe::DefaultCtor);
  ~GroupManager();

  /**
   * @brief Checks if the current instance is real.
   * 
   * An "unreal" instance implies typically a failed event.
   * 
   * Relies on the \c Default \c constructor
   * GroupManager(cbe::DefaultCtor)
   * 
   * @return \c true  : is real
   * @return \c false : unreal; got nullptr; if current instance is unbound/undefined. I.e., if it is
   *         only default constructed.
   */
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
