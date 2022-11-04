/*
     Copyright © CloudBackend AB 2022.
*/

#ifndef CBE_MEMBER_H
#define CBE_MEMBER_H

#include "cbe/Types.h"

#include "cbe/delegate/KickDelegate.h"
#include "cbe/delegate/BanDelegate.h"
#include "cbe/delegate/UnBanDelegate.h"

namespace CBI {
class Member;
using MemberPtr = std::shared_ptr<CBI::Member>;
class GroupEventProtocol;
using GroupDelegatePtr = std::shared_ptr<GroupEventProtocol>;
} // namespace CBI

namespace cbe {

#ifndef DOXYGEN_SHOULD_SKIP_THIS

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

struct Role {
  std::string role;
};

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/**
 * @brief list of members of a Group.
 * 
 */
class Member {
public:
  using KickDelegatePtr = delegate::KickDelegatePtr;
  /**
   * Kick out a member from a group
   * 
   * @param kickComment free text describing the reason for this decision
   * @param delegate a shared pointer to the class in which you implement
   * cbe::delegate::KickDelegate to receive the callback on server completion.
   */
  void kick(std::string kickComment, KickDelegatePtr delegate);
  
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
  
  using UnBanDelegatePtr = delegate::UnBanDelegatePtr;
  /**
   * Revoke a ban of a member from a group
   * 
   * @param delegate a shared pointer to the class in which you implement
   * cbe::delegate::UnBanDelegate to receive the callback on server completion.
   */
  void unBan(UnBanDelegatePtr delegate);

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

  Member(cbe::DefaultCtor);
  ~Member();

  explicit operator bool() const;
private:
  struct Impl;

  std::shared_ptr<Impl> pImpl{};

  friend class Group;
  Member(CBI::MemberPtr cbiPeerPtr);
  CBI::Member& getCbiPeer() const;
}; // class Member

} // namespace cbe

#endif //CBE_MEMBER_H
