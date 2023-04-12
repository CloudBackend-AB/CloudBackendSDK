#ifndef CBE__delegate__Error_h__
#define CBE__delegate__Error_h__

//#include "CBE/Types.h"
//#include <string>
#import <Foundation/Foundation.h>

//namespace cbe
//{
  //namespace delegate
  //{ 
    /**
     * Entity class containing the error information 
     */
    @interface CBEError : NSObject {
    //class CBEError {
    
      /*!
       * Mimics the general error code encoding in the www.
       * \see [Wikipedia: List of HTTP status codes]
       *      (https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)
       */
      // @public uint32_t     errorCode;
      // /**
      //  * Human readable description of the reason for the failure.
      //  */
      // @public NSString*   reason;
      // /**
      //  * Human readable additional information about the error.
      //  */
      // @public NSString*   message;

      //! @cond Doxygen_Suppress
      // - (id) CBEError;

      //  - (id) Error:(uint32_t)errorCode
      //        reason:(NSString*) reason
      //        message:(NSString*) message;
      //! @endcond

      /**
       * Output stream operator for CloudBackend::LogInDelegate::Error.
       */
      //friend std::ostream& operator<<(std::ostream& os, const Error& error);
    } 
    @property(nonatomic, copy)		NSString*	message;
    @property(nonatomic, copy)		NSString*	reason;
    @property(nonatomic, assign)	uint32_t		errorCode;
    // @synthesize message;
    // @synthesize reason;
    // @synthesize errorCode;




 @end
 // }
//}
#endif