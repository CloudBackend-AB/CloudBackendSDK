/*
     Copyright © CloudBackend AB 2020.
*/

#ifndef CBE_TRANSFEREVENTPROTOCOL_H_
#define CBE_TRANSFEREVENTPROTOCOL_H_

#include "CBE/Item.h"
#include "CBE/QueryResult.h"

#include "CBE/Object.h"
#include "CBE/Types.h"
#include <memory>
#include "CBE/Filter.h"


namespace CBE
{
    class Item;
/** Enum class for the different transfer failures that can occur. */
    enum FailedStatus : CBE::failed_status_t
    {
    FileNotFound = 1,
    NotEnoughSpace = 2,
    Aborted = 3,
    Unknown = 4
    }; // enum FailedStatus



  class TransferUploadEventProtocol {
    public:
        virtual ~TransferUploadEventProtocol(){
            }

            /**Gets called when a upload has been completed. */
            virtual void onObjectUploaded(CBE::ObjectPtr object){};

            /** Gets called when a error has occured in the upload stream. */
            virtual void onObjectUploadFailed(std::string name, CBE::object_id_t objectId, CBE::container_id_t parentId, CBE::persistence_t atState, CBE::failed_status_t status){};

            /** Gets called when a chunk of data has successfully been recieved. */
            virtual void onChunkSent(CBE::ObjectPtr object,
                                     size_t sent,
                                     size_t total) {
            }
    };

/** Protocol for implement a TransferDownloadEvent delegate.
   * Abstract base class which is used for getting notified on
   * downloading progress. */
    class TransferDownloadEventProtocol
    {
    public:
      virtual ~TransferDownloadEventProtocol() {}

      /** Gets called when a chunk of data has successfully been recieved. */
      virtual void onChunkReceived(CBE::ObjectPtr object,
                                   size_t received,
                                   size_t total) {
      }

      /** Gets called when a download has completed. */
      virtual void onObjectDownloaded(CBE::ObjectPtr object,
                                      CBE::user_id_t user_id,
                                      std::string path){};

      /** Gets called when a download has completed. */
      /** IMPORTANT!!   This data is on the heap and you are responsible for calling delete on it.  We may change data to a shared pointer in the future. */
      virtual void onObjectDownloaded(CBE::ObjectPtr object,
                                      CBE::user_id_t user_id,
                                      char* data){};

      /** Gets called when a error has occured in the download stream */
      virtual void onObjectDownloadFailed(CBE::ObjectPtr object, CBE::failed_status_t status) {
      }

  };



/** Inherits both the TransferUploadEventProtocol and the
   * TransferUploadEventProtocol. */
    class TransferEventProtocol
        : virtual public TransferUploadEventProtocol,
          virtual public TransferDownloadEventProtocol
    {
    public:
        virtual ~TransferEventProtocol() {}

    };

}

#endif // CBE_TRANSFEREVENTPROTOCOL_H_
