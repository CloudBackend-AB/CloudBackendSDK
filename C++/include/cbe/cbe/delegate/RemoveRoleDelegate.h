#ifndef CBE__delegate__RemoveRoleDelegate_h__
#define CBE__delegate__RemoveRoleDelegate_h__

#include <memory>
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
     *   <li> cbe::Role::removeRole
     * </ul>
     */
    class RemoveRoleDelegate {
      public:
      using Success = cbe::RoleId;
      /**
       * Called upon successful removal of the Role.
       * @param Role the role id of the removed role.
       */
      virtual void onRemoveRoleSuccess(cbe::RoleId&& roleId) = 0;

      using Error = delegate::Error;
      /**
       * Called if an error is encountered.
       */
      virtual void onRemoveRoleError(Error&& error,
                                     cbe::util::Context&& context) = 0;

      /**
       * Contains all information about a failed RemoveRole.
       */
      struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
        using Base::Base;  // Inherit base class' constructors
      };                   // struct ErrorInfo

#ifndef CBE_NO_SYNC
      /**
       * @brief exception thrown by
       * cbe::Role::removeRole()
       * if the request fails.
       */
      struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
        using Base::Base;  // Inherit base class' constructors
      };                   // class struct CloudBackend::RemoveRoleException
#endif                     // #ifndef CBE_NO_SYNC

      virtual ~RemoveRoleDelegate();
    };  // class RemoveRoleDelegate
    /**
     * Pointer to RemoveRoleDelegate that is passed into: \n
     * Group::removeRole(std::string, RemoveRoleDelegatePtr).
     */
    using RemoveRoleDelegatePtr = std::shared_ptr<RemoveRoleDelegate>;
  }  // namespace delegate
}  // namespace cbe

#endif  // !CBE__delegate__RemoveRoleDelegate_h__