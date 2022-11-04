/*
      Copyright © CloudBackend AB 2020 - 2022.
 */

#ifndef INCLUDE_CBE_ACCOUNT_H_
#define INCLUDE_CBE_ACCOUNT_H_

#include <string>
#include <vector>
#include <map>

#include "cbe/Types.h"
#include "cbe/Container.h"
#include "cbe/Database.h"

namespace CBI {
class Account;
using AccountPtr = std::shared_ptr<CBI::Account>;
} // namespace CBI

namespace cbe
{
/**
 * @brief login account information
 * 
 */
class Account {
public:
  /** Returns the account id of the user. */
  cbe::UserId userId() const;

  /** Returns the username of the account. */
  std::string username() const;

  /** Returns the password of the account. */
  std::string password() const;

  /** Returns the tenant admin of the account 
   * and owner of the tenant group database. */
  std::string source() const;

  /** Returns the client of the account. */
  std::string client() const;

  /** Returns the given name of the user. */
  std::string firstName() const;

  /** Returns the surname of the user. */
  std::string lastName() const;

  /** Returns the rootContainer for the account. */
  cbe::Container rootContainer() const;

  /** Returns the libContainerId for the account. For more information about
   * libContainers see documentation. */
  cbe::ContainerId    containerId() const;

  /** Returns the rootContainerId for the account. */
  cbe::ContainerId    rootDriveId() const;

  /** Returns a std::map of databases available for the account. */
  cbe::DataBases databases() const;

  Account(cbe::DefaultCtor);
  ~Account();

  explicit operator bool() const;
private:
  struct Impl;
  std::shared_ptr<Impl> pImpl{};


  friend class CloudBackend;
  Account(CBI::AccountPtr cbiPeerPtr);
  /**
   * Provides a reference to the peer object.
   */
  CBI::Account& getCbiPeer() const;
}; // class Account

} // namespace cbe

#endif // INCLUDE_CBE_ACCOUNT_H_
