/*
     Copyright © CloudBackend AB 2020 - 2022.
*/

#ifndef INCLUDE_CBE_OBJECT_H_
#define INCLUDE_CBE_OBJECT_H_

#include <string>
#include <vector>

#include "cbe/Item.h"
#include "cbe/Stream.h"
#include "cbe/Types.h"

#include "cbe/delegate/AclDelegate.h"
#include "cbe/delegate/DownloadDelegatePtr.h"
#include "cbe/delegate/DownloadBinaryDelegatePtr.h"
#include "cbe/delegate/UpdateKeyValuesDelegate.h"
#include "cbe/delegate/GetStreamsDelegate.h"
#include "cbe/delegate/ShareDelegate.h"
#include "cbe/delegate/UnShareDelegate.h"
#include "cbe/delegate/PublishDelegate.h"
#include "cbe/delegate/UploadDelegate.h"
#include "cbe/delegate/UnpublishDelegate.h"
#include "cbe/delegate/UnsubscribeDelegate.h"
#include "cbe/delegate/Error.h"
#include "cbe/delegate/object/MoveDelegate.h"
#include "cbe/delegate/object/RenameDelegate.h"
#include "cbe/delegate/object/RemoveDelegate.h"

#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

namespace CBI {
class Object;
using ObjectPtr = std::shared_ptr<CBI::Object>;
class ItemEventProtocol;
using ItemDelegatePtr = std::shared_ptr<ItemEventProtocol>;
class TransferDownloadEventProtocol;
using TransferDownloadDelegatePtr =
                                std::shared_ptr<TransferDownloadEventProtocol>;
class TransferUploadEventProtocol;
using TransferUploadDelegatePtr = std::shared_ptr<TransferUploadEventProtocol>;
} // namespace CBI

namespace cbe
{

/**
 * @brief holder of a set of data, can represent a table row.
 * 
 */
class Object : public cbe::Item
{
public:
  using MoveDelegatePtr = delegate::object::MoveDelegatePtr;
  /**
   * to relocate an object to a different container.
   * 
   * @param destinationContainerId the id number of the container moving to.
   * @param delegate  Pointer to a delegate::object::MoveDelegate instance
   *                  that is implemented by the user.
   */
  void move(cbe::ContainerId      destinationContainerId,
            MoveDelegatePtr       delegate);

  using RenameDelegatePtr = delegate::object::RenameDelegatePtr;
  /** 
   * Rename object:
   * @param name string name of the object.
   * @param delegate  Pointer to a delegate::object::RenameDelegate instance
   *                  that is implemented by the user.
   */
  void rename(const std::string &name, RenameDelegatePtr delegate);

  using RemoveDelegatePtr = delegate::object::RemoveDelegatePtr;
  /**
   * Remove the object from cloud and locally
   * @param delegate  Pointer to a delegate::object::RemoveDelegate instance
   *                  that is implemented by the user.
   */
  void remove(RemoveDelegatePtr delegate);

  using DownloadDelegatePtr = delegate::DownloadDelegatePtr;
  /**
   * @brief Download the data of current object to the the local file system.
   * 
   * The data will be contained in file, named after the name of current object
   * (#name()), to the location given by paramter \p path.  
   * 
   * @param path      Folder location, on the local file system, of the file to
   *                  be downloaded. This string must end with a slash ("/").
   * @param delegate  Pointer to a delegate::DownloadDelegate instance
   *                  that is implemented by the user.
   */
  void download(const std::string&   path,
                DownloadDelegatePtr  delegate);

  using DownloadBinaryDelegatePtr = delegate::DownloadBinaryDelegatePtr;
  /**
   * @brief Download the binary data associated with current object.
   * 
   * The data, delivered as a BLOB (Binary Large OBject), via paramter \p data
   * in the the callback method
   * cbe::delegate::DownloadBinaryDelegate::onDownloadBinarySuccess().
   * 
   * @param delegate  Pointer to a delegate::DownloadBinaryDelegate instance
   *                  that is implemented by the user.
   */ 
  void download(DownloadBinaryDelegatePtr delegate);

