#ifndef CBE__delegate__RenameSuccess_h__
#define CBE__delegate__RenameSuccess_h__

#include "cbe/Group.h"

#include <string>

namespace cbe {
  namespace delegate {
    namespace group {

/**
 * @brief
 * Convenience type that bundles all parameters passed to method
 * cbe::delegate::group::RenameDelegate::onRenameSuccess.
 */
class RenameSuccess {
public:
  cbe::Group group{cbe::DefaultCtor{}};
  std::string newName{};
  
  RenameSuccess();
  RenameSuccess(cbe::DefaultCtor);
  RenameSuccess(cbe::Group&& group, std::string&& newName);

/**
   * @brief Checks if current instance is valid.
   * 
   * @return \c true: is valid
   */
  explicit operator bool() const;
}; // class RenameSuccess

    } // namespace group
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__RenameSuccess_h__