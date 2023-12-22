/*
     Copyright © CloudBackend AB 2020-2023.
*/

#ifndef INCLUDE_CBE_CLOUDBACKEND_H_
#define INCLUDE_CBE_CLOUDBACKEND_H_

#include "cbe/Types.h"
#include "cbe/delegate/CreateAccountDelegate.h"
#include "cbe/delegate/Error.h"
#include "cbe/delegate/CloudBackendListenerDelegate.h"
#include "cbe/delegate/LogInDelegate.h"
#include "cbe/delegate/QueryJoinDelegate.h"
#include "cbe/delegate/QueryDelegate.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"
#include "cbe/util/Optional.h"

#include <iosfwd>
#include <string>

namespace CBI {
class CloudBackend;
using CloudBackendPtr = std::shared_ptr<CBI::CloudBackend>;
class AccountEventProtocol;
using AccountDelegatePtr = std::shared_ptr<AccountEventProtocol>;
} // namespace CBI

namespace cbe
{
class QueryChainExt;
class QueryChainSync;

class Filter;
class GroupManager;
class PublishManager;
class ShareManager;
class SubscribeManager;

/**
 * @brief The session that holds the connection with the cloud.
 * 
 * It ensures the connection is authenticated.
 */
class CloudBackend {
public:
  /**
   * Pointer to cbe::delegate::LogInDelegate() that is passed into asynchronous
   * version of method
   * @ref logIn(const std::string&,const std::string&,const std::string&,LogInDelegatePtr) 
   * "logIn()"
   */
  using LogInDelegatePtr = delegate::LogInDelegatePtr;
  /**
   * @brief Logs in to the %CloudBackend service.
   * 
   * When finished with the usage of the %CloudBackend service, call
   * method CloudBackend::terminate() in all cases.
   * 
   * The authentication to the account is determined by \p username,
   * \p password and \p tenant.
   * 
   * <b>Asynchronous</b> version of this service function.
   *
   * @param username  Name of the user to be signed in.
   * @param password  Password for the user to be signed in.
   * @param tenant    The identifier for the tenant,
   *                  formerly known as \c source.
   * @param client    Identifier of what type of client the program is running
   *                  on. This is used by the tenant for statistics and
   *                  selective communication with the users.
   *                  The tenant administrator defines what client names to use.
   * @param delegate  Pointer to a be::CloudBackend::LogInDelegate() instance
   *                  that is implemented by the user to receive the response
   *                  of this login request.<br>
   *                  This is accomplished in terms of the LogInDelegate
   *                  callback functions
   *                  @ref delegate::LogInDelegate::onLogInSuccess(CloudBackend&&)
   *                       "onLogInSuccess()" and
   *                  @ref delegate::LogInDelegate::onLogInError(LogInDelegate::Error&&,cbe::util::Context&&)
   *                       "onLogInError()".
   *
   * @anchor anchorEx__cbe__CloudBackend__logIn_async 
   * @par Example
   *      Async login
   * @include example/login_async.cpp
   */
  static cbe::CloudBackend logIn(const std::string&       username,
                                 const std::string&       password,
                                 const std::string&       tenant,
                                 const std::string&       client,
                                 LogInDelegatePtr         delegate);

#ifndef CBE_NO_SYNC
  /**
   * \see delegate::LogInDelegate::Exception
   */
  using LogInException = delegate::LogInDelegate::Exception;
  /**
   * @anchor anchorEx__cbe__CloudBackend__logIn_syncExcept
   * @brief Synchronous [exception] logIn
   * 
   * <b>Synchronous</b> version of
   * @ref
   * logIn(const std::string&,const std::string&,const std::string&,const std::string&,LogInDelegatePtr)
   * "logIn()",
   * and <b>throws an exception</b>, #LogInException, in case of a failed
   * login.
   *
   * @return A %CloudBackend instance &mdash; if login was successful.
   * @throws #LogInException
   *
   * @par Example
   *      Synchronous [exception] logIn
   * @include example/login_syncExcept.cpp
   */
  static cbe::CloudBackend logIn(const std::string&  username,
                                 const std::string&  password,
                                 const std::string&  tenant,
                                 const std::string&  client);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref logIn(const std::string&,const std::string&,const std::string&,LogInError&)
   * "logIn()"
   * \see delegate::LogInDelegate::ErrorInfo
   */
  using LogInError = delegate::LogInDelegate::ErrorInfo;
  /**
   * @anchor anchorEx__cbe__CloudBackend__logIn_syncNoExcept
   * @brief Synchronous [non-throwing] logIn
   * 
   * <b>Synchronous</b> version of
   * @ref
   * logIn(const std::string&,const std::string&,const std::string&,const std::string&,LogInDelegatePtr)
   * "logIn()",
   * and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information.
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed login. <br>
   *              The return value, of type #CloudBackend, will indicate
   *              <code><b>false</b></code> on a failed login, and the
   *              LogInError object passed in will we be populated with the
   *              error information.
   *
   * @return If empty, <code><b>false</b></code>, an error has occurred.
   *         I.e., the login has failed, and the error information has been
   *         passed out via the \p error out/return parameter.
   * @par Example
   *      Synchronous [non-throwing] logIn
   * @include example/login_syncNoThrow.cpp
   */
  static cbe::CloudBackend logIn(const std::string& username,
                                 const std::string& password,
                                 const std::string& tenant,
                                 const std::string& client,
                                 LogInError&        error);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif // #ifndef CBE_NO_SYNC

