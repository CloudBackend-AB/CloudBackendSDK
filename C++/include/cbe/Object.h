/*
     Copyright © CloudBackend AB 2020-2023.
*/

#ifndef INCLUDE_CBE_OBJECT_H_
#define INCLUDE_CBE_OBJECT_H_

#include "cbe/Item.h"
#include "cbe/Stream.h"
#include "cbe/Types.h"

#include "cbe/delegate/AclDelegate.h"
#include "cbe/delegate/DownloadBinaryDelegate.h"
#include "cbe/delegate/DownloadDelegate.h"
#include "cbe/delegate/GetStreamsDelegate.h"
#include "cbe/delegate/object/MoveDelegate.h"
#include "cbe/delegate/object/RemoveDelegate.h"
#include "cbe/delegate/object/RenameDelegate.h"
#include "cbe/delegate/PublishDelegate.h"
#include "cbe/delegate/ShareDelegate.h"
#include "cbe/delegate/UnPublishDelegate.h"
#include "cbe/delegate/UnShareDelegate.h"
#include "cbe/delegate/UnSubscribeDelegate.h"
#include "cbe/delegate/UpdateKeyValuesDelegate.h"
#include "cbe/delegate/UploadDelegate.h"

#ifndef CBE_NO_SYNC
#include "cbe/delegate/ProgressEventFn.h"
#endif

#include "cbe/delegate/Error.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"
#include "cbe/util/Optional.h"

#include <string>
#include <vector>

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
 * @brief Holder of a set of data, can represent a table row.
 * 
 * \note The object name may not contain characters < & : / <br>
 * Any key name must start with a letter or _ <br>
 * The following key names are reserved and should not be used:
 * category, content, id, link and date <br>
 * Key names are case sensitive, hence variations with uppercase are permitted.
 */
