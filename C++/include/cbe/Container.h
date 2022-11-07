/*
     Copyright © CloudBackend AB 2020 - 2022.
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
#include "cbe/delegate/ShareDelegate.h"
#include "cbe/delegate/UnpublishDelegate.h"
#include "cbe/delegate/UnShareDelegate.h"
#include "cbe/delegate/UnsubscribeDelegate.h"
#include "cbe/delegate/UploadDelegate.h"
#include "cbe/delegate/QueryDelegate.h"
#include "cbe/delegate/QueryJoinDelegate.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

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
 * @brief collection of Item, can represent a table.
 * 
 */
class Container : public cbe::Item
{
public:
  using CreateContainerDelegatePtr = delegate::CreateContainerDelegatePtr;
  /**
   * Creates a container inside this container to be used for adding objects.
   * @param name Name of the container to be created.
   * @param delegate Pointer to a delegate::CreateContainerDelegate instance that is
   *                 implemented by the user.
   */
  cbe::Container createContainer(const std::string&         name, 
                                 CreateContainerDelegatePtr delegate);

  using MoveDelegatePtr = delegate::container::MoveDelegatePtr;
  /**
   * Move is used to move container with its content to user specified location 
   * e.g. other container or to root container.
   * @param destinationId: id of the container to which it should be moved to.
   * @param delegate Pointer to a delegate::container::MoveDelegate instance 
   *                 that is implemented by the user.
   */
  void move(cbe::ContainerId    destinationId, MoveDelegatePtr delegate);

  using RenameDelegatePtr = delegate::container::RenameDelegatePtr;
  /**
   * Rename the container.
   * @param name New name of the container.
   * @param delegate Pointer to a delegate::container::RenameDelegate instance
   *                 that is implemented by the user.
   */
  void rename(const std::string& name, RenameDelegatePtr delegate);

  using RemoveDelegatePtr = delegate::container::RemoveDelegatePtr;
  /**
   * Removes/deletes the container and all its content.
   * @param delegate Pointer to a delegate::container::RemoveDelegate instance
   *                 that is implemented by the user.
   */
  void remove(RemoveDelegatePtr delegate);

  using CreateObjectDelegatePtr = delegate::CreateObjectDelegatePtr;
  /**
   * Creates an object with indexed tags or indexed tags + non indexed tags.
   * @param name      Name of the object.
   * @param delegate  Pointer to a delegate::CreateObjectDelegate instance
   *                  that is implemented by the user.
   * @param keyValues Optional map of key/value pairs (metadata).
   */
  cbe::Object createObject(std::string              name,
                           CreateObjectDelegatePtr  delegate,
                           cbe::KeyValues           keyValues =
                                                      cbe::KeyValues{});

