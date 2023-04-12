#ifndef CBE__delegate__GetStreamsDelegate_h__
#define CBE__delegate__GetStreamsDelegate_h__

// #include "CBE/QueryResult.h"
// #include "CBE/Types.h"

// #include "CBE/delegate/Error.h"

// #include "CBE/util/Context.h"
// #include "CBE/util/ErrorInfo.h"
// #include "CBE/util/Exception.h"

// #include <memory>

// namespace cbe {
//   namespace delegate {
// /**
//  * Delegate class for the asynchronous version of method:
//  * <ul>
//  *   <li> cbe::Object::getStreams
//  * </ul>
//  */
@class CBEObject; 

@protocol GetStreamsDelegatePtr <NSObject>
@required
// class GetStreamsDelegate {
// public:
//   using Success = cbe::Object;
//   /**
//    * Called upon successful GetStreams.
//    * @param object Instance of object that is being streamed.
//    */
//   virtual void onGetStreamsSuccess(cbe::Object&& object) = 0;
- (void) onGetStreamsSuccess:(CBEObject*)object;

//   using Error = delegate::Error;
//   /**
//    * Called if an error is encountered.
//    */
//   virtual void onGetStreamsError(Error&&                error, 
//                                   cbe::util::Context&&   context) = 0;
- (void) onGetStreamsError:(CBEError*)error  context:(CBEContext*)context;

//   /**
//    * Contains all information about a failed GetStreams.
//    */
//   struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
//     using Base::Base; // Inherit base class' constructors
//   }; // struct ErrorInfo

// #ifndef CBE_NO_SYNC
//   /**
//    * @brief exception thrown by
//    * cbe::Object::getStreams()
//    * if the request fails.
//    */
//   struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
//     using Base::Base; // Inherit base class' constructors
//   }; // class struct CloudBackend::GetStreamsException
// #endif // #ifndef CBE_NO_SYNC

//   virtual ~GetStreamsDelegate();
// }; // class GetStreamsDelegate

// /**
//  * Pointer to GetStreamsDelegate that is passed into: \n 
//  * Object::getStreams(GetStreamsDelegatePtr).
//  */
// using GetStreamsDelegatePtr = std::shared_ptr<GetStreamsDelegate>;
//   } // namespace delegate
// } // namespace cbe
@end
#endif // !CBE__delegate__GetStreamsDelegate_h__