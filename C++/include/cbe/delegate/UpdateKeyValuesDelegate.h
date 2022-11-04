#ifndef CBE__delegate__UpdateKeyValuesDelegate_h__
#define CBE__delegate__UpdateKeyValuesDelegate_h__

#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/Error.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
/**
 * Delegate class for the asynchronous version of method:
 * <ul>
 *   <li> cbe::Object::updateKeyValues()
 * </ul>
 */
class UpdateKeyValuesDelegate {
public:
  using Success = cbe::Object;
  /**
   * Called upon successful UpdateKeyValues.
   * @param object Instance of object that is getting its Key/Values updated.
   */
  virtual void onUpdateKeyValuesSuccess(cbe::Object&& object) = 0;
  
  using Error = delegate::Error;
  /**
   * Called if an error is encountered.
   */
  virtual void onUpdateKeyValuesError(Error&&               error, 
                                      cbe::util::Context&&  context) = 0;

  /**
   * Contains all information about a failed UpdateKeyValues.
   */
  struct ErrorInfo : cbe::util::ErrorInfoImpl<Error> {
    using Base::Base; // Inherit base class' constructors
  }; // struct ErrorInfo

#ifndef CBE_NO_SYNC
  /**
   * @brief exception thrown by
   * cbe::Object::updateKeyValues()
   * if the request fails.
   */
  struct Exception : cbe::util::ExceptionImpl<ErrorInfo> {
    using Base::Base; // Inherit base class' constructors
  }; // class struct CloudBackend::UpdateKeyValuesException
#endif // #ifndef CBE_NO_SYNC

  virtual ~UpdateKeyValuesDelegate();
}; // class UpdateKeyValuesDelegate

/**
 * Pointer to UpdateKeyValuesDelegate that is passed into:
 * <ul>
 *   <li> cbe::Object::updateKeyValues()
 * </ul>
 */
using UpdateKeyValuesDelegatePtr = std::shared_ptr<UpdateKeyValuesDelegate>;
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__UpdateKeyValuesDelegate_h__