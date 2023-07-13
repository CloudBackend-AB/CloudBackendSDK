#ifndef CBE__delegate__AddMemberDelegate_h__
#define CBE__delegate__AddMemberDelegate_h__

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
    class AddMemberSuccess;
    /**
     * Delegate class for the asynchronous version of method:
     * <ul>
     *   <li> cbe::Role::AddMember
     * </ul>
     */
    class AddMemberDelegate {
      public:
      using Success = AddMemberSuccess;
      /**
       * Called upon successful AddMember.
       */
      virtual void onAddMemberSuccess(cbe::MemberId&& memberId) = 0;

      using Error = delegate::Error;
      /**
       * Called if an error is encountered.
       */
      virtual void onAddMemberError(Error&& error, cbe::util::Context&& context) = 0;

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
      };                   // class struct CloudBackend::AddMemberException
#endif                     // #ifndef CBE_NO_SYNC

      virtual ~AddMemberDelegate();
    };  // class AddMemberDelegate

    /**
     * Pointer to AddMemberDelegate that is passed into:
     * <ul>
     *   <li> cbe::Group::AddMember(AddMemberDelegatePtr).
     * </ul>
     */
    using AddMemberDelegatePtr = std::shared_ptr<AddMemberDelegate>;
  #ifndef HAL_INCLUDE
    class AddMemberSuccess{
      bool success;
    public:
      MemberId memberId;
      RoleId   roleId;

      AddMemberSuccess();
      AddMemberSuccess(cbe::DefaultCtor);
      AddMemberSuccess(MemberId&& memberId, RoleId&& roleId);
    
    explicit operator bool() const;
    };  // class AddMemberSuccess 
    #endif // !HAL_INCLUDE
  }  // namespace delegate

}  // namespace cbe

#endif  // !CBE__delegate__AddMemberDelegate_h__