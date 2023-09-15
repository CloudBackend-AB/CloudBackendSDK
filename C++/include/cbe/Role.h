/*
      Copyright © CloudBackend AB 2020-2023.
 */

#ifndef INCLUDE_CBE_Role_H_
#define INCLUDE_CBE_Role_H_

#include <string>
#include <vector>
#include <map>

#include "cbe/Types.h"
#include "cbe/delegate/AddRoleMemberDelegate.h"
#include "cbe/delegate/ListMembersDelegate.h"
#include "cbe/delegate/RemoveRoleMemberDelegate.h"
#include "cbe/Database.h"

#include "cbe/delegate/Error.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"
#include "util/Optional.h"

namespace CBI {
class Role;
using RolePtr = std::shared_ptr<CBI::Role>;
} // namespace CBI

namespace cbe
{
/**
 * @brief User %role information.
 * 
 */
class Role {
public:
  /**
   * Name of the role.
   */
  std::string name() const;

  /**
   * Id of the role.
   */
  cbe::RoleId id() const;

  /**
   * Id of the group the role belongs to.
   */
  cbe::GroupId groupId() const;

  /**
   * Pointer to cbe::delegate::ListMembersDelegate that is passed into
   * asynchronous version of method
   * @ref listMembers(ListMembersDelegatePtr) "listMembers()"
   */
  using ListMembersDelegatePtr = delegate::ListMembersDelegatePtr;
  /**
   * @brief Lists the members of this role,
   * 
   * @param delegate Pointer to a delegate::ListMembersDelegate instance that is 
   *        implemented by the user.
   */
  void listMembers(ListMembersDelegatePtr delegate);
  /**
   * Pointer to cbe::delegate::ListMembersDelegate that is passed into
   * asynchronous version of method
   * @ref listMembers(ListMembersDelegatePtr) "listMembers()"
   */

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::ListMembersDelegate::Exception
  */
  using ListMembersException = delegate::ListMembersDelegate::Exception;
  /**
   * @brief Synchronous [exception]
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
  std::vector<cbe::Member> listMembers();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref listMembers(ListMembersError&) "listMembers()"
   * <br>See delegate::ListMembersDelegate::ErrorInfo
   */
  using ListMembersError = delegate::ListMembersDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing]
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
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<std::vector<cbe::Member>> listMembers(
                                              ListMembersError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /**
   * Pointer to AddRoleMemberDelegate that is passed into:
   * <ul>
   *   <li> cbe::Group::AddRoleMember(AddRoleMemberDelegatePtr).
   * </ul>
   */
  using AddRoleMemberDelegatePtr = delegate::AddRoleMemberDelegatePtr;
  /**
   * @brief Adds a member to this role.
   * 
   * @param memberId The id of the member to add to the role.
   * 
   * @param delegate Pointer to a delegate::AddRoleMemberDelegate instance that is 
   *        implemented by the user.
   */
  void addRoleMember(MemberId memberId, AddRoleMemberDelegatePtr delegate);
  /**
   * Pointer to cbe::delegate::AddRoleMemberDelegate that is passed into
   * asynchronous version of method
   * @ref AddRoleMember(AddRoleMemberDelegatePtr)   "AddRoleMember()"
   */

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::AddRoleMemberDelegate::Exception
  */
  using AddRoleMemberException = delegate::AddRoleMemberDelegate::Exception;
  /**
   * @brief Synchronous [exception]
   * <b>Synchronous</b> version of
   * AddRoleMember(MemberId, AddRoleMemberDelegatePtr)
   * , and <b>throws an exception</b>, #AddRoleMemberException, in case of a failed
   * call.
   * <br>See AddRoleMember(AddRoleMemberDelegatePtr)
   *
   * @return Information about the AddRoleMember object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::AddRoleMemberDelegate::Success " "
   * @throws #AddRoleMemberException
   */
  MemberId addRoleMember(MemberId memberId);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref AddRoleMember(memberId, AddRoleMemberError&) "AddRoleMember()"
   * <br>See delegate::AddRoleMemberDelegate::ErrorInfo
   */
  using AddRoleMemberError = delegate::AddRoleMemberDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing]
   * <b>Synchronous</b> version of
   * AddRoleMember(memberId, AddRoleMemberDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See AddRoleMember(memberId, AddRoleMemberDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #AddRoleMemberError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<MemberId> addRoleMember(MemberId            memberId,
                                              AddRoleMemberError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /**
   * Pointer to RemoveRoleMember that is passed into: \n
   * Group::RemoveRoleMember(std::string, RemoveRoleMemberDelegatePtr).
   */
  using RemoveRoleMemberDelegatePtr = delegate::RemoveRoleMemberDelegatePtr;
  /**
   * @brief Removes a member to this role.
   * 
   * @param memberId The id of the member to remove from the role.
   *
   * @param delegate Pointer to a delegate::RemoveRoleMember instance that 
   *        is implemented by the user.
   */
  void removeRoleMember(MemberId memberId, RemoveRoleMemberDelegatePtr delegate);
  /**
   * Pointer to cbe::delegate::RemoveRoleMember that is passed into
   * asynchronous version of method
   * @ref RemoveRoleMember(RemoveRoleMemberDelegatePtr) "RemoveRoleMember()"
   */

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::RemoveRoleMember::Exception
  */
  using RemoveRoleMemberException = delegate::RemoveRoleMemberDelegate::Exception;
  /**
   * @brief Synchronous [exception]
   * <b>Synchronous</b> version of
   * RemoveRoleMember(MemberId memberId, RemoveRoleMemberDelegatePtr)
   * , and <b>throws an exception</b>, #RemoveRoleMemberException, in case of a failed
   * call.
   * <br>See RemoveRoleMember(RemoveRoleMemberDelegatePtr)
   *
   * @return Information about the RemoveRoleMember object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::RemoveRoleMember::Success " "
   * @throws #RemoveRoleMemberException
   */
  MemberId removeRoleMember(MemberId memberId);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref RemoveRoleMember(memberId,RemoveRoleMemberError&) "RemoveRoleMember()"
   * <br>See delegate::RemoveRoleMember::ErrorInfo
   */
  using RemoveRoleMemberError = delegate::RemoveRoleMemberDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing]
   * <b>Synchronous</b> version of
   * RemoveRoleMember(memberId,RemoveRoleMemberDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See RemoveRoleMember(memberId, RemoveRoleMemberDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #RemoveRoleMemberError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<MemberId> removeRoleMember(MemberId               memberId,
                                                 RemoveRoleMemberError& error);
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
  Role(cbe::DefaultCtor);
  ~Role();

  /**
   * @brief Checks if the current instance is real.
   * 
   * An "unreal" instance implies typically a failed event.
   * 
   * Relies on the \c Default \c constructor
   * Role(cbe::DefaultCtor)
   * 
   * @return \c true  : is real
   * @return \c false : unreal; got nullptr; if current instance is unbound/undefined. I.e., if it is
   *         only default constructed.
   */
  explicit operator bool() const;
private:
  struct Impl;
  std::shared_ptr<Impl> pImpl{};

  friend class Group;
  Role(CBI::RolePtr cbiPeerPtr);
  CBI::Role& getCbiPeer() const;
}; // class Role

} // namespace cbe

#endif // INCLUDE_CBE_Role_H_
