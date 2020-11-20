/*
      Copyright © CloudBackend AB 2020.
 */

#ifndef INCLUDE_CBE_ACCOUNT_H_
#define INCLUDE_CBE_ACCOUNT_H_

#include <string>
#include <vector>

#include "CBE/Types.h"
#include "CBE/Container.h"

namespace CBE
{

class Account
{
public:
  /** Returns the account id of the user. */
  virtual CBE::user_id_t userId() const;

  /// Returns the username of the account.
  virtual std::string username() const;

  /// Returns the password of the account.
  virtual std::string password() const;

  /// Returns the source of the account.
  virtual std::string source() const;

  /// Returns the name of the user.
  virtual std::string firstName() const;

  /// Returns the surname of the user.
  virtual std::string lastName() const;

  /// Returns the rootContainer for the account
  virtual CBE::ContainerPtr rootContainer() const;

  /// Returns the libContainerId for the account.  For more information about libContainers see documentation.
  virtual CBE::container_id_t containerId() const;

  /// Returns the rootContainerId for the account
  virtual CBE::container_id_t rootDriveId() const;

  /// Default the destructor for Account.
  virtual ~Account();

protected:
  Account() {}

};
} // namespace CBE
// namespace CBE

#endif // INCLUDE_CBE_ACCOUNT_H_