  /**
   * Pointer to
   * delegate::CreateAccountDelegate
   * that is passed into asynchronous version of method 
   * @ref createAccount()
   */
  using CreateAccountDelegatePtr = delegate::CreateAccountDelegatePtr;
  /**
   * @brief Creates a user account.
   * 
   * This method is only granted to the tenant administrator.
   * 
   * <b>Asynchronous</b> version of this service function.
   * @param username   Username for the new account
   * @param password   Password for the new account
   * @param email      Email address to owner of the new account
   * @param firstName  First name of the owner of the new account
   * @param lastName   Last name of the owner of the new account
   * @param tenant     The identifier for the tenant, formerly known as
   *                   \c source.
   * @param client     Describing platform running the software. 
   *                   Names defined by the tenant administrator.
   * @param delegate   Pointer to a delegate::CreateAccountDelegate instance 
   *                   that is implemented by the user.
   * 
   */
  static cbe::UserId createAccount(const std::string&       username,
                            const std::string&       password,
                            const std::string&       email,
                            const std::string&       firstName,
                            const std::string&       lastName,
                            const std::string&       tenant,
                            const std::string&       client,
                            CreateAccountDelegatePtr delegate);
#ifndef CBE_NO_SYNC
  /**
   * See delegate::CreateAccountDelegate::Exception
   */
  using CreateAccountException = delegate::CreateAccountDelegate::Exception;
  /**
   * @brief Synchronous [exception] Creates a user account.
   * 
   * <b>Synchronous</b> version of
   * @ref
   * createAccount(const std::string&,const std::string&,const std::string&,const std::string&,const std::string&,const std::string&,const std::string&,CreateAccountDelegatePtr)
   * "createAccount()",
   * and <b>throws an exception</b>, #CreateAccountException,
   * in case of a failed account creation.
   * 
   * See @ref createAccount(const std::string&,const std::string&,const std::string&,const std::string&,const std::string&,const std::string&,const std::string&,CreateAccountDelegatePtr)
   * "createAccount()"
   *
   * @return A CloudBackend instance associated with the created account &mdash;
   *         if account creation was successful.
   * @throws #CreateAccountException
   */
  static cbe::UserId createAccount(const std::string&       username,
                            const std::string&       password,
                            const std::string&       email,
                            const std::string&       firstName,
                            const std::string&       lastName,
                            const std::string&       tenant,
                            const std::string&       client);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref createAccount(username,password,email,firstname,lastname,tenant,client,CreateAccountError&) "createAccount()"
   * <br>See delegate::CreateAccountDelegate::ErrorInfo
   */
  using CreateAccountError = delegate::CreateAccountDelegate::ErrorInfo;
  /**
   * @brief Synchronous [no exception]
   * <b>Synchronous</b> version of
   * createAccount(username,password,email,firstname,lastname,tenant,client,CreateAccountDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call. <br>See createAccount(username,password,email,firstname,lastname,tenant,client,CreateAccountDelegatePtr) @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed call. <br>
   *              An empty return value will indicate failure, and the
   *              #CreateAccountError object passed in will we be populated with the
   *              error information.
   *
   * @return Empty &mdash; i.e., <code><b>false</b></code> &mdash; indicates a
   *         failed call, and the error information is passed out via the
   *         \p error out/return parameter.
   */
  static cbe::util::Optional<cbe::UserId> createAccount(const std::string&  username,
                                                 const std::string&  password,
                                                 const std::string&  email,
                                                 const std::string&  firstName,
                                                 const std::string&  lastName,
                                                 const std::string&  tenant,
                                                 const std::string&  client,
                                                 CreateAccountError& error);
#endif // #ifndef CBE_NO_SYNC


