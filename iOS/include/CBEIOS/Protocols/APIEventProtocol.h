/*
 Copyright © CloudBackend AB 2020 - 2023..
 */

#ifndef CBE_APIEVENTPROTOCOL_H_
#define CBE_APIEVENTPROTOCOL_H_

#include "CBE/Types.h"

namespace cbe {

/** Protocol for implementing a Item delegate.
 * Abstract base class which is used when you want notification about
 * events from group calls. */

class APIEventProtocol {
public:
  virtual ~APIEventProtocol();

  /**
   * Returns the respons from server/node as xml string. 
   * \note can be used to create a node list in Poco if using c++ or as XML document in java.
   */
  virtual void onAPIResponse(std::string response);

  /**
   * Gets called when an error have occured.
   */
  virtual void onAPIRequestError(uint32_t     code,
                                 std::string  reason,
                                 std::string  message);
}; // class APIEventProtocol

} // namespace cbe

#endif  //CBE_GROUPEVENTPROTOCOL_H_
