#ifndef CBE__delegate__ACLDelegate_h__
#define CBE__delegate__ACLDelegate_h__

// #include "CBE/Types.h"
// #include "CBE/delegate/Error.h"
// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

#import <Foundation/Foundation.h>
#include "CBEIOS/delegate/Error.h"

#include "CBEIOS/util/Context.h"
#include "CBEIOS/Types.h"


// namespace cbe {
//   namespace delegate {
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Container::getAcl()
 *   <li> cbe::Container::setAcl()
 *   <li> cbe::Object::getAcl()
 *   <li> cbe::Object::setAcl()
 * </ul>
 */
// class AclDelegate {
// public:
//   using Success = AclMap;
@protocol AclDelegatePtr <NSObject> 
@required
  /**
    * Called upon successful GetACL.
    * @param aclMap  The @ref cbe::Permission "permissions" for requested
    *                cbe::Container or cbe::Object. 
    */
  -(void) onAclSuccess:(NSDictionary*) aclMap;

  // using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  -(void) onAclError:(CBEError*)error  context:(CBEContext*)context;

  /**
   * Contains all information about a failed GetACL.
   */
  // struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
  //   using Base::Base; // Inherit base class' constructors
  // }; // struct ErrorInfo 

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    *  cbe::Object::getAcl()
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::GetAclException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~AclDelegate();
// }; // class GetAclDelegate

// /**
//  * Pointer to AclDelegate that is passed into:
//  * <ul>
//  *   <li> cbe::Container::getAcl()
//  *   <li> cbe::Container::setAcl()
//  *   <li> cbe::Object::getAcl()
//  *   <li> cbe::Object::setAcl()
//  * </ul>
//  */
// using AclDelegatePtr = std::shared_ptr<AclDelegate>;
//   } // namespace delegate
// } // namespace cbe
@end
#endif //ACLdelegate