  /**
   * Pointer to
   * delegate::CloudBackendListenerDelegate
   * that is passed into asynchronous version of method 
   * addListener(CloudBackendListenerDelegatePtr)
   */
  using CloudBackendListenerDelegatePtr =
                        std::shared_ptr<delegate::CloudBackendListenerDelegate>;
  /**
   * @brief Handle identifying a registered Listener.
   */
  using ListenerHandle = std::uint64_t;
  /**
   * @brief Listen for changes to specific items.
   * 
   * Adds a listener that will receive updates as changes occur on the account.  
   * \note removeListener()
   *       should always be called when no longer using the delegate.
   * @param listener Delegate is a shared pointer to the class
   *                 delegate::CloudBackendListenerDelegate
   *                 that the user has implemented
   * @return Handle identifying the registration of the listener.<br>
   *         To be used when de-registering with method
   *         removeListener(ListenerHandle).
   */
  ListenerHandle addListener(CloudBackendListenerDelegatePtr listener);

  /**
   * @brief Delete a specific listener.
   * 
   * Removes a listener that will receive updates as changes occur on the account
   * @param handle  Handle previously retrieved from method
   *                addListener(CloudBackendListenerDelegatePtr).
   */
  void removeListener(ListenerHandle handle);

  /**
   * Pointer to 
   * delegate::QueryDelegate 
   * that is passed into asynchronous version of methods:
   * <ul>
   *   <li> query(ContainerId,QueryDelegatePtr),
   *   <li> query(ContainerId,Filter,QueryDelegatePtr),
   *   <li> queryWithPath(std::string, QueryDelegatePtr)
   *   <li> queryWithPath(std::string, cbe::ContainerId, QueryDelegatePtr),
   *   <li> search(std::string,cbe::ContainerId,QueryDelegatePtr),
   *   <li> search(Filter,cbe::ContainerId,QueryDelegatePtr)
   * </ul>
   */  
  using QueryDelegatePtr = delegate::QueryDelegatePtr; 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * @brief Select %Item's of a container (table).
   * 
   * Inquires for a set of @ref Item "Item"s from the provided %Container,
   * identified by \p containerId.
   * Response is delivered asynchronously via the delegate::QueryDelegate
   * callback interface passed in as argument via parameter 
   * \p queryDelegate .<br>
   * This overload of join() accepting a
   * @ref delegate::QueryDelegate "QueryDelegate"-pointer as parameter has two
   * use cases:
   * <ol>
   *   <li> To make a solely query() call <b>without</b> an additional chained
   *      @ref QueryChain::join(Container,std::string,std::string,delegate::JoinDelegatePtr) "join()"-call.
   *   <li> To make a
   *      @ref query(ContainerId,delegate::QueryJoinDelegatePtr) "query()" call
   *      <b>with</b> a desired chained join() call.<br>
   *      In this latter case, the overloaded 
   *      @ref query(ContainerId,delegate::QueryJoinDelegatePtr)
   *      "query()"-functions:
   *   <ul>
   *     <li> query(ContainerId,delegate::QueryJoinDelegatePtr)
   *     <li> query(ContainerId,Filter,delegate::QueryJoinDelegatePtr)
   *   </ul>
   *      both accepting a 
   *      @ref delegate::QueryJoinDelegate "QueryJoinDelegate" 
   *      as \p delegate argument, must be used.
   * </ol>
   * 
   * <b>Asynchronous</b> version of this service function.
   *
   * @param containerId
   *   Id of the container which contents will inquired.
   * @param queryDelegate
   *   Pointer to a @ref delegate::QueryDelegate "QueryDelegate"
   *   instance, implemented by the user, that receives the response of
   *   this query request.<br>
   *   I.e., either the
   *   @ref delegate::QueryDelegate "QueryDelegate"
   *   callback function 
   *   @ref delegate::QueryDelegate::onQuerySuccess(delegate::QueryDelegate::Success&&)
   *   "onQuerySuccess()"
   *   or
   *   @ref delegate::QueryDelegate::onQueryError(delegate::QueryDelegate::Error&&,cbe::util::Context&&)
   *   "onQueryError()"
   *   will be called in the event of success, or failure respectively.
   *
   * @anchor anchorEx__cbe__CloudBackend__query_async 
   * @par Example
   *      Async query
   * @include example/cloudbackend_query_async.cpp
   * To use the code above, we must first login and then declare a
   * @ref cbe::delegate::QueryDelegate "QueryDelegate" class.<br>
   * See also:
   * <ul>
   *   <li> @ref anchorEx__cbe__CloudBackend__logIn_async
   *             "Async login"<br>
   *             Here, we retrieve the \c cloudBackend object. 
   *   <li> @ref anchorEx__cbe__delegate_QueryDelegate
   *             "Example QueryDelegatePtr of a QueryDelegate implementation class"<br>
   *             Here, class MyQueryDelegate is defined. 
   * </ul> 
   */
  cbe::QueryChain query(ContainerId      containerId,
                   QueryDelegatePtr queryDelegate);
  /**
   * @brief Select %Item's of a container (table) with a filter (where).
   * 
   * Same as query(ContainerId,QueryDelegatePtr),
   * but with an additional parameter \p filter.
   *
   * @param filter Filter specifying the constraints of the requested items.
   */
  cbe::QueryChain query(ContainerId      containerId,
                   Filter           filter,
                   QueryDelegatePtr queryDelegate);

