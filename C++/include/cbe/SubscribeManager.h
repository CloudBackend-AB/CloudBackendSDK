/*
  Copyright © CloudBackend AB 2022.
*/

#ifndef INCLUDE_CBE_SUBSCRIBE_MANAGER_H_
#define INCLUDE_CBE_SUBSCRIBE_MANAGER_H_

#include <vector>

#include "cbe/Subscribe.h"
#include "cbe/Types.h"

#include "cbe/delegate/SubscribeDelegate.h"
#include "cbe/delegate/GetSubscriptionsDelegate.h"

namespace CBI {
class SubscribeManager;
} // namespace CBI

namespace cbe {
/**
 * @brief Class for managing Publish, represented as a list.
 */
class SubscribeManager {
public:
  using GetSubscriptionsDelegatePtr = delegate::GetSubscriptionsDelegatePtr;
  /**
   * Lists of publish that you are sharing with others. Listing is
   * done independently of where in the actual directory tree the files
   * are located.
   *
   * @param subscribeDelegate Pointer to a delegate::GetSubscriptionsDelegate
   *                          instance that is implemented by the user.
   */
  void getSubscriptions(GetSubscriptionsDelegatePtr subscribeDelegate);

  using SubscribeDelegatePtr = delegate::SubscribeDelegatePtr;
  /**
   * Subscribes to a publish shared by some other user (or yourself).
   * It must not require a password, 
   * otherwise use the other subscribe() method overload.
   * @param sharingUserName   User name of the owner of the publish
   * @param publishName       Publish name that will also be the name of created
   *                          favorite.
   * @param subscribeDelegate Gets notified of the result
   */
  void subscribe(std::string          sharingUserName,
                 std::string          publishName,
                 SubscribeDelegatePtr subscribeDelegate) {
    subscribe(0, sharingUserName, 0, publishName, "", publishName,
              subscribeDelegate);
  }

  /**
   * Subscribes to a publish shared by some other user (or yourself).
   * It must not require a password, otherwise use the other subscribe() 
   * method overload.
   * @param sharingUserId User id of the owner of the publish
   * @param publishId publish id (publish id)
   * @param subscribeName Name of created subscribe 
   * (usually same as the publish)
   * @param subscribeDelegate Gets notified of the result
   */
  void subscribe(cbe::UserId          sharingUserId,
                 cbe::PublishId       publishId,
                 std::string          subscribeName,
                 SubscribeDelegatePtr subscribeDelegate) {
    subscribe(sharingUserId, "", publishId, "", "", subscribeName,
              subscribeDelegate);
  }

  /**
   * Subscribes to a publish shared by some other user (or yourself).
   * @param sharingUserId User id of the owner of the publish 
   * or 0 if sharingUserName should be used instead
   * @param sharingUserName User name of the owner of the publish 
   * or empty if sharingUserId should be used instead
   * @param publishId Publish id or 0 if publishName should be used instead
   * @param publishName Publish name or empty if publishId should be used instead
   * @param password Required password or an empty string if no password
   * @param subscribeName Name of created favorite (usually same as the publish)
   * @param subscribeDelegate Gets notified of the result
   */
  void subscribe(cbe::UserId          sharingUserId,
                 std::string          sharingUserName,
                 cbe::PublishId       publishId,
                 std::string          publishName,
                 std::string          password,
                 std::string          subscribeName,
                 SubscribeDelegatePtr subscribeDelegate);

  SubscribeManager(cbe::DefaultCtor);
  ~SubscribeManager();

  explicit operator bool() const;
private:
  struct Impl;
  std::shared_ptr<Impl> pImpl{};

  friend class CloudBackend;
  SubscribeManager(CBI::SubscribeManager& cbiPeer);
  CBI::SubscribeManager& getCbiPeer() const;
}; // class SubscribeManager

} // namespace cbe

#endif  // INCLUDE_CBE_SUBSCRIBE_MANAGER_H_
