#ifndef CBE__delegate__SearchGroupsDelegate_h__
#define CBE__delegate__SearchGroupsDelegate_h__

#include "cbe/Types.h"
#include "cbe/delegate/Error.h"
#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

namespace cbe {
  namespace delegate {

/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::GroupManager::searchGroups()
 * </ul>
 */
class SearchGroupsDelegate {
public:
  using Success = cbe::GroupQueryResult;
  /**
   * Called upon successful search.
   * @param queryResult Ref. instance of cbe::GroupQueryResult holding the result of
   * the search.
   */
  virtual void onSearchGroupsSuccess(cbe::GroupQueryResult&& queryResult) = 0;
  
  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onSearchGroupsError(delegate::Error&& error, cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed SearchGroups.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::GroupManager::searchGroups()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::SearchGroupsException
#endif // #ifndef CBE_NO_SYNC

  virtual ~SearchGroupsDelegate();
}; // class SearchGroupsDelegate

/**
 * Pointer to SearchGroupsDelegate that is passed into:
 * <ul>
 *   <li> cbe::GroupManager::searchGroups()
 * </ul>
 */
using SearchGroupsDelegatePtr = std::shared_ptr<SearchGroupsDelegate>;
  } // namespace delegate
} // namespace cbe
#endif