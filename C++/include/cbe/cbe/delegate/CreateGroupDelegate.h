#ifndef CBE__delegate__CreateGroupDelegate_h__
#define CBE__delegate__CreateGroupDelegate_h__

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
 *   <li> cbe::Group::createGroup
 * </ul>
 */
class CreateGroupDelegate {
public:
  using Success = cbe::Group;
  /**
   * Called upon successful create group.
   * @param group Instance of a cbe::Group.
   */
  virtual void onCreateGroupSuccess(cbe::Group&& group) = 0;

  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onCreateGroupError(Error&&               error,
                                  cbe::util::Context&&  context) = 0;

  /**
   * Contains all information about a failed CreateGroup.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::Group::createGroup()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::CreateGroupException
#endif // #ifndef CBE_NO_SYNC

  virtual ~CreateGroupDelegate();
}; // class CreateGroupDelegate
/**
 * Pointer to CreateGroupDelegate that is passed into: \n 
 * Group::createGroup(std::string,std::string,CreateGroupDelegatePtr,cbe::Visibility).
 */
using CreateGroupDelegatePtr = std::shared_ptr<CreateGroupDelegate>;
/**
 * @brief
 * Convenience type that bundles all parameters passed to method
 * cbe::delegate::CreateGroupDelegate::onCreateGroupSuccess.
 */
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__CreateGroupDelegate_h__