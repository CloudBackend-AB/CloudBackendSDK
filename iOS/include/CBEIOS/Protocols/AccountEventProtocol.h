/*
 Copyright © CloudBackend AB 2020 - 2023..
 */

#ifndef CBE_ACCOUNTEVENTPROTOCOL_H_
#define CBE_ACCOUNTEVENTPROTOCOL_H_

#include "CBE/Types.h"

namespace cbe {
/** Protocol for implementing a Item delegate.
 * Abstract base class which is used when you want notification about
 * changes in folder and on documents. */

class AccountEventProtocol {
public:
  virtual ~AccountEventProtocol();

  /** Gets called when the account status has changed (required). */
  virtual void onLogin(cbe::CloudBackend&& cloudbackend);

  /** Gets called when the account status has changed (required). */
  virtual void onLogout();

  /** Gets called when the account status has changed (required). */
  virtual void onCreated();

  /** Gets called when the account status has changed (required). */
  virtual void onError(ErrorCode errorCode,
                       std::string reason, std::string message);
};

} // namespace cbe

#endif //CBE_ACCOUNTEVENTPROTOCOL_H_
