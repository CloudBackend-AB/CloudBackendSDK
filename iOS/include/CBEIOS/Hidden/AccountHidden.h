/*
      Copyright © CloudBackend AB 2020 - 2023.
 */

#ifndef INCLUDE_CBEIOS_ACCOUNTHIDDEN_H_
#define INCLUDE_CBEIOS_ACCOUNTHIDDEN_H_

//#include <string>
//#include <vector>
//#include <map>

//#include "CBE/Types.h"
// #include "CBE/Container.h"
// #include "CBE/Database.h"
#include "CBEIOS/Account.h"

// namespace CBI {
// class Account;
// using AccountPtr = std::shared_ptr<CBI::Account>;
// } // namespace CBI

// namespace cbe
// {
/**
 * @brief login account information
 * 
 */
@interface CBEAccountHidden : CBEAccount {

//class Account {
public:

// private:
//   struct Impl;
//   std::shared_ptr<Impl> pImpl{};

//@interface CBEAccount

//   friend class CloudBackend;
//   Account(CBI::AccountPtr cbiPeerPtr);
//   /**
//    * Provides a reference to the peer object.
//    */
//   CBI::Account& getCbiPeer() ;
} // class Account
- (void) setCBIPeer(void* cbiPeerPtr)
//} // namespace cbe
@end
#endif // INCLUDE_CBE_ACCOUNT_H_
