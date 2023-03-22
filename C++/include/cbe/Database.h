/*
     Copyright © CloudBackend AB 2022.
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
 * @brief list of available databases.
 * 
 */
class Database {
public:
  /**
   * the userId number of the owner
   * 
   * @return cbe::UserId 
   */
  cbe::UserId ownerId() const;

  /**
   * the databaseId number
   * 
   * @return cbe::DatabaseId 
   */
  cbe::DatabaseId databaseId() const;

  /**
   * the containerId number
   * 
   * @return cbe::ContainerId    
   */
  cbe::ContainerId containerId() const;

  /**
   * the database name
   * 
   * @return std::string 
   */
  std::string name() const;

  /**
   * if it has a read lock
   * 
   * @return true 
   * @return false 
   */
  bool readLocked() const;

  /**
   * if it has a write lock
   * 
   * @return true 
   * @return false 
   */
  bool writeLocked() const;

  /**
   *  when it was created, as unix epoch number
   * 
   * @return cbe::Date 
   */
  cbe::Date created() const;

  /**
   * the top container of the database
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
