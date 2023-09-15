/*
     Copyright © CloudBackend AB 2022-2023.
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
 * @brief Managing a \c published Item.
 * 
 */
class Publish {
public:
  /**
   * @brief Set the Title of this publication
   * 
   * @param title name
   */
  void setTitle(const std::string &title);

  /**
   * @brief Set the Security of this publication
   * 
   * @param security of type cbe::PublishAccess enum
   */
  void setSecurity(cbe::PublishAccess security);

  /**
   * @brief Set the Privacy of this publication
   * 
   * @param privacy of type cbe::PublishVisibility enum
   */
  void setPrivacy(cbe::PublishVisibility privacy);

  /**
   * @brief Set the Password of this publication
   * 
   * @param password text
   */
  void setPassword(const std::string &password);

  /**
   * @brief Set the Description of this publication
   * 
   * @param description free text
   */
  void setDescription(const std::string &description);

  /**
   * Gets the title
   */
  std::string getTitle() const;
  /**
   * Gets the security setting,
   * see cbe::PublishAccess enum
   */
  cbe::PublishAccess getSecurity() const;
  /**
   * Gets the privacy setting,
   * see cbe::PublishVisibility enum
   */
  cbe::PublishVisibility getPrivacy() const;
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
   * @brief Default constructor.
   * 
   * Construct a new object with 
   * the \c DefaultCtor to enable the
   * @ref operator bool()
   * test
   */
  Publish(cbe::DefaultCtor);
  ~Publish();

  /**
   * @brief Checks if the current instance is real.
   * 
   * An "unreal" instance implies typically a failed event.
   * 
   * Relies on the \c Default \c constructor
   * Publish(cbe::DefaultCtor)
   * 
   * @return \c true  : is real
   * @return \c false : unreal; got nullptr; if current instance is unbound/undefined. I.e., if it is
   *         only default constructed.
   */
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