class Object : public cbe::Item
{
public:
  /**
   * Pointer to cbe::delegate::object::MoveDelegate that is passed into
   * asynchronous version of method
   * @ref move(cbe::ContainerId,MoveDelegatePtr) "move()"
   */
  using MoveDelegatePtr = delegate::object::MoveDelegatePtr;
  /**
   * @brief Relocates an object to a different container.
   * 
   * <b>Asynchronous</b> version of this service function.
   *
   * @param dstId Id of the destination container.
   * @param delegate  Pointer to a delegate::object::MoveDelegate instance
   *                  that is implemented by the user.
   */
  void move(
            cbe::ContainerId      dstId,
            MoveDelegatePtr       delegate);
#ifndef CBE_NO_SYNC
  /**
   * See delegate::object::MoveDelegate::Exception
   */
  using MoveException = delegate::object::MoveDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * move(cbe::ContainerId,MoveDelegatePtr)
   * , and <b>throws an exception</b>, #MoveException, in case of a failed
   * call.
   * <br>See move(cbe::ContainerId,MoveDelegatePtr)
   *
   * @return The moved object &mdash; if the call was successful.
   * 
   * @throws #MoveException
   */
  cbe::Object move(
                   cbe::ContainerId  dstId);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref move(cbe::ContainerId,MoveError&) "move()"
   * <br>See delegate::object::MoveDelegate::ErrorInfo
   */
  using MoveError = delegate::object::MoveDelegate::ErrorInfo;
  /**
   * <b>Synchronous</b> version of
   * move(cbe::ContainerId,MoveDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See move(cbe::ContainerId,MoveDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #MoveError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::Object> move(
                                        cbe::ContainerId  dstId,
                                        MoveError&        error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::object::RenameDelegate that is passed into
   * asynchronous version of method
   * @ref rename(const std::string&, RenameDelegatePtr) "rename()"
   */
  using RenameDelegatePtr = delegate::object::RenameDelegatePtr;
  /** 
   * @brief Rename object.
   * 
   * <b>Asynchronous</b> version of this service function.
   * 
   * @param name string name of the object.
   * @param delegate  Pointer to a delegate::object::RenameDelegate instance
   *                  that is implemented by the user.
   */
  void rename(
              const std::string& name, RenameDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
   * See delegate::object::RenameDelegate::Exception
   */
  using RenameException = delegate::object::RenameDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * rename(const std::string&,RenameDelegatePtr)
   * , and <b>throws an exception</b>, #RenameException, in case of a failed
   * call.
   * <br>See rename(const std::string&,RenameDelegatePtr)
   *
   * @return The renamed object &mdash; if the call was successful.
   * 
   * @throws #RenameException
   */
  cbe::Object rename(
                     const std::string& name);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref rename(const std::string&,RenameError&) "rename()"
   * <br>See delegate::object::RenameDelegate::ErrorInfo
   */
  using RenameError = delegate::object::RenameDelegate::ErrorInfo;
  /**
   * <b>Synchronous</b> version of
   *  rename(const std::string&,RenameDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See rename(const std::string&,RenameDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #RenameError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::Object> rename(
                                    const std::string&  name,
                                    RenameError&        error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::object::RenameDelegate that is passed into
   * asynchronous version of method
   * @ref remove(RemoveDelegatePtr) "remove()"
   */
  using RemoveDelegatePtr = delegate::object::RemoveDelegatePtr;
  /**
   * @brief Remove the object from cloud and locally.
   * 
   * <b>Asynchronous</b> version of this service function.
   * 
   * @param delegate  Pointer to a delegate::object::RemoveDelegate instance
   *                  that is implemented by the user.
   */
  void remove(
              RemoveDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
   * See delegate::object::RemoveDelegate::Exception
   */
  using RemoveException = delegate::object::RemoveDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * remove(RemoveDelegatePtr)
   * , and <b>throws an exception</b>, #RemoveException, in case of a failed
   * call.
   * <br>See remove(RemoveDelegatePtr)
   *
   * @return Information about the removed object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::object::RemoveSuccess "RemoveSuccess"
   * 
   * @throws #RemoveException
   */
  delegate::object::RemoveSuccess remove();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref remove(RemoveError&) "remove()"
   * <br>See delegate::object::RemoveDelegate::ErrorInfo
   */
  using RemoveError = delegate::object::RemoveDelegate::ErrorInfo;
  /**
   * <b>Synchronous</b> version of
   *  remove(RemoveDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See remove(RemoveDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #RemoveError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::object::RemoveSuccess> remove(
                                                            RemoveError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::DownloadDelegate that is passed into
   * asynchronous version of methods:
   * <ul>
   *   <li> download(const std::string&,DownloadDelegatePtr)
   *   <li> downloadStream(const std::string&,cbe::Stream,DownloadDelegatePtr)
   * </ul>
   */
  using DownloadDelegatePtr = delegate::DownloadDelegatePtr;
  /**
   * @brief Download the data of current object to the the local file system.
   * 
   * <b>Asynchronous</b> version of this service function.
   *
   * The data will be contained in file, named after the name of current object
   * (method #name()), to the location given by parameter \p path.  
   * 
   * @param path      Folder location, on the local file system, of the file to
   *                  be downloaded. This string must end with a slash ("/").
   * @param delegate  Pointer to a delegate::DownloadDelegate instance
   *                  that is implemented by the user.
   */
  void download(
                const std::string&   path,
                DownloadDelegatePtr  delegate);
#ifndef CBE_NO_SYNC
  /**
   * The exception type thrown by the synchronous version of methods:
   * <ul>
   *   <li> download(const std::string&,delegate::ProgressEventFn&&) 
   *   <li> download(const std::string&) 
   *   <li> downloadStream(const std::string&,cbe::Stream,delegate::ProgressEventFn&&) 
   *   <li> downloadStream(const std::string&,cbe::Stream) 
   * </ul>
   * See delegate::DownloadDelegate::Exception
   */
  using DownloadException = delegate::DownloadDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * download(const std::string&,DownloadDelegatePtr)
   * , and <b>throws an exception</b>, #DownloadException, in case of a failed
   * call.
   * <br>See download(const std::string&,DownloadDelegatePtr)
   * 
   * @param progressEventFn Callback function that is called for each memory
   *                        chunk downloaded.<br>
   *                        The callback function will be executed the calling
   *                        thread's context.<br>
   *                        Also see cbe::delegate::ProgressEventFn.<br>
   *                        For the other parameters, see
   *                        download(const std::string&,DownloadDelegatePtr)
   *
   * @return Information about the download object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::DownloadSuccess "DownloadSuccess"
   *         \note To use this method, header file<br>
   *         <b>cbe/delegate/DownloadSuccess.h</b><br>
   *         must be included
   * 
   * @throws #DownloadException
   */
  delegate::DownloadSuccess download(
                                const std::string&          path,
                                delegate::ProgressEventFn&& progressEventFn);
  /**
   * Same as
   * download(const std::string&,delegate::ProgressEventFn&&), but
   * without the parameter, \p progressEventFn.
   */
  delegate::DownloadSuccess download(
                                const std::string&          path);
  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of methods:
   * <ul>
   *   <li> download(const std::string&,delegate::ProgressEventFn&&,DownloadError&) 
   *   <li> download(const std::string&,DownloadError&) 
   *   <li> downloadStream(const std::string&,cbe::Stream,delegate::ProgressEventFn&&,DownloadError&) 
   *   <li> downloadStream(const std::string&,cbe::Stream,DownloadError&) 
   * </ul>
   * <br>See delegate::DownloadDelegate::ErrorInfo
   */
  using DownloadError = delegate::DownloadDelegate::ErrorInfo;
  /**
   * Similar to synchronous method 
   * download(const std::string&,delegate::ProgressEventFn&&)
   * , but <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See download(const std::string&,delegate::ProgressEventFn&&)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #DownloadError object passed in will we be populated with the
   *              error information.<br>
   *              For the other parameters, see
   *              download(const std::string&,delegate::ProgressEventFn&&)
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::DownloadSuccess> download(
                                  const std::string&          path,
                                  delegate::ProgressEventFn&& progressEventFn,
                                  DownloadError&              error);
  /**
   * Same as
   * download(const std::string&,delegate::ProgressEventFn&&,DownloadError&)
   * , but without the parameter, \p progressEventFn.
   */
  cbe::util::Optional<delegate::DownloadSuccess> download(
                                  const std::string&          path,
                                  DownloadError&              error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::DownloadBinaryDelegate that is passed into
   * asynchronous version of method
   * @ref download(DownloadBinaryDelegatePtr) "download()"
   */
  using DownloadBinaryDelegatePtr = delegate::DownloadBinaryDelegatePtr;
  /**
   * @brief Download the binary data associated with current object.
   * 
   * <b>Asynchronous</b> version of this service function.
   *
   * The data, delivered as a BLOB (Binary Large OBject), via parameter
   * \p data in the the callback method
   * cbe::delegate::DownloadBinaryDelegate::onDownloadBinarySuccess().
   * 
   * @param delegate  Pointer to a delegate::DownloadBinaryDelegate instance
   *                  that is implemented by the user.
   * @param sizeLimit Blocks anything larger than the size limit the 
   *                  user inputs. Prevents accidental downloads of 
   *                  too large objects on to the device.
   */ 
  void download(std::size_t&& sizeLimit,
                DownloadBinaryDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
   * See delegate::DownloadBinaryDelegate::Exception
   */
  using DownloadBinaryException = delegate::DownloadBinaryDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * download(DownloadBinaryDelegatePtr)
   * , and <b>throws an exception</b>, #DownloadBinaryException, in case of a
   * failed call.
   * <br>See download(DownloadBinaryDelegatePtr)
   *
   * @param progressEventFn See download(const std::string&,delegate::ProgressEventFn&&).<br>
   *                        For the other parameters, see
   *                        download(DownloadBinaryDelegatePtr)
   * 
   * @return Information about the downloaded object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::DownloadBinarySuccess "DownloadBinarySuccess"
   *         \note To use this method, header file<br>
   *         <b>cbe/delegate/DownloadBinarySuccess.h</b><br>
   *         must be included
   *
   * @throws #DownloadBinaryException
   */
  delegate::DownloadBinarySuccess download(std::size_t&& sizeLimit,
                                  delegate::ProgressEventFn&& progressEventFn);
  /**
   * Same as
   * download(delegate::ProgressEventFn&&)
   * , but without the parameter, \p progressEventFn.
   */
  delegate::DownloadBinarySuccess download(std::size_t&& sizeLimit);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of methods:
   * <ul>
   *   <li> download(delegate::ProgressEventFn&&,DownloadBinaryError&) 
   *   <li> download(DownloadBinaryError&) 
   * </ul>
   * <br>See delegate::DownloadBinaryDelegate::ErrorInfo
   */
  using DownloadBinaryError = delegate::DownloadBinaryDelegate::ErrorInfo;
  /**
   * Similar to synchronous method 
   * download(delegate::ProgressEventFn&&)
   * , but <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See download(delegate::ProgressEventFn&&)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #DownloadError object passed in will we be populated with the
   *              error information.<br>
   *              For the other parameters, see
   *              download(delegate::ProgressEventFn&&)
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::DownloadBinarySuccess> download(
                                    std::size_t&& sizeLimit,
                                    delegate::ProgressEventFn&& progressEventFn,
                                    DownloadBinaryError&        error);
  /**
   * Same as
   * download(delegate::ProgressEventFn&&,DownloadBinaryError&)
   * , but without the parameter, \p progressEventFn.
   */
  cbe::util::Optional<delegate::DownloadBinarySuccess> download(
                                    std::size_t&& sizeLimit,
                                    DownloadBinaryError&        error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Download a stream of an Object to local filesystem.
   * 
   * <b>Asynchronous</b> version of this service function.
   * 
   * @param path   Folder location, on the local file system, of the file to be
   *               downloaded. This string must end with a slash ("/").
   * @param stream Get which stream you want by first calling getStream() and 
   *               then choose which one to download.
   * @param delegate  Pointer to a delegate::DownloadDelegate instance
   *                  that is implemented by the user.
   * 
   * @anchor anchorEx__cbe__Object__downloadStream_async 
   * @par Example
   *      <b>Async</b> downloading the data attached to a cbe::Object via a Stream with <b>%Object::downloadStream()</b><br>
   *      Continuation of:
   *      @ref anchorEx__cbe__Object__uploadStream_async
   *      "Async uploading data to a cbe::Object via a Stream with Object::uploadStream()"  
   * @include example/downloadStream_async.cpp
   */
  void downloadStream(
                      const std::string&  path,
                      cbe::Stream         stream,
                      DownloadDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
   * <b>Synchronous</b> version of
   * downloadStream(const std::string&,cbe::Stream,DownloadDelegatePtr)
   * , and <b>throws an exception</b>, #DownloadException, in case of a failed
   * call.
   * <br>See downloadStream(DownloadDelegatePtr)
   *
   * @param progressEventFn See download(const std::string&,delegate::ProgressEventFn&&).<br>
   *                        For the other parameters, see
   *                        downloadStream(DownloadDelegatePtr)
   * 
   * @return Information about the downloaded stream
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::DownloadSuccess "DownloadSuccess"
   * 
   * @throws #DownloadException
   * 
   * @anchor anchorEx__cbe__Object__downloadStream_syncExcept 
   * @par Example
   *      <b>Sync</b> [exception] downloading the data attached to a cbe::Object via a Stream with <b>%Object::downloadStream()</b><br>
   *      Continuation of:
   *      @ref anchorEx__cbe__Object__uploadStream_syncExcept
   *      "Sync [exception] uploading data to a cbe::Object via a Stream with Object::uploadStream()"  
   * @include example/downloadStream_syncExcept.cpp
   */
  delegate::DownloadSuccess downloadStream(
                                  const std::string&          path,
                                  cbe::Stream                 stream,
                                  delegate::ProgressEventFn&& progressEventFn);
  /**
   * Same as
   * downloadStream(const std::string&,cbe::Stream,delegate::ProgressEventFn&&)
   * , but without the parameter, \p progressEventFn.
   */
  delegate::DownloadSuccess downloadStream(
                                  const std::string&          path,
                                  cbe::Stream                 stream);
  /**
   * Similar to synchronous method 
   * downloadStream(const std::string&,cbe::Stream,DownloadDelegatePtr)
   * , but <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See downloadStream(const std::string&,cbe::Stream,DownloadDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #DownloadError object passed in will we be populated with the
   *              error information.<br>
   *              For the other parameters, see
   *              downloadStream(const std::string&,cbe::Stream,DownloadDelegatePtr)
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   * 
   * @anchor anchorEx__cbe__Object__downloadStream_syncNoExcept 
   * @par Example
   *      <b>Sync</b> [non-throwing] downloading the data attached to a cbe::Object via a Stream with <b>%Object::downloadStream()</b><br>
   *      Continuation of:
   *      @ref anchorEx__cbe__Object__uploadStream_syncNoExcept
   *      "Sync [non-throwing] uploading data to a cbe::Object via a Stream with Object::uploadStream()"  
   * @include example/downloadStream_syncNoThrow.cpp
   */
  cbe::util::Optional<delegate::DownloadSuccess> downloadStream(
                                  const std::string&          path,
                                  cbe::Stream                 stream,
                                  delegate::ProgressEventFn&& progressEventFn,
                                  DownloadError&              error);
  /**
   * Same as
   * downloadStream(const std::string&,cbe::Stream,delegate::ProgressEventFn&&,DownloadError&)
   * , but without the parameter, \p progressEventFn.
   */
  cbe::util::Optional<delegate::DownloadSuccess> downloadStream(
                                  const std::string&          path,
                                  cbe::Stream                 stream,
                                  DownloadError&              error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::UploadDelegate that is passed into
   * asynchronous version of method
   * @ref uploadStream(const std::string&,cbe::StreamId,UploadDelegatePtr) "uploadStream()"
   */
  using UploadDelegatePtr = delegate::UploadDelegatePtr; 
  /**
   * @brief Upload a file for adding a new or replacing existing stream attached
   *        to this object.
   * 
   * <b>Asynchronous</b> version of this service function.
   * 
   * Requires that method getStreams(GetStreamsDelegatePtr) is called to
   * identify all streams associated with current object.
   * 
   * @param filePath  Fully qualified file name. I.e., the path, relative or
   *                  absolute, including file name. 
   * @param streamId If the stream id already exists, it will be 
   * overwritten.
   * @param delegate Pointer to a delegate::UploadDelegate instance
   *                 that is implemented by the user.
   * 
   * @anchor anchorEx__cbe__Object__uploadStream_async 
   * @par Example
   *      <b>Async</b> uploading data to a cbe::Object via a Stream with <b>%Object::uploadStream()</b><br>
   *      Continuation of:
   *      @ref anchorEx__cbe__Object__getStreams_async
   *      "Async retrieving the Streams attached to a cbe::Object with the Object::getStreams()"  
   * @include example/uploadStream_async.cpp
   * Continues at: @ref anchorEx__cbe__Object__downloadStream_async 
   *               "Async downloading the data attached to a cbe::Object via a Stream with Object::downloadStream()"
   */
  void uploadStream(
                    const std::string&        filePath,
                    cbe::StreamId             streamId,
                    UploadDelegatePtr         delegate);
#ifndef CBE_NO_SYNC
  /**
   * See delegate::object::UploadDelegate::Exception
   */
  using UploadException = delegate::UploadDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * uploadStream(const std::string&,cbe::StreamId,UploadDelegatePtr)
   * , and <b>throws an exception</b>, #UploadException, in case of a failed
   * call.
   * <br>See uploadStream(const std::string&,cbe::StreamId,UploadDelegatePtr)
   *
   * @param progressEventFn See download(const std::string&,delegate::ProgressEventFn&&).<br>
   *                        For the other parameters, see
   *                        uploadStream(const std::string&,cbe::StreamId,UploadDelegatePtr)
   * 
   * @return Current object that was uploaded
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::Object "cbe::Object"
   * 
   * @throws #UploadException
   * 
   * @anchor anchorEx__cbe__Object__uploadStream_syncExcept 
   * @par Example
   *      <b>Sync</b> [exception] uploading data to a cbe::Object via a Stream with <b>%Object::uploadStream()</b><br>
   *      Continuation of:
   *      @ref anchorEx__cbe__Object__getStreams_syncExcept
   *      "Sync [exception] retrieving the Streams attached to a cbe::Object with the Object::getStreams()"  
   * @include example/uploadStream_syncExcept.cpp
   * Continues at: @ref anchorEx__cbe__Object__downloadStream_syncExcept 
   *               "Sync [exception] downloading the data attached to a cbe::Object via a Stream with Object::downloadStream()"
   */
  cbe::Object uploadStream(
                           const std::string&           filePath,
                           cbe::StreamId                streamId,
                           delegate::ProgressEventFn&&  progressEventFn);
  /**
   * Same as
   * uploadStream(const std::string&,cbe::StreamId,delegate::ProgressEventFn&&)
   * , but without the parameter, \p progressEventFn.
   */
  cbe::Object uploadStream(
                           const std::string&           filePath,
                           cbe::StreamId                streamId);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of methods:
   * <ul>
   *   <li> uploadStream(const std::string&,cbe::StreamId,delegate::ProgressEventFn&&,UploadError&) 
   *   <li> uploadStream(const std::string&,cbe::StreamId,UploadError&) 
   * </ul>
   * <br>See delegate::UploadDelegate::ErrorInfo
   */
  using UploadError = delegate::UploadDelegate::ErrorInfo;
  /**
   * Similar to synchronous method 
   * uploadStream(const std::string&,cbe::StreamId,delegate::ProgressEventFn&&)
   * , but <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See uploadStream(const std::string&,cbe::StreamId,UploadDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #DownloadError object passed in will we be populated with the
   *              error information.<br>
   *              For the other parameters, see
   *              uploadStream(const std::string&,cbe::StreamId,delegate::ProgressEventFn&&)
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   * 
   * @anchor anchorEx__cbe__Object__uploadStream_syncNoExcept 
   * @par Example
   *      <b>Sync</b> [non-throwing] uploading data to a cbe::Object via a Stream with <b>%Object::uploadStream()</b><br>
   *      Continuation of:
   *      @ref anchorEx__cbe__Object__getStreams_syncNoExcept
   *      "Sync non-throwing of retrieving the Streams attached to a cbe::Object with the Object::getStreams()"  
   * @include example/uploadStream_syncNoThrow.cpp
   * Continues at: @ref anchorEx__cbe__Object__downloadStream_syncNoExcept 
   *               "Sync [non-throwing] downloading the data attached to a cbe::Object via a Stream with Object::downloadStream()"
   */
  cbe::util::Optional<cbe::Object> uploadStream(
                                  const std::string&           filePath,
                                  cbe::StreamId                streamId,
                                  delegate::ProgressEventFn&&  progressEventFn,
                                  UploadError&                 error);
  /**
   * Same as 
   * uploadStream(const std::string&,cbe::StreamId,delegate::ProgressEventFn&&,UploadError&)
   * , but without the parameter, \p progressEventFn.
   */
  cbe::util::Optional<cbe::Object> uploadStream(
                                  const std::string&           filePath,
                                  cbe::StreamId                streamId,
                                  UploadError&                 error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::UpdateKeyValuesDelegate that is passed into
   * asynchronous version of method
   * @ref updateKeyValues(UpdateKeyValuesDelegatePtr,KeyValues) "updateKeyValues()"
   */
using UpdateKeyValuesDelegatePtr = delegate::UpdateKeyValuesDelegatePtr;
  /**
   * @brief  Adds key/value pair data to the object.
   * 
   * <b>Asynchronous</b> version of this service function.
   * \note All existing key will be overwritten, new created. <br>
   * Any key name must start with a letter or _ <br>
   * The following key names are reserved and should not be used:
   * category, content, id, link and date <br>
   * Key names are case sensitive, hence variations with uppercase are permitted.
   * @param keyValues Map of key/value pairs (metadata).
   * @param delegate  Pointer to a delegate::UpdateKeyValuesDelegate instance
   *                  that is implemented by the user.
   */
  void updateKeyValues(
                       KeyValues                  keyValues,
                       UpdateKeyValuesDelegatePtr delegate);

  /**
   * @brief  Deletes all key/value pairs of data to the object.
   * 
   * Same as
   * updateKeyValues(KeyValues,UpdateKeyValuesDelegatePtr),
   * but without the \p keyValues parameter.
   * \note Any and all existing key/values will be erased.
   */
  void updateKeyValues(
                       UpdateKeyValuesDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
   * See delegate::object::UpdateKeyValuesDelegate::Exception
   */
  using UpdateKeyValuesException = delegate::UpdateKeyValuesDelegate::Exception;
  /**
   * @brief Synchronous [exception] Adds key/value pair data to the object.
   * 
   * <b>Synchronous</b> version of
   * updateKeyValues(KeyValues,UpdateKeyValuesDelegatePtr)
   * , and <b>throws an exception</b>, #UpdateKeyValuesException,
   * in case of a failed call.
   * <br>See updateKeyValues(UpdateKeyValuesDelegatePtr)
   *
   * @return Current object that was uploaded
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::Object "cbe::Object"
   * 
   * @throws #UpdateKeyValuesException
   */
  cbe::Object updateKeyValues(
                              KeyValues keyValues);
  /**
   * @brief Synchronous [exception] Deletes all key/value pairs of data
   *        to the object.
   * 
   * Same as
   * updateKeyValues(KeyValues), but without
   * the \p keyValues parameter.
   */
  cbe::Object updateKeyValues();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref updateKeyValues(KeyValues,UpdateKeyValuesError&) "updateKeyValues()"
   * <br>See delegate::UpdateKeyValuesDelegate::ErrorInfo
   */
  using UpdateKeyValuesError = delegate::UpdateKeyValuesDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] Adds key/value pair data to the object.
   * 
   * <b>Synchronous</b> version of
   * updateKeyValues(KeyValues,UpdateKeyValuesDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See updateKeyValues(KeyValues, UpdateKeyValuesDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #UpdateKeyValuesError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::Object>  updateKeyValues(
                                              KeyValues             keyValues,
                                              UpdateKeyValuesError& error);
  /**
   * @brief Synchronous [non-throwing] Deletes all key/value pairs of data
   *        to the object.
   * 
   * Same as
   * updateKeyValues(KeyValues,UpdateKeyValuesError&), but without
   * the \p keyValues parameter.
   */
  cbe::util::Optional<cbe::Object> updateKeyValues(
                                                  UpdateKeyValuesError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::GetStreamsDelegate that is passed
   * into asynchronous version of method
   * @ref getStreams(GetStreamsDelegatePtr) "getStream()"
   */
  using GetStreamsDelegatePtr = delegate::GetStreamsDelegatePtr;
  /**
   * @brief Downloads the streams meta data associated with current object to
   *        the SDK's cache.
   * 
   * <b>Asynchronous</b> version of this service function.
   *
   * The meta data i delivered as cbe::Streams via the delegate callback method
   * cbe::delegate::GetStreamsDelegate::onGetStreamsSuccess()<br>
   * Further, the actual stream data are retrieved through method
   * downloadStream(const std::string&,cbe::Stream,DownloadDelegatePtr).   
   * \note This method must be re-called if you upload more streams, 
   * see @c uploadStream()
   * @param delegate  Pointer to a delegate::GetStreamsDelegate instance
   *                  that is implemented by the user.
   *
   * @anchor anchorEx__cbe__Object__getStreams_async 
   * @par Example
   *      <b>Async</b> retrieving the Streams attached to a cbe::Object with the <b>%Object::getStreams()</b><br>
   *      Continuation of:
   *      @ref anchorEx__cbe__Container__upload_async
   *      "Async creation of a cbe::Object by using Container::upload()"  
   * @include example/getStreams_async.cpp
   * Continues at: @ref anchorEx__cbe__Object__uploadStream_async 
   *               "Async uploading data to a cbe::Object via a Stream with Object::uploadStream()"
   */
  void getStreams(
                  GetStreamsDelegatePtr delegate);
  /**
   * @brief Collection of @ref cbe::Stream "Stream" objects.
   * 
   * See cbe::Streams 
   */
  using Streams = cbe::Streams;
#ifndef CBE_NO_SYNC
  /**
   * See delegate::GetStreamsDelegate::Exception
   */
  using GetStreamsException = delegate::GetStreamsDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * getStreams(GetStreamsDelegatePtr)
   * , and <b>throws an exception</b>, #GetStreamException, in case of a failed
   * call.
   * <br>See getStreams(GetStreamsDelegatePtr)
   *
   * @return The streams attached to current object
   *         &mdash; if the call was successful.
   * 
   * @throws #GetStreamException
   *
   * @anchor anchorEx__cbe__Object__getStreams_syncExcept 
   * @par Example
   *      <b>Sync</b> [exception] retrieving the Streams attached to a cbe::Object with the <b>%Object::getStreams()</b><br>
   *      Continuation of:
   *      @ref anchorEx__cbe__Container__upload_syncExcept
   *      "Sync [exception] creation of a cbe::Object by using Container::upload()"  
   * @include example/getStreams_syncExcept.cpp
   * Continues at: @ref anchorEx__cbe__Object__uploadStream_syncExcept 
   *               "Sync [exception] uploading data to a cbe::Object via a Stream with Object::uploadStream()"
   */
  Streams getStreams();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * getStreams(GetStreamsError&)
   * <br>See delegate::GetStreamsDelegate::ErrorInfo
   */
  using GetStreamsError = delegate::GetStreamsDelegate::ErrorInfo;
  /**
   * <b>Synchronous</b> version of
   * getStreams(GetStreamsDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See getStreams(GetStreamsDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #GetStreamsError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   *
   * @anchor anchorEx__cbe__Object__getStreams_syncNoExcept 
   * @par Example
   *      <b>Sync</b> [non-throwing] retrieving the Streams attached to a cbe::Object with the <b>%Object::getStreams()</b><br>
   *      Continuation of:
   *      @ref anchorEx__cbe__Container__upload_syncNoExcept
   *      "Sync [non-throwing] creation of a cbe::Object by using Container::upload()"  
   * @include example/getStreams_syncNoThrow.cpp
   * Continues at: @ref anchorEx__cbe__Object__uploadStream_syncNoExcept 
   *               "Sync [non-throwing] uploading data to a cbe::Object via a Stream with Object::uploadStream()"
   */
  cbe::util::Optional<Streams> getStreams(
                                          GetStreamsError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * @brief Returns the mime type of the object.
   * 
   * E.g., application/pdf, audio/wav, image/jpg, text/xml, video/mp4 etc.
   */
  std::string getMimeType() const;

  /**
   * @brief Returns the binary length/size in bytes of current object.
   */
  uint64_t length() const;

  /**
   * @brief Returns the Object type.
   * 
   * See @ref cbe::ObjectType.
   */
  cbe::object_t getObjectType() const;

  /**
   * @brief Returns all the key/values for current object as a map.
   */
  cbe::KeyValues keyValues();
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::AclDelegate that is passed into
   * asynchronous version of method
   * @ref getAcl(GetAclDelegatePtr) "getAcl()"
   */  
  using GetAclDelegatePtr = delegate::AclDelegatePtr;
  /**
   * @brief Returns the Access Control List for current Object.
   * 
   * <b>Asynchronous</b> version of this service function.
   * 
   * @param delegate  Pointer to a delegate::AclDelegate instance
   *                  that is implemented by the user.
   */
  void getAcl(
              GetAclDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
   * See delegate::object::AclDelegate::Exception
   */
  using GetAclException = delegate::AclDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * getAcl(GetAclDelegatePtr)
   * , and <b>throws an exception</b>, #GetAclException, in case of a failed
   * call.
   * <br>See getAcl(GetAclDelegatePtr)
   *
   * @return Information about the object from which the ACL is fetched
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::AclSuccess "AclSuccess"
   * 
   * @throws #GetAclException
   */
  cbe::AclMap getAcl();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref getAcl(GetAclError&) "getAcl()"
   * <br>See delegate::AclDelegate::ErrorInfo
   */
  using GetAclError = delegate::AclDelegate::ErrorInfo;
  /**
   * <b>Synchronous</b> version of
   * getAcl(GetAclDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See getAcl(GetAclDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #GetAclError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::AclMap> getAcl(
                                          GetAclError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::AclDelegate that is passed into
   * asynchronous version of method
   * @ref setAcl(cbe::AclMap,SetAclDelegatePtr) "setAcl()"
   */  
  using SetAclDelegatePtr = delegate::AclDelegatePtr;
  /**
   * @brief Sets the Access Control List for current object.
   * 
   * <b>Asynchronous</b> version of this service function.
   * 
   * @param aclMap  The desired @ref cbe::Permissions "permission"
   *                for current object. 
   * @param delegate  Pointer to a delegate::AclDelegate instance that is
   *                  implemented by the user.
   */
  void setAcl(
              cbe::AclMap       aclMap,
              SetAclDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
   * See delegate::object::AclDelegate::Exception
   */
  using SetAclException = delegate::AclDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * setAcl(cbe::AclMap,SetAclDelegatePtr)
   * , and <b>throws an exception</b>, #SetAclException, in case of a failed
   * call.
   * <br>See setAcl(cbe::AclMap,SetAclDelegatePtr)
   *
   * @return Information about the object from which the ACL has been set
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::AclSuccess "AclSuccess"
   * 
   * @throws #SetAclException
   */
  cbe::AclMap setAcl(
                     cbe::AclMap aclMap);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref setAcl(cbe::AclMap,SetError&) "setAcl()"
   * <br>See delegate::AclDelegate::ErrorInfo
   */
  using SetAclError = delegate::AclDelegate::ErrorInfo;
  /**
   * <b>Synchronous</b> version of
   * setAcl(cbe::AclMap,SetAclDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See setAcl(cbe::AclMap,SetAclDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #SetAclError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::AclMap> setAcl(
                                    cbe::AclMap   aclMap,
                                    SetAclError&  error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::ShareDelegate that is passed into
   * asynchronous version of method
   * @ref share(cbe::UserId,std::string,ShareDelegatePtr) "share()"
   */
  using ShareDelegatePtr = delegate::ShareDelegatePtr;
  /**
   * @brief Share current object to a user.
   * 
   * <b>Asynchronous</b> version of this service function.
   * 
   * Notifies the user that a share has ocurred so that
   * the user can check what permissions the have been given. Sharing gives read 
   * permissions as of right now but might change in the future.
   * @param toUserGroup Takes a user id or group id (lastly named is for the
   *                    future) and share to.
   * @param description Names the specific share between you and the user/group.
   * @param delegate  Pointer to a delegate::ShareDelegatePtr instance
   *                  that is implemented by the user.
   */
  void share(
             cbe::UserId       toUserGroup,
             std::string       description,
             ShareDelegatePtr  delegate);
#ifndef CBE_NO_SYNC
  /**
   * See delegate::object::ShareDelegate::Exception
   */
  using ShareException = delegate::ShareDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * share(cbe::UserId,std::string,ShareDelegatePtr)
   * , and <b>throws an exception</b>, #ShareException, in case of a failed
   * call.
   * <br>See share(ShareDelegatePtr)
   *
   * @return Information about the id of the shared object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::ShareSuccess "ShareSuccess"
   * 
   * @throws #ShareException
   */
  delegate::ShareSuccess share(
                               cbe::UserId       toUserGroup,
                               std::string       description);
  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref share(cbe::UserId,std::string,ShareError&) "share()"
   * <br>See delegate::ShareDelegate::ErrorInfo
   */
  using ShareError = delegate::ShareDelegate::ErrorInfo;
  /**
   * <b>Synchronous</b> version of
   * share(cbe::UserId,std::string,ShareDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See share(cbe::UserId,std::string,ShareDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #ShareError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::ShareSuccess> share(
                                                    cbe::UserId  toUserGroup,
                                                    std::string  description,
                                                    ShareError&  error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::UnShareDelegate that is passed into
   * asynchronous version of method
   * @ref unShare(cbe::ShareId,UnShareDelegatePtr) "unShare()"
   */
  using UnShareDelegatePtr = delegate::UnShareDelegatePtr;
  /**
   * @anchor anchorMr__cbe__Object__unshareAsync
   * 
   * @brief Unshare the object to a specific shareId created when sharing.
   * 
   * <b>Asynchronous</b> version of this service function.
   * 
   * Each share is unique between user/group and the one sharing.
   * This is represented with a unique share id.
   * @param shareId The unique id for a share between the owner and other
   *                user/group.
   * @param delegate  Pointer to a delegate::UnShareDelegate instance
   *                  that is implemented by the user.
   * 
   */
  void unShare(
               cbe::ShareId shareId, 
               UnShareDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
   * See delegate::object::UnShareDelegate::Exception
   */
  using UnShareException = delegate::UnShareDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * unShare(cbe::ShareId,UnShareDelegatePtr)
   * , and <b>throws an exception</b>, #UnShareException, in case of a failed
   * call.
   * <br>See unShare(cbe::ShareId,UnShareDelegatePtr)
   *
   * @return Information about the unshared object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::UnShareSuccess "UnShareSuccess"
   * 
   * @throws #UnShareException
   */
  delegate::UnShareSuccess unShare(
                                   cbe::ShareId shareId);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref unShare(cbe::ShareId,UnShareError&) "unShare()"
   * <br>See delegate::UnShareDelegate::ErrorInfo
   */
  using UnShareError = delegate::UnShareDelegate::ErrorInfo;
  /**
   * <b>Synchronous</b> version of
   * unShare(cbe::ShareId,UnShareDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See unShare(cbe::ShareId,UnShareDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #UnShareError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::UnShareSuccess> unShare(
                                                        cbe::ShareId  shareId,
                                                        UnShareError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::PublishDelegate that is passed into
   * asynchronous version of method
   * @ref publish(cbe::PublishAccess,cbe::PublishVisibility,std::string,std::string,PublishDelegatePtr) "publish()"
   */
  using PublishDelegatePtr = delegate::PublishDelegatePtr;
  /**
   * @brief Publishes current object to any user.
   * 
   * <b>Asynchronous</b> version of this service function.
   * 
   * Can be revoked with unPublish()
   * 
   * @param security A cbe::PublishAccess enum
   * @param privacy  A cbe::WebShareVisibility enum
   * @param description Free text
   * @param password Password
   * @param delegate  Pointer to a delegate::PublishDelegate instance
   *                  that is implemented by the user.
   */
  void publish(
               cbe::PublishAccess      security,
               cbe::PublishVisibility  privacy,
               std::string             description,
               std::string             password,
               PublishDelegatePtr      delegate);
#ifndef CBE_NO_SYNC
  /**
   * See delegate::object::PublishDelegate::Exception
   */
  using PublishException = delegate::PublishDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * publish(cbe::PublishAccess,cbe::PublishVisibility,std::string,std::string,PublishDelegatePtr)
   * , and <b>throws an exception</b>, #PublishException, in case of a failed
   * call.
   * <br>See publish(cbe::PublishAccess,cbe::PublishVisibility,std::string,std::string,PublishDelegatePtr)
   *
   * @return Information about the published object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::PublishSuccess "PublishSuccess"
   *         \note To use this method, header file<br>
   *         <b>cbe/delegate/PublishSuccess.h</b><br>
   *         must be included
   * 
   * @throws #PublishException
   */
  delegate::PublishSuccess publish(
                              cbe::PublishAccess                    security,
                              cbe::PublishVisibility                privacy,
                              std::string                           description,
                              std::string                           password);
  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref publish(cbe::PublishAccess,cbe::PublishVisibility,std::string,std::string,PublishError&) "publish()"
   * <br>See delegate::PublishDelegate::ErrorInfo
   */
  using PublishError = delegate::PublishDelegate::ErrorInfo;
  /**
   * <b>Synchronous</b> version of
   * publish(cbe::PublishAccess,cbe::PublishVisibility,std::string,std::string,PublishDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See publish(cbe::PublishAccess,cbe::PublishVisibility,std::string,std::string,PublishDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #PublishError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   *         \note To use this method, header file<br>
   *         <b>cbe/delegate/PublishSuccess.h</b><br>
   *         must be included
   */
  cbe::util::Optional<delegate::PublishSuccess> publish(
                                            cbe::PublishAccess      security,
                                            cbe::PublishVisibility  privacy,
                                            std::string             description,
                                            std::string             password,
                                            PublishError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::UnPublishDelegate that is passed into
   * asynchronous version of method
   * @ref unPublish(UnPublishDelegatePtr) "unPublish()"
   */
  using UnPublishDelegatePtr = delegate::UnPublishDelegatePtr;
  /**
   * @brief UnPublishes current object.
   * 
   * <b>Asynchronous</b> version of this service function.
   * 
   * Revokes previous publish().
   * 
   * @param delegate  Pointer to a delegate::UnPublishDelegate instance
   *                  that is implemented by the user.
   */
  void unPublish(
                 UnPublishDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::UnPublishDelegate::Exception
  */
  using UnPublishException = delegate::UnPublishDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * unPublish(UnPublishDelegatePtr)
   * , and <b>throws an exception</b>, #UnPublishException, in case of a failed
   * call.
   * <br>See unPublish(UnPublishDelegatePtr)
   *
   * @return Information about the unpublished object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::UnPublishSuccess "UnPublishSuccess"
   * 
   * @throws #UnPublishException
   */
  delegate::UnPublishSuccess unPublish();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref unPublish(UnPublishError&) "unPublish()"
   * <br>See delegate::UnPublishDelegate::ErrorInfo
   */
  using UnPublishError = delegate::UnPublishDelegate::ErrorInfo;
  /**
   * <b>Synchronous</b> version of
   * unPublish(UnPublishDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See unPublish( UnPublishDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #UnPublishError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::UnPublishSuccess> unPublish(
                                                        UnPublishError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::UnSubscribeDelegate that is passed into
   * asynchronous version of method
   * @ref unSubscribe(UnSubscribeDelegatePtr) "unSubscribe()"
   */
  using UnSubscribeDelegatePtr = delegate::UnSubscribeDelegatePtr;
  /**
   * @brief UnSubscribes from this object.
   * 
   * <b>Asynchronous</b> version of this service function.
   * 
   * Revokes the subscription previously established with
   * cbe::SubscribeManager::subscribe() 
   * 
   * @param delegate  Pointer to a delegate::UnSubscribeDelegate instance
   *                  that is implemented by the user.
   */
  void unSubscribe(
                   UnSubscribeDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
   * See delegate::object::UnSubscribeDelegate::Exception
   */
  using UnSubscribeException = delegate::UnSubscribeDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * unSubscribe(UnSubscribeDelegatePtr)
   * , and <b>throws an exception</b>, #UnSubscribeException, in case of a failed
   * call.
   * <br>See unSubscribe(UnSubscribeDelegatePtr)
   *
   * @return Information about the unsubscribed object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::UnSubscribeSuccess "UnSubscribeSuccess"
   * 
   * @throws #UnSubscribeException
   */
  delegate::UnSubscribeSuccess unSubscribe();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref unSubscribe(UnSubscribeError&) "unSubscribe()"
   * <br>See delegate::UnSubscribeDelegate::ErrorInfo
   */
  using UnSubscribeError = delegate::UnSubscribeDelegate::ErrorInfo;
  /**
   * <b>Synchronous</b> version of
   * unSubscribe(UnSubscribeDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   *  parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See unSubscribe(UnSubscribeDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #UnSubscribeError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<delegate::UnSubscribeSuccess> unSubscribe(
                                                      UnSubscribeError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * @brief URL to current object.
   */ 
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
