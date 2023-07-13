#ifndef CBE__delegate__AddRoleMemberDelegate_h__
#define CBE__delegate__AddRoleMemberDelegate_h__

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
     *   <li> cbe::Group::AddMember
     * </ul>
     */
    class AddRoleMemberDelegate {
      public:
      using Success = MemberId;
      /**
       * Called upon successful AddMember.
       */
      virtual void onAddRoleMemberSuccess(MemberId memberId) = 0;

      using Error = delegate::Error;
      /**
       * Called if an error is encountered.
       */
      virtual void onAddRoleMemberError(Error&& error, cbe::util::Context&& context) = 0;

      /**
       * Contains all information about a failed AddMember.
       */
      struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
        using Base::Base;  // Inherit base class' constructors
      };                   // struct ErrorInfo

#ifndef CBE_NO_SYNC
      /**
       * @brief exception thrown by
       *  cbe::Group::AddMember()
       * if the request fails.
       */
      struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
        using Base::Base;  // Inherit base class' constructors
      };                   // class struct CloudBackend::AddRoleMemberException
#endif                     // #ifndef CBE_NO_SYNC

      virtual ~AddRoleMemberDelegate();
    };  // class AddRoleMemberDelegate

    /**
     * Pointer to AddRoleMemberDelegate that is passed into:
     * <ul>
     *   <li> cbe::Group::AddMember(AddRoleMemberDelegatePtr).
     * </ul>
     */
    using AddRoleMemberDelegatePtr = std::shared_ptr<AddRoleMemberDelegate>;
  }  // namespace delegate
}  // namespace cbe

#endif  // !CBE__delegate__AddRoleMemberDelegate_h__