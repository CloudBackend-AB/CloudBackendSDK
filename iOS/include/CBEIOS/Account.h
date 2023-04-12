/*
      Copyright © CloudBackend AB 2020 - 2023.
 */

#ifndef INCLUDE_CBEIOS_ACCOUNT_H_
#define INCLUDE_CBEIOS_ACCOUNT_H_
#import <Foundation/Foundation.h>

#include "CBEIOS/Types.h"
#include "CBEIOS/Container.h"

/**
 * @brief login account information
 * 
 */
@interface CBEAccount : NSObject {

}
  /** Returns the account id of the user. */
- (CBEUserId) userId;

/** Returns the username of the account. */
- (NSString*) username ;

/** Returns the password of the account. */
- (NSString*) password ;

/** Returns the tenant admin of the account 
 * and owner of the tenant group database. */
- (NSString*) source ;

/** Returns the given name of the user. */
- (NSString*) firstName ;

/** Returns the surname of the user. */
- (NSString*) lastName ;

- (id) initWith:(void*) cbiPeerPtr;

/** Returns the rootContainer for the account. */
- (CBEContainer*) rootContainer;

@end
#endif // INCLUDE_CBE_ACCOUNT_H_