  /**
   * Pointer to 
   * delegate::QueryJoinDelegate
   * that is passed into asynchronous version of methods 
   * <ul>
   *   <li> query(ContainerId,QueryJoinDelegatePtr)
   *   <li> query(ContainerId,Filter,QueryJoinDelegatePtr)
   * </ul>
   */  
  using QueryJoinDelegatePtr = delegate::QueryJoinDelegatePtr;                   
  /**
   * @brief Join multiple tables.
   * 
   * Same as query(ContainerId,delegate::QueryDelegatePtr), but with a
   * @ref delegate::QueryJoinDelegate "QueryJoinDelegate" as delegate,
   * \p queryJoinDelegate.
   * \see query(ContainerId,delegate::QueryDelegatePtr)
   *
   * @param queryJoinDelegate
   * Pointer to a @ref delegate::QueryJoinDelegate "QueryJoinDelegate"
   * instance, implemented by the user, that receives the response of
   * this query request.<br>
   * I.e., either its callback function 
   * @ref delegate::QueryJoinDelegate::onQueryJoinSuccess(cbe::QueryResult&&)
   *      "onQueryJoinSuccess()" or 
   * @ref delegate::QueryJoinDelegate::onQueryJoinError(QueryJoinError &&, cbe::util::Context &&)
   *      "onQueryJoinError()" 
   * will be called in the event of success or failure respectively.
   */
  cbe::QueryChainExt query(ContainerId           containerId,
                      QueryJoinDelegatePtr  queryJoinDelegate);
  /**
   * @brief Join multiple tables using filter (where).
   * 
   * Same as query(ContainerId,delegate::QueryJoinDelegatePtr), but with an
   * additional Filter parameter \p filter.
   *
   * @param filter Filter specifying the constraints of the requested items.
   */
  cbe::QueryChainExt query(ContainerId           containerId,
                      Filter                filter,
                      QueryJoinDelegatePtr  queryJoinDelegate);

#ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /*! \see delegate::QueryDelegate::Exception */
  using QueryException = delegate::QueryDelegate::Exception;
  /**
   * @brief Synchronous [exception]
   * 
   * <b>Synchronous</b> version of
   * query(ContainerId,delegate::QueryDelegatePtr), and <b>throws an
   * exception</b>, #QueryException, in case of a failed query.
   *
   * @return Upon success, a QueryResult object - in fact an instance of
   *         QueryChainSync that is a subclass of QueryResult.
   * @throws #QueryException
   * @anchor anchorEx__cbe__CloudBackend__query_syncExcept
   * @par Example
   *      Sync [exception] query
   * @include example/cloudbackend_query_syncExcept.cpp
   * To use the code above, we must first login.<br>
   * See also:
   * <ul>
   *   <li> @ref anchorEx__cbe__CloudBackend__logIn_syncExcept
   *             "Sync [exception] login"<br>
   *             Here, we retrieve the \c cloudBackend class object. 
   * </ul> 
   */
  cbe::QueryChainSync query(ContainerId  containerId);

