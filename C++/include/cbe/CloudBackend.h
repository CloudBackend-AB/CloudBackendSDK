
/*
     Copyright © CloudBackend AB 2020 - 2022.
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
   * Pointer to cbe::CloudBackend::LogInDelegate() that is passed into method
   * @ref logIn(const std::string&,const std::string&,const std::string&,
   *            LogInDelegatePtr) 
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
   * \p password and \p tenant.<br>
   * <b>Asynchronous</b> version of this service function.
   *
   * @param username  Name of the user to be signed in.
   * @param password  Password for the user to be signed in.
   * @param tenant    The identifier for the tenant, formerly known as
   *                  \c source.
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
   * @anchor anchorEx__cbe__CloudBackend__logIn 
   * @par Example of asynchronous login
   * @code
  #include "cbe/CloudBackend.h"
  ~~~
  #include <condition_variable>
  #include <mutex>
  ~~~
  int main() {
    // First, declare and implement a LogInDelegate class
    class MyLogInDelegate : public cbe::delegate::LogInDelegate {
      std::mutex              mutex{};
      std::condition_variable conditionVariable{};
      // Indicates operation completed - success or failure
      bool                    called{};
    public:
      // Default construct this CloudBackend member. If the method
      // onLogInSuccess() will not not be called, this default constructed state
      // implies that this cloudBackend object is not valid due to a successful
      // log in
      cbe::CloudBackend cloudBackend{ cbe::DefaultCtor{} };

      // Default construct this cbe::delegate::Error member, If the method
      // onLogInError() will not not be called, this default constructed state
      // implies no error. Otherwise, thus error object will contain the error
      // information. 
      cbe::delegate::Error error{};
      // Implement the cbe::delegate::LogInDelegate interface callbacks (private
      // cause they are only used by the SDK):
    private:
      // Called upon successful login.
      void onLogInSuccess(cbe::CloudBackend&& cloudBackend) final {
        {
          std::lock_guard<std::mutex> lock(mutex);
          // Put error member into the default constructed state to
          // indicate success (i.e., no error)
          error = cbe::delegate::Error{};
          // Change state of cloudBackend member to indicate success
          this->cloudBackend = std::move(cloudBackend);
          // Indicate operation completed, member cloudBackend non-default
          // constructed state indicates success
          called = true;
        }
        conditionVariable.notify_one();
      }
      // Called upon a failed login.
      void onLogInError(Error&& error, cbe::util::Context&& context) final {
        {
          std::lock_guard<std::mutex> lock(mutex);
          // Put cloudBackend member into the default constructed state to
          // indicate no-success
          cloudBackend = cbe::CloudBackend{ cbe::DefaultCtor{} };
          // Change state of error member to indicate failure
          this->error = std::move(error);
          // Indicate operation completed, member member cloudBackend default
          // constructed state indicates failure 
          called = true;
        }
        conditionVariable.notify_one();
      }
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    public:
      void wait() {
        std::unique_lock<std::mutex> lock(mutex);
        conditionVariable.wait(lock, [this]{ return called; });
        // Reset called flag, so current delegate instance can be reused
        called = false;
      }
    }; // struct MyLogInDelegate
    ~~~
    // Then,  carry out the login:
    static constexpr const char* const username = "~~~";
    static constexpr const char* const password = "~~~";
    static constexpr const char* const tenant   = "~~~";
    ~~~
    std::shared_ptr<MyLogInDelegate> myLogInDelegate =
                                      std::make_shared<MyLogInDelegate>();
    // Invoke the login (cast to return value to void to indicate is deliberately 
    // discarded)                                     
    (void) cbe::CloudBackend::logIn(username, password, tenant, myLogInDelegate);
    // Await the login procedure to run to completion
    myLogInDelegate->wait();

    // Test if login failed
    if (myLogInDelegate->error) {
      // Failed to login!
      std::cout << "Failed to login: error={" << myLogInDelegate->error << '}'
                << std::endl;
      //  Bail out!
      ~~~
    }
    // Since we have a complete CloudBackend object now, copy it from the delegate 
    cbe::CloudBackend cloudBackend = myLogInDelegate->cloudBackend;
    // Dispose the delegate object, not needed anymore
    myLogInDelegate.reset();
    // Do your stuff
    ~~~
    // End of session, tell cbe that you are terminating
    cloudBackend.terminate();
  }
   * @endcode
   */
  static cbe::CloudBackend logIn(const std::string&       username,
                                 const std::string&       password,
                                 const std::string&       tenant,
                                 LogInDelegatePtr         delegate);
  /**
   * @brief Logs in to the CloudBackend service
   * The authentication to the account is determined by \p username,
   * \p password, \p tenant and \p client.<br>
   * <b>Asynchronous</b> version of this service function.
   *
   * @param username  Name of the user to be signed in.
   * @param password  Password for the user to be signed in.
   * @param tenant    The identifier for the tenant, formerly known as
   *                  \c source.
   * @param client    The identifier for the client or an empty string
   * @param delegate  Pointer to a LogInDelegate instance that is
   *                  implemented by the user to receive the response of this
   *                  login request.<br>
   *                  This is accomplished in terms of the LogInDelegate
   *                  callback functions
   *                  @ref delegate::LogInDelegate::onLogInSuccess(CloudBackend&&)
   *                       "onLogInSuccess()" and
   *                  @ref delegate::LogInDelegate::onLogInError(LogInDelegate::Error&&,cbe::util::Context&&)
   *                       "onLogInError()".
   *
   * @par Example of asynchronous login
   * @code
  #include "cbe/CloudBackend.h"

  int main() {
    static constexpr const char* const username = "~~~";
    static constexpr const char* const password = "~~~";
    static constexpr const char* const tenant   = "~~~";
    static constexpr const char* const client   = "~~~";

    struct MyDelegate : cbe::CloudBackend::LogInDelegate {
      void onLogInSuccess(cbe::CloudBackend&& cloudBackend) final {
        // ~~~
      }
      void onLogInError(Error&& error, cbe::util::Context&& context) final {
        // Type of parameter error actually is:
        // cbe::CloudBackend::LogInDelegate::Error&&
        // ~~~
      }
    }; // struct MyDelegate

    const auto delegate = std::make_shared<MyDelegate>();
    auto cloudBackend =
        cbe::CloudBackend::logIn(username, password, tenant, client, delegate);
    // ~~~
  }
   * @endcode
   */
  static cbe::CloudBackend logIn(const std::string&       username,
                                 const std::string&       password,
                                 const std::string&       tenant,
                                 const std::string&       client,
                                 LogInDelegatePtr         delegate);
