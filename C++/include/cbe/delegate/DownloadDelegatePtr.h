#ifndef CBE__delegate__DownloadDelegatePtr_h__
#define CBE__delegate__DownloadDelegatePtr_h__

#include <memory>

namespace cbe {
  namespace delegate {

class DownloadDelegate;
/**
 * Pointer to DownloadDelegate that is passed into:
 * <ul>
 *   <li> cbe::Object::download(const std::string&,DownloadDelegate)
 * </ul>
 */
using DownloadDelegatePtr = std::shared_ptr<DownloadDelegate>;

  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__DownloadDelegatePtr_h__