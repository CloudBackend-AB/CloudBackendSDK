#ifndef CBE__delegate__BanDelegate_h__
#define CBE__delegate__BanDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/Error.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
class BanSuccess;
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Member::ban
 * </ul>
 */
class BanDelegate {
public:
  using Success = BanSuccess;
  /**
   * Called upon successful ban.
   * @param memberName  Name of the member being banned.
   * @param memberId    Id of the member being banned.
   */
  virtual void onBanSuccess(std::string&& memberName,
                            cbe::MemberId memberId) = 0;

  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onBanError(Error&&               error,
                          cbe::util::Context&&  context) = 0;
  
  /**
   * Contains all information about a failed Ban.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
      using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo
  
#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::Member::ban(std::string)
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
      using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::BanException
#endif // #ifndef CBE_NO_SYNC

  virtual ~BanDelegate();
}; // class BanDelegate

/**
 * Pointer to BanDelegate that is passed into:
 * <ul>
 *   <li> cbe::Member::ban(std::string,BanDelegatePtr)
 * </ul>
 */
using BanDelegatePtr = std::shared_ptr<BanDelegate>;

/**
 * @brief
 * Convenience type that bundles all parameters passed to method
 * cbe::delegate::BanDelegate::onBanSuccess.
 */
class BanSuccess {
public:
  std::string   memberName{};
  cbe::MemberId memberId{};

  BanSuccess();
  BanSuccess(cbe::DefaultCtor);
  BanSuccess(std::string&& memberName,
             cbe::MemberId memberId);
};
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__BanDelegate_h__