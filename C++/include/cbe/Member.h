/*
     Copyright © CloudBackend AB 2023.
*/

#ifndef CBE_MEMBER_H
#define CBE_MEMBER_H

#include "cbe/Types.h"

#include "cbe/delegate/KickDelegate.h"
#include "cbe/delegate/BanDelegate.h"
#include "cbe/delegate/UnBanDelegate.h"

#include "cbe/delegate/Error.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"
#include "cbe/util/Optional.h"
namespace CBI {
class Member;
using MemberPtr = std::shared_ptr<CBI::Member>;
class GroupEventProtocol;
using GroupDelegatePtr = std::shared_ptr<GroupEventProtocol>;
} // namespace CBI

namespace cbe {

/**
 * Contains information on a request for a Member to join a group.
 */
struct Request {
  cbe::UserId userId{};
  std::string alias{};
  std::string applicationComment{};
  cbe::Date   date{};

  Request(cbe::UserId userId, std::string alias, std::string applicationComment,
          cbe::Date date)
      : userId{userId}, alias{std::move(alias)},
        applicationComment{std::move(applicationComment)}, date{date} {}
};

/**
 * @brief A of member of a Group.
 */
class Member {
public:
  /**
   * Pointer to KickDelegate that is passed into:
   * <ul>
   *   <li> cbe::Member::kick(std::string,KickDelegatePtr)
   * </ul>
   */
  using KickDelegatePtr = delegate::KickDelegatePtr;
  /**
   * Kick out a member from a group
   * 
   * @param kickComment free text describing the reason for this decision
   * @param delegate a shared pointer to the class in which you implement
   * cbe::delegate::KickDelegate to receive the callback on server completion.
   */
  void kick(std::string kickComment, KickDelegatePtr delegate);
  /**
   * Pointer to cbe::delegate::KickDelegate that is passed into
   * asynchronous version of method
   * @ref kick(KickDelegatePtr) "kick()"
   */

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::KickDelegate::Exception
  */
  using KickException = delegate::KickDelegate::Exception;
  /**
   * @brief Synchronous [exception]
   * <b>Synchronous</b> version of
   * kick(std::string kickComment, KickDelegatePtr)
   * , and <b>throws an exception</b>, #KickException, in case of a failed
   * call.
   * <br>See kick(KickDelegatePtr)
   *
   * @return Information about the kick object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::KickSuccess " "
   * @throws #KickException
   */
  delegate::KickSuccess kick(
                           std::string kickComment);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref kick(kickComment, KickError&) "kick()"
   * <br>See delegate::KickDelegate::ErrorInfo
   */
  using KickError = delegate::KickDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing]
   * <b>Synchronous</b> version of
   * kick(kickComment, KickDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See kick(kickComment,  KickDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #KickError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::KickSuccess> kick(
                                              std::string kickComment,
                                              KickError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /**
   * Pointer to BanDelegate that is passed into:
   * <ul>
   *   <li> cbe::Member::ban(std::string,BanDelegatePtr)
   * </ul>
   */
  using BanDelegatePtr = delegate::BanDelegatePtr;
  /**
   * Ban or evict a member from a group
   * 
   * @param banComment  Free text describing the reason for this decision
   * @param delegate    Shared pointer to the class in which you implement
   *                    cbe::delegate::BanDelegate to receive the callback upon
   *                    completion of this request.
   */
  void ban(std::string banComment, BanDelegatePtr delegate);
  /**
   * Pointer to cbe::delegate::BanDelegate that is passed into
   * asynchronous version of method
   * @ref ban(BanDelegatePtr) "ban()"
   */

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::BanDelegate::Exception
  */
  using BanException = delegate::BanDelegate::Exception;
  /**
   * @brief Synchronous [exception]
   * <b>Synchronous</b> version of
   * ban(std::string,BanDelegatePtr)
   * , and <b>throws an exception</b>, #BanException, in case of a failed
   * call.
   * <br>See ban(BanDelegatePtr)
   *
   * @return Information about the ban object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::BanSuccess " "
   * @throws #BanException
   */
  delegate::BanSuccess ban(std::string banComment);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref ban(banComment,BanError&) "ban()"
   * <br>See delegate::BanDelegate::ErrorInfo
   */
  using BanError = delegate::BanDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing]
   * <b>Synchronous</b> version of
   * ban(banComment,BanDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See ban(banComment, BanDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #BanError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::BanSuccess> ban(std::string         banComment,
                                                BanError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /**
   * Pointer to UnBanDelegate that is passed into:
   * <ul>
   *   <li> cbe::Member::unBan(UnBanDelegatePtr)
   * </ul>
   */
  using UnBanDelegatePtr = delegate::UnBanDelegatePtr;
  /**
   * Revoke a ban of a member from a group
   * 
   * @param delegate a shared pointer to the class in which you implement
   * cbe::delegate::UnBanDelegate to receive the callback on server completion.
   */
  void unBan(UnBanDelegatePtr delegate);
  /**
   * Pointer to cbe::delegate::UnBanDelegate that is passed into
   * asynchronous version of method
   * @ref unBan(UnBanDelegatePtr) "unBan()"
   */

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::UnBanDelegate::Exception
  */
  using UnBanException = delegate::UnBanDelegate::Exception;
  /**
   * @brief Synchronous [exception]
   * <b>Synchronous</b> version of
   * unBan(UnBanDelegatePtr)
   * , and <b>throws an exception</b>, #UnBanException, in case of a failed
   * call.
   * <br>See unBan(UnBanDelegatePtr)
   *
   * @return Information about the unBan object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::UnBanSuccess " "
   * @throws #UnBanException
   */
  delegate::UnBanSuccess unBan();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref unBan(UnBanError&) "unBan()"
   * <br>See delegate::UnBanDelegate::ErrorInfo
   */
  using UnBanError = delegate::UnBanDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing]
   * <b>Synchronous</b> version of
   * unBan(UnBanDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See unBan(UnBanDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #UnBanError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::UnBanSuccess> unBan(UnBanError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /** the member id number */
  cbe::MemberId memberId() const;

  /** Returns the member name.*/
  std::string name() const;

  /** see the enum \c Visibility in Types.h */
  cbe::Visibility visibility() const;

  /** the group id number */
  cbe::GroupId    groupId() const;

  /**
   * @brief Gets the @ref cbe::Member "member" ban info map.
   * 
   */
  cbe::MemberBanInfo getMemberBanInfo();

  /**
   * @brief Default constructor.
   * 
   * Construct a new object with 
   * the \c DefaultCtor to enable the
   * @ref operator bool()
   * test
   */
  Member(cbe::DefaultCtor);
  ~Member();

  /**
   * @brief Checks if the current instance is real.
   * 
   * An "unreal" instance implies typically a failed event.
   * 
   * Relies on the \c Default \c constructor
   * Member(cbe::DefaultCtor)
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
  friend class Role;
  Member(CBI::MemberPtr cbiPeerPtr);
  CBI::Member& getCbiPeer() const;
}; // class Member

} // namespace cbe

#endif //CBE_MEMBER_H
