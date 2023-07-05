#ifndef CBE__delegate__UnBanDelegate_h__
#define CBE__delegate__UnBanDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/Error.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
class UnBanSuccess;
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Member::unBan
 * </ul>
 */
class UnBanDelegate {
public:
  using Success = UnBanSuccess;
  /**
   * Called upon successful unBan.
   * @param memberName  Name of the member being unbanned.
   * @param memberId    Id of the member being unbanned.
   */
  virtual void onUnBanSuccess(std::string&& memberName,
                              cbe::MemberId memberId) = 0;

  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onUnBanError(Error&&               error,
                            cbe::util::Context&&  context) = 0;
  
  /**
   * Contains all information about a failed UnBan.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
      using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo
  
#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::Member::unBan()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
      using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::UnBanException
#endif // #ifndef CBE_NO_SYNC

  virtual ~UnBanDelegate();
}; // class UnBanDelegate
/**
 * Pointer to UnBanDelegate that is passed into:
 * <ul>
 *   <li> cbe::Member::unBan(UnBanDelegatePtr)
 * </ul>
 */
using UnBanDelegatePtr = std::shared_ptr<UnBanDelegate>;

/**
 * @brief
 * Convenience type that bundles all parameters passed to method
 * cbe::delegate::UnBanDelegate::onUnBanSuccess.
 */
class UnBanSuccess {
public:
  std::string   memberName{};
  cbe::MemberId memberId{};

  UnBanSuccess();
  UnBanSuccess(cbe::DefaultCtor);
  UnBanSuccess(std::string&& memberName,
               cbe::MemberId memberId);
};

  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__UnBanDelegate_h__