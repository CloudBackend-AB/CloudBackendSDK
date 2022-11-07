/*
  SDK type definitions.
  Copyright © CloudBackend AB 2020 - 2022.
*/
#ifndef INCLUDE_CBE_TYPES_H_
#define INCLUDE_CBE_TYPES_H_

#include <stdint.h>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

/**
 * @brief Root namespace for the %CloudBackend SDK API.
 * 
 */
namespace cbe {

  // Type definitions for ids, enums, settings and failure codes

  /**
   * @brief Unique Id of a cbe::Container
   */ 
  using ContainerId = std::uint64_t;
  /**
   * @brief time-stamp in the unix epoch format.
   * 
   * A.k.a. POSIX time or time-stamp. \n 
   * The equivalent in Linux shell can be found using e.g.,
   * \code {.sh}
   * date +%s --utc --date='now'
   * date +%s --utc --date='today 17:30:00'
   * \endcode
   * To convert a time-stamp to human readable format use e.g.,
   * \code {.sh}
   * date --utc --date=@1678902345
   * \endcode
   */
  using Date        = std::uint64_t;
  /**
   * @brief The id of a drive.
   */
  using DriveId     = std::uint64_t;
  /**
   * @brief Uniquely identifies the Group.
   */
  using GroupId     = std::uint64_t;
  /**
   * @brief Id of a cbe::Container or cbe::Object
   */ 
  using ItemId      = std::uint64_t;
  /**
   * @brief Unique Id of a cbe::Object
   */ 
  using ObjectId    = std::uint64_t;
  /**
   * @brief Uniquely identifies the CBE user.
   */
  using UserId      = std::uint64_t;
  /**
   * @brief Represents the cbe::Group membership. 
   */
  using MemberId    = std::uint64_t;
  /**
   * @brief Id of a cbe::Container or cbe::Object
   */ 
  using SubscribeId = std::uint64_t;
  /**
   * @brief Id of a subscribed cbe::Container or cbe::Object.
   */ 
  using PublishId   = std::uint64_t;
  /**
   * @brief Uniquely identifies a sharing of a cbe::Container or cbe::Object
   * 
   */
  using ShareId     = std::uint64_t;
  /**
   * @brief Uniquely identifies a cbe::Stream.
   * 
   */
  using StreamId    = std::uint64_t;

  using account_status_t      = std::uint32_t;
  using failed_status_t       = std::uint32_t;
  using http_t                = std::uint32_t;
  using sync_direction_t      = std::uint32_t;
  using sync_status_t         = std::uint32_t;
  using transfer_t            = std::uint32_t;
  using publish_access_t      = std::uint32_t;
  using publish_visibility_t  = std::uint32_t;

  using permission_status_t   = int;
  using item_t                = int;
  using stream_t              = int;
  using visibility            = int;
  using application_t         = int;
  using object_t              = int;

  /**
   * ApplicationType is not used in this version of groups but will be added
   * later.
   */
  enum class ApplicationType : application_t {
    Open = 1,
    Invite = 2,
    Review = 3,
    Closed = 4
  };

  /**
   * Type of invite. 
   */
  enum class ObjectType : object_t {
    Other = 1,
    GroupInvites = 2,
    ShareInvite = 3
  };

  //! @cond Doxygen_Suppress
  /**
   * HttpType is used when you want to call rest requests directly.
   * \note This might in the future be taken out since we may parse 
   * the URI string where the request type would be present instead.
  */
  enum class HttpType : http_t {
    GET = 1,
    POST = 2,
    PUT = 3,
    DELETE = 4,
    HEAD = 5
  };
  //! @endcond

  /**
   * Visibility is used for both groups and members, in this version 
   * the member visibility will be Public for all members who join a group.
   * 
   * Members will in the future also have the option of visibility friends.
  */
  enum class Visibility : visibility {
    Public = 1,
    Private = 2
  }; 

  /**
   * @brief Represents the access permission that can be set for any
   * cbe::Object or cbe::Container.
   * Forms the the possible different combinations of:
   * <ul>
   *   <li> 1: Read
   *   <li> 2: Write
   *   <li> 4: Delete
   *   <li> 8: ChangeACL
   * </ul> 
   * combinations give access for users to use different API calls.
   * 
   * E.g.: 
   * 6 = ReadDelete gives the ability to call Move on a Container or an Object.
  */
  enum class Permissions : permission_status_t
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
  enum class FilterOrder : uint32_t {
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
   * ItemType can be used to sort out cbe objects if the user would like to
   * create a container to put all different kinds of cbe objects in.
  */
  enum class ItemType : cbe::item_t  {
    Unapplicable =  1,
    Unknown =       2,
    Object =        4,
    Container =     8,
    Tag =           16,
    Group =         32
  };