  /**
   * @brief Synchronous [exception] filtered
   * 
   * Extended version of query(ContainerId) with an  additional filter parameter
   * \p filter.
   *
   * @param filter Filter specifying the constraints of the requested items.
   */
  cbe::QueryChainSync query(ContainerId  containerId,
                       Filter       filter);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /*! \see delegate::QueryJoinDelegate::ErrorInfo */
  using QueryJoinError = delegate::QueryJoinDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] query
   * 
   * In line with synchronous query(ContainerId), but <b>throws
   * <u>no</u> exception</b> on error, instead the out/return parameter \p error
   * is used to provide the error information.
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed query. <br>
   *              The return value, of type QueryChainSync, will indicate
   *              <code><b>false</b></code> on a failed query, and the passed in
   *              QueryJoinError object will we be populated with the error
   *              information.<br>
   *              Since there might be multiple chained calls after the return
   *              from this method, the error information will also contain the
   *              index of the failed chained join() call.
   *
   * @return An object of QueryResult - in fact an instance of QueryChainSync
   *         that is a subclass of QueryResult.<br>
   *         If empty - i.e., <code><b>false</b></code> an error has occurred,
   *         i.e., a failed query, and the error information has been passed out
   *         via the \p error out/return parameter.
   * @anchor anchorEx__cbe__CloudBackend__query_syncNoExcept
   * @par Example
   *      Sync [non-throwing] query
   * @include example/cloudbackend_query_syncNoThrow.cpp
   */
  cbe::QueryChainSync query(ContainerId      containerId,
                       QueryJoinError&  error);

  /**
   * @brief Synchronous [non-throwing] filtered query
   * 
   * Extended version of query(ContainerId,QueryJoinError&) with an  additional
   * filter parameter \p filter.
   * \see query(ContainerId,QueryJoinError&)
   * To use the code above, we must first login.<br>
   * See also:
   * <ul>
   *   <li> @ref anchorEx__cbe__CloudBackend__logIn_syncNoExcept
   *             "Sync [non-throwing] login"<br>
   *             Here, we retrieve the \c cloudBackend class object. 
   * </ul> 
   */
  cbe::QueryChainSync query(ContainerId      containerId,
                       Filter           filter,
                       QueryJoinError&  error);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /*! \see delegate::QueryDelegate::ErrorInfo */
  using QueryError = delegate::QueryDelegate::ErrorInfo;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif // #ifndef CBE_NO_SYNC

  /**
   * @brief Queries items of a container with a given path.
   * 
   * \note <code>..</code> or <code>.</code>
   *       relative path options are not
   *       permitted. <br>
   *       Only top down search from start point, \p queryRoot id
   *       to downwards path in container tree.
   * @param relativePath The relative path from the \p queryRoot.<br> 
   *                     E.g.: 
   *                     <code>/Documents/Pictures</code><br>
   *                     from a \p queryRoot that has the nested containers
   *                     <code>Documents</code> and <code>Pictures</code>. 
   * @param queryRoot    The container id forming the root of this query.
   * @param delegate     Pointer to a delegate::QueryDelegate instance 
   *                     that is implemented by the user.
   */
  cbe::QueryChain queryWithPath(std::string         relativePath,
                                cbe::ContainerId    queryRoot,
                                QueryDelegatePtr    delegate);
  /**
   * @brief Queries items of a container with a given path relative the home root
   *        container.
   * 
   * Same as queryWithPath(std::string, cbe::ContainerId, QueryDelegatePtr),
   * but with the <code>queryRoot</code> parameter omitted. Instead, 
   * here the home root container is used as top container in the search tree.
   */
  cbe::QueryChain queryWithPath(std::string         relativePath,
                                QueryDelegatePtr    delegate);
   
