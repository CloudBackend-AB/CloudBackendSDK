#ifndef CBE__delegate__ACLDelegate_h__
#define CBE__delegate__ACLDelegate_h__

#include "cbe/Types.h"
#include "cbe/delegate/Error.h"
#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

namespace cbe {
  namespace delegate {
class AclSuccess;
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Container::getAcl()
 *   <li> cbe::Container::setAcl()
 *   <li> cbe::Object::getAcl()
 *   <li> cbe::Object::setAcl()
 * </ul>
 */
class AclDelegate {
public:
  using Success = AclMap;
  /**
    * Called upon successful GetACL.
    * @param aclMap  The @ref cbe::Permissions "permissions" for requested
    *                cbe::Container or cbe::Object. 
    */
  virtual void onAclSuccess(AclMap&& aclMap) = 0;

  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onAclError(Error&& error, cbe::util::Context&& context) = 0;

  /**
   * Contains all information about a failed GetACL.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo 

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   *  cbe::Object::getAcl()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::GetAclException
#endif // #ifndef CBE_NO_SYNC

  virtual ~AclDelegate();
}; // class AclDelegate

/**
 * Pointer to AclDelegate that is passed into:
 * <ul>
 *   <li> cbe::Container::getAcl()
 *   <li> cbe::Container::setAcl()
 *   <li> cbe::Object::getAcl()
 *   <li> cbe::Object::setAcl()
 * </ul>
 */
using AclDelegatePtr = std::shared_ptr<AclDelegate>;

/**
 * @brief
 * Convenience type that bundles all parameters passed to method
 * cbe::delegate::AclDelegate::onAclSuccess.
 */
#ifdef HAL_INCLUDE
class AclSuccess {
  bool success;
public:
  AclMap aclMap{};
  
  AclSuccess();
  AclSuccess(cbe::DefaultCtor);
  AclSuccess(AclMap&& aclMap);
/**
   * @brief Checks if current instance is valid.
   * 
   * @return \c true: is valid
   */
  explicit operator bool() const;
}; // class AclSuccess
#endif // #ifdef HAL_INCLUDE

  } // namespace delegate
} // namespace cbe
#endif //ACLdelegate