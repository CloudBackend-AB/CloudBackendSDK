/**
 * Copyright © CloudBackend AB 2021.
 */

/** Protocol to implement a TransferEvent delegate. (TransferEventProtocol implements both TransferUpload- and TransferDownloadProtocol.)
  * Abstract base class which is used for getting notified on
  * Upload/Download progress. */

/** @Override the functions that you want to use ex: upload needs onObjectUploaded and onObjectUploadFailed at least. */

package com.cbe;

public class TransferEventProtocol implements TransferUploadEventProtocol, TransferDownloadEventProtocol {

  /** Delete the underlying shared pointer object to TransferEventProtocol */
  public synchronized void delete() {}

  /**Gets called when a upload has been completed. */
  public void onObjectUploaded(Object object) {}

  /** Gets called when a error has occured in the upload stream. */
  public void onObjectUploadFailed(String name, java.math.BigInteger objectId, java.math.BigInteger parentId, long atState, long status) {}

  /** Gets called when a chunk of data has successfully been recieved. (optional) */
  public void onChunkSent(Object object, long sent, long total) {}

  /** Gets called when a chunk of data has successfully been recieved. (optional) */
  public void onChunkReceived(Object object, long received, long total) {}

  /** Gets called when a download has completed. */
  public void onObjectDownloaded(Object object, String path) {}

  /** Gets called when a download has completed. */
  public void onObjectBinaryDownloaded(Object object, byte[] data) {}

  /** Gets called when a error has occured in the download stream */
  public void onObjectDownloadFailed(Object object, long status) {}

}
