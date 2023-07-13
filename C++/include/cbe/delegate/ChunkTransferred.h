#ifndef cbe__delegate__ChunkTransferred_h__
#define cbe__delegate__ChunkTransferred_h__

#include "cbe/delegate/ProgressEventFn.h"
#include "cbe/Object.h"

namespace cbe {
  namespace delegate {

/**
 * @brief
 * Bundles the progress event information in connection with download and
 * upload.
 * 
 * Convenience type that bundles all parameters passed to methods:
 * <ul>
 *   <li> cbe::delegate::DownloadDelegate::onChunkReceived()
 *   <li> cbe::delegate::DownloadBinaryDelegate::onChunkReceived()
 *   <li> cbe::delegate::UploadDelegate::onChunkSent()
 *   <li> cbe::delegate::UploadBinaryDelegate::onChunkSent()
 * </ul>
 */
class ChunkTransferred {
public:
  cbe::Object   object{cbe::DefaultCtor{}};
  std::uint64_t transferred{};
  std::uint64_t total{};

  ChunkTransferred();
  ChunkTransferred(cbe::DefaultCtor);
  ChunkTransferred(
                  cbe::Object&& object, 
                  std::uint64_t transferred,
                  std::uint64_t total);

}; // class ChunkTransferred

  } // namespace delegate
} // namespace cbe

#endif // #ifndef cbe__delegate__ChunkTransferred_h__