#ifndef CBE_NO_SYNC
  /**
   * \see LogInDelegate::Exception
   */
  using LogInException = delegate::LogInDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * logIn(const std::string&,const std::string&,const std::string&,
   *       LogInDelegatePtr)
   * , and <b>throws an exception</b>, #LogInException, in case of a failed
   * login.
   * \see logIn(const std::string&,const std::string&,const std::string&,
   *            LogInDelegatePtr)
   *
   * @return A CloudBackend instance \- if login was successful.
   * @throws #LogInException
   */
  static cbe::CloudBackend logIn(const std::string&  username,
                                 const std::string&  password,
                                 const std::string&  tenant);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * <b>Synchronous</b> version of
   * logIn(const std::string&,const std::string&,const std::string&,
   *       const std::string&,LogInDelegatePtr)
   * , and <b>throws an exception</b>, #LogInException, in case of a failed
   * login.
   * \see logIn(const std::string&,const std::string&,const std::string&,
   *            const std::string&,LogInDelegatePtr)
   *
   * @return A CloudBackend instance \- if login was successful.
   * @throws #LogInException
   */
  static cbe::CloudBackend logIn(const std::string&  username,
                                 const std::string&  password,
                                 const std::string&  tenant,
                                 const std::string&  client);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Forms the type of the \p error parameter in synchronous method
   * logIn(const std::string&,const std::string&,const std::string&,LogInError&)
   * \see LogInDelegate::ErrorInfo
   */
  using LogInError = delegate::LogInDelegate::ErrorInfo;
  /**
   * <b>Synchronous</b> version of
   * logIn(const std::string&,const std::string&,
   *       const std::string&,
   *       LogInDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \c error is used to provide the error information.
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed login. <br>
   *              The return value, of type #CloudBackendOpt, will indicate
   *              <code><b>false</b></code> on a failed login, and the
   *              LogInError object passed in will we be populated with the
   *              error information.
   *
   * @return If empty, <code><b>false</b></code>, an error has occurred.
   *         I.e., the login has failed, and the error information has been
   *         passed out via the \p error out/return parameter.
   */
  static cbe::CloudBackend logIn(const std::string& username,
                                 const std::string& password,
                                 const std::string& tenant,
                                 LogInError&        error);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * <b>Synchronous</b> version of
   * logIn(const std::string&,const std::string&,
   *       const std::string&,const std::string&,
   *       LogInDelegatePtr)
   * , and <b>throws <u>no</u> exception</b> on error, instead the out/return
   * parameter \c error is used to provide the error information.
   *
   * @param[out] error
   *              Return parameter containing the error information in case
   *              of a failed login. <br>
   *              The return value, of type #CloudBackendOpt, will indicate
   *              <code><b>false</b></code> on a failed login, and the
   *              LogInError object passed in will we be populated with the
   *              error information.
   *
   * @return If empty, <code><b>false</b></code>, an error has occurred.
   *         I.e., the login has failed, and the error information has been
   *         passed out via the \p error out/return parameter.
   */
  static cbe::CloudBackend logIn(const std::string& username,
                                 const std::string& password,
                                 const std::string& tenant,
                                 const std::string& client,
                                 LogInError&        error);

