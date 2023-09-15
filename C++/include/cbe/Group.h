/*
 Copyright © CloudBackend AB 2020-2023.
 */
#ifndef INCLUDE_CBE_GROUP_H_
#define INCLUDE_CBE_GROUP_H_

#include <string>
#include "cbe/util/Context.h"
#include "cbe/Member.h"
#include "cbe/Types.h"

#include "cbe/delegate/Error.h"
#include "cbe/delegate/CreateGroupDelegate.h"
#include "cbe/delegate/CreateRoleDelegate.h"
#include "cbe/delegate/LeaveDelegate.h"
#include "cbe/delegate/ListMembersDelegate.h"
#include "cbe/delegate/ListRolesDelegate.h"
#include "cbe/delegate/RemoveRoleDelegate.h"

#include "cbe/delegate/group/JoinDelegate.h"
#include "cbe/delegate/group/RemoveDelegate.h"
#include "cbe/delegate/group/RenameDelegate.h"

#include "cbe/delegate/Error.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"
#include "cbe/util/Optional.h"

namespace CBI {
class Group;
using GroupPtr = std::shared_ptr<CBI::Group>;
class GroupEventProtocol;
using GroupDelegatePtr = std::shared_ptr<GroupEventProtocol>;
} // namespace CBI

namespace cbe {

/**
 * @brief A group of members.
 * 
 */
class Group {
public:
  /**
   * Returns the name of the group.
   */
  std::string name() const;

  /**
   * Returns the id number of the group.
   */
  cbe::GroupId id() const;

  /**
   * Group's Parent id number.
   */
  cbe::GroupId parentId() const;

  /**
   * Every group has a drive/container where resources can be shared with the 
   * members of the group. Works like a shared container.
   */
  cbe::Container groupContainer() const;

  /**
   * Visibility of the Group, Public is searchable and Private is not. 
   * In this early version you can create Private groups but the ability to 
   * invite has been blocked. (work in progress)
   * \see Types.h for cbe::PublishVisibility enum
   */
  cbe::Visibility getVisibility() const;

  /**
   * Searched groups are obtained through the GroupQuery response. This list of 
   * groups have non-joined and joined groups. Already joined groups can be 
   * found on the <code>groupManager.groups()</code> and they are also cached.
   * The joined bool is used to easily sort out groups in the GroupQuery.
   */
  bool joined() const;

