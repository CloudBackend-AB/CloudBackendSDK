#ifndef CBE__delegate__ListGroupsDelegate_h__
#define CBE__delegate__ListGroupsDelegate_h__

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
 *   <li> cbe::GroupManager::listGroups
 * </ul>
 */
class ListGroupsDelegate {
public:
  using Groups = std::vector<cbe::Group>;
  using Success = Groups;
  /**
   * Called upon successful listGroup
   * @param groups Ref to vector of cbe::Group holding the joined groups.
   */
  virtual void onListGroupsSuccess(Groups&& groups) = 0;

  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onListGroupsError(delegate::Error&& error, cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed ListGroup.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   *  cbe::GroupManager::listGroups()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::ListGroupException
#endif // #ifndef CBE_NO_SYNC

  virtual ~ListGroupsDelegate();
}; // class ListGroupsDelegate

/**
 * Pointer to ListGroupsDelegate that is passed into: \n 
 * GroupManager::listGroups(ListGroupsDelegatePtr).
 */
using ListGroupsDelegatePtr = std::shared_ptr<ListGroupsDelegate>;
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__ListGroupsDelegate_h__