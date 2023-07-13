#ifndef CBE__delegate__ListRolesDelegate_h__
#define CBE__delegate__ListRolesDelegate_h__

#include <memory>
#include <unordered_map>
#include "cbe/QueryResult.h"
#include "cbe/Types.h"
#include "cbe/Role.h"
#include "cbe/delegate/Error.h"
#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

namespace cbe {
  namespace delegate {
    /**
     * Delegate class for the asynchronous version of method:
     * <ul>
     *   <li> cbe::Group::ListRoles
     * </ul>
     */
    class ListRolesDelegate {
      public:
      using Roles = std::vector<cbe::Role>;
      using Success = Roles;
      /**
       * Called upon successful ListRoles.
       * @param roles A unordered map with role id as key and role name as value
       * of a group.
       */
      virtual void onListRolesSuccess(Roles&& roles) = 0;

      using Error = delegate::Error;
      /**
       * Called if an error is encountered.
       */
      virtual void onListRolesError(Error&& error, cbe::util::Context&& context) = 0;

      /**
       * Contains all information about a failed ListRoles.
       */
      struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
        using Base::Base;  // Inherit base class' constructors
      };                   // struct ErrorInfo

#ifndef CBE_NO_SYNC
      /**
       * @brief exception thrown by
       *  cbe::Group::ListRoles()
       * if the request fails.
       */
      struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
        using Base::Base;  // Inherit base class' constructors
      };                   // class struct CloudBackend::ListRolesException
#endif                     // #ifndef CBE_NO_SYNC

      virtual ~ListRolesDelegate();
    };  // class ListRolesDelegate

    /**
     * Pointer to ListRolesDelegate that is passed into:
     * <ul>
     *   <li> cbe::Group::ListRoles(ListRolesDelegatePtr).
     * </ul>
     */
    using ListRolesDelegatePtr = std::shared_ptr<ListRolesDelegate>;
  }  // namespace delegate
}  // namespace cbe

#endif  // !CBE__delegate__ListRolesDelegate_h__