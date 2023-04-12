/*
    Copyright © CloudBackend AB 2022.
*/

#ifndef CBE_MEMBER_H
#define CBE_MEMBER_H

#include "CBEIOS/delegate/KickDelegate.h"
#include "CBEIOS/delegate/BanDelegate.h"
#include "CBEIOS/delegate/UnBanDelegate.h"

@interface Request : NSObject {
CBEUserId userId;
NSString* alias;
NSString* applicationComment;
CBEDate   date;
}
@end
@interface Role : NSObject {
NSString* role;
}
@end

/**
 * @brief list of members of a Group.
 * 
 */
@interface CBEMember : NSObject {
}
/**
 * Kick out a member from a group
 * 
 * @param kickComment free text describing the reason for this decision
 * @param delegate a shared pointer to the class in which you implement
 * cbe::delegate::KickDelegate to receive the callback on server completion.
 */
-(void) kick:     (NSString*)           kickComment 
        delegate: (id<KickDelegatePtr>) delegate;

/**
 * Ban or evict a member from a group
 * 
 * @param banComment  Free text describing the reason for this decision
 * @param delegate    Shared pointer to the class in which you implement
 *                    cbe::delegate::BanDelegate to receive the callback upon
 *                    completion of this request.
 */
-(void) ban:      (NSString*)          banComment 
        delegate: (id<BanDelegatePtr>) delegate;

/**
 * Revoke a ban of a member from a group
 * 
 * @param delegate a shared pointer to the class in which you implement
 * cbe::delegate::UnBanDelegate to receive the callback on server completion.
 */
-(void) unBan:(id<UnBanDelegatePtr>) delegate;

/** the member id number */
-(CBEMemberId) memberId;

/** Returns the member name.*/
-(NSString*) name;

/** see the enum \c Visibility in Types.h */
-(CBEVisibility) visibility;

/** the group id number */
-(CBEGroupId)    groupId;

/**
 * @brief Gets the @ref cbe::Member "member" ban info map.
 * 
 */
- (id) initWith:(void*) cbiPeerPtr;

@end
#endif //CBE_MEMBER_H
