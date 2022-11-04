/*
     Copyright © CloudBackend AB 2022.
*/

#ifndef INCLUDE_CBE_PUBLISH_H_
#define INCLUDE_CBE_PUBLISH_H_

#include <string>

#include "cbe/Types.h"

namespace CBI {
class Publish;
using PublishPtr = std::shared_ptr<CBI::Publish>;
} // namespace CBI

namespace cbe {

/**
 * @brief manages a \c published Object.
 * 
 */
class Publish {
public:
  /**
   * Invites users
   */
  void invite();
  /**
   * Blocks users
   */
  void block();
  /**
   * Sets the title
   * 
   * @param title a name describing this
   */
  void setTitle(const std::string &title);
  /**
   * Sets the privacy
   * \see Types.h for cbe::PublishVisibility
   *
   * @param privacy New privacy (cbe::PublishVisibility)
   */
  void setPrivacy(cbe::PublishVisibility privacy);
  /**
   * Sets the security
   * \see Types.h for cbe::Access
  *
  * @param security New security (cbe::Access)
  */
  void setSecurity(cbe::PublishAccess security);
  /**
   * Sets the password
   */
  void setPassword(const std::string &password);
  /**
   * Sets the description
   */
  void setDescription(const std::string &description);
  /**
   * Gets the title
   */
  std::string getTitle() const;
  /**
   * Gets the privacy (PublishVisibility)
   * \see Types.h for cbe::PublishVisibility
   */
  cbe::PublishVisibility getPrivacy() const;
  /**
   * Gets the security (cbe::Access)
   * \see Types.h for cbe::Access
   */
  cbe::PublishAccess getSecurity() const;
  /**
   * Gets the password if applicable
   */
  std::string getPassword() const;
  /**
   * Gets the description
   */
  std::string getDescription() const;
  /**
   * Gets the publish id number
   */
  cbe::PublishId getPublishId() const;
  /**
   * Gets the subscribers
   */
  void getSubscribers();
  /**
   * Gets the QR code
   */
  void getQRCode();

  Publish(cbe::DefaultCtor);
  ~Publish();

 explicit operator bool() const;
private:
  struct Impl;
  std::shared_ptr<Impl> pImpl{};

  friend class CloudBackend;
  friend class Item;
  Publish(CBI::PublishPtr cbiPeerPtr);
  CBI::Publish& getCbiPeer() const;
}; // class Publish

} // namespace cbe

#endif  // INCLUDE_CBE_PUBLISH_H_
