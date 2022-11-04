/*
 Copyright © CloudBackend AB 2020 - 2022.
 */

#ifndef INCLUDE_CBE_SHARE_MANAGER_H_
#define INCLUDE_CBE_SHARE_MANAGER_H_

#include "cbe/delegate/Error.h"
#include "cbe/util/Context.h"
#include "cbe/util/Exception.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/Types.h"

#include "cbe/delegate/ListSharesDelegate.h"

namespace CBI {
class ShareManager;
} // namespace CBI


namespace cbe {
/**
 * @brief a list of shares.
 * 
 * For Managing Shares.
 * This class represents a list of Shares.
 */
class ShareManager {
public:
  using ListSharesDelegatePtr = delegate::ListSharesDelegatePtr;
  /**
   * @brief Lists the shares exposed by other users to current user. 
   * This will give specific share information.
   * @param delegate Pointer to a delegate::ListSharesDelegate
   *                 instance that is implemented by the user.
   */
  void listAvailableShares(ListSharesDelegatePtr delegate);

  /**
   * @brief Lists the shares exposed by current user. 
   * This will give specific share information.
   * @param delegate Pointer to a delegate::ListSharesDelegate
   *                 instance that is implemented by the user.
   */
  void listMyShares(ListSharesDelegatePtr delegate);

  ShareManager(cbe::DefaultCtor);
  ~ShareManager();

  explicit operator bool() const;
private:
  struct Impl;
  std::shared_ptr<Impl> pImpl{};

  friend class CloudBackend;
  ShareManager(CBI::ShareManager& cbiPeer);
  CBI::ShareManager& getCbiPeer() const;
}; // class ShareManager

} // namespace cbe

#endif  // INCLUDE_CBE_SHARE_MANAGER_H_
