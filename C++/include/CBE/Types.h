/*
  SDK type definitions.
  Copyright © CloudBackend AB 2020 - 2022.-2022.
*/
#ifndef INCLUDE_CBE_TYPES_H_
#define INCLUDE_CBE_TYPES_H_

#include <stdint.h>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

namespace CBE {
  typedef uint64_t container_id_t;
  typedef uint64_t date_t;
  typedef uint64_t device_id_t;
  typedef uint64_t document_id_t;
  typedef uint64_t drive_id_t;
  typedef uint64_t favorite_id_t;
  typedef uint64_t folder_id_t;
  typedef uint64_t group_id_t;
  typedef uint64_t item_id_t;
  typedef uint64_t object_id_t;
  typedef uint64_t sync_id_t;
  typedef uint64_t user_id_t;
  typedef uint64_t webshare_id_t;
  typedef uint64_t share_id_t;

  typedef uint32_t account_status_t;
  typedef uint32_t failed_status_t;
  typedef uint32_t http_t;
  typedef uint32_t network_action_t;
  typedef uint32_t persistence_t;
  typedef uint32_t service_t;
  typedef uint32_t sync_direction_t;
  typedef uint32_t sync_status_t;
  typedef uint32_t transfer_t;
  typedef uint32_t webshare_access_t;
  typedef uint32_t webshare_visibility_t;

  typedef signed int permission_status_t;
  typedef int item_t;
  typedef int stream_t;
  typedef int visibility;
  typedef int application_t;
  typedef int object_t;
  namespace XmlNamespaces {
    static const std::string ATOM = "http://www.w3.org/2005/Atom";
    static const std::string OS = "http://a9.com/-/spec/opensearch/1.1/";
    static const std::string DC = "http://xcerion.com/directory.xsd";
    static const std::string NI = "http://xcerion.com/noindex.xsd";
  }

  namespace Service {
    static const CBE::service_t Network = 1;
    static const CBE::service_t LocalDB = 2;
  }

  /**
   * ApplicationType is not used in this version of groups but will be added later.
  */
  enum ApplicationType : application_t {
    Open = 1,
    Invite = 2,
    Review = 3,
    Closed = 4
  };

  enum ObjectType : object_t {
    Other = 1,
    GroupInvites = 2,
    ShareInvite = 3
  };

  /**
   * Visibility is used for both groups and members, in this version the member visibility will be Public for all members who join a group. 
   * Members will in the future also have the option of visibility friends.
  */
  enum Visibility : visibility {
    Public = 1,
    Private = 2
  }; 

  /**
   * Permission is can be set for any Object or Container the different combinations give access for users to use different API calls.
   * Example: 6 = ReadDelete gives the ability to call Move on a Container or an Object.
  */
  enum Permissions : permission_status_t
  {
    Read = 1,
    Write = 2,
    ReadWrite = 3,
    Delete = 4,
    ReadDelete = 5,
    WriteDelete = 6,
    ReadWriteDelete = 7,
    ChangeACL = 8,
    ReadChangeACL = 9,
    WriteChangeACL = 10,
    ReadWriteChangeACL = 11,
    DeleteChangeACL = 12,
    ReadDeleteChangeACL = 13,
    WriteDeleteChangeACL = 14,
    AllPermissions = 15,
    NoPermissions = 0
  };

  /**
   * Set the filter order in which the search or query will be sorted after.
  */
  enum FilterOrder : uint32_t {
    Title = 1,
    Relevance = 2,    //Note* group Searches does not use Relevance as order.
    Published = 3,
    Updated = 4,
    Length = 5,
    S1 = 6,
    S2 = 7,
    S3 = 8,
    S4 = 9
  };
  
  /**
   * This is used Internally do not change.
  */
  namespace Operations {
    enum Type {
      NoneOperation = 1,
      FolderOperation = 2,
      FileOperation = 3,
      DataLoaderOperation = 4,
      AccountOperation = 5,
      ShareOperation = 6,
      GroupOperation = 7
    };
  }

/**
 * ItemType can be used to sort out CBE objects if the user would like to create a container to put all different kinds of CBE objects in.
 * Currently the ItemType Tag is not used. 
*/
  namespace ItemType {
    static const CBE::item_t Unapplicable = 1;
    static const CBE::item_t Unknown = 2;
    static const CBE::item_t Object = 4;
    static const CBE::item_t Document = 4;
    static const CBE::item_t Container = 8;
    static const CBE::item_t Folder = 8;
    static const CBE::item_t Tag = 16;
    static const CBE::item_t Group = 32;    
  }

  namespace SyncFolderType {
    static const uint32_t Folder = 1;
    static const uint32_t Favorite = 2;
  }

  namespace Access {
    static const CBE::webshare_access_t Read = 1;
    static const CBE::webshare_access_t Update = 2;
    static const CBE::webshare_access_t Create = 3;
  }

  namespace WebShareVisibility {
    static const CBE::webshare_visibility_t Public = 1;
    static const CBE::webshare_visibility_t Friends = 2;
    static const CBE::webshare_visibility_t Private = 3;
  }

  namespace AccountStatus {
    static const CBE::account_status_t NotLoggedIn = 1;
    static const CBE::account_status_t LoggedIn = 2;
    static const CBE::account_status_t Failed = 3;
  }