  /**
   * Download a stream of an Object to local filesystem.
   * 
   * @param path   Folder location, on the local file system, of the file to be
   *               downloaded. This string must end with a slash ("/").
   * @param stream Get which stream you want by first calling getStream() and 
   *               then choose which one to download.
   * @param delegate  Pointer to a delegate::DownloadDelegate instance
   *                  that is implemented by the user.
   * 
   * @anchor anchorEx__cbe__Object__downloadStream 
   * @par Example of downloading the data attached to a cbe::Object via a Stream with Object::downloadStream()<br>
   *      Continuation of:
   *      @ref anchorEx__cbe__Object__uploadStream
   *      "Example of uploading data to a cbe::Object via a Stream with Object::uploadStream()"  
   * @code
  ~~~  
  #include "cbe/delegate/DownloadDelegate.h"
  ~~~  
  struct MyDownloadDelegate : cbe::delegate::DownloadDelegate {
    std::mutex              mutex{};
    std::condition_variable conditionVariable{};
    bool                    called{};

    Success success{};

    void onDownloadStreamSuccess(cbe::Object&& object,
                                 std::string   path) final {
      {
        std::lock_guard<std::mutex> lock{mutex};
        success = Success{std::move(object), std::move(path)};
        called = true;
      }
      conditionVariable.notify_one();
    }

    void onDownloadStreamError(cbe::delegate::TransferError&& transferError, 
                               cbe::util::Context&&           context) final {
      {
        std::lock_guard<std::mutex> lock{mutex};
        success = Success{};
        called = true;
      }
      conditionVariable.notify_one();
    }

    cbe::Object wait() {
      std::unique_lock<std::mutex> lock{mutex};
      conditionVariable.wait(lock, [this]{ return called; } );
      // Reset called flag, so current delegate instance can be reused
      called = false;
      // If object is still default constructed, this implies a failed
      // cbe::Object::downloadStream() operation                
      return success.object;                      
    }
  }; // struct MyDownloadDelegate
  ~~~  
  std::shared_ptr<MyDownloadDelegate> myDownloadDelegate =
                                  std::make_shared<MyDownloadDelegate>();
  constexpr const char* const downloadPath = "/tmp/download/";
  for (const auto& stream : streams2) {
    const auto path =  std::string{downloadPath} +
                         std::to_string(stream._streamId);
    object.downloadStream(path, stream, myDownloadDelegate);
    if (!myDownloadDelegate->wait()) {
      // Failure, bail out
      ~~~  
    }
  }
   * @endcode
   */
  void downloadStream(const std::string&          path,
                      cbe::Stream                 stream,
                      DownloadDelegatePtr   delegate);


  using UploadDelegatePtr = delegate::UploadDelegatePtr; 
  /**
   * @brief Upload a file for adding a new or replacing existing stream attached
   *        to this object.
   * 
   * You will need to call getStreams() again to identify all streams.
   * 
   * @param filePath  Fully qualified file name. I.e., the path, relative or
   *                  absolute, including file name. 
   * @param streamId If the stream id already exists, it will be 
   * overwritten.
   * @param delegate Pointer to a delegate::UploadDelegate instance
   *                 that is implemented by the user.
   * 
   * @anchor anchorEx__cbe__Object__uploadStream 
   * @par Example of uploading data to a cbe::Object via a Stream with Object::uploadStream<br>
   *      Continuation of:
   *      @ref anchorEx__cbe__Object__getStreams
   *      "Example of retrieving the Streams attached to a cbe::Object with Object::getStreams()"  
   * @code
  ~~~  
  #include "cbe/delegate/UploadDelegate.h"
  ~~~  
  #include <algorithm>  // std::max_element
  #include <vector>     // std::cbegin, std::cend
  ~~~  
  struct MyUploadDelegate : cbe::delegate::UploadDelegate {
    std::mutex              mutex{};
    std::condition_variable conditionVariable{};
    bool                    called{};

    // Default construct, further see comment for object member in
    // MyUploadDelegate above
    cbe::Object object{ cbe::DefaultCtor{} };

    void onUploadStreamSuccess(cbe::Object&& object) final {
      {
        std::lock_guard<std::mutex> lock{mutex};
        this->object = std::move(object);
        called = true;
      }
      conditionVariable.notify_one();
    }

    void onUploadStreamError(cbe::delegate::TransferError&& transferError, 
                             cbe::util::Context&&           context) final {
      {
        std::lock_guard<std::mutex> lock{mutex};
        object = cbe::Object{ cbe::DefaultCtor{} };
        called = true;
      }
      conditionVariable.notify_one();
    }

    cbe::Object wait() {
      std::unique_lock<std::mutex> lock{mutex};
      conditionVariable.wait(lock, [this]{ return called; } );
      // Reset called flag, so current delegate instance can be reused
      called = false;
      // If object is still default constructed, this implies a failed
      // cbe::Object::uploadStream() operation                
      return object;                      
    }
  }; // struct MyUploadDelegate
  ~~~  
  constexpr const char* const myFile2Name = "myStream2";
  const std::string           qualFile2Name = std::string{uploadPath} + myFile2Name;
  
  std::ofstream ofs2{qualFile2Name};
  ofs2 << "Line 21\n"  << "Line 22\n" << "Line 23\n" << "Line 24\n"
       << std::flush;
  ofs2.close();

  const auto nextStreamId =
              std::max_element(std::cbegin(streams1), std::cend(streams1),
                               [](const cbe::Stream& stream1,
                                  const cbe::Stream& stream2) {
                                 return stream1._streamId < stream2._streamId;
                               })->_streamId + 1;

  std::shared_ptr<MyUploadDelegate> myUploadDelegate =
                                  std::make_shared<MyUploadDelegate>();
  object.uploadStream(qualFile2Name, nextStreamId, myUploadDelegate);
  // Use of the bool type conversion operator to detect success or failure
  // as above
  if (!myUploadDelegate->wait()) {
    // Failure, bail out
    ~~~  
  }
  object.getStreams(getStreamsDelegate);
  if (!getStreamsDelegate->wait()) {
    // Failure, bail out
    ~~~  
  }
  const cbe::Object::Streams streams2 = object.streams();
  ~~~  
   * @endcode
   * Continues at: @ref anchorEx__cbe__Object__downloadStream 
   *               "Example of downloading the data attached to a cbe::Object via a Stream with Object::downloadStream()"
   */
  void uploadStream(const std::string&        filePath,
                    cbe::StreamId             streamId,
                    UploadDelegatePtr   delegate);