#endif // #ifndef CBE_NO_SYNC
  /**
   * Pointer to CreateAccountDelegate that is passed into method 
   * @ref createAccount()
   * 
   */
  using CreateAccountDelegatePtr = delegate::CreateAccountDelegatePtr;
  /**
  * @brief Creates a user account 
  * @param username   Username for the new account
  * @param password   Password for the new account
  * @param email      Email address to owner of the new account
  * @param firstName  First name of the owner of the new account
  * @param lastName   Last name of the owner of the new account
  * @param tenant     The identifier for the tenant, formerly known as
  *                   \c source.
  * @param client     Tenant client or an empty string
  * @param delegate   Pointer to a delegate::CreateAccountDelegate instance 
  *                   that is implemented by the user.
  */
  static cbe::CloudBackend createAccount(const std::string&       username,
                                         const std::string&       password,
                                         const std::string&       email,
                                         const std::string&       firstName,
                                         const std::string&       lastName,
                                         const std::string&       tenant,
                                         const std::string&       client,
                                         CreateAccountDelegatePtr delegate);

  /**
   * Pointer to delegate::CloudBackendListenerDelegate.
   */
  using CloudBackendListenerDelegatePtr =
                        std::shared_ptr<delegate::CloudBackendListenerDelegate>;
  /**
   * @brief Handle identifying a registered Listener.
   */
  using ListenerHandle = std::uint64_t;
  /**
  * Adds a listener that will receive updates as changes occur on the account.  
  * removeListener should always be called when you stop using the delegate.<br>
  * @param listener Delegate is a shared pointer to the class
  *                 delegate::CloudBackendListenerDelegate
  *                 that the user has implemented
  * @return Handle identifying the registration of the listener.<br>
  *         Used when deregistering with method removeListener(ListenerHandle).
  */
  ListenerHandle addListener(CloudBackendListenerDelegatePtr listener);

  /**
  * removes a listener that will receive updates as changes occur on the account
  * @param handle  Handle previously retrieved from method
  *                addListener(CloudBackendListenerDelegatePtr).
  */
  void removeListener(ListenerHandle handle);

  /**
   * Pointer to QueryDelegate that is passed into method 
   * @ref query()
   * 
   */  
  using QueryDelegatePtr = delegate::QueryDelegatePtr; 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /**
   * Inquires for a set of @ref Item "Item"s from the provided %Container,
   * identified by \p containerId.
   * Response is delivered asynchronously via the delegate::QueryDelegate
   * callback interface passed in as argument via parameter 
   * \p queryDelegate .<br>
   * This overload of join() accepting a
   * @ref delegate::QueryDelegate "QueryDelegate"-pointer as parameter has two
   * use cases:
   * <ul>
   * <li> To make a solely query() call <b>without</b> an additional chained
   *      @ref QueryChain::join(Container,std::string,std::string,delegate::JoinDelegatePtr) "join()"-call.
   * <li> To make a
   *      @ref query(ContainerId,delegate::QueryJoinDelegatePtr) "query()" call
   *      <b>with</b> a desired chained join() call.<br>
   *      In this latter case, the overloaded 
   *      @ref query(ContainerId,delegate::QueryJoinDelegatePtr)
   *      "query()"-functions, \n 
   *      query(ContainerId,delegate::QueryJoinDelegatePtr) and \n 
   *      query(ContainerId,Filter,delegate::QueryJoinDelegatePtr) , \n 
   *      both accepting a 
   *      @ref delegate::QueryJoinDelegate "QueryJoinDelegate" 
   *      as \p delegate argument, must be used.
   * </ul>
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
   *   @ref delegate::QueryDelegate::onQuerySuccess(delegate::QueryDelegate::Success&&) "onQuerySuccess()" or
   *   @ref delegate::QueryDelegate::onQueryError(delegate::QueryDelegate::Error&&,cbe::util::Context&&) "onQueryError()"
   *   will be called in the event of success, or failure respectively.
   *
   * @anchor anchorEx__cbe__CloudBackend__query 
   * @par Example of asynchronous query
   * To use the code below, we must first login, and then declare a
   * @ref cbe::delegate::QueryDelegate "QueryDelegate" class.<br>
   * See:
   * <ul>
   *   <li> @ref anchorEx__cbe__CloudBackend__logIn
   *             "Example of asynchronous login"<br>
   *             Here, we retrieve the \c cloudBackend object. 
   *   <li> @ref anchorEx__cbe__delegate_QueryDelegate
   *             "Example of a QueryDelegate implementation class"<br>
   *             Here, class MyQueryDelegate is defined. 
   * </ul> 
   * @code
  ~~~
  std::shared_ptr<MyQueryDelegate> queryDelegate =
                                     std::make_shared<MyQueryDelegate>();
  const cbe::ContainerId myContainerId =
                                    cloudBackend.account().rootContainer().id();
  cloudBackend.query(myContainerId, queryDelegate);
  queryDelegate->wait();
  if (queryDelegate->errorInfo) {
    std::cout << "Query failed:" << std::endl;
    std::cout << "ErrorInfo = " << queryDelegate->errorInfo << std::endl;
  } else {
    cbe::QueryResult::ItemsSnapshot itemsSnapshot = queryDelegate->queryResult.getItemsSnapshot();
    for (auto& item : itemsSnapshot) {
        std::cout << item.name() << std::endl;
    }
  }
  ~~~
   * @endcode
   */
  QueryChain query(ContainerId      containerId,
                   QueryDelegatePtr queryDelegate);
  /**
   * Same as query(ContainerId,delegate::QueryDelegatePtr), but with an
   * additional filter parameter \p filter.
   *
   * @param filter Filter specifying the constraints of the requested items.
   */
  QueryChain query(ContainerId      containerId,
                   Filter           filter,
                   QueryDelegatePtr queryDelegate);

  /**
   * Pointer to QueryJoinDelegate that is passed into method 
   * @ref query()
   * 
   */  
  using QueryJoinDelegatePtr = delegate::QueryJoinDelegatePtr;                   
  /**
   * Same as query(ContainerId,delegate::QueryDelegatePtr), but with a
   * @ref delegate::QueryJoinDelegate "QueryJoinDelegate" as delegate,
   * \p queryJoinDelegate.
   * \see query(ContainerId,delegate::QueryDelegatePtr)
   *
   * @param queryJoinDelegate
   * Pointer to a @ref delegate::QueryJoinDelegate "QueryJoinDelegate"
   * instance, implemented by the user, that receives the response of
   * this query request.<br>
   * I.e., either the 
   * @ref delegate::QueryJoinDelegate "QueryJoinDelegate"
   * callback function
   * @ref delegate::QueryJoinDelegate::onSuccess(
   *                                     delegate::QueryJoinDelegate::Success&&)
   *      "onSuccess()", \n 
   * or
   * @ref delegate::QueryJoinDelegate::onError(
   *                                       delegate::QueryJoinDelegate::Error&&,
   *                                       cbe::util::Context&&)
   *      "onError()".
   * will be called in the event of success, or failure respectively.
   */
  QueryChainExt query(ContainerId           containerId,
                      QueryJoinDelegatePtr  queryJoinDelegate);
  /**
   * Same as query(ContainerId,delegate::QueryJoinDelegatePtr), but with an
   * additional Filter parameter \p filter.
   *
   * @param filter Filter specifying the constraints of the requested items.
   */
  QueryChainExt query(ContainerId           containerId,
                      Filter                filter,
                      QueryJoinDelegatePtr  queryJoinDelegate);

