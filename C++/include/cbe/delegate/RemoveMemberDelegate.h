#ifndef CBE__delegate__RemoveMemberDelegate_h__
#define CBE__delegate__RemoveMemberDelegate_h__

#include <memory>
#include "cbe/QueryResult.h"
#include "cbe/Types.h"
#include "cbe/delegate/Error.h"
#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

namespace cbe {
  namespace delegate {
    class RemoveMemberSuccess;
    /**
     * Delegate class for the asynchronous version of method:
     * <ul>
     *   <li> cbe::Role::removeMember
     * </ul>
     */
    class RemoveMemberDelegate {
      public:
      using Success = RemoveMemberSuccess;
      /**
       * Called upon successful removal of the Role.
       * @param Role the role id of the removed role.
       */
      virtual void onRemoveMemberSuccess(cbe::RoleId&& roleId) = 0;

      using Error = delegate::Error;
      /**
       * Called if an error is encountered.
       */
      virtual void onRemoveMemberError(Error&& error,
                                     cbe::util::Context&& context) = 0;

      /**
       * Contains all information about a failed RemoveMember.
       */
      struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
        using Base::Base;  // Inherit base class' constructors
      };                   // struct ErrorInfo

#ifndef CBE_NO_SYNC
      /**
       * @brief exception thrown by
       * cbe::Role::removeMember()
       * if the request fails.
       */
      struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
        using Base::Base;  // Inherit base class' constructors
      };                   // class struct CloudBackend::RemoveMemberException
#endif                     // #ifndef CBE_NO_SYNC

      virtual ~RemoveMemberDelegate();
    };  // class RemoveMemberDelegate
    /**
     * Pointer to RemoveMemberDelegate that is passed into: \n
     * Group::removeMember(std::string, RemoveMemberDelegatePtr).
     */
    using RemoveMemberDelegatePtr = std::shared_ptr<RemoveMemberDelegate>;
    #ifndef HAL_INCLUDE
    class RemoveMemberSuccess{
      bool success;
    public:
      MemberId memberId;
      RoleId roleId;

      RemoveMemberSuccess();
      RemoveMemberSuccess(cbe::DefaultCtor);
      RemoveMemberSuccess(MemberId&& memberId, RoleId&& roleId);
    
      explicit operator bool() const;
    }; // class RemoveMemberSuccess()
    #endif // !HAL_INCLUDE
  }  // namespace delegate
}  // namespace cbe

#endif  // !CBE__delegate__RemoveMemberDelegate_h__