  /**
   * @brief Select Object's with specified key/values.
   * 
   * Search the whole container for tags related to
   * @ref Object "Objects" in the container structure.<br>
   * E.g., <c> Key = Name, Value Contract/Object/Song => Name:Contract1 </c>.
   *
   * Search handles tags in combination / conjunction of keys and/or key values
   * separated by |.<br>
   * E.g., Name:*|Country:Sweden|Country:Norway. <br>
   * This would search for objects with key Name but any value and where key
   * Country is either Sweden or Norway.
   * @param tags          Is a string of key tags or key:value pairs that are
   *                      separated by |.
   * @param containerId   Is the cbe::ContainerId id of the rootContainer to
   *                      start the search of @ref cbe::Object "objects" in. \n 
   *                      E.g., if starting in the rootContainer,
   *                      the whole account will be searched for
   *                      matching tags, key/value's.
   * @param delegate      Is the callback pointer to where the API returns from
   *                      either cache or Server.
  */
  cbe::QueryResult search(std::string         tags,
                          cbe::ContainerId    containerId,
                          QueryDelegatePtr    delegate);

  /**
   * Pointer to cbe::delegate::QueryDelegate that is passed into
   * asynchronous version of method
   * @ref search(QueryDelegatePtr)   "search()"
   */

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::QueryDelegate::Exception
  */
  using SearchException = delegate::QueryDelegate::Exception;
  /**
   * @brief Synchronous [exception]
   * <b>Synchronous</b> version of
   * search(std::string, cbe::ContainerId, QueryDelegatePtr)
   * , and <b>throws an exception</b>, #SearchException, in case of a failed
   * call.
   * <br>See search(QueryDelegatePtr)
   *
   * @return Information about the search
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::cbe::QueryResult " "
   * @throws #SearchException
   */
  cbe::QueryResult search(std::string         tags,
                          cbe::ContainerId    containerId);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref search(tags, containerId, SearchError&) "search()"
   * <br>See delegate::QueryDelegate::ErrorInfo
   */
  using SearchError = delegate::QueryDelegate::ErrorInfo;
  /**
   * @brief Synchronous [non-throwing] search
   * <b>Synchronous</b> version of
   * search(tags, containerId, QueryDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See search(tags, containerId,  QueryDelegatePtr)
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
  cbe::util::Optional<cbe::QueryResult> search(
                                              std::string         tags,
                                              cbe::ContainerId    containerId,
                                              SearchError&        error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /**
   * @brief Select Object's with specified key/values using filter.
   * 
   * Search the whole container for tags related to Objects in the container 
   * structure. \n 
   * E.g., <c>Key = Name, Value Contract/Object/Song => Name:Contract1 </c>.
   *
   * Search handles tags in combination / conjunction of keys and/or key values
   * separated by |. \n 
   * E.g., Name:*|Country:Sweden|Country:Norway, this would search for objects
   * with key Name but any value and where key Country is either Sweden or 
   * Norway.
   * 
   * See Filter.
   * @param filter is a cbe::Filter on which you can set how you want data to be 
   * ordered when searching. Remember to set the queryString to be keys/tags or 
   * key:value pairs that are separated by |.
   * @param containerId is the ContainerId of the rootContainer to start the 
   * search of Objects in. \n 
   * E.g., if starting in the rootContainer, the whole 
   * account will be searched for matching tags, key/value's.
   * @param delegate is the callback pointer to where the API returns from either
   * cache or Server.
  */
  cbe::QueryResult search(cbe::Filter         filter,
                          cbe::ContainerId    containerId,
                          QueryDelegatePtr    delegate);


