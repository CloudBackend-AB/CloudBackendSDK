/*
 Copyright © CloudBackend AB 2020-2023.
 */

#ifndef INCLUDE_CBE_SHARE_MANAGER_H_
#define INCLUDE_CBE_SHARE_MANAGER_H_

#include "cbe/delegate/Error.h"
#include "cbe/util/Context.h"
#include "cbe/util/Exception.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Optional.h"

#include "cbe/delegate/ListSharesDelegate.h"

namespace CBI {
class ShareManager;
} // namespace CBI


namespace cbe {
/**
 * @brief Managing Shares.
 * 
 * This class represents a list of Shares.
 */
class ShareManager {
public:
  /**
   * Pointer to ListSharesDelegate that is passed into:
   * <ul>
   *   <li> ShareManager::listAvailableShares(ListSharesDelegatePtr)
   *   <li> ShareManager::listMyShares(ListSharesDelegatePtr)
   * </ul>
   */
  using ListSharesDelegatePtr = delegate::ListSharesDelegatePtr;
  /**
   * @brief Lists the shares exposed by other users to current user. 
   * This will give specific share information.
   * @param delegate Pointer to a delegate::ListSharesDelegate
   *                 instance that is implemented by the user.
   */
  void listAvailableShares(ListSharesDelegatePtr delegate);
  /**
   * Pointer to cbe::delegate::ListSharesDelegate that is passed into
   * asynchronous version of method
   * @ref listAvailableShares(ListSharesDelegatePtr) "listAvailableShares()"
   */

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::ListSharesDelegate::Exception
  */
  using ListSharesException = delegate::ListSharesDelegate::Exception;
  /**
   * @brief Synchronous [exception]
   * <b>Synchronous</b> version of
   * listAvailableShares(ListSharesDelegatePtr)
   * , and <b>throws an exception</b>, #ListSharesException, in case of a failed
   * call.
   * <br>See listAvailableShares(ListSharesDelegatePtr)
   *
   * @return Information about the listAvailableShares object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::ListSharesDelegate::Success " "
   * @throws #ListSharesException
   */
  cbe::QueryResult listAvailableShares();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref listAvailableShares(ListSharesError&) "listAvailableShares()"
   * <br>See delegate::ListSharesDelegate::ErrorInfo
   */
  using ListSharesError = delegate::ListSharesDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing]
   * <b>Synchronous</b> version of
   * listAvailableShares(ListSharesDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See listAvailableShares(ListSharesDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #ListSharesError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::QueryResult> listAvailableShares(
                                              ListSharesError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /**
   * @brief Lists the shares exposed by current user. 
   * This will give specific share information.
   * @param delegate Pointer to a delegate::ListSharesDelegate
   *                 instance that is implemented by the user.
   */
  void listMyShares(ListSharesDelegatePtr delegate);
  /**
   * Pointer to cbe::delegate::ListSharesDelegate that is passed into
   * asynchronous version of method
   * @ref listMyShares(ListSharesDelegatePtr) "listMyShares()"
   */

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::ListSharesDelegate::Exception
  */
  using ListMySharesException = delegate::ListSharesDelegate::Exception;
  /**
   * @brief Synchronous [exception]
   * <b>Synchronous</b> version of
   * listMyShares(ListSharesDelegatePtr)
   * , and <b>throws an exception</b>, #ListMySharesException, in case of a 
   * failed call.
   * <br>See listMyShares(ListSharesDelegatePtr)
   *
   * @return Information about the listMyShares object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::ListSharesDelegate::Success " "
   * @throws #ListMySharesException
   */
  delegate::ListSharesDelegate::Success listMyShares();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref listMyShares(ListMySharesError&) "listMyShares()"
   * <br>See delegate::ListSharesDelegate::ErrorInfo
   */
  using ListMySharesError = delegate::ListSharesDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing]
   * <b>Synchronous</b> version of
   * listMyShares(ListSharesDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See listMyShares(ListSharesDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #ListMySharesError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::ListSharesDelegate::Success> listMyShares(
                                              ListSharesError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /**
   * @brief Default constructor.
   * 
   * Construct a new object with 
   * the \c DefaultCtor to enable the
   * @ref operator bool()
   * test
   */
  ShareManager(cbe::DefaultCtor);
  ~ShareManager();

  /**
   * @brief Checks if the current instance is real.
   * 
   * An "unreal" instance implies typically a failed event.
   * 
   * Relies on the \c Default \c constructor
   * ShareManager(cbe::DefaultCtor)
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
  ShareManager(CBI::ShareManager& cbiPeer);
  CBI::ShareManager& getCbiPeer() const;
}; // class ShareManager

} // namespace cbe

#endif  // INCLUDE_CBE_SHARE_MANAGER_H_