  using Requests = std::vector<cbe::Request>;
  /**
   * Requests to join the group. To retrieve the list call
   *  listJoinRequests(cbe::GroupDelegatePtr delegate) on the group object.
   */
  Requests requests() const;
  /**
   * Pointer to CreateGroupDelegate that is passed into: \n 
   * Group::createGroup(std::string,std::string,CreateGroupDelegatePtr,cbe::Visibility).
   */
  using CreateGroupDelegatePtr = delegate::CreateGroupDelegatePtr;
  /**
   * @brief Create a new Group.
   * 
   * \note Can only be used by Tenant admin/owners to create new groups.
   * @param name of the group
   * @param memberAlias another popular name
   * @param delegate Pointer to a delegate::CreateGroupDelegate instance 
   *                 that is implemented by the user.   
   * @param visibility default: public
   * @return cbe::Group
   * \see Types.h for cbe::visibility enum 
   * cbe::Visibility::PublicGroup or cbe::Visibility::Private
   */
  cbe::Group createGroup(std::string            name,
                         std::string            memberAlias,
                         cbe::Visibility        visibility, 
                         CreateGroupDelegatePtr delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::group::CreateGroupDelegate::Exception
  */
  using CreateGroupException = delegate::CreateGroupDelegate::Exception;
  /**
   * @brief Synchronous [exception] creates a new group 
   * <b>Synchronous</b> version of
   * createGroup(name, memberAlias, delegate,
   *             visibility = cbe::Visibility::Public, 
   *             CreateGroupDelegatePtr)
   * , and <b>throws an exception</b>, #CreateGroupException, in case of a failed
   * call.
   * <br>See createGroup(CreateGroupDelegatePtr)
   *
   * @return Information about the createGroup object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::CreateGroupSuccess " "
   * @throws #CreateGroupException
   */
  cbe::Group createGroup(
                           std::string            name,
                           std::string            memberAlias,
                           cbe::Visibility        visibility);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref createGroup(name, memberAlias, delegate, visibility, CreateGroupError&) "createGroup()"
   * <br>See delegate::CreateGroupDelegate::ErrorInfo
   */
  using CreateGroupError = delegate::CreateGroupDelegate::ErrorInfo;
  /**
   * <b>Synchronous</b> version of
   * createGroup(name, memberAlias, delegate, visibility, CreateGroupDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See createGroup(name, memberAlias, delegate, visibility, CreateGroupDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #CreateGroupError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicating a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::Group> createGroup(
                                            std::string            name,
                                            std::string            memberAlias,
                                            cbe::Visibility        visibility,
                                            CreateGroupError&  error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::group::JoinDelegate that is passed into
   * asynchronous version of method
   * @ref join(JoinDelegatePtr) "join()"
   */
  using JoinDelegatePtr = delegate::group::JoinDelegatePtr;
  /**
   * @brief Synchronous [exception] Ask to join a group.
   * In this first version All members will be Public, 
   * meaning visible for other member inside the group. All groups will also be
   * open so all join requests should be accepted directly.
   * 
   * \note This is completely different from the query chain join.

   * @param alias    name
   * @param delegate Pointer to a delegate::group::JoinDelegate instance 
   *                 that is implemented by the user.   
   * @param memberVisibility default: public
   * @param applicationComment message to the owner regarding 
   * the application to join
   */
  void join(std::string           alias,
            cbe::Visibility       memberVisibility,
            std::string           applicationComment,
            JoinDelegatePtr       delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::group::JoinDelegate::Exception
  */
  using JoinException = delegate::group::JoinDelegate::Exception;
  /**
   * @brief Synchronous [exception] Ask to join a group.
   * <b>Synchronous</b> version of
   * join(std::string, cbe::Visibility, std::string)
   * , and <b>throws an exception</b>, #JoinException, in case of a failed
   * call.
   * <br>See join(JoinDelegatePtr)
   *
   * @return Information about the Join
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::JoinSuccess " "
   * @throws #JoinException
   */
  cbe::Group join(
            std::string           alias,
            cbe::Visibility       memberVisibility,
            std::string           applicationComment);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref join(alias, memberVisibility, applicationComment, JoinError&) "join()"
   * <br>See delegate::JoinDelegate::ErrorInfo
   */
  using JoinError = delegate::group::JoinDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] Ask to join a group.
   * <b>Synchronous</b> version of
   * join(alias, memberVisibility, applicationComment, JoinError&,JoinDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See join(alias, memberVisibility, applicationComment, JoinError&, JoinDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #JoinError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicating a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::Group> join(
            std::string           alias,
            cbe::Visibility       memberVisibility,
            std::string           applicationComment,
            JoinError&            error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::LeaveDelegate that is passed into
   * asynchronous version of method
   * @ref leave(LeaveDelegatePtr) "leave()"
   */
  using LeaveDelegatePtr = delegate::LeaveDelegatePtr;  
  /**
   * @brief Leave group.
   * 
   * @param delegate Pointer to a delegate::LeaveDelegate instance 
   *                 that is implemented by the user.   
   */
  void leave(LeaveDelegatePtr delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::group::LeaveDelegate::Exception
  */
  using LeaveException = delegate::LeaveDelegate::Exception;
  /**
   * @brief Synchronous [exception] leaves the specified group.
   * <b>Synchronous</b> version of
   * leave(LeaveDelegatePtr)
   * , and <b>throws an exception</b>, #LeaveException, in case of a failed
   * call.
   * <br>See leave(LeaveDelegatePtr)
   *
   * @return Information about the leave
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::LeaveSuccess " "
   * @throws #LeaveException
   */
  delegate::LeaveSuccess leave();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref leave(LeaveError&) "leave()"
   * <br>See delegate::LeaveDelegate::ErrorInfo
   */
  using LeaveError = delegate::LeaveDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] leaves the specified group.
   * <b>Synchronous</b> version of
   * leave(LeaveDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See leave(LeaveDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #LeaveError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicating a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::LeaveSuccess> leave(LeaveError&           error);

#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::RemoveDelegate that is passed into
   * asynchronous version of method
   * @ref remove(RemoveDelegatePtr) "remove()"
   */

  using RemoveDelegatePtr = delegate::group::RemoveDelegatePtr;
  /**
   * @brief Remove group.
   * 
   * \note This is exlusive for Tenant group owners.
   * 
   * @param delegate Pointer to a delegate::group::RemoveDelegate instance 
   *                 that is implemented by the user.   
   */
  void remove(RemoveDelegatePtr delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::group::RemoveDelegate::Exception
  */
  using RemoveException = delegate::group::RemoveDelegate::Exception;
  /**
   * @brief Synchronous [exception] removes the specified group.
   * <b>Synchronous</b> version of
   * remove(RemoveDelegatePtr)
   * , and <b>throws an exception</b>, #RemoveException, in case of a failed
   * call.
   * <br>See remove(RemoveDelegatePtr)
   *
   * @return Information about the removed group
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::group::RemoveSuccess " "
   * @throws #RemoveException
   */
  delegate::group::RemoveSuccess remove();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref remove(RemoveError&) "remove()"
   * <br>See delegate::group::RemoveDelegate::ErrorInfo
   */
  using RemoveError = delegate::group::RemoveDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] removes the specified group.
   * <b>Synchronous</b> version of
   * remove(RemoveDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See remove(RemoveDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #RemoveError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicating a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::group::RemoveSuccess> remove(RemoveError& error);

#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::RenameDelegate that is passed into
   * asynchronous version of method
   * @ref rename(RenameDelegatePtr)   "rename()"
   */
  using RenameDelegatePtr = delegate::group::RenameDelegatePtr;
  /**
   * @brief Rename the Group; group id does not change.
   * 
   * @param newName  the new name for the group
   * @param delegate Pointer to a delegate::group::RenameDelegate instance 
   *                 that is implemented by the user.   
   */
  void rename(std::string newName, RenameDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
  * See delegate::group::RenameDelegate::Exception
  */
  using RenameException = delegate::group::RenameDelegate::Exception;
  /**
   * @brief Synchronous [exception] renames a group
   * <b>Synchronous</b> version of
   * rename(std::string ,RenameDelegatePtr)
   * , and <b>throws an exception</b>, #RenameException, in case of a failed
   * call.
   * <br>See rename(RenameDelegatePtr)
   *
   * @return Information about the renamed group 
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::RenameSuccess " "
   * @throws #RenameException
   */
  delegate::group::RenameSuccess rename(std::string newName);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref rename(newName, RenameError&) "rename()"
   * <br>See delegate::group::RenameDelegate::ErrorInfo
   */
  using RenameError = delegate::group::RenameDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] renames a group
   * <b>Synchronous</b> version of
   * rename(newName,RenameDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See rename(newName, RenameDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #RenameError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicating a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::group::RenameSuccess> rename(
                                              std::string newName, 
                                              RenameError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::ListMembersDelegate that is passed into
   * asynchronous version of method
   * @ref listMembers(ListMembersDelegatePtr) "listMembers()"
   */
  using ListMembersDelegatePtr = delegate::ListMembersDelegatePtr;
  /**
   * @brief List all members in the group.
   * 
   * The member list is then retrieved via the delegate callback function
   * cbe::delegate::ListMembersDelegate::onListMembersSuccess()
   * 
   * @param delegate Pointer to a delegate::ListMembersDelegate instance
   *                 that is implemented by the user.
   */
  void listMembers(ListMembersDelegatePtr delegate);
  
#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::ListMembersDelegate::Exception
  */
  using ListMembersException = delegate::ListMembersDelegate::Exception;
  /**
   * @brief Synchronous [exception] list members of the specifed group.
   * <b>Synchronous</b> version of
   * listMembers(ListMembersDelegatePtr)
   * , and <b>throws an exception</b>, #ListMembersException, in case of a failed
   * call.
   * <br>See listMembers(ListMembersDelegatePtr)
   *
   * @return Information about the listMembers object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::ListMembersSuccess " "
   * @throws #ListMembersException
   */
  delegate::ListMembersDelegate::Members listMembers();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref listMembers(ListMembersError&) "listMembers()"
   * <br>See delegate::ListMembersDelegate::ErrorInfo
   */
  using ListMembersError = delegate::ListMembersDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] list members of the specifed group.
   * <b>Synchronous</b> version of
   * listMembers(ListMembersDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See listMembers(ListMembersDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #ListMembersError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicating a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::ListMembersDelegate::Members> listMembers(
                                             ListMembersError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::ListMembersDelegate that is passed into
   * asynchronous version of method
   * @ref listBannedMembers(ListMembersDelegatePtr)   "listBannedMembers()"
   */
  using ListBannedMembersDelegatePtr = delegate::ListMembersDelegatePtr;
  /**
   * @brief Lists all banned former members, or users.
   * 
   * @param delegate Pointer to a delegate::ListMembersDelegate instance 
   *                 that is implemented by the user.
   */
  void listBannedMembers(ListBannedMembersDelegatePtr delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::ListMembersDelegate::Exception
  */
  using ListBannedMembersException = delegate::ListMembersDelegate::Exception;
  /**
   * @brief Synchronous [exception] list the banned members of the specified group.
   * <b>Synchronous</b> version of
   * listBannedMembers(ListMembersDelegatePtr)
   * , and <b>throws an exception</b>, #ListBannedMembersException, in case of a 
   * failed call.
   * <br>See listBannedMembers(ListMembersDelegatePtr)
   *
   * @return Information about the listBannedMembers object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::ListBannedMembersSuccess " "
   * @throws #ListBannedMembersException
   */
  std::vector<cbe::Member> listBannedMembers();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref listBannedMembers(ListBannedMembersError&) "listBannedMembers()"
   * <br>See delegate::ListMembersDelegate::ErrorInfo
   */
  using ListBannedMembersError = delegate::ListMembersDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] list the banned members of the specified 
   * group. <b>Synchronous</b> version of
   * listBannedMembers(ListMembersDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See listBannedMembers(ListMembersDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #ListBannedMembersError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicating a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<std::vector<cbe::Member>> listBannedMembers(
                                                  ListBannedMembersError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::ListRolesDelegate that is passed into
   * asynchronous version of method
   * @ref listRoles(ListRolesDelegatePtr) "listRoles()"
   */
  using ListRolesDelegatePtr = delegate::ListRolesDelegatePtr;
  /**
   * @brief Lists all roles in the group.
   * 
   * @param delegate Pointer to a delegate::ListRolesDelegate instance 
   *                 that is implemented by the user.
   */
  void listRoles(ListRolesDelegatePtr delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::ListRolesDelegate::Exception
  */
  using ListRolesException = delegate::ListRolesDelegate::Exception;
  /**
   * @brief Synchronous [exception] list roles of a group.
   * <b>Synchronous</b> version of
   * listRoles(ListRolesDelegatePtr)
   * , and <b>throws an exception</b>, #ListRolesException, in case of a failed
   * call.
   * <br>See listRoles(ListRolesDelegatePtr)
   *
   * @return Information about the ListRoles object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::ListRolesDelegate::Roles " "
   * @throws #ListRolesException
   */
  delegate::ListRolesDelegate::Roles listRoles();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref listRoles(ListRolesError&) "listRoles()"
   * <br>See delegate::ListRolesDelegate::ErrorInfo
   */
  using ListRolesError = delegate::ListRolesDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] list roles of a group.
   * <b>Synchronous</b> version of
   * listRoles(ListRolesDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See listRoles(ListRolesDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #ListRolesError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicating a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::ListRolesDelegate::Roles> listRoles(
                                              ListRolesError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::CreateRoleDelegate that is passed into
   * asynchronous version of method
   * @ref createRole(CreateRoleDelegatePtr) "createRole()"
   */
  using CreateRoleDelegatePtr = delegate::CreateRoleDelegatePtr;
  /**
   * @brief Creates a new role in the group.
   * 
   * @param name The name of the role.
   *
   * @param delegate Pointer to a delegate::CreateRoleDelegate instance
   * that is implemented by the user.
   */
  void createRole(std::string name, CreateRoleDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::CreateRoleDelegate::Exception
  */
  using CreateRoleException = delegate::CreateRoleDelegate::Exception;
  /**
   * @brief Synchronous [exception] creates a new role in the specified group.
   * <b>Synchronous</b> version of
   * createRole(std::string, CreateRoleDelegatePtr)
   * , and <b>throws an exception</b>, #CreateRoleException, in case of a failed
   * call.
   * <br>See createRole(CreateRoleDelegatePtr)
   *
   * @return Information about the createRole object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::CreateRoleSuccess " "
   * @throws #CreateRoleException
   */
  cbe::Role createRole(std::string name);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref createRole(name,CreateRoleError&) "createRole()"
   * <br>See delegate::CreateRoleDelegate::ErrorInfo
   */
  using CreateRoleError = delegate::CreateRoleDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] creates a new role in the specified group.
   * <b>Synchronous</b> version of
   * createRole(name,CreateRoleDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See createRole(name, CreateRoleDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #CreateRoleError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicating a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::Role> createRole(std::string name,
                                            CreateRoleError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::RemoveRoleDelegate that is passed into
   * asynchronous version of method
   * @ref removeRole(RemoveRoleDelegatePtr) "removeRole()"
   */
  using RemoveRoleDelegatePtr = delegate::RemoveRoleDelegatePtr;
  /**
   * @brief Removes a role in the group.
   * 
   * @param roleId The id of the role to remove.
   *
   * @param delegate Pointer to a delegate::RemoveRoleDelegate instance
that is implemented by the user.
   */
  void removeRole(RoleId roleId, RemoveRoleDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::RemoveRoleDelegate::Exception
  */
  using RemoveRoleException = delegate::RemoveRoleDelegate::Exception;
  /**
   * @brief Synchronous [exception] removes a role in the specified group.
   * <b>Synchronous</b> version of
   * removeRole(RoleId roleId, RemoveRoleDelegatePtr)
   * , and <b>throws an exception</b>, #RemoveRoleException, in case of a failed
   * call.
   * <br>See removeRole(RemoveRoleDelegatePtr)
   *
   * @return Information about the removeRole object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::RemoveRoleSuccess " "
   * @throws #RemoveRoleException
   */
  cbe::RoleId removeRole(RoleId roleId);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref removeRole(roleId, RemoveRoleError&) "removeRole()"
   * <br>See delegate::RemoveRoleDelegate::ErrorInfo
   */
  using RemoveRoleError = delegate::RemoveRoleDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] removes a role in the specified group.
   * <b>Synchronous</b> version of
   * removeRole(roleId, RemoveRoleDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See removeRole(roleId,  RemoveRoleDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #RemoveRoleError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicating a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::RoleId> removeRole(RoleId roleId,
                                            RemoveRoleError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /**
   * @brief Default constructor.
   * 
   * Construct a new object with 
   * the \c DefaultCtor to enable the
   * @ref operator bool()
   * test
   */
  Group(cbe::DefaultCtor);
  ~Group();

  /**
   * @brief Checks if the current instance is real.
   * 
   * An "unreal" instance implies typically a failed event.
   * 
   * Relies on the \c Default \c constructor
   * Group(cbe::DefaultCtor)
   * 
   * @return \c true  : is real
   * @return \c false : unreal; got nullptr; if current instance is unbound/undefined. I.e., if it is
   *         only default constructed.
   */
  explicit operator bool() const;

private:
  struct Impl;
  std::shared_ptr<Impl> pImpl{};
  friend class GroupManager;
  friend class GroupQueryResult;
  Group(CBI::GroupPtr cbiPeerPtr);
  CBI::Group& getCbiPeer() const;
}; // class Group

} // namespace cbe

#endif  // INCLUDE_CBE_GROUP_H_
