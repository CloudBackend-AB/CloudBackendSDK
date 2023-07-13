#ifndef CBE__delegate__RemoveRoleMemberDelegate_h__
#define CBE__delegate__RemoveRoleMemberDelegate_h__

#include <memory>
#include <unordered_map>
#include "cbe/QueryResult.h"
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
     *   <li> cbe::Group::RemoveMember
     * </ul>
     */
    class RemoveRoleMemberDelegate {
      public:
      using Success = MemberId;
      /**
       * Called upon successful RemoveMember.
       */
      virtual void onRemoveRoleMemberSuccess(MemberId memberId) = 0;

      using Error = delegate::Error;
      /**
       * Called if an error is encountered.
       */
      virtual void onRemoveRoleMemberError(Error&& error, cbe::util::Context&& context) = 0;

      /**
       * Contains all information about a failed RemoveMember.
       */
      struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
        using Base::Base;  // Inherit base class' constructors
      };                   // struct ErrorInfo

#ifndef CBE_NO_SYNC
      /**
       * @brief exception thrown by
       *  cbe::Group::RemoveMember()
       * if the request fails.
       */
      struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
        using Base::Base;  // Inherit base class' constructors
      };                   // class struct CloudBackend::RemoveRoleMemberException
#endif                     // #ifndef CBE_NO_SYNC

      virtual ~RemoveRoleMemberDelegate();
    };  // class RemoveRoleMemberDelegate

    /**
     * Pointer to RemoveRoleMemberDelegate that is passed into:
     * <ul>
     *   <li> cbe::Group::RemoveMember(RemoveRoleMemberDelegatePtr).
     * </ul>
     */
    using RemoveRoleMemberDelegatePtr = std::shared_ptr<RemoveRoleMemberDelegate>;
  }  // namespace delegate
}  // namespace cbe

#endif  // !CBE__delegate__RemoveRoleMemberDelegate_h__