#ifndef CBE__delegate__DownloadStreamDelegatePtr_h__
#define CBE__delegate__DownloadStreamDelegatePtr_h__

#include <memory>

namespace cbe {
  namespace delegate {
class DownloadStreamDelegate;

/**
 * Pointer to DownloadStreamDelegate that is passed into:
 * <ul>
 *   <li> cbe::Object::downloadStream(const std::string&,cbe::Stream,DownloadStreamDelegatePtr)
 * </ul>
 */
using DownloadStreamDelegatePtr = std::shared_ptr<DownloadStreamDelegate>;

  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__DownloadStreamDelegatePtr_h__