  /**
   * Access permission for publish
   */
  enum class PublishAccess : cbe::publish_access_t {
    Read = 1,
    Update = 2,
    Create = 3
  };

  /**
   * Visibility for publish
   */
  enum class PublishVisibility : cbe::publish_visibility_t {
    Public =  1,
    Friends = 2,
    Private = 3,
    Invited = 4
  };

  /**
   * Callback for login returns one of these three in callback.
   */
  enum class AccountStatus : cbe::account_status_t {
    NotLoggedIn = 1,
    LoggedIn =    2,
    Failed =      3
  };

  /**
   * @brief Default constructor marker.
   * 
   * To default construct objects from most of the %CloudBackend classes, this
   * marker type is required.
   * \par Example use
   * \code {.cpp}
     ~~~
     // Conceptually, a default construction of an instance of cbe::Container 
     cbe::Container myContainer{ cbe::DefaultCtor{} }; 
     // Ditto
     cbe::Object    myObject{ cbe::DefaultCtor{} };
     ~~~
   * \endcode
   * 
   */
  enum DefaultCtor {};

  /**
    * @brief Mimics the general error code encoding in the www.
    * \see [Wikipedia: List of HTTP status codes]
    *      (https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)
    */
  using ErrorCode = std::uint32_t;

  /**
   * @brief ACL map (<b>Access</b> <b>Control</b> <b>List</b>) of users and
   * groups.
   * Map of a specific user, in terms of cbe::UserId, and its cbe::Permission,
   * to reperesent permissions for specific users.
   */
  using AclMap = std::map<cbe::UserId, cbe::Permissions>;

  /**
   * @brief Map with key/value pairs, a.k.a. metadata.<br>
   * Can be applied on @ref cbe::Object but not on @ref cbe::Container.<br>
   * <ul>
   *   <li> <b>key</b> uniquely identifies the value
   *   <li> <b>value</b> and <b>index flag</b>
   *   <ul>
   *     <li> a string value
   *     <li> a boolean flag indicating whether this key/value entry is index
   *          (\c true) or not indexed (\c false).  
   *   </ul>
   * </ul>
   */ 
  using KeyValues = std::map<std::string, std::pair<std::string, bool>>;

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
  class GroupQueryResult;
  class QueryChain;
  class Database;
  class Publish;
  class Subscribe;

  /**
   * @brief @ref cbe::Database "Databases" available for the
   * @ref cbe::Account "account".
   */
  using DataBases = std::map<std::string, cbe::Database>;


  /**
   * @brief Collection of @ref cbe::Item "items".
   */
  using Items = std::vector<cbe::Item>;

  struct ShareData;
  /**
   * @brief Map of cbe::ShareData for a specific cbe::ShareId.
   */
  using ShareIds = std::map<cbe::ShareId, std::vector<cbe::ShareData>>;

  /**
   * @brief Map of a pair of @ref cbe::Member "members", associated with ban
   *        information, where:
   * <ul>
   *   <li> <b>Key</b> is formed by a [<b><c>std::pair</c></b>]
   *        (https://en.cppreference.com/w/cpp/utility/pair) of
   *        @ref cbe::MemberId "members",
   *        where:
   *     <ul>
   *       <li> <code>first</code> represents the banned member.
   *       <li> <code>second</code> represents the banning member with
   *            adminstration priviliges.
   *     </ul>
   *   <li> <b>Value</b> is formed by another pair:
   *     <ul>
   *       <li> @ref cbe::Date "date" of the ban
   *       <li> a free text reason message.
   */
  using MemberBanInfo = std::map<std::pair<cbe::MemberId, cbe::MemberId>,
                                 std::pair<cbe::Date, std::string>>;

  /**
   * @brief collection of delegates for the main classes
   */
  namespace delegate {}

  /**
   * @brief general utilities
   */
  namespace util {}

}// namespace cbe




#endif // INCLUDE_CBE_TYPES_H_