  using UpdateKeyValuesDelegatePtr = delegate::UpdateKeyValuesDelegatePtr;
  /**
   * Adds key/value pair data to the object.
   * \note Existing key will be overwritten, otherwise created.
   * 
   * @param delegate  Pointer to a delegate::UpdateKeyValuesDelegate instance
   *                  that is implemented by the user.
   * @param keyValues Optional map of key/value pairs (metadata).
   */
  void updateKeyValues(UpdateKeyValuesDelegatePtr delegate,
                       KeyValues                  keyValues = KeyValues{});

  using GetStreamsDelegatePtr = delegate::GetStreamsDelegatePtr;
  /**
   * @brief Downloads the streams attached to current object to the SDK's cache.
   * 
   * Use @ref streams() "object.streams()" after this
   * call to use the streams e.g., in downloadStream().
   * \note This method must be re-called if you upload more streams, 
   * see @c uploadStream()
   * @param delegate  Pointer to a delegate::GetStreamsDelegate instance
   *                  that is implemented by the user.
   *
   * @anchor anchorEx__cbe__Object__getStreams 
   * @par Example of retrieving the Streams attached to a cbe::Object with Object::getStreams()<br>
   *      Continuation of:
   *      @ref anchorEx__cbe__Container__upload
   *      "Example of creation of a cbe::Object by using Container::upload()"  
   * @code
  ~~~
  #include "cbe/delegate/GetStreamsDelegate.h"
  ~~~
  struct MyGetStreamsDelegate : cbe::delegate::GetStreamsDelegate {
    std::mutex              mutex{};
    std::condition_variable conditionVariable{};
    bool                    called{};

    // Default construct, further see comment for object member in
    // MyUploadDelegate above
    cbe::Object object{ cbe::DefaultCtor{} };

    void onGetStreamsSuccess(cbe::Object&& object) final {
      {
        std::lock_guard<std::mutex> lock{mutex};
        this->object = std::move(object);
        called = true;
      }
      conditionVariable.notify_one();
    }

    void onGetStreamsError(cbe::delegate::Error&& error, 
                           cbe::util::Context&&   context) final {
      {
        std::lock_guard<std::mutex> lock{mutex};
        object = cbe::Object{ cbe::DefaultCtor{} };
        called = true;
      }
      conditionVariable.notify_one();
    }

    cbe::Object wait() {
      std::unique_lock<std::mutex> lock{mutex};
      conditionVariable.wait(lock, [this]{ return called; } );
      // Reset called flag, so current delegate instance can be reused
      called = false;
      // If object is still default constructed, this implies a failed
      // cbe::Object::getStreams() operation                 
      return object;                      
    }
  }; // struct MyGetStreamsDelegate
  ~~~
  std::shared_ptr<MyGetStreamsDelegate> getStreamsDelegate =
                                      std::make_shared<MyGetStreamsDelegate>();
  object.getStreams(getStreamsDelegate);
  // Simply make use of the bool type conversion operator on the Object returned
  // from the wait() method to determine the outcome of the getStreams()
  // invocation above
  if (!getStreamsDelegate->wait()) {
    // Failure, bail out
    ~~~
  }
  // Now, since the meta data about the streams has been updated, we can
  // retrieve current streams attached to the object
  const cbe::Object::Streams streams1 = object.streams();
  ~~~
   * @endcode
   * Continues at: @ref anchorEx__cbe__Object__uploadStream 
   *               "Example of uploading data to a cbe::Object via a Stream with Object::uploadStream()"
   */
  void getStreams(GetStreamsDelegatePtr delegate);

