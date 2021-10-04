/*
     Copyright © CloudBackend AB 2020.
*/

#ifndef CBE_ACCOUNTEVENTPROTOCOL_H_
#define CBE_ACCOUNTEVENTPROTOCOL_H_

#include "CBE/Types.h"
#include "CBE/Container.h"



namespace CBE {
  /** Protocol for implementing a Item delegate.
   * Abstract base class which is used when you want notification about
   * changes in folder and on documents. */
  class Account;
  class Item;

  class AccountEventProtocol {
    public:
      virtual ~AccountEventProtocol() {
      }

      /** Gets called when the account status has changed (required). */
      virtual void onLogin(uint32_t atState, CBE::CloudBackendPtr cloudbackend){};

      /** Gets called when the account status has changed (required). */
      virtual void onLogout(uint32_t atState){};

      /** Gets called when the account status has changed (required). */
      virtual void onCreated(uint32_t atState){};

      /** Gets called when the account status has changed (required). */
      virtual void onError(uint32_t failedAtState, uint32_t code, std::string reason, std::string message){};

  };
}

#endif //CBE_ACCOUNTEVENTPROTOCOL_H_
