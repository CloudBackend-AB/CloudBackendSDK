/*
     Copyright © CloudBackend AB 2022-2023.
*/

#ifndef INCLUDE_CBE_PUBLISH_MANAGER_H_
#define INCLUDE_CBE_PUBLISH_MANAGER_H_

#include <vector>

#include "cbe/Types.h"


namespace CBI {
class PublishManager;
} // namespace CBI
namespace cbe {

/** 
 * @brief Managing the list of web shares.
 */ 
class PublishManager {
public:
  /**
   * @brief Lists publications that are shared with other
   * @ref cbe::UserId "userids".
   * 
   * Listing is done independently of where in the actual
   * @ref cbe::Container "container/subcontainer" tree the
   * @ref cbe::Item "items" are located.
   * 
   * (Publications are also known as web shares.)
   *
   * @return The publications in terms of @ref cbe::Item "items".
   */
  cbe::Items getPublished() const;

  /**
   * @brief Default constructor.
   * 
   * Construct a new object with 
   * the \c DefaultCtor to enable the
   * @ref operator bool()
   * test
   */
  PublishManager(cbe::DefaultCtor);
  ~PublishManager();

  /**
   * @brief Checks if the current instance is real.
   * 
   * An "unreal" instance implies typically a failed event.
   * 
   * Relies on the \c Default \c constructor
   * PublishManager(cbe::DefaultCtor)
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
  PublishManager(CBI::PublishManager& cbiPeer);
  CBI::PublishManager& getCbiPeer() const;
}; // class PublishManager

} // namespace cbe

#endif  // INCLUDE_CBE_PUBLISH_MANAGER_H_