#ifndef CBE_NO_SYNC
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /*! \see delegate::QueryDelegate::Exception */
  using QueryException = delegate::QueryDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * query(ContainerId,delegate::QueryDelegatePtr), and <b>throws an
   * exception</b>, #QueryException, in case of a failed query.
   * \see query(ContainerId,delegate::QueryDelegatePtr)
   *
   * @return Upon success, a QueryResult object - in fact an instance of
   *         QueryChainSync that is a subclass of QueryResult.
   * @throws #QueryException
   */
  QueryChainSync query(ContainerId  containerId);

  /**
   * Extended version of query(ContainerId) with an  additional filter parameter
   * \p filter.
   *
   * @param filter Filter specifying the constraints of the requested items.
   */
  QueryChainSync query(ContainerId  containerId,
                       Filter       filter);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /*! \see delegate::QueryJoinDelegate::ErrorInfo */
  using QueryJoinError = delegate::QueryJoinDelegate::ErrorInfo;
  /**
   * In line with synchronous query(ContainerId), but <b>throws
   * <u>no</u> exception</b> on error, instead the out/return parameter \c error
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
   */
  QueryChainSync query(ContainerId      containerId,
                       QueryJoinError&  error);

  /**
   * Extended version of query(ContainerId,QueryJoinError&) with an  additional
   * filter parameter \p filter.
   * \see query(ContainerId,QueryJoinError&)
   */
  QueryChainSync query(ContainerId      containerId,
                       Filter           filter,
                       QueryJoinError&  error);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  /*! \see delegate::QueryDelegate::ErrorInfo */
  using QueryError = delegate::QueryDelegate::ErrorInfo;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif // #ifndef CBE_NO_SYNC

  /**
   * @brief Queries for a container with a given path.
   * \note <code>..</code> or <code>.</code> relative path options are not
   *       permitted. Only top down search from start point, \p queryRoot id, to
   *       downwards path in container tree.
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
   * @brief Queries for a container with a given path relative the home root
   *        container.
   * 
   * Same as queryWithPath(std::string, cbe::ContainerId, QueryDelegatePtr),
   * but with the <code>queryRoot</code> parameter omitted. Instead, 
   * here the home root container is used as top container in the search tree.
   */
  cbe::QueryChain queryWithPath(std::string         relativePath,
                                QueryDelegatePtr    delegate);
   
  /**
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
   * Search the whole container for tags related to Objects in the container 
   * structure. \n 
   * E.g., <c>Key = Name, Value Contract/Object/Song => Name:Contract1 </c>.
   *
   * Search handles tags in combination / conjunction of keys and/or key values
   * seperated by |. \n 
   * E.g., Name:*|Country:Sweden|Country:Norway, this would search for objects
   * with key Name but any value and where key Country is either Sweden or 
   * Norway.
   * @param filter is a cbe::Filter on which you can set how you want data to be 
   * ordered when searching, remember to set the queryString to be keys/tags or 
   * key:value pairs that are seperated by |.
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

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  /**
  * Send REST API request:
  * @param uri set uri with request E.g., /v1/users/{user_id}
  * @param requestType set the request type e.g: GET, POST, PUT, DELETE.
  * @param contentType e.g.: text/xml, application/octet-stream, 
  * application/x-www-form-urlencoded; charset=utf-8, application/xml.
  * @param delegate is the callback pointer to where the API returns from 
  * server/edge node. \n 
  * Implement APIEventProtocol and the callback 
  * onAPIRespons(std::string respons).
  */
  // void restRequest(std::string          uri,
  //                  cbe::http_t          requestType,
  //                  std::string          contentType,
  //                  cbe::APIDelegatePtr  delegate);

  /**
  * Send SOAP API request:
  * \note No need to add SOAP prefix or suffix, this is handled by the sdk. 
  * only the body and request action/call is needed here.
  * @param body set the body of the request.
  * @param soapAction set api call ex queryFolder
  * @param delegate is the callback pointer to where the API returns from 
  * server/edge node. \n 
  * Implement APIEventProtocol and the callback 
  * onAPIRespons(std::string respons).
  */

  // void soapRequest(std::string          body,
  //                  std::string          soapAction,
  //                  cbe::APIDelegatePtr  delegate);
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

  /**
   * @brief Casts an item to a container
   * If the provided cbe::Item "item" is not a cbe::Container "container", an
   * empty container instance is returned
   */
  static cbe::Container castContainer(cbe::Item item);

  /**
   * @brief Casts an item to an object
   * If the provided cbe::Item "item" is not a cbe::Object "object", an
   * empty object instance is returned
   */
  static cbe::Object castObject(cbe::Item item);

  /**
   * Clears the cache if you are having memory issues.
   * 
   * @return true  success
   * @return false failed
   */
  bool clearCache();

  /**
   * @brief Returns an account objct with information on the user
   * 
   * @return cbe::Account 
   */
  cbe::Account account();


  /**
   * @brief Returns the version number of the SDK
   * 
   * @return std::string 
   */
  std::string version() const;

  /**
   * @brief Gives the groups available
   * 
   * @return cbe::GroupManager 
   */
  cbe::GroupManager     groupManager();  // Group - NYI

  /**
   * gives the shares available
   * 
   * @return cbe::ShareManager 
   */
  cbe::ShareManager     shareManager(); // Shares

  /**
   * gives the published web shares
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
   * gives the subscriptions being followed
   * 
   * @return cbe::SubscribeManager 
   */
  cbe::SubscribeManager subscribeManager(); // Subscribe (following)

  /**
   * Construct a new \c CloudBackend object with 
   * the \c DefaultCtor to enable the @c bool() test
   */
  CloudBackend(cbe::DefaultCtor);
  ~CloudBackend();

  /**
   * @brief Checks if current \c CloudBackend instance is real.
   * 
   * An "unreal" \c CloudBackend instance implies typically the event of a 
   * failed login.
   * 
   * @return true  : is real
   * @return false : got nullptr
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
