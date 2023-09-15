/*
      Copyright © CloudBackend AB 2020-2023.
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
 * @brief Login account information.
 */
class Account {
public:
  /**
   * @brief Returns the account id of the user.
   * 
   */
  cbe::UserId userId() const;

  /**
   * @brief Returns the username of the account.
   *
   * Stored in lowercase.
   */
  std::string username() const;

  /**
   * @brief Returns the password of the account.
   *
   * What was used to login.
   */
  std::string password() const;

  /**
   * @brief Returns the tenant name of the account.
   *
   * \note Stored in lowercase.
   */
  std::string source() const;

  /**
   * @brief Returns the client of the account.
   *
   * If it was set when the account was created.
   */
  std::string client() const;

  /**
   * @brief  Returns the given name of the user.
   *
   * Stored in lowercase.
   */
  std::string firstName() const;

  /**
   * @brief Returns the surname of the user.
   * 
   */
  std::string lastName() const;

  /**
   * @brief Returns the rootContainer for the account.
   * 
   * The top container is also known as home://
   */
  cbe::Container rootContainer() const;

  /**
   * @brief Returns the tenant ContainerId 
   * 
   * Returns the ContainerId of the tenant database for the account.
   * The top container is also known as tenant://
   * 
   * To get the Container, see
   * @ref anchorEx__cbe__Databases "Databases"
   */
  cbe::ContainerId tenantContainerId() const;
  
  /**
   * @brief Returns the library ContainerId 
   * 
   * The ContainerId of the library database for the account,
   * which is where application specific settings and
   * application state can be permanently stored
   * in order to be available next time the app starts.
   */
  cbe::ContainerId libContainerId() const;

  /**
   * @brief Returns the DatabaseId for home://
   * 
   * Returns the databaseId for root a.k.a. home:// of the account.
   */
  cbe::DatabaseId rootDatabase() const;

  /**
   * @brief Returns the Databases available.
   * 
   * Returns a std::map of databases available for the account.
   */
  cbe::DataBases databases() const;

  /**
   * @brief Default constructor.
   * 
   * Construct a new object with 
   * the \c DefaultCtor to enable the
   * @ref operator bool()
   * test
   */
  Account(cbe::DefaultCtor);
  ~Account();

  /**
   * @brief Checks if the current instance is real.
   * 
   * An "unreal" instance implies typically a failed event.
   * 
   * Relies on the \c Default \c constructor
   * Account(cbe::DefaultCtor)
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
  Account(CBI::AccountPtr cbiPeerPtr);
  /**
   * Provides a reference to the peer object.
   */
  CBI::Account& getCbiPeer() const;
}; // class Account

} // namespace cbe

#endif // INCLUDE_CBE_ACCOUNT_H_
