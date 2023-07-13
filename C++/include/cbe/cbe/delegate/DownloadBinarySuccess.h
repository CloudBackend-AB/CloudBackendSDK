#ifndef CBE__delegate__DownloadBinarySuccess_h__
#define CBE__delegate__DownloadBinarySuccess_h__

#include "cbe/Object.h"

#include <memory>
#include <string>

namespace cbe {
  namespace delegate {

/**
 * @brief
 * Convenience type that bundles all parameters passed to method
 * cbe::delegate::DownloadBinaryDelegate::onDownloadBinarySuccess.
 */
class DownloadBinarySuccess {
public:
  cbe::Object             object{cbe::DefaultCtor{}};
  std::unique_ptr<char[]> data{};
  
  DownloadBinarySuccess();
  DownloadBinarySuccess(cbe::DefaultCtor);
  DownloadBinarySuccess(cbe::Object&&            object,  
                        std::unique_ptr<char[]>  data);

/**
   * @brief Checks if current instance is valid.
   * 
   * @return \c true: is valid
   */
  explicit operator bool() const;
}; // class DownloadBinarySuccess

  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__DownloadBinarySuccess_h__