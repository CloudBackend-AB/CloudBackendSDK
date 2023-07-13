#ifndef CBE__delegate__DownloadSuccess_h__
#define CBE__delegate__DownloadSuccess_h__

#include "cbe/Object.h"

#include <string>

namespace cbe {
  namespace delegate {

/**
 * @brief
 * Convenience type that bundles all parameters passed to method
 * cbe::delegate::DownloadDelegate::onDownloadSuccess.
 */
class DownloadSuccess {
public:
  cbe::Object object{cbe::DefaultCtor{}};
  std::string path{};
  
  DownloadSuccess();
  DownloadSuccess(cbe::DefaultCtor);
  DownloadSuccess(cbe::Object&& object, 
                  std::string   path);

/**
   * @brief Checks if current instance is valid.
   * 
   * @return \c true: is valid
   */
  explicit operator bool() const;
}; // class DownloadSuccess

  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__DownloadSuccess_h__