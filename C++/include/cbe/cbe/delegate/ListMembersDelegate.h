#ifndef CBE__delegate__ListMembersDelegate_h__
#define CBE__delegate__ListMembersDelegate_h__

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
 *   <li> cbe::Group::listMembers
 * </ul>
 */
class ListMembersDelegate {
public:
  using Members = std::vector<cbe::Member>;
  using Success = Members; 
  /**
   * Called upon successful ListMembers.
   * @param members Vector of cbe::Member holding the members
   * of a group.
   */
  virtual void onListMembersSuccess(Members&& members) = 0;

  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onListMembersError(Error&& error, 
                                  cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed ListMembers.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   *  cbe::Group::listMembers()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::ListMembersException
#endif // #ifndef CBE_NO_SYNC

  virtual ~ListMembersDelegate();
}; // class ListMembersDelegate

/**
 * Pointer to ListMembersDelegate that is passed into:
 * <ul>
 *   <li> cbe::Group::listMembers(ListMembersDelegatePtr).
 * </ul>
 */
using ListMembersDelegatePtr = std::shared_ptr<ListMembersDelegate>;
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__ListMembersDelegate_h__