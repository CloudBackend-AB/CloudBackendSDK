#ifndef CBE__delegate__LeaveDelegate_h__
#define CBE__delegate__LeaveDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/Error.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
class LeaveSuccess;
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Group::leave
 * </ul>
 */
class LeaveDelegate {
public:
  using Success = LeaveSuccess;
  /**
   * Called upon successful leave.
   */
  virtual void onLeaveSuccess(std::string&& memberName,
                              cbe::MemberId memberId) = 0;

  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onLeaveError(Error&& error, cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed Leave.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::Group::leave() if the
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::LeaveException
#endif // #ifndef CBE_NO_SYNC

  virtual ~LeaveDelegate();
}; // class LeaveDelegate
/**
 * Pointer to LeaveDelegate that is passed into: \n 
 * Group::leave(LeaveDelegatePtr).
 */
using LeaveDelegatePtr = std::shared_ptr<LeaveDelegate>;
class LeaveSuccess {
public:
  std::string memberName{};
  cbe::MemberId memberId{};
  
  LeaveSuccess();
  LeaveSuccess(cbe::DefaultCtor);
  LeaveSuccess(std::string&& memberName, cbe::MemberId memberId);

/**
   * @brief Checks if current instance is valid.
   * 
   * @return \c true: is valid
   */
  explicit operator bool() const;
}; // class LeaveSuccess
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__LeaveDelegate_h__