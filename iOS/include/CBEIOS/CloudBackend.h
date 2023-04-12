/*
     Copyright © CloudBackend AB 2020 - 2023.
*/
#import <Foundation/Foundation.h>

#import "CBEIOS/delegate/LogInDelegate.h"
#import "CBEIOS/Account.h"
#import "CBEIOS/ShareManager.h"
#import "CBEIOS/SubscribeManager.h"
#import "CBEIOS/PublishManager.h"
#import "CBEIOS/GroupManager.h"

/**
 * @brief The session that holds the connection with the cloud.
 * 
 */

@interface CloudBackend : NSObject {

@private
  void * pImpl;
}

+(id)login:(NSString*)username password:(NSString*)password tenant:(NSString*)tenant delegate:(id<LogInDelegatePtr>)delegate;
/**
 * Clears the cache if you are having memory issues.
 * 
 * @return true  success
 * @return false failed
 */
-(BOOL) clearCache;

/**
 * returns an account objct with information on the user
 * 
 * @return cbe::Account 
 */
-(CBEAccount*) account;

/**
 * returns the version number of the SDK
 * 
 * @return std::string 
 */
-(NSString*) version;

/**
 * gives the groups available
 * 
 * @return cbe::GroupManager 
 */
-(CBEGroupManager*) groupManager;  // Group - NYI

/**
 * gives the shares available
 * 
 * @return cbe::ShareManager 
 */
-(CBEShareManager*) shareManager;
/**
 * gives the published web shares
 * 
 * @return cbe::PublishManager 
 */
-(CBEPublishManager*) publishManager;
/**
 * gives the subscriptions being followed
 * 
 * @return cbe::SubscribeManager 
 */
-(CBESubscribeManager*) subscribeManager;

-(id) init_;
@end
