
#ifndef ACCOUNT_EVENT_PROTOCOL_H_
#define ACCOUNT_EVENT_PROTOCOL_H_

#include "CBE/Protocols/AccountEventProtocol.h"
#include "Logic.h"

class AccountEventProtocol : public CBE::AccountEventProtocol {
 public:
  
  void onLogin(uint32_t atState, CBE::CloudBackendPtr cloudbackend) final;

  /** Gets called when the account status has changed (required). */
  void onError(CBE::persistence_t failedAtState, uint32_t code, std::string reason, std::string message) final;
  
  AccountEventProtocol(Logic* ptr);

  Logic* cbeTL{};

  AccountEventProtocol(const AccountEventProtocol&) = delete;
  AccountEventProtocol& operator=(const AccountEventProtocol&) = delete;
};

#endif
