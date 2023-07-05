#ifndef CBE__delegate__PublishSuccess_h__
#define CBE__delegate__PublishSuccess_h__

#include "cbe/Item.h"

namespace cbe {
  namespace delegate {

/**
 * @brief
 * Convenience type that bundles the parameter passed to method
 * cbe::delegate::ShareDelegate::onShareSuccess.
 */
class PublishSuccess {
public:
  cbe::Items  items{};

  PublishSuccess();
  PublishSuccess(cbe::DefaultCtor);
  PublishSuccess(cbe::Items&& items);
 /**
   * @brief Checks if current instance is valid.
   * 
   * @return \c true: is valid
   */
  explicit operator bool() const;
}; // class PublishSuccess

  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__PublishSuccess_h__