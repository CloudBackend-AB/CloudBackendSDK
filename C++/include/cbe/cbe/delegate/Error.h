#ifndef CBE__delegate__Error_h__
#define CBE__delegate__Error_h__

#include "cbe/Types.h"
#include <string>

namespace cbe
{
  namespace delegate
  { 
    /**
     * Entity class containing the error information 
     */
    class Error {
    public:
      /*!
       * Mimics the general error code encoding in the www.
       * \see [Wikipedia: List of HTTP status codes]
       *      (https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)
       */
      ErrorCode     errorCode{};
      /**
       * Human readable description of the reason for the failure.
       */
      std::string   reason{};
      /**
       * Human readable additional information about the error.
       */
      std::string   message{};

      /**
       * Checks whether current object holds error information.
       * @return <code>true</code> implies that current object contains an
       *         error,l while <code>false</code> indicates no error.   
       */
      explicit operator bool() const;

      //! @cond Doxygen_Suppress
      Error();

      Error(ErrorCode     errorCode,
            std::string&& reason,
            std::string&& message);
      //! @endcond

      /**
       * Output stream operator for CloudBackend::LogInDelegate::Error.
       */
      friend std::ostream& operator<<(std::ostream& os, const Error& error);
    }; // class LogInDelegate::Error
  }
}
#endif