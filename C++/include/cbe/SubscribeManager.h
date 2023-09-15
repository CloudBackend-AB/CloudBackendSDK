/*
  Copyright © CloudBackend AB 2022-2023.
*/

#ifndef INCLUDE_CBE_SUBSCRIBE_MANAGER_H_
#define INCLUDE_CBE_SUBSCRIBE_MANAGER_H_

#include <vector>

#include "cbe/Subscribe.h"
#include "cbe/Types.h"

#include "cbe/delegate/SubscribeDelegate.h"
#include "cbe/delegate/GetSubscriptionsDelegate.h"

#include "cbe/delegate/Error.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"
#include "cbe/util/Optional.h"

namespace CBI {
class SubscribeManager;
} // namespace CBI

namespace cbe {
/**
 * @brief For managing subscriptions.
 * 
 */
class SubscribeManager {
public:
  /**
   * Pointer to GetSubscriptionsDelegate that is passed into
   * asynchronous version of method:
   * <ul>
   *   <li> @ref cbe::SubscribeManager::getSubscriptions(GetSubscriptionsDelegatePtr) "getSubscriptions()"
   * </ul>
   */
  using GetSubscriptionsDelegatePtr = delegate::GetSubscriptionsDelegatePtr;
  /**
   * @brief List your current subscriptions.
   * 
   * Listing is done independently of where in the actual directory tree
   * the files are located.
   *
   * @param subscribeDelegate Pointer to a delegate::GetSubscriptionsDelegate
   *                          instance that is implemented by the user.
   */
  void getSubscriptions(GetSubscriptionsDelegatePtr subscribeDelegate);
  /**
   * Pointer to cbe::delegate::GetSubscriptionsDelegate that is passed into
   * asynchronous version of method
   * @ref getSubscriptions(GetSubscriptionsDelegatePtr) "getSubscriptions()"
   */

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::GetSubscriptionsDelegate::Exception
  */
  using GetSubscriptionsException = delegate::GetSubscriptionsDelegate::Exception;
  /**
   * @brief Synchronous [exception]
   * <b>Synchronous</b> version of
   * getSubscriptions(GetSubscriptionsDelegatePtr)
   * , and <b>throws an exception</b>, #GetSubscriptionsException, in case of a 
   * failed call.
   * <br>See getSubscriptions(GetSubscriptionsDelegatePtr)
   *
   * @return Information about the getSubscriptions object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::GetSubscriptionsDelegate::Success " "
   * @throws #GetSubscriptionsException
   */
  delegate::GetSubscriptionsDelegate::Success getSubscriptions();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref getSubscriptions(GetSubscriptionsError&) "getSubscriptions()"
   * <br>See delegate::GetSubscriptionsDelegate::ErrorInfo
   */
  using GetSubscriptionsError = delegate::GetSubscriptionsDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing]
   * <b>Synchronous</b> version of
   * getSubscriptions(GetSubscriptionsDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See getSubscriptions(GetSubscriptionsDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #GetSubscriptionsError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::GetSubscriptionsDelegate::Success> 
  getSubscriptions(GetSubscriptionsError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /**
   * Pointer to SubscribeDelegate that is passed into
   * asynchronous version of method:
   * <ul>
   *   <li> @ref cbe::SubscribeManager::subscribe(cbe::UserId,std::string,cbe::PublishId,std::string,std::string,std::string,SubscribeDelegatePtr) "subscribe()"
   * </ul>
   */
  using SubscribeDelegatePtr = delegate::SubscribeDelegatePtr;

  /**
   * @brief Subscribes to a Publish using all parameters.
   * 
   * %Subscribe to a Publish that was issued by some other user (or yourself).
   * 
   * A Publish must not require a password,
   * if required it should be provided by the publisher.
   * 
   * The retrieved subscription may be revoked with
   * cbe::Object::unSubscribe() or cbe::Container::unSubscribe().
   * 
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
  /**
   * Pointer to cbe::delegate::SubscribeDelegate that is passed into
   * asynchronous version of method
   * @ref subscribe(SubscribeDelegatePtr) "subscribe()"
   */

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::SubscribeDelegate::Exception
  */
  using SubscribeException = delegate::SubscribeDelegate::Exception;
  /**
   * @brief Synchronous [exception]
   * <b>Synchronous</b> version of
   * subscribe(cbe::UserId, 
   *           std::string, 
   *           cbe::PublishId, 
   *           std::string, 
   *           std::string, 
   *           std::string, 
   *           SubscribeDelegatePtr)
   * , and <b>throws an exception</b>, #SubscribeException, in case of a failed
   * call.
   * <br>See subscribe(SubscribeDelegatePtr)
   *
   * @return Information about the subscribe object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::SubscribeDelegate::Success " "
   * @throws #SubscribeException
   */
  delegate::SubscribeDelegate::Success subscribe(cbe::UserId    sharingUserId,
                                                 std::string    sharingUserName,
                                                 cbe::PublishId publishId,
                                                 std::string    publishName,
                                                 std::string    password,
                                                 std::string    subscribeName);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref subscribe(sharingUserId, 
   *                sharingUserName, 
   *                publishId,
   *                publishName,
   *                password,
   *                subscribeName,
   *                SubscribeError&) "subscribe()"
   * <br>See delegate::SubscribeDelegate::ErrorInfo
   */
  using SubscribeError = delegate::SubscribeDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing]
   * <b>Synchronous</b> version of
   * subscribe(sharingUserId, 
   *           sharingUserName, 
   *           publishId, 
   *           publishName, 
   *           password, 
   *           subscribeName, 
   *           SubscribeDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See subscribe(sharingUserId, 
   *                   sharingUserName, 
   *                   publishId, 
   *                   publishName,
   *                   password, 
   *                   subscribeName, 
   *                   SubscribeDelegatePtr)
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #SubscribeError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::SubscribeDelegate::Success> subscribe(
                                              cbe::UserId     sharingUserId,
                                              std::string     sharingUserName,
                                              cbe::PublishId  publishId,
                                              std::string     publishName,
                                              std::string     password,
                                              std::string     subscribeName,
                                              SubscribeError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  /**
   * @brief Subscribes to a Publish using names.
   * 
   * Overload of subscribe()
   * 
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
   * @brief Subscribes to a Publish using ids.
   * 
   * Overload of subscribe()
   * 
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
