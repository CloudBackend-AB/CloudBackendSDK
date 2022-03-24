
#include "AccountEventProtocol.h"

void AccountEventProtocol::onLogin(uint32_t atState, CBE::CloudBackendPtr cloudbackend) {
  std::cout << "Account Login complete"
            << " - name: "<< cloudbackend->account()->username()
            << " - id: "<< cloudbackend->account()->userId() << std::endl;
  cbeTL->rootContainer = cloudbackend->account()->rootContainer();
  cbeTL->logic();
}

void AccountEventProtocol::onError(CBE::persistence_t failedAtState, uint32_t code, std::string reason, std::string message) {
  std::cout << "Account Event Error: "
            << "Login was " << reason
            << " due to " << message << std::endl;
  cbeTL->programFinished();
}

AccountEventProtocol::AccountEventProtocol(Logic* ptr) {
  cbeTL = ptr;
}
