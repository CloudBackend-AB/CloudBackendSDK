/*
     Copyright © CloudBackend AB 2020 - 2022..
*/

#ifndef INCLUDE_CBE_SHARE_MANAGER_H_
#define INCLUDE_CBE_SHARE_MANAGER_H_

#include "CBE/Types.h"

namespace CBE {
/** Class for Managing Shares.
   * This is class represents a list of Shares.
   */
class ShareManager {
 public:
  virtual ~ShareManager() {};
  /**
   * lists shares that have been shared to you. this will give you information similar to a query but with specific share information.
   * @param delegate ShareDelegatePtr is a shared smart pointer pointing back to shareManager() and the callbacks defined in ShareEventProtocol.
  */
  virtual void listAvailableShares(CBE::ShareDelegatePtr delegate);
  
  /**
   * lists shares that have been shared by you. this will give you information similar to a query but with specific share information.
   * @param delegate ShareDelegatePtr is a shared smart pointer pointing back to shareManager() and the callbacks defined in ShareEventProtocol.
  */
  virtual void listMyShares(CBE::ShareDelegatePtr delegate);

 protected:
  ShareManager() {};
};

}  // namespace CBE

#endif  // INCLUDE_CBE_SHARE_MANAGER_H_
