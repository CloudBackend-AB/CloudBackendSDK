#ifndef cbe__delegate__ProgressEventFn_h__
#define cbe__delegate__ProgressEventFn_h__

#include <functional>

namespace cbe {
  namespace delegate {

// Forward declaration
class ChunkTransferred;

/**
 * @brief
 * Callback interface function that the CloudBackend SDK uses to indicate the
 * progress of an upload/download.
 *
 * . 
 */
using ProgressEventFn = std::function<void(const ChunkTransferred&)>;

  } // namespace delegate
} // namespace cbe

#endif // #ifndef cbe__delegate__ProgressEventFn_h__
