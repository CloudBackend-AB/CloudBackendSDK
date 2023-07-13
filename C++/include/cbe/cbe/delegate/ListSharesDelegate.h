#ifndef CBE__delegate__ListSharesDelegate_h__
#define CBE__delegate__ListSharesDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/Error.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::ShareManager::listMyShares
 * </ul>
 */
class ListSharesDelegate {
public:
  using Success = cbe::QueryResult;
  /**
   * Called upon successful Share.
   * @param qResult Instance of cbe::QueryResult containing list of shares.
   */
  virtual void onListSharesSuccess(cbe::QueryResult&& qResult) = 0;
  
  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onListSharesError(Error&&              error, 
                                 cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed Share.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
  using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::ShareManager::listAvailableShares()
   * or
   * cbe::ShareManager::listMyShares() 
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
  using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::ShareException
#endif // #ifndef CBE_NO_SYNC

  virtual ~ListSharesDelegate();
}; // class ShareDelegate

/**
 * Pointer to ListSharesDelegate that is passed into:
 * <ul>
 *   <li> ShareManager::listAvailableShares(ListSharesDelegatePtr)
 *   <li> ShareManager::listMyShares(ListSharesDelegatePtr)
 * </ul>
 */
using ListSharesDelegatePtr = std::shared_ptr<ListSharesDelegate>;
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__ListSharesDelegate_h__