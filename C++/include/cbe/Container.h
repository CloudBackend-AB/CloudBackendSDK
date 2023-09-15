/*
     Copyright © CloudBackend AB 2020 - 2023.
*/

#ifndef INCLUDE_CBE_CONTAINER_H_
#define INCLUDE_CBE_CONTAINER_H_

#include "cbe/Types.h"
#include "cbe/delegate/container/MoveDelegate.h"
#include "cbe/delegate/container/RenameDelegate.h"
#include "cbe/delegate/container/RemoveDelegate.h"
#include "cbe/delegate/AclDelegate.h"
#include "cbe/delegate/CreateContainerDelegate.h"
#include "cbe/delegate/CreateObjectDelegate.h"
#include "cbe/delegate/Error.h"
#include "cbe/delegate/PublishDelegate.h"
#include "cbe/delegate/PublishSuccess.h"
#include "cbe/delegate/ShareDelegate.h"
#include "cbe/delegate/UnPublishDelegate.h"
#include "cbe/delegate/UnShareDelegate.h"
#include "cbe/delegate/UnSubscribeDelegate.h"
#include "cbe/delegate/UploadDelegate.h"
#include "cbe/delegate/QueryDelegate.h"
#include "cbe/delegate/QueryJoinDelegate.h"

#ifndef CBE_NO_SYNC
#include "cbe/delegate/ProgressEventFn.h"
#endif

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"
#include "cbe/util/Optional.h"

#include "cbe/Item.h"
#include "cbe/Object.h"
#include "cbe/Types.h"

namespace CBI {
class Container;
using ContainerPtr = std::shared_ptr<CBI::Container>;
class ItemEventProtocol;
using ItemDelegatePtr = std::shared_ptr<ItemEventProtocol>;
} // namespace CBI

