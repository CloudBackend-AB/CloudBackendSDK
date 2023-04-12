/*
 Copyright © CloudBackend AB 2020 - 2023..
 */

#ifndef CBE_TRANSFEREVENTPROTOCOL_H_
#define CBE_TRANSFEREVENTPROTOCOL_H_

#include "CBE/Types.h"

#include <memory> // std::unique_ptr

namespace cbe {

/** Enum class for the different transfer failures that can occur. */
enum FailedStatus : cbe::failed_status_t {
  FileNotFound = 1,
  NotEnoughSpace = 2,
  Aborted = 3,
  Unknown = 4
}; // enum FailedStatus
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class TransferUploadEventProtocol {
public:
  virtual ~TransferUploadEventProtocol();

  /**Gets called when a upload has been completed. */
  virtual void onObjectUploaded(cbe::Object&& object);

  /** Gets called when a error has occured in the upload stream. */
  virtual void onObjectUploadFailed(std::string           name,
                                    cbe::ObjectId         objectId,
                                    cbe::ContainerId      parentId,
                                    cbe::failed_status_t  status);

  /** Gets called when a chunk of data has successfully been recieved. */
  virtual void onChunkSent(cbe::Object&&  object,
                           std::uint64_t    sent,
                           std::uint64_t    total);
}; // class TransferUploadEventProtocol
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/** Protocol for implement a TransferDownloadEvent delegate.
 * Abstract base class which is used for getting notified on
 * downloading progress. */
class TransferDownloadEventProtocol {
public:
  virtual ~TransferDownloadEventProtocol();

  /** Gets called when a chunk of data has successfully been recieved. */
  virtual void onChunkReceived(cbe::Object&&  object,
                               std::uint64_t    received,
                               std::uint64_t    total);

  /** Gets called when a download has completed. */
  virtual void onObjectDownloaded(cbe::Object&&   object,
                                  std::string     path);

  /** Gets called when a download has completed. */
  /** IMPORTANT!!   This data is on the heap and you are responsible for calling delete on it.  We may change data to a shared pointer in the future. */
  virtual void onObjectDownloaded(cbe::Object&&           object,
                                  std::unique_ptr<char[]> data);

  /** Gets called when a error has occured in the download stream */
  virtual void onObjectDownloadFailed(cbe::Object&&         object,
                                      cbe::failed_status_t  status);
}; // class TransferDownloadEventProtocol
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/** Inherits both the TransferUploadEventProtocol and the
 * TransferUploadEventProtocol. */
class TransferEventProtocol: virtual public TransferUploadEventProtocol,
                             virtual public TransferDownloadEventProtocol {
public:
  virtual ~TransferEventProtocol();
}; // class TransferEventProtocol

} // namespace cbe

#endif // CBE_TRANSFEREVENTPROTOCOL_H_
