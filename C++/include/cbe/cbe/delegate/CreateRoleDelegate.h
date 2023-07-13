#ifndef CBE__delegate__CreateRoleDelegate_h__
#define CBE__delegate__CreateRoleDelegate_h__

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
     *   <li> cbe::Role::createRole
     * </ul>
     */
    class CreateRoleDelegate {
      public:
      using Success = cbe::Role;
      /**
       * Called upon successful create Role.
       * @param Role the role id of the created role.
       */
      virtual void onCreateRoleSuccess(cbe::Role&& role) = 0;

      using Error = delegate::Error;
      /**
       * Called if an error is encountered.
       */
      virtual void onCreateRoleError(Error&& error,
                                     cbe::util::Context&& context) = 0;

      /**
       * Contains all information about a failed CreateRole.
       */
      struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
        using Base::Base;  // Inherit base class' constructors
      };                   // struct ErrorInfo

#ifndef CBE_NO_SYNC
      /**
       * @brief exception thrown by
       * cbe::Role::createRole()
       * if the request fails.
       */
      struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
        using Base::Base;  // Inherit base class' constructors
      };                   // class struct CloudBackend::CreateRoleException
#endif                     // #ifndef CBE_NO_SYNC

      virtual ~CreateRoleDelegate();
    };  // class CreateRoleDelegate
    /**
     * Pointer to CreateRoleDelegate that is passed into: \n
     * Group::createRole(std::string, CreateRoleDelegatePtr).
     */
    using CreateRoleDelegatePtr = std::shared_ptr<CreateRoleDelegate>;
  }  // namespace delegate
}  // namespace cbe

#endif  // !CBE__delegate__CreateRoleDelegate_h__