  /**
   * Pointer to cbe::delegate::QueryDelegate that is passed into
   * asynchronous version of method
   * @ref search(QueryDelegatePtr)   "search()"
   */

#ifndef CBE_NO_SYNC
  /**
  * See delegate::object::QueryDelegate::Exception
  */
  /**
   * @brief Synchronous [exception]
   * <b>Synchronous</b> version of
   * search(cbe::Filter, cbe::ContainerId, QueryDelegatePtr)
   * , and <b>throws an exception</b>, #SearchException, in case of a failed
   * call.
   * <br>See search(QueryDelegatePtr)
   *
   * @return Information about the search
   *         &mdash; if the call was successful.<br>
   *         See @ref cbe::cbe::QueryResult " "
   * @throws #SearchException
   */
  cbe::QueryResult search(cbe::Filter         filter,
                          cbe::ContainerId    containerId);

  /**
   * Forms the type of the \p error return parameter for the synchronous version
   * of method
   * @ref search(filter,containerId,SearchError&) "search()"
   * <br>See delegate::QueryDelegate::ErrorInfo
   */
  /**
   * @brief Synchronous [non-throwing] search
   * <b>Synchronous</b> version of
   * search(filter,containerId,QueryDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \p error is used to provide the error information in connection
   * with a failed call.
   * <br>See search(filter,containerId, QueryDelegatePtr)
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
  cbe::util::Optional<cbe::QueryResult> search(cbe::Filter         filter,
                                               cbe::ContainerId    containerId,
                                               SearchError&           error);
#endif // #ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /**
   * @brief Casts an item to a container
   * 
   * If the provided cbe::Item "item" is not a cbe::Container "container", an
   * empty container instance is returned
   */
  static cbe::Container castContainer(cbe::Item item);

  /**
   * @brief Casts an item to an object
   * 
   * If the provided cbe::Item "item" is not a cbe::Object "object", an
   * empty object instance is returned
   */
  static cbe::Object castObject(cbe::Item item);

  /**
   * @brief Clear the local cache.
   * 
   * Use if there is a local SDK memory issue.
   * 
   * @return true  success
   * @return false failed
   */
  bool clearCache();

  /**
   * @brief Returns an account object with information on the user.
   * 
   * @return cbe::Account 
   */
  cbe::Account account();


  /**
   * @brief Returns the version number of the SDK.
   * 
   * @return std::string 
   */
  std::string version() const;

  /**
   * @brief Gets the group manager.
   * 
   * @return cbe::GroupManager 
   */
  cbe::GroupManager     groupManager();  // Group - NYI

  /**
   * @brief Gets the share manager.
   * 
   * @return cbe::ShareManager 
   */
  cbe::ShareManager     shareManager(); // Shares

  /**
   * @brief Gets the publish manager.
   * 
   * @return cbe::PublishManager 
   */
  cbe::PublishManager   publishManager(); // Publish (web shares)


  /**
   * @brief Terminates the %CloudBackend service.
   * 
   * Shall also be called in connection with a failed log in.
   * 
   */
  void terminate(); 

  /**
   * @brief Gets the subscribe manager.
   * 
   * @return cbe::SubscribeManager 
   */
  cbe::SubscribeManager subscribeManager(); // Subscribe (following)

  /**
   * @brief Default constructor.
   * 
   * Construct a new object with 
   * the \c DefaultCtor to enable the
   * @ref operator bool()
   * test
   */
  CloudBackend(cbe::DefaultCtor);
  ~CloudBackend();

  /**
   * @brief Checks if the current instance is real.
   * 
   * An "unreal" instance implies typically a failed login event.
   * 
   * Relies on the \c Default \c constructor
   * CloudBackend(cbe::DefaultCtor)
   * 
   * @return \c true  : is real
   * @return \c false : unreal; got nullptr; if current instance is unbound/undefined. I.e., if it is
   *         only default constructed.
   */
  explicit operator bool() const;
private:
  struct Impl;
  std::shared_ptr<Impl> pImpl{};

  CloudBackend(CBI::CloudBackendPtr cbiPeerPtr);
  /**
   * Provides a reference to the peer object.
   */
  CBI::CloudBackend& getCbiPeer() const;

}; // class CloudBackend

} // namespace cbe
// namespace cbe

#endif // INCLUDE_CBE_CLOUDBACKEND_H_
