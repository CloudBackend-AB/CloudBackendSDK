/*
  Copyright © CloudBackend AB 2022.
 */

#ifndef INCLUDE_CBE_SUBSCRIBE_H_
#define INCLUDE_CBE_SUBSCRIBE_H_

#include <string>

#include "cbe/Types.h"

namespace CBI {
class Subscribe;
using SubscribePtr = std::shared_ptr<CBI::Subscribe>;
} // namespace CBI

namespace cbe {

/**
 * @brief to subscribe to a \c published Item.
 * 
 */
class Subscribe {
public:
  /**
   * Gets the date as unix epoch number
   */
  cbe::Date getDate() const;
  /**
   * Gets the title
   */
  std::string getTitle() const;
  /**
   * Gets the description
   */
  std::string getDescription() const;
  /**
   * Gets the password
   */
  std::string getPassword() const;
  /**
   * Gets the security 
   * \see Types.h for cbe::PublishAccess enum
   */
  cbe::PublishAccess getSecurity() const;
  /**
   * Gets the privacy 
   * \see Types.h for cbe::PublishVisibility
   */
  cbe::PublishVisibility getPrivacy() const;
  /**
   * Gets the subscribe id number
   */
  cbe::SubscribeId getSubscribeId() const;
  /**
   * Gets the publish id number
   */
  cbe::PublishId getPublishId() const;
  /**
   * Gets the owner id number
   */
  cbe::UserId getOwner() const;
  /**
   * Unsubscribe
   */
  void unsubscribe();

  Subscribe(cbe::DefaultCtor);
  ~Subscribe();

  explicit operator bool() const;
private:
  struct Impl;
  std::shared_ptr<Impl> pImpl{};

  friend class CloudBackend;
  friend class Item;
  Subscribe(CBI::SubscribePtr cbiSubscribePtr);
  CBI::Subscribe& getCbiPeer() const;
}; // class Subscribe
} // namespace cbe

#endif  // INCLUDE_CBE_SUBSCRIBE_H_