namespace cbe
{
class QueryChainExt;
class QueryChainSync;

namespace delegate {
  class AclDelegate;
  class ShareDelegate;
  class UnShareDelegate;
}

class Filter;

/**
 * @brief A collection of Item, can also represent a table or folder.
 */
class Container : public cbe::Item
{
public:
  /**
   * Pointer to cbe::delegate::CreateContainerDelegate that is passed into
   * asynchronous version of method
   * @ref createContainer(CreateContainerDelegatePtr) "createContainer()"
   */
  using CreateContainerDelegatePtr = delegate::CreateContainerDelegatePtr;
  /**
   * @brief Create a container.
   * 
   * Creates a container inside this container to be used for adding objects.
   * @param name Name of the container to be created.
   * @param delegate Pointer to a delegate::CreateContainerDelegate
   *                 instance that is implemented by the user.
   */
  cbe::Container createContainer(const std::string&         name, 
                                 CreateContainerDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
  * See delegate::container::CreateContainerDelegate::Exception
  */
  using CreateContainerException = delegate::CreateContainerDelegate::Exception;
  /**
   * @brief Synchronous [exception] createContainer
   * 
   * <b>Synchronous</b> version of
   * createContainer(const std::string&, CreateContainerDelegatePtr), 
   * and <b>throws an exception</b>, #CreateContainerException, in case of a 
   * failed call.
   * <br>See createContainer(CreateContainerDelegatePtr)
   *
   * @return Information about the createContainer object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::CreateContainerDelegate::Success " "
   * @throws #CreateContainerException
   */
  cbe::Container createContainer(const std::string& name);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref createContainer(const std::string&, CreateContainerError&) "createContainer()"
   * <br>See delegate::CreateContainerDelegate::ErrorInfo
   */
  using CreateContainerError = delegate::CreateContainerDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] createContainer
   * 
   * <b>Synchronous</b> version of
   * createContainer(const std::string&, CreateContainerDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See createContainer(const std::string& , CreateContainerDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #CreateContainerError object passed in will we be populated 
   *              with the error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::Container> createContainer(
                                              const std::string&    name,
                                              CreateContainerError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::MoveDelegate that is passed into
   * asynchronous version of method
   * @ref move(MoveDelegatePtr) "move()"
   */
  using MoveDelegatePtr = delegate::container::MoveDelegatePtr;
  /**
   * @brief Move a container to a different parent.
   * 
   * Used to move container with its content to user specified location 
   * e.g., other container or to root container.
   * @param dstId id of the container to which it should be moved to.
   * @param delegate Pointer to a delegate::container::MoveDelegate instance 
   *                 that is implemented by the user.
   */
  void move(cbe::ContainerId dstId, MoveDelegatePtr delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::MoveDelegate::Exception
  */
  using MoveException = delegate::container::MoveDelegate::Exception;
  /**
   * @brief Synchronous [exception] move
   * 
   * <b>Synchronous</b> version of
   * move(cbe::ContainerId, MoveDelegatePtr)
   * , and <b>throws an exception</b>, #MoveException, in case of a failed
   * call.
   * <br>See move(MoveDelegatePtr)
   *
   * @return Information about the moved object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::container::MoveDelegate::Success " "
   * @throws #MoveException
   */
  cbe::Container move(cbe::ContainerId dstId);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref move(dstId, MoveError&) "move()"
   * <br>See delegate::container::MoveDelegate::ErrorInfo
   */
  using MoveError = delegate::container::MoveDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] move
   * 
   * <b>Synchronous</b> version of
   * move(dstId, MoveDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See move(dstId, MoveDelegatePtr)
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
  cbe::util::Optional<cbe::Container> move(
                                              cbe::ContainerId     dstId,
                                              MoveError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::container::RenameDelegate that is passed into
   * asynchronous version of method
   * @ref rename(RenameDelegatePtr)   "rename()"
   */
  using RenameDelegatePtr = delegate::container::RenameDelegatePtr;
  /**
   * @brief Change the name of the container.
   * 
   * Rename the container.
   * @param name New name of the container.
   * @param delegate Pointer to a delegate::container::RenameDelegate instance
   *                 that is implemented by the user.
   */
  void rename(const std::string& name, RenameDelegatePtr delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::RenameDelegate::Exception
  */
  using RenameException = delegate::container::RenameDelegate::Exception;
  /**
   * @brief Synchronous [exception] rename
   * 
   * <b>Synchronous</b> version of
   * rename(const std::string&, RenameDelegatePtr)
   * , and <b>throws an exception</b>, #RenameException, in case of a failed
   * call.
   * <br>See rename(RenameDelegatePtr)
   *
   * @return Information about the renamed object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::container::RenameDelegate::Success " "
   * @throws #RenameException
   */
  cbe::Container rename(const std::string& name);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref rename(name, RenameError&) "rename()"
   * <br>See delegate::container::RenameDelegate::ErrorInfo
   */
  using RenameError = delegate::container::RenameDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] rename
   * 
   * <b>Synchronous</b> version of
   * rename(name, RenameDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See rename(name, RenameDelegatePtr)
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
  cbe::util::Optional<cbe::Container> rename(const std::string&     name,
                                             RenameError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::RemoveDelegate that is passed into
   * asynchronous version of method
   * @ref remove(RemoveDelegatePtr) "remove()"
   */
  using RemoveDelegatePtr = delegate::container::RemoveDelegatePtr;
  /**
   * @brief Delete the container.
   * 
   * Removes/deletes the container and all its content.
   * 
   * @param delegate Pointer to a delegate::container::RemoveDelegate instance
   *                 that is implemented by the user.
   */
  void remove(RemoveDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::RemoveDelegate::Exception
  */
  using RemoveException = delegate::container::RemoveDelegate::Exception;
  /**
   * @brief Synchronous [exception] remove/delete
   * 
   * <b>Synchronous</b> version of
   * remove(RemoveDelegatePtr)
   * , and <b>throws an exception</b>, #RemoveException, in case of a failed
   * call.
   * <br>See remove(RemoveDelegatePtr)
   *
   * @return Information about the removed object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::container::RemoveSuccess " "
   * @throws #RemoveException
   */
  delegate::container::RemoveSuccess remove();

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref remove(RemoveError&) "remove()"
   * <br>See delegate::RemoveDelegate::ErrorInfo
   */
  using RemoveError = delegate::container::RemoveDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] remove/delete
   * 
   * <b>Synchronous</b> version of
   * remove(RemoveDelegatePtr)
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
  cbe::util::Optional<delegate::container::RemoveSuccess> remove(
                                                            RemoveError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::CreateObjectDelegate that is passed into
   * asynchronous version of method
   * @ref createObject(CreateObjectDelegatePtr) "createObject()"
   */
  using CreateObjectDelegatePtr = delegate::CreateObjectDelegatePtr;
  /**
   * @brief Create a new object.
   * 
   * Creates an object with indexed tags or indexed tags + non indexed tags
   * a.k.a. metadata, key/value pairs.
   * @param name      Name of the object.
   * @param keyValues Optional map of key/value pairs (metadata).
   * @param delegate  Pointer to a delegate::CreateObjectDelegate instance
   *                  that is implemented by the user.
   * \note If an old object with the same name already exists
   * that will be removed.<br>
   * The object name may not contain characters < & : / <br>
   * Any key name must start with a letter or _ <br>
   * The following key names are reserved and should not be used:
   * category, content, id, link and date <br>
   * Key names are case sensitive, hence variations with uppercase are permitted.
   */
  cbe::Object createObject(std::string              name,
                           cbe::KeyValues           keyValues,
                           CreateObjectDelegatePtr  delegate);
  /**
   * Same as createObject(std::string, cbe::KeyValues, CreateObjectDelegatePtr),
   * but without the \p keyValues parameter.
   */
  cbe::Object createObject(std::string              name,
                           CreateObjectDelegatePtr  delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::CreateObjectDelegate::Exception
  */
  using CreateObjectException = delegate::CreateObjectDelegate::Exception;
  /**
   * @brief Synchronous [exception] createObject
   * 
   * <b>Synchronous</b> version of
   * createObject(std::string, cbe::KeyValues, CreateObjectDelegatePtr)
   * , and <b>throws an exception</b>, #CreateObjectException, in case of a 
   * failed call.
   * <br>See createObject(CreateObjectDelegatePtr)
   *
   * @return Information about the created object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::CreateContainerDelegate::Success " "
   * @throws #CreateObjectException
   */
  cbe::Object createObject(std::string              name,
                           cbe::KeyValues           keyValues);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref createObject(name, keyValues, CreateObjectError&) "createObject()"
   * <br>See delegate::CreateObjectDelegate::ErrorInfo
   */
  using CreateObjectError = delegate::CreateObjectDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] createObject
   * 
   * <b>Synchronous</b> version of
   * createObject(name, keyValues, CreateObjectDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See createObject(name, keyValues, CreateObjectDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #CreateObjectError object passed in will we be populated with 
   *              the error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::Object> createObject(
                                              std::string              name,
                                              cbe::KeyValues           keyValues,
                                              CreateObjectError&       error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::UploadDelegate that is passed into
   * asynchronous version of methods:
   * <ul>
   *   <li> upload(const std::string&,UploadDelegatePtr)
   *   <li> upload(const std::string&,const std::string&,UploadDelegatePtr)
   *   <li> upload(const std::string&,std::uint64_t,const char*,UploadDelegatePtr)
   * </ul>
   */
  using UploadDelegatePtr = delegate::UploadDelegatePtr;
  /**
   * @brief Upload object to container with file given by \p filePath.
   * 
   * See upload(const std::string&,const std::string& path,UploadDelegatePtr)
   * @param filePath  Fully qualified file name. I.e., the path, relative or
   *                  absolute, including file name.
   * 
   * @anchor anchorEx__cbe__Container__upload_async 
   * @par Example
   *      <b>Async</b> creation of a cbe::Object by using <b>%Container::upload()</b> 
   * @include example/upload_async.cpp
   * Continues at: @ref anchorEx__cbe__Object__getStreams_async 
   *               "Async retrieving the Stream attached to a cbe::Object with 
   *                the  Object::getStreams()"
   */
  cbe::Object upload(const std::string& filePath,
                     UploadDelegatePtr  delegate);

  /**
   * @brief Create an object in current container by uploading a file.
   * 
   * Object is named by \p name, residing at \p path.
   * 
   * The @ref cbe::Object "object" being created is instantly returned with 
   * a temporary id. When the response is retrieved from from the server,
   * via callback method
   * @ref cbe::delegate::UploadDelegate::onUploadSuccess() "onUploadSuccess()"
   * the object will be updated with its final unique id.
   * @param name      Name of local file name. The object, that is created, will
   *                  be given the same name.
   * @param path      Path to local folder where the file is located.
   *                  The can be relative or absolute and <b>must end with a
   *                  slash</b> (/).
   * @param delegate  Pointer to a delegate::UploadDelegate instance
   *                  that is implemented by the user.
   * @return  The created Object, first with its temporary id, and after
   *          successful with is final id.
   */
  cbe::Object upload(const std::string& name,
                     const std::string& path,
                     UploadDelegatePtr  delegate);

  /**
   * @brief Upload from local memory to an object.
   * 
   * @param name   name that the uploaded object will get
   * @param length size of file in Bytes
   * @param byteData (char pointer to an array containing the data).
   * @param delegate  Pointer to a delegate::UploadDelegate instance
   *                  that is implemented by the user.
   */
  cbe::Object upload(const std::string& name,
                     std::uint64_t      length,
                     const char*        byteData,
                     UploadDelegatePtr  delegate);
#ifndef CBE_NO_SYNC
  /**
   * The exception type thrown by the synchronous version of methods:
   * <ul>
   *   <li> upload(const std::string&,delegate::ProgressEventFn&&)
   *   <li> upload(const std::string&)
   *   <li> upload(const std::string&,const std::string&,delegate::ProgressEventFn&&)
   *   <li> upload(const std::string&,const std::string&)
   *   <li> upload(const std::string&,std::uint64_t,const char*,delegate::ProgressEventFn&&)
   *   <li> upload(const std::string&,std::uint64_t,const char*)
   * </ul>
   * See delegate::UploadDelegate::Exception
   */
  using UploadException = delegate::UploadDelegate::Exception;
  /**
   * @brief Synchronous [exception] upload.
   * 
   * <b>Synchronous</b> version of
   * upload(const std::string&,UploadDelegatePtr)
   * , and <b>throws an exception</b>, #UploadException, in case of a failed
   * call.
   * <br>See upload(const std::string&,UploadDelegatePtr)
   * 
   * @param progressEventFn Callback function that is called for each memory
   *                        chunk uploaded.<br>
   *                        The callback function will be executed the calling
   *                        thread's context.<br>
   *                        Also see cbe::delegate::ProgressEventFn.<br>
   *                        For the other parameters, see
   *                        upload(const std::string&,UploadDelegatePtr)
   *
   * @return The uploaded and created object
   *         &mdash; if the call was successful.
   * @throws #UploadException
   * 
   * @anchor anchorEx__cbe__Container__upload_syncExcept 
   * @par Example
   *      <b>Sync</b> [exception] creation of a cbe::Object by using 
   *      <b>%Container::upload()</b> 
   * @include example/upload_syncExcept.cpp
   * Continues at: @ref anchorEx__cbe__Object__getStreams_syncExcept 
   *               "Sync exception example of retrieving the Stream attached to 
   *                a cbe::Object with the  Object::getStreams()"
   */
  cbe::Object upload(
                    const std::string&          filePath,
                    delegate::ProgressEventFn&& progressEventFn);

  /**
   * @brief Synchronous [exception] upload.
   * 
   * Same as
   * upload(const std::string&,delegate::ProgressEventFn&&), but
   * without the parameter, \p progressEventFn.
   */
  cbe::Object upload(
                    const std::string& filePath);

  /**
   * @brief Synchronous [exception] upload with progress.
   * 
   * <b>Synchronous</b> version of
   * upload(const std::string&,const std::string&,UploadDelegatePtr)
   * , and <b>throws an exception</b>, #UploadException, in case of a failed
   * call.
   * <br>See upload(const std::string&,const std::string&,UploadDelegatePtr)
   * 
   * @param progressEventFn See upload(const std::string&,delegate::ProgressEventFn&&).<br>
   *                        For the other parameters, see
   *                        upload(const std::string&,UploadDelegatePtr)
   *
   * @return The uploaded and created object
   *         &mdash; if the call was successful.
   * @throws #UploadException
   */
  cbe::Object upload(
                    const std::string&          name,
                    const std::string&          path,
                    delegate::ProgressEventFn&& progressEventFn);
  /**
   * @brief Synchronous [exception] upload.
   * 
   * Same as
   * upload(const std::string&, const std::string&, delegate::ProgressEventFn&&, 
   *        UploadError&)
   * , but without the parameter, \p progressEventFn.
   */
  cbe::Object upload(
                    const std::string&          name,
                    const std::string&          path);

  /**
   * @brief Synchronous [exception] upload from memory with progress.
   * 
   * <b>Synchronous</b> version of
   * upload(const std::string&,std::uint64_t,const char*,UploadDelegatePtr)
   * , and <b>throws an exception</b>, #UploadException, in case of a failed
   * call.
   * <br>See upload(const std::string&,std::uint64_t,const char*,UploadDelegatePtr)
   * 
   * @param progressEventFn See upload(const std::string&,delegate::ProgressEventFn&&).<br>
   *                        For the other parameters, see
   *                        upload(const std::string&,UploadDelegatePtr)
   *
   * @return The uploaded and created object
   *         &mdash; if the call was successful.
   * @throws #UploadException
   */
  cbe::Object upload(
                    const std::string&          name,
                    std::uint64_t               length,
                    const char*                 byteData,
                    delegate::ProgressEventFn&& progressEventFn);
  /**
   * @brief Synchronous [exception] upload from memory.
   * 
   * Same as
   * upload(const std::string&,std::uint64_t,const char*,delegate::ProgressEventFn&&)
   * , but without the parameter, \p progressEventFn.
   */
  cbe::Object upload(
                    const std::string&          name,
                    std::uint64_t               length,
                    const char*                 byteData);
  /**
   * Forms the type of the \p error return parameter for the synchronous
   * versions of methods:
   * <ul>
   *   <li> upload(const std::string&,delegate::ProgressEventFn&&,UploadError&)
   *   <li> upload(const std::string&,UploadError&)
   *   <li> upload(const std::string&,const std::string&,delegate::ProgressEventFn&&,UploadError&)
   *   <li> upload(const std::string&,const std::string&,UploadError&)
   *   <li> upload(const std::string&,std::uint64_t,const char*,delegate::ProgressEventFn&&,UploadError&)
   *   <li> upload(const std::string&,std::uint64_t,const char*,UploadError&)
   * </ul>
   * See delegate::UploadDelegate::ErrorInfo
   */
  using UploadError = delegate::UploadDelegate::ErrorInfo;

  /**
   * @brief Synchronous [non-throwing] upload with progress.
   * 
   * Similar to synchronous method 
   * upload(const std::string&,delegate::ProgressEventFn&&)
   * , but <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See upload(const std::string&,delegate::ProgressEventFn&&)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #UploadError object passed in will we be populated with the
   *              error information.<br>
   *              For the other parameters, see
   *              upload(const std::string&,delegate::ProgressEventFn&&)
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   * 
   * @anchor anchorEx__cbe__Container__upload_syncNoExcept 
   * @par Example
   *      <b>Sync</b> [non-throwing] creation of a cbe::Object by using 
   *      <b>%Container::upload()</b> 
   * @include example/upload_syncNoThrow.cpp
   * Continues at: @ref anchorEx__cbe__Object__getStreams_syncNoExcept 
   *               "Sync [non-throwing] retrieving the Stream attached to a 
   *                cbe::Object with the  Object::getStreams()"
   */
  cbe::util::Optional<cbe::Object> upload(
                                    const std::string&          filePath,
                                    delegate::ProgressEventFn&& progressEventFn,
                                    UploadError&                error);
  /**
   * @brief Synchronous [non-throwing] upload.
   * 
   * Same as
   * upload(const std::string&,delegate::ProgressEventFn&&,UploadError&)
   * , but without the parameter, \p progressEventFn.
   */
  cbe::util::Optional<cbe::Object> upload(
                                    const std::string&          filePath,
                                    UploadError&                error);

  /**
   * @brief Synchronous [non-throwing] upload with progress.
   * 
   * Similar to synchronous method 
   * upload(const std::string&,const std::string&,delegate::ProgressEventFn&&)
   * , but <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See upload(const std::string&, const std::string&, 
   *                delegate::ProgressEventFn&&)
   *
   * @param[out] error See upload(const std::string&, delegate::ProgressEventFn&&,
   *                              UploadError&).<br>
   *                   For the other parameters, see
   *                   upload(const std::string&, const std::string&, 
   *                          delegate::ProgressEventFn&&)
   *
   * @return See upload(const std::string&, delegate::ProgressEventFn&&, 
   *                    UploadError&)
   */
  cbe::util::Optional<cbe::Object> upload(
                                    const std::string&          name,
                                    const std::string&          path,
                                    delegate::ProgressEventFn&& progressEventFn,
                                    UploadError&                error);
  /**
   * @brief Synchronous [non-throwing] upload.
   * 
   * Same as
   * upload(const std::string&, const std::string&, delegate::ProgressEventFn&&, 
   *        UploadError&)
   * , but without the parameter, \p progressEventFn.
   */
  cbe::util::Optional<cbe::Object> upload(
                                    const std::string&          name,
                                    const std::string&          path,
                                    UploadError&                error);

  /**
   * @brief Synchronous [non-throwing] upload from memory with progress.
   * 
   * Similar to synchronous method 
   * upload(const std::string&, std::uint64_t, const char*, delegate::ProgressEventFn&&)
   * , but <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See upload(const std::string&, std::uint64_t, const char*, 
   *                delegate::ProgressEventFn&&)
   *
   * @param[out] error See upload(const std::string&, delegate::ProgressEventFn&&, UploadError&).
   *
   * @return See upload(const std::string&, delegate::ProgressEventFn&&, UploadError&)
   */
  cbe::util::Optional<cbe::Object> upload(
                                    const std::string&          name,
                                    std::uint64_t               length,
                                    const char*                 byteData,
                                    delegate::ProgressEventFn&& progressEventFn,
                                    UploadError&                error);
  /**
   * @brief Synchronous [non-throwing] upload from memory.
   * 
   * Same as
   * upload(const std::string&, std::uint64_t, const char*, 
   *        delegate::ProgressEventFn&&, UploadError&)
   * , but without the parameter, \p progressEventFn.
   */
  cbe::util::Optional<cbe::Object> upload(
                                    const std::string&          name,
                                    std::uint64_t               length,
                                    const char*                 byteData,
                                    UploadError&                error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::QueryDelegatePtr that is passed into
   * asynchronous version of method
   * @ref query(QueryDelegatePtr) "query()"
   */
  using QueryDelegatePtr = delegate::QueryDelegatePtr;
  /**
   * @brief Select list of objects.
   * 
   * In line with function
   * CloudBackend::query(ContainerId, QueryDelegatePtr), but with
   * its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId, QueryDelegatePtr)
   */
  cbe::QueryChain query(QueryDelegatePtr  queryDelegate);
  /**
   * @brief Select list of objects using filter.
   * 
   * In line with function
   * CloudBackend::query(ContainerId, Filter, QueryDelegatePtr), but
   * with its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId, Filter, QueryDelegatePtr)
   */
  cbe::QueryChain query(Filter           filter,
                   QueryDelegatePtr delegate);
                   
  /**
   * Pointer to cbe::delegate::QueryJoinDelegate that is passed into
   * asynchronous version of method
   * @ref query(QueryJoinDelegatePtr) "query()"
   */
  using QueryJoinDelegatePtr = std::shared_ptr<delegate::QueryJoinDelegate>;
  /**
   * @brief Select list of objects, for join.
   * 
   * In line with function
   * CloudBackend::query(ContainerId, delegate::QueryJoinDelegatePtr), but with
   * its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId, delegate::QueryJoinDelegatePtr)
   */
  cbe::QueryChainExt query(delegate::QueryJoinDelegatePtr delegate);
  /**
   * @brief Select list of objects using filter, for join.
   * 
   * In line with function
   * CloudBackend::query(ContainerId, Filter, delegate::QueryJoinDelegatePtr),
   * but with its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId, Filter, delegate::QueryJoinDelegatePtr)
   */
  cbe::QueryChainExt query(Filter                          filter,
                      delegate::QueryJoinDelegatePtr  delegate);

#ifndef CBE_NO_SYNC
  /*! \see delegate::QueryDelegate::Exception */
  using QueryException = delegate::QueryDelegate::Exception;
  /**
   * @brief Synchronous [exception] Select list of objects.
   * 
   * In line with function
   * CloudBackend::query(ContainerId),
   * but with its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId)
   *
   * @throws #QueryException
   */
  cbe::QueryChainSync query();
  /**
   * @brief Synchronous [exception] Select list of objects using filter.
   * 
   * In line with function
   * CloudBackend::query(ContainerId, Filter),
   * but with its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId, Filter)
   *
   * @throws #QueryException
   */
  cbe::QueryChainSync query(Filter filter);

  /*! \see delegate::QueryJoinDelegate::ErrorInfo */
  using QueryJoinError = delegate::QueryJoinDelegate::ErrorInfo;

  /**
   * @brief Synchronous [non-throwing] Select list of objects, for join.
   * 
   * In line with function
   * CloudBackend::query(ContainerId, QueryJoinError&),
   * but with its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId, QueryJoinError&)
   */
  cbe::QueryChainSync query(QueryJoinError&  error);
  /**
   * @brief Synchronous [non-throwing] Select list of objects using filter, for join.
   * 
   * In line with function
   * CloudBackend::query(ContainerId, Filter, QueryJoinError&),
   * but with its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId, Filter, QueryJoinError&)
   */
  cbe::QueryChainSync query(Filter           filter,
                       QueryJoinError&  error);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /*! \see delegate::QueryDelegate::ErrorInfo */
  using QueryError = delegate::QueryDelegate::ErrorInfo;
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::queryWithPathDelegate that is passed into
   * asynchronous version of method
   * @ref queryWithPath(queryWithPathDelegatePtr) "queryWithPath()"
   */

  /**
   * @brief Select list of objects in hierarchy.
   * 
   * Queries the container with a given relative path, returns container with 
   * objects. \n 
   * E.g. /Documents/Pictures will return objects and subContainers for 
   * Pictures. \n 
   * \note .. or . path options are not available, 
   *       top down Paths in the container tree are available.
   * @param relativePath container path, e.g. /Documents/Pictures
   * @param delegate  Pointer to a delegate::QueryDelegate instance
   *                  that is implemented by the user.
   */
  cbe::QueryChain queryWithPath(std::string       relativePath,
                                QueryDelegatePtr  delegate);
#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::queryWithPathDelegate::Exception
  */
  using queryWithPathException = delegate::QueryDelegate::Exception;
  /**
   * @brief Synchronous [exception] queryWithPath.
   * 
   * <b>Synchronous</b> version of
   * queryWithPath(relativePath, queryWithPathDelegatePtr)
   * , and <b>throws an exception</b>, #queryWithPathException, in case of a 
   * failed call.
   * <br>See queryWithPath(queryWithPathDelegatePtr)
   *
   * @return Information about the query
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::queryWithPathSuccess " "
   * @throws #queryWithPathException
   */
  cbe::QueryResult queryWithPath(std::string relativePath);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref queryWithPath(relativePath, queryWithPathError&) "queryWithPath()"
   * <br>See delegate::queryWithPathDelegate::ErrorInfo
   */
  using queryWithPathError = delegate::QueryDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] queryWithPath.
   * 
   * <b>Synchronous</b> version of
   * queryWithPath(relativePath, queryWithPathDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See queryWithPath(relativePath, queryWithPathDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #queryWithPathError object passed in will we be populated with 
   *              the error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::QueryResult> queryWithPath(
                                              std::string         relativePath,
                                              queryWithPathError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::QueryDelegate that is passed into
   * asynchronous version of method
   * @ref search(std::string, QueryDelegatePtr) "search()"
   */
  /**
   * @brief Search by tags.
   * 
   * Search the whole container for tags related to Objects in the container 
   * structure. \n 
   * E.g. Key = Name, Value Contract/Object/Song => Name:Contract1.
   *
   * Search handles tags in combination of conjunctions of keys and/or key 
   * values separated by |. \n 
   * E.g. Name:*|Country:Sweden|Country:Norway, this would search for objects 
   * with key Name but any value and where key Country is either Sweden or 
   * Norway.
   * @param tags is a string of key tags or key:value pairs 
   *             that are separated by |.
   * @param delegate  Pointer to a delegate::QueryDelegate instance
   *                  that is implemented by the user.
  */
  cbe::QueryResult search(std::string tags, QueryDelegatePtr delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::container::QueryDelegate::Exception
  */
  using SearchException = delegate::QueryDelegate::Exception;
  /**
   * @brief Synchronous [exception] search.
   * 
   * <b>Synchronous</b> version of
   * search(std::string, QueryDelegatePtr)
   * , and <b>throws an exception</b>, #SearchException, in case of a failed
   * call.
   * <br>See search(QueryDelegatePtr)
   *
   * @return Information about the search 
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::cbe::QueryResult " "
   * @throws #SearchException
   */
  cbe::QueryResult search(std::string tags);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref search(tags, SearchError&) "search()"
   * <br>See delegate::QueryDelegate::ErrorInfo
   */
  using SearchError = delegate::QueryDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] search.
   * 
   * <b>Synchronous</b> version of
   * search(tags, QueryDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See search(tags, QueryDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #SearchError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::QueryResult> search(std::string  tags,
                                               SearchError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * @brief Search using filter.
   * 
   * Search the whole container for tags related to Objects in the container 
   * structure. \n 
   * E.g. Key = Name, Value Contract/Object/Song => Name:Contract1.
   *
   * Search handles tags in combination / conjunction of keys and/or key values 
   * separated by |. \n 
   * E.g. Name:*|Country:Sweden|Country:Norway, this would search for objects 
   * with key Name but any value and where key Country is either Sweden or 
   * Norway.
   * @param filter is a cbe::Filter on which you can set 
   * how you want data to be ordered when searching. \n 
   * Remember to set the queryString to be keys/tags 
   * or key:value pairs that are separated by |.
   * @param delegate  Pointer to a delegate::QueryDelegate instance
   *                  that is implemented by the user.
  */
  cbe::QueryResult search(cbe::Filter filter, QueryDelegatePtr delegate);
  /**
   * Pointer to cbe::delegate::SearchDelegate that is passed into
   * asynchronous version of method
   * @ref search(cbe::Filter, SearchDelegatePtr) "search()"
   */

#ifndef CBE_NO_SYNC
  /**
   * @brief Synchronous [exception] search using filter.
   * 
   * <b>Synchronous</b> version of
   * search(cbe::Filter, SearchDelegatePtr)
   * , and <b>throws an exception</b>, #SearchException, in case of a failed
   * call.
   * <br>See search(cbe::Filter, SearchDelegatePtr)
   *
   * @return Information about the searched object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::cbe::QueryResult " "
   * @throws #SearchException
   */
  cbe::QueryResult search(cbe::Filter);
  
  /**
   * @brief Synchronous [non-throwing] search.
   * 
   * <b>Synchronous</b> version of
   * search(filter, SearchDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See search(filter, SearchDelegatePtr)
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #SearchError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  cbe::util::Optional<cbe::QueryResult> search(cbe::Filter  filter,
                                               SearchError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::AclDelegate that is passed into
   * asynchronous version of method
   * @ref setAcl(SetAclDelegatePtr) "setAcl()"
   */
  using SetAclDelegatePtr = delegate::AclDelegatePtr;
  /**
   * @brief set ACL.
   * 
   * Set the Access Control List \c ACL for the container. 
   * For containers set does set the whole container tree, 
   * so all its sub items as well. 
   * Remember this is  set and not update so every time you set 
   * all ids that should be there should be added.
   * @param aclMap    The desired @ref cbe::Permissions "permissions"
   *                  for current container. 
   * @param delegate  Pointer to a delegate::AclDelegate instance
   *                  that is implemented by the user.
   */
  void setAcl(cbe::AclMap       aclMap,
              SetAclDelegatePtr delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::AclDelegate::Exception
  */
  using SetAclException = delegate::AclDelegate::Exception;
  /**
   * @brief Synchronous [exception] setAcl.
   * 
   * <b>Synchronous</b> version of
   * setAcl(cbe::AclMap, SetAclDelegatePtr)
   * , and <b>throws an exception</b>, #SetAclException, in case of a failed
   * call.
   * <br>See setAcl(SetAclDelegatePtr)
   *
   * @return Information about the shared object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::AclDelegate::Success " "
   * @throws #SetAclException
   */
  cbe::AclMap setAcl(cbe::AclMap aclMap);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref setAcl(aclMap, SetAclError&) "setAcl()"
   * <br>See delegate::AclDelegate::ErrorInfo
   */
  using SetAclError = delegate::AclDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] setAcl.
   * 
   * <b>Synchronous</b> version of
   * setAcl(aclMap, SetAclDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See setAcl(aclMap, SetAclDelegatePtr)
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
                                                            cbe::AclMap  aclMap,
                                                            SetAclError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::AclDelegate that is passed into
   * asynchronous version of method
   * @ref getAcl(GetAclDelegatePtr) "getAcl()"
   */
  using GetAclDelegatePtr = delegate::AclDelegatePtr;
  /**
   * @brief Retrieves its ACL map.
   * 
   * get the Access Control List \c ACL of the Container.
   * @param delegate  Pointer to a delegate::AclDelegate instance
   *                  that is implemented by the user.
   */
  void getAcl(GetAclDelegatePtr delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::container::AclDelegate::Exception
  */
  using GetAclException = delegate::AclDelegate::Exception;
  /**
   * @brief Synchronous [exception] getAcl.
   * 
   * <b>Synchronous</b> version of
   * getAcl(GetAclDelegatePtr)
   * , and <b>throws an exception</b>, #GetAclException, in case of a failed
   * call.
   * <br>See getAcl(GetAclDelegatePtr)
   *
   * @return Information about the shared object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::AclDelegate::Success " "
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
   * @brief Synchronous [non-throwing] getAcl.
   * 
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
  cbe::util::Optional<cbe::AclMap> getAcl(GetAclError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::ShareDelegate that is passed into
   * asynchronous version of method
   * @ref share(ShareDelegatePtr) "share()"
   */
  using ShareDelegatePtr = delegate::ShareDelegatePtr;
  /**
   * @brief Make accessible by other user.
   * 
   * Shares a container and its content to a user.  This provides the user the 
   * ability to access what has been shared to them via the listAvailableShares
   * command.  To allow users to view and change shared information see \c ACL .
   * \note At present Sharing the container gives the user read permissions for
   * the container and all its sub-items, this might change in the future.
   * @param toUserGroup takes a user id or group id to share to.
   * @param description names the specific share between you and the user/group.
   * @param delegate  Pointer to a delegate::ShareDelegate instance
   *                  that is implemented by the user.
   */
  void share(cbe::UserId      toUserGroup,
             std::string      description,
             ShareDelegatePtr delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::container::ShareDelegate::Exception
  */
  using ShareException = delegate::ShareDelegate::Exception;
  /**
   * @brief Synchronous [exception] share.
   * 
   * <b>Synchronous</b> version of
   * share(cbe::UserId, std::string, ShareDelegatePtr)
   * , and <b>throws an exception</b>, #ShareException, in case of a failed
   * call.
   * <br>See share(ShareDelegatePtr)
   *
   * @return Information about the shared container
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::ShareDelegate::Success " "
   * @throws #ShareException
   */
  delegate::ShareDelegate::Success share(cbe::UserId      toUserGroup,
                                         std::string      description);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref share(toUserGroup, description, ShareError&) "share()"
   * <br>See delegate::ShareDelegate::ErrorInfo
   */
  using ShareError = delegate::ShareDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] share.
   * 
   * <b>Synchronous</b> version of
   * share(toUserGroup, description, ShareDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See share(toUserGroup, description, ShareDelegatePtr)
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
  cbe::util::Optional<delegate::ShareDelegate::Success> share(
                                                    cbe::UserId    toUserGroup,
                                                    std::string    description,
                                                    ShareError&    error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::UnShareDelegate that is passed into
   * asynchronous version of method
   * @ref unShare(UnShareDelegatePtr) "unShare()"
   */
  using UnShareDelegatePtr = std::shared_ptr<delegate::UnShareDelegate>;
  /**
   * @brief Revoke a previous share.
   * 
   * unShare the container to a specific shareId created when sharing. Each 
   * share is unique between user/group and the one sharing. This is represented
   * with a unique share id.
   * @param shareId is as mentioned the unique id for a share between the owner 
   * and other user/group.
   * @param delegate  Pointer to a delegate::UnShareDelegate instance
   *                  that is implemented by the user.
   */
  void unShare(cbe::ShareId shareId, UnShareDelegatePtr delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::UnShareDelegate::Exception
  */
  using UnShareException = delegate::UnShareDelegate::Exception;
  /**
   * @brief Synchronous [exception] unShare.
   * 
   * <b>Synchronous</b> version of
   * unShare(UnShareDelegatePtr)
   * , and <b>throws an exception</b>, #UnShareException, in case of a failed
   * call.
   * <br>See unShare(UnShareDelegatePtr)
   *
   * @return Information about the unshared object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::UnShareSuccess " "
   * @throws #UnShareException
   */
  delegate::UnShareDelegate::Success unShare(cbe::ShareId shareId);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref unShare(UnShareError&) "unShare()"
   * <br>See delegate::UnShareDelegate::ErrorInfo
   */
  using UnShareError = delegate::UnShareDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] unShare.
   * 
   * <b>Synchronous</b> version of
   * unShare(UnShareDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See unShare(UnShareDelegatePtr)
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
  cbe::util::Optional<delegate::UnShareDelegate::Success> unShare(
                                                        cbe::ShareId  shareId, 
                                                        UnShareError& error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Pointer to cbe::delegate::PublishDelegate that is passed into
   * asynchronous version of method
   * @ref publish(PublishDelegatePtr) "publish()"
   */
  using PublishDelegatePtr = delegate::PublishDelegatePtr;
  /**
   * @brief Publishes a container and its content to any user.
   * 
   * <b>Asynchronous</b> version of this service function.
   * 
   * Can be revoked with unPublish()
   * 
   * @param security A cbe::PublishAccess enum
   * @param privacy  A cbe::PublishVisibility enum
   * @param description Free text
   * @param password Password
   * @param delegate  Pointer to a delegate::PublishDelegate instance
   *                  that is implemented by the user.
   */
  void publish(cbe::PublishAccess      security,
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
   * @brief Synchronous [exception] publish.
   * 
   * <b>Synchronous</b> version of
   * publish(cbe::PublishAccess, cbe::PublishVisibility, std::string,
   *         std::string, PublishDelegatePtr)
   * , and <b>throws an exception</b>, #PublishException, in case of a failed
   * call.
   * <br>See publish(PublishDelegatePtr)
   *
   * @return Information about the published object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::PublishSuccess " "
   * @throws #PublishException
   */
  delegate::PublishSuccess publish(cbe::PublishAccess      security,
                                   cbe::PublishVisibility  privacy,
                                   std::string             description,
                                   std::string             password);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref publish(security, privacy, description, password, PublishError&) 
   * "publish()"
   * <br>See delegate::PublishDelegate::ErrorInfo
   */
  using PublishError = delegate::PublishDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] publish.
   * 
   * <b>Synchronous</b> version of
   * publish(security, privacy, description, password, PublishDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See publish(security, privacy, description, password, 
   *                 PublishDelegatePtr)
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
   * Pointer to cbe::delegate::UnPublishDelegatePtr that is passed into
   * asynchronous version of method
   * @ref unPublish(UnPublishDelegatePtr) "unPublish()"
   */
  using UnPublishDelegatePtr = delegate::UnPublishDelegatePtr;
  /**
   * @brief UnPublishes this container.
   * 
   * <b>Asynchronous</b> version of this service function.
   * 
   * Revokes previous publish().
   * 
   * @param delegate Gets notified when the container has been 
   * unPublished (or if there was an error)
   */    
  void unPublish(UnPublishDelegatePtr delegate);
  /**
   * Pointer to cbe::delegate::UnPublishDelegate that is passed into
   * asynchronous version of method
   * @ref unPublish(UnPublishDelegatePtr)   "unPublish()"
   */

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::UnPublishDelegate::Exception
  */
  using UnPublishException = delegate::UnPublishDelegate::Exception;
  /**
   * @brief Synchronous [exception] unPublish.
   * 
   * <b>Synchronous</b> version of
   * unPublish(UnPublishDelegatePtr)
   * , and <b>throws an exception</b>, #UnPublishException, in case of a failed
   * call.
   * <br>See unPublish(UnPublishDelegatePtr)
   *
   * @return Information about the unPublished object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::UnPublishSuccess " "
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
   * @brief Synchronous [non-throwing] unPublish.
   * 
   * <b>Synchronous</b> version of
   * unPublish(UnPublishDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See unPublish(UnPublishDelegatePtr)
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
   * @brief UnSubscribes from this container.
   * 
   * <b>Asynchronous</b> version of this service function.
   * 
   * Revokes the subscription previously established with
   * cbe::SubscribeManager::subscribe() 
   * 
   * @param delegate Gets notified when the container has been 
   * unSubscribed (or if there was an error)
   */
  void unSubscribe(UnSubscribeDelegatePtr delegate);

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::UnSubscribeDelegate::Exception
  */
  using UnSubscribeException = delegate::UnSubscribeDelegate::Exception;
  /**
   * @brief Synchronous [exception] unSubscribe.
   * 
   * <b>Synchronous</b> version of
   * unSubscribe(UnSubscribeDelegatePtr)
   * , and <b>throws an exception</b>, #UnSubscribeException, in case of a failed
   * call.
   * <br>See unSubscribe(UnSubscribeDelegatePtr)
   *
   * @return Information about the unSubscribe object
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::delegate::UnSubscribeSuccess " "
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
   * @brief Synchronous [non-throwing] unSubscribe.
   * 
   * <b>Synchronous</b> version of
   * unSubscribe(UnSubscribeDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
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
  Container(cbe::DefaultCtor);
private:
  struct Impl;
  Impl& getImpl() const;

  friend class Account;
  friend class CloudBackend;
  friend class Database;
  friend class Filter;
  friend class Group;
  friend class QueryChain;

  Container(CBI::ContainerPtr cbiPeerPtr);
 // CBI::Container&   getCbiPeer() const;
  CBI::ContainerPtr getCbiPeerPtr() const;
}; // class Container

} // namespace cbe

#endif // INCLUDE_CBE_CONTAINER_H_
