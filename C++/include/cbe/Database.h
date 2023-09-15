/*
     Copyright © CloudBackend AB 2022-2023.
*/

#ifndef _CBE_DATABASE_H_
#define _CBE_DATABASE_H_

#include "cbe/Types.h"

#include <string>

  namespace CBI {
class Database;
using DatabasePtr = std::shared_ptr<CBI::Database>;
  } // namespace CBI

namespace cbe {

/**
 * @anchor anchorEx__cbe__Databases
 * @brief A database.
 * 
 * @par Example
 *      Get the tenant Container object
 * @include example/databases.cpp
 */
class Database {
public:
  /**
   * @brief The database name.
   * 
   * @return std::string 
   */
  std::string name() const;

  /**
   * @brief The databaseId number.
   * 
   * @return cbe::DatabaseId 
   */
  cbe::DatabaseId databaseId() const;

  /**
   * @brief The containerId number
   * 
   * Refers to the top rootContainer of the database.
   * 
   * @return cbe::ContainerId    
   */
  cbe::ContainerId containerId() const;

  /**
   * @brief The userId number of the owner
   * 
   * @return cbe::UserId 
   */
  cbe::UserId ownerId() const;

  /**
   * @brief Timestamp of when it was created.
   * 
   * Given in unix epoch number format.
   * 
   * @return cbe::Date 
   */
  cbe::Date created() const;

  /**
   * @brief Checks if it has a read lock.
   * 
   * @return true 
   * @return false 
   */
  bool readLocked() const;

  /**
   * @brief Checks if it has a write lock.
   * 
   * @return true 
   * @return false 
   */
  bool writeLocked() const;

  /**
   * @brief The top container of the database.
   * 
   * The whole container object.
   * 
   * @return cbe::Container 
   */
  cbe::Container rootContainer() const;

  Database(cbe::DefaultCtor);
  ~Database();

  explicit operator bool() const;
private:
  struct Impl;
  std::shared_ptr<Impl> pImpl{};

  friend Account;
  Database(CBI::DatabasePtr cbiPeerPtr);
  CBI::Database& getCbiPeer() const;
}; // class Database

} // namespace cbe

#endif  // _CBE_DATABASE_H_