  using UploadDelegatePtr = delegate::UploadDelegatePtr;
  /**
   * @brief Create an object in current container by uploading a file, named by
   *        \p name, residing at \p path.
   * 
   * The @ref cbe::Object "object" being created is instantly returned with 
   * a temporary id. When the response is retrieved from from the server,
   * via callback method
   * @ref cbe::delegate::UploadDelegate::onUploadSuccess() "onUploadSuccess()" the
   * object will be updated with its final unique id.
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
   * @brief Upload object to container with file given by \p filePath.
   * 
   * See upload(const std::string&,const std::string& path,UploadDelegatePtr)
   * @param filePath  Fully qualified file name. I.e., the path, relative or
   *                  absolute, including file name.
   * 
   * @anchor anchorEx__cbe__Container__upload 
   * @par Example of creation of a cbe::Object by using Container::upload() 
   * @code
  #include "cbe/Object.h"
  #include "cbe/Container.h"

  #include "cbe/delegate/UploadDelegate.h"

  #include <condition_variable>
  #include <fstream>            // std::ofstream
  #include <mutex>
  ~~~
  struct MyUploadDelegate : cbe::delegate::UploadDelegate {
    std::mutex              mutex{};
    std::condition_variable conditionVariable{};
    // Indicates operation completed - success or failure
    bool                    called{};

    // Default construct the result object. If the method
    // onUploadSuccess() has not been called, this default constructed state
    // implies that the object is not valid
    cbe::Object object{ cbe::DefaultCtor{} };

    void onUploadSuccess(cbe::Object&& object) final {
      {
        std::lock_guard<std::mutex> lock{mutex};
        // Change state of object member to indicate success
        this->object = std::move(object);
        // Indicate operation completed, member object indicates success
        called = true;
      }
      conditionVariable.notify_one();
    }

    void onUploadError(cbe::delegate::TransferError&& transferError, 
                      cbe::util::Context&&           context) final {
      {
        std::lock_guard<std::mutex> lock{mutex};
        // Put object into the default constructed state to indicate no-success
        object = cbe::Object{ cbe::DefaultCtor{} };
        // Indicate operation completed, member object indicates failure 
        called = true;
      }
      conditionVariable.notify_one();
    }

    cbe::Object wait() {
      std::unique_lock<std::mutex> lock{mutex};
      conditionVariable.wait(lock, [this]{ return called; });
      // Reset called flag, so current delegate instance can be reused
      called = false;
      // If object is still in its default constructed state, this implies a
      // failed cbe::object::upload() operation                 
      return object;                      
    }
  }; // struct MyUploadDelegate
  ~~~
  constexpr const char* const uploadPath = "/tmp/upload/";
  constexpr const char* const myObjectFileName = "myObject";
  const std::string           qualFile1Name =
                                std::string{uploadPath} + myObjectFileName;
  std::ofstream ofs{qualFile1Name};
  ofs << "Line 11\n"  << "Line 12\n" << "Line 13\n" << std::flush;
  ofs.close();
  // Access container previously created with cbe::Container::createContainer()  
  cbe::Container myContainer = ~~~;
  std::shared_ptr<MyUploadDelegate> myUploadDelegate =
                                      std::make_shared<MyUploadDelegate>();
  // Create an object named after file name in variable qualFile1Name 
  myContainer.upload(qualFile1Name, myUploadDelegate);
  // Continuously, use the Object instance passed into the delegate.  
  cbe::Object object = myUploadDelegate->wait();
  // Check if upload was successful
  if (!object) {
    // Not, bail out
    ~~~
  }
  ~~~
   * @endcode
   * Continues at: @ref anchorEx__cbe__Object__getStreams 
   *               "Example of retrieving the Stream attached to a cbe::Object with Object::getStreams()"

   */
  cbe::Object upload(const std::string& filePath,
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

  using QueryDelegatePtr = delegate::QueryDelegatePtr;
  /**
   * In line with function
   * CloudBackend::query(ContainerId,QueryDelegatePtr), but with
   * its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId,QueryDelegatePtr)
   */
  QueryChain query(QueryDelegatePtr  queryDelegate);
  /**
   * In line with function
   * CloudBackend::query(ContainerId,Filter,QueryDelegatePtr), but
   * with its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId,Filter,QueryDelegatePtr)
   */
  QueryChain query(Filter           filter,
                   QueryDelegatePtr delegate);
                   
  using QueryJoinDelegatePtr = std::shared_ptr<delegate::QueryJoinDelegate>;
  /**
   * In line with function
   * CloudBackend::query(ContainerId,delegate::QueryJoinDelegatePtr), but with
   * its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId,delegate::QueryJoinDelegatePtr)
   */
  QueryChainExt query(delegate::QueryJoinDelegatePtr delegate);
  /**
   * In line with function
   * CloudBackend::query(ContainerId,Filter,delegate::QueryJoinDelegatePtr),
   * but with its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId,Filter,delegate::QueryJoinDelegatePtr)
   */
  QueryChainExt query(Filter                          filter,
                      delegate::QueryJoinDelegatePtr  delegate);

#ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /*! \see delegate::QueryDelegate::Exception */
  using QueryException = delegate::QueryDelegate::Exception;
  /**
   * In line with function
   * CloudBackend::query(ContainerId),
   * but with its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId)
   *
   * @throws #QueryException
   */
  QueryChainSync query();
  /**
   * In line with function
   * CloudBackend::query(ContainerId,Filter),
   * but with its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId,Filter)
   *
   * @throws #QueryException
   */
  QueryChainSync query(Filter filter);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /*! \see delegate::QueryJoinDelegate::ErrorInfo */
  using QueryJoinError = delegate::QueryJoinDelegate::ErrorInfo;

  /**
   * In line with function
   * CloudBackend::query(ContainerId,QueryJoinError&),
   * but with its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId,QueryJoinError&)
   */
  QueryChainSync query(QueryJoinError&  error);
  /**
   * In line with function
   * CloudBackend::query(ContainerId,Filter,QueryJoinError&),
   * but with its \p ContainerId parameter excluded.
   * \see CloudBackend::query(ContainerId,Filter,QueryJoinError&)
   */
  QueryChainSync query(Filter           filter,
                       QueryJoinError&  error);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /*! \see delegate::QueryDelegate::ErrorInfo */
  using QueryError = delegate::QueryDelegate::ErrorInfo;
#endif // #ifndef CBE_NO_SYNC

  /**
   * Queries the container with a given relative path, returns container with 
   * objects. \n 
   * E.g. /Documents/Pictures will return objects and subContainers for 
   * Pictures. \n 
   * \note .. or . path options are not available, 
   *       top down Paths in the container tree are available.
   * @param path container path, e.g. /Documents/Pictures
   * @param delegate  Pointer to a delegate::QueryDelegate instance
   *                  that is implemented by the user.
   */
  cbe::QueryChain queryWithPath(std::string       relativePath,
                                QueryDelegatePtr  delegate);

  /**
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

  /**
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

  
  using SetAclDelegatePtr = delegate::AclDelegatePtr;
  /**
   * Set the Access Control List \c ACL for the container. 
   * For containers set does set the whole container tree, 
   * so all its sub items as well. 
   * Remember this is  set and not update so every time you set 
   * all ids that should be there should be added.
   * @param aclMap    The desired @ref cbe::Permission "permission" for current
   *                  container. 
   * @param delegate  Pointer to a delegate::AclDelegate instance
   *                  that is implemented by the user.
   */
  void setAcl(cbe::AclMap       aclMap,
              SetAclDelegatePtr delegate);

  using GetAclDelegatePtr = delegate::AclDelegatePtr;
  /**
   * get the Access Control List \c ACL of the Container.
   * @param delegate  Pointer to a delegate::AclDelegate instance
   *                  that is implemented by the user.
   */
  void getAcl(GetAclDelegatePtr delegate);

  using ShareDelegatePtr = delegate::ShareDelegatePtr;
  /**
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

  using UnShareDelegatePtr = std::shared_ptr<delegate::UnShareDelegate>;
  /**
   * unShare the container to a specific shareId created when sharing. Each 
   * share is unique between user/group and the one sharing. This is represented
   * with a unique share id.
   * @param shareId is as mentioned the unique id for a share between the owner 
   * and other user/group.
   * @param delegate  Pointer to a delegate::UnShareDelegate instance
   *                  that is implemented by the user.
   */
  void unShare(cbe::ShareId shareId, UnShareDelegatePtr delegate);

  using PublishDelegatePtr = delegate::PublishDelegatePtr;
  /**
   * Publishes a container and its content to a user.
   * @param security A cbe::Access enum
   * @param privacy A cbe::PublishVisibility enum
   * @param description Description
   * @param password Password
   * @param delegate  Pointer to a delegate::PublishDelegate instance
   *                  that is implemented by the user.
   */
  void publish(cbe::PublishAccess      security,
               cbe::PublishVisibility  privacy,
               std::string             description,
               std::string             password,
               PublishDelegatePtr      delegate);

  using UnpublishDelegatePtr = delegate::UnpublishDelegatePtr;
  /**
   * Unpublishes this container.
   * @param delegate Gets notified when the container has been 
   * unpublished (or if there was an error)
   */    
  void unpublish(UnpublishDelegatePtr delegate);

  using UnsubscribeDelegatePtr = delegate::UnsubscribeDelegatePtr;
  /**
   * Unsubscribes from this container.
   * @param delegate Gets notified when the container has been 
   * unsubscribed (or if there was an error)
   */
  void unsubscribe(UnsubscribeDelegatePtr delegate);

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
