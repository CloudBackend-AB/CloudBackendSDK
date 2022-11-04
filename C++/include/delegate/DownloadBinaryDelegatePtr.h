#ifndef CBE__delegate__DownloadBinaryDelegatePtr_h__
#define CBE__delegate__DownloadBinaryDelegatePtr_h__

#include <memory>

namespace cbe {
  namespace delegate {

class DownloadBinaryDelegate;
/**
 * Pointer to DownloadBinaryDelegate that is passed into:
 * <ul>
 *   <li> cbe::Object::download(DownloadBinaryDelegatePtr)
 * </ul>
 */
using DownloadBinaryDelegatePtr = std::shared_ptr<DownloadBinaryDelegate>;

  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__DownloadBinaryDelegatePtr_h__