  /**
   * @brief Collection of @ref cbe::Stream "Stream" objects. 
   */
  using Streams = std::vector<cbe::Stream>;
  /**
   * @brief Returns the currently loaded stream / streams attached to the
   *        object. 
   * If you upload a new stream with @c uploadStream(), 
   * you must call getStreams() again to get the new/updated streams.
   * \note If @c getStreams() has not been called, then @c streams() returns an
   * empty Streams object.
   * 
   * @par Example<br>
   *      See @ref anchorEx__cbe__Object__getStreams
   *      "Example of retrieving the Streams attached to a cbe::Object with Object::getStreams()"  
   */
  Streams streams() const;

  /**
   * Returns the mime type of the object. \n 
   * E.g.  application/pdf, audio/wav, image/jpg, text/xml, video/mp4 etc.
   */
  std::string getMimeType() const;

  /**
   * Returns the binary length/size of the object in Bytes.
   */
  uint64_t length() const;

  /**
   * Returns the Object type currently: Other | GroupInvite | ShareInvite
   */
  cbe::object_t getObjectType() const;

  /**
   * Returns all the key/values in a map.
   */
  cbe::KeyValues keyValues();

  using SetAclDelegatePtr = delegate::AclDelegatePtr;
  /**
   * Set the Access control list for the object.
   * @param aclMap  The desired @ref cbe::Permission "permission" for current
   *                object. 
   * @param delegate  Pointer to a delegate::AclDelegate instance that is
   *                  implemented by the user.
   */
  void setAcl(cbe::AclMap       aclMap,
              SetAclDelegatePtr delegate);

  using GetAclDelegatePtr = delegate::AclDelegatePtr;
  /**
   * get the Access Control List for the Object.
   * @param delegate  Pointer to a delegate::AclDelegate instance
   *                  that is implemented by the user.
   */
  void getAcl(GetAclDelegatePtr delegate);

  using ShareDelegatePtr = delegate::ShareDelegatePtr;
  /**
   * Share Object to a user. Notifies the user that a share has occured so that
   * the user can check what permissions the have been given. Sharing gives read 
   * permissions as of right now but might change in the future.
   * @param toUserGroup takes a user id or group id (lastly named is for the
   * future) and share to.
   * @param description names the specific share between you and the user/group.
   * @param delegate  Pointer to a delegate::ShareDelegatePtr instance
   *                  that is implemented by the user.
   */
  void share(cbe::UserId       toUserGroup,
             std::string       description,
             ShareDelegatePtr  delegate);

  using UnShareDelegatePtr = delegate::UnShareDelegatePtr;
  /**
   * unShare the object to a specific shareId created when sharing. Each share 
   * is unique between user/group and the one sharing. This is represented with 
   * a unique share id.
   * @param shareId is as mentioned the unique id for a share between the owner 
   * and other user/group.
   * @param delegate  Pointer to a delegate::UnShareDelegate instance
   *                  that is implemented by the user.
   */
  void unShare(cbe::ShareId shareId, UnShareDelegatePtr delegate);

  using PublishDelegatePtr = delegate::PublishDelegatePtr;
  /**
   * Publishes an object to a user.
   * @param security A cbe::Access enum
   * @param privacy A cbe::WebShareVisibility enum
   * @param description Description
   * @param password Password
   * @param publishDelegate Gets notified when the object has been published 
   * (or if there was an error)
   */
  void publish(cbe::PublishAccess      security,
               cbe::PublishVisibility  privacy,
               std::string             description,
               std::string             password,
               PublishDelegatePtr      publishDelegate);

  using UnpublishDelegatePtr = delegate::UnpublishDelegatePtr;
  /**
   * Unpublishes this object.
   * @param publishDelegate Gets notified when the object has been unpublished 
   * (or if there was an error)
   */
  void unpublish(UnpublishDelegatePtr publishDelegate);

  using UnsubscribeDelegatePtr = delegate::UnsubscribeDelegatePtr;
  /**
   * Unsubscribes from this object.
   * @param subscribeDelegate Gets notified when the object has been 
   * unsubscribed (or if there was an error)
   */
  void unsubscribe(UnsubscribeDelegatePtr subscribeDelegate);

  //url to item
  std::string url();
  Object(cbe::DefaultCtor);

private:
  struct Impl;
  Impl& getImpl() const;

  friend class CloudBackend;
  friend class Container;
  Object(CBI::ObjectPtr cbiPeerPtr);
  CBI::Object& getCbiPeer() const;
}; // class Object

} // namespace cbe

#endif // INCLUDE_CBE_OBJECT_H_