  namespace NetworkAction {
    static const CBE::network_action_t Copy = 1;
    static const CBE::network_action_t Create = 2;
    static const CBE::network_action_t Login = 3;
    static const CBE::network_action_t Move = 4;
    static const CBE::network_action_t Remove = 5;
    static const CBE::network_action_t Rename = 6;
    static const CBE::network_action_t Restore = 7;
  }

  namespace PersistenceOperation {
    static const persistence_t None = 0;
    static const persistence_t Copy = 1;
    static const persistence_t Create = 2;
    static const persistence_t Login = 3;
    static const persistence_t Move = 4;
    static const persistence_t Remove = 5;
    static const persistence_t Rename = 6;
    static const persistence_t Restore = 7;
    static const persistence_t Update = 8;
    static const persistence_t ReadOnly = 9;
    static const persistence_t List = 10;
    static const persistence_t Upload = 11;
    static const persistence_t Download = 12;
    static const persistence_t Save = 13;
    static const persistence_t Share = 14;
    static const persistence_t listShares = 15;
    static const persistence_t listMyShares = 16;
    static const persistence_t Invite = 17;
    static const persistence_t UnShare = 18;
    static const persistence_t SetContainerACL = 19;
    static const persistence_t SetObjectACL = 20;
    static const persistence_t GetContainerACL = 21;
    static const persistence_t GetObjectACL = 22;
    static const persistence_t Join = 23;
    static const persistence_t Leave = 24;
    static const persistence_t Search = 25;
    static const persistence_t AcceptJoinRequest = 26;
    static const persistence_t RejectJoinRequest = 27;
    static const persistence_t Kick = 28;
    static const persistence_t Ban = 29;
    static const persistence_t UnBan = 30;

  }

  namespace PersistenceState {
    static const CBE::persistence_t Memory = 1;
    static const CBE::persistence_t Local = 2;
    static const CBE::persistence_t Cloud = 3;
  }

  /* Forward declarations */
  class AccountEventProtocol;
  class EventProtocol;
  class GroupEventProtocol;
  class ItemEventProtocol;
  class ItemDataEventProtocol;
  class NetworkEventProtocol;
  class RemoteEventProtocol;
  class ShareEventProtocol;
  class TransferDownloadEventProtocol;
  class TransferEventProtocol;
  class TransferUploadEventProtocol;

  class Account;
  class CloudBackend;
  class Connection;
  class Container;
  class Document;
  class Folder;
  class Group;
  class Item;
  class Object;
  class Stream;
  class Tag;
  class Member;
  class Transfer;
  class GroupInvite;
  class QueryResult;
  class GroupQuery;
  class QueryChain;

  //This is the data used from shares(shareId) to keep track of user/group-id relating to a shareid
  struct ShareData {
    //DEFAULT = 0, ID for userId or groupId
    uint64_t id = 0;
    //DEFAULT == TRUE, UserId = True if userID and false if groupId
    bool isUserId = true;
  };

  //This class is used to be able to search lists with ID: Example:
  //bool alreadyExists = std::find_if(vector.begin(), vector.end(), CBE::MatchesID<uint64_t>(id)) != vector.end();
  template<typename IDType>
  class MatchesID
  {
      IDType _id;
      
  public:
      MatchesID(const IDType &id) : _id(id) {}

      template<class ItemType>
      bool operator()(const ItemType &item) const
      {
          return item.id == _id;
      }
  };

     // if you have c++11 or higher SDK_CBE will be defined as std else it will be boost.
    typedef std::shared_ptr<CBE::Item> ItemPtr;
    typedef std::shared_ptr<CBE::Object> ObjectPtr;
    typedef std::shared_ptr<CBE::Container> ContainerPtr;
    typedef std::shared_ptr<CBE::Group> GroupPtr;
    typedef std::shared_ptr<CBE::Member> MemberPtr;
    typedef std::shared_ptr<CBE::GroupInvite> GroupInvitePtr;
    typedef std::shared_ptr<CBE::Account> AccountPtr;
    typedef std::shared_ptr<CBE::CloudBackend> CloudBackendPtr;

    typedef std::shared_ptr<AccountEventProtocol> AccountDelegatePtr;
    typedef std::shared_ptr<ItemEventProtocol> ItemDelegatePtr;
    typedef std::shared_ptr<ItemDataEventProtocol> ItemDataDelegatePtr;
    typedef std::shared_ptr<TransferEventProtocol> TransferDelegatePtr;
    typedef std::shared_ptr<TransferDownloadEventProtocol> TransferDownloadDelegatePtr;
    typedef std::shared_ptr<TransferUploadEventProtocol> TransferUploadDelegatePtr;
    typedef std::shared_ptr<ShareEventProtocol> ShareDelegatePtr;
    typedef std::shared_ptr<GroupEventProtocol> GroupDelegatePtr;
    typedef std::shared_ptr<QueryResult> QueryResultPtr;
    typedef std::shared_ptr<GroupQuery> GroupQueryPtr;
    typedef std::shared_ptr<QueryChain> QueryChainPtr;

    //The generic metadata_type for keyValue
    typedef std::map<std::string, std::pair<std::string, bool>> metadata_type;
    //Data index values, just because its nice
    #define metadata_dataindex_value 0
    #define metadata_dataindex_indexed 1

}// namespace CBE
#endif // INCLUDE_CBE_TYPES_H_