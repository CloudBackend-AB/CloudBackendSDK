#include "cbe/Account.h"
#include "cbe/CloudBackend.h"
#include "cbe/Filter.h"
#include "cbe/Types.h"

#include <condition_variable>
#include <iostream>
#include <mutex>

#include "cbe/delegate/LogInDelegate.h"
#include "cbe/delegate/CreateContainerDelegate.h"
#include "cbe/delegate/QueryDelegate.h"
#include "cbe/delegate/QueryJoinDelegate.h"
#include "cbe/delegate/container/RemoveDelegate.h"

class LogInDelegate :  public cbe::delegate::LogInDelegate
{
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};

  bool                    called = false;

  void onLogInSuccess(cbe::CloudBackend&& cloudBackend) {
     {
      std::lock_guard<std::mutex> lock(mutex);
      this->cloudBackend = std::move(cloudBackend); 
      called = true;
     }
     conditionVariable.notify_one();

   }
   void onLogInError(cbe::delegate::Error&& error, cbe::util::Context&& context) {
     {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
     }
     conditionVariable.notify_one();
   }
public:
  // implementation of delegates
  cbe::CloudBackend cloudBackend{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, to be logged in" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
    // Reset called flag, so current delegate instance can be reused
    called = false;
  }

}; // class LogInDelegate

//------------------------------------------------------------------------------

class QueryDelegate :  public cbe::delegate::QueryDelegate
{
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};
  bool                    called = false;

  /**
   * Called upon successful query.
   * @param queryResult Instance of a QueryResult containing the result set.
   */
  void onQuerySuccess(cbe::QueryResult&& queryResult) {
    {      
      std::lock_guard<std::mutex> lock(mutex);
      this->queryResult = std::move(queryResult); 
      called = true;
    }           
    conditionVariable.notify_one();
  };

  /**
   * Called upon a failed query() or join() call.
   * @param error   Error information passed from %CloudBackend SDK.
   * @param context Additional context information about the original service
   *                call that has failed.
   */
  void onQueryError(cbe::delegate::QueryError&& error,
                    cbe::util::Context&&        context){
    {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  };

public:
  // implementation of delegates
  cbe::QueryResult queryResult{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for query" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
    // Reset called flag, so current delegate instance can be reused
    called = false;
  }

  /** SDK requests, user defined functionality: 
   *  Feel free to implement more functionality using the library as you please.
  */

  /** Example of how to implement the library call to query. */
  // void query(cbe::Container container, std::string name);

  /** Temporarily saving the name of the container that we want to create, to see if it is already existing on the account.  */
  std::string _queryName;
}; // class QueryDelegate

//------------------------------------------------------------------------------

class QueryJoinDelegate :  public cbe::delegate::QueryJoinDelegate
{
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};
  bool                    called = false;

  /**
   * Called upon successful query.
   * @param queryResult Instance of a QueryResult containing the result set.
   */
  void onQueryJoinSuccess(cbe::QueryResult&& queryResult) {
    {      
      std::lock_guard<std::mutex> lock(mutex);
      this->queryResult = std::move(queryResult); 
      called = true;
    }           
    conditionVariable.notify_one();
  };

  /**
   * Called upon a failed query() or join() call.
   * @param error   Error information passed from %CloudBackend SDK.
   * @param context Additional context information about the original service
   *                call that has failed.
   */
  void onQueryJoinError(cbe::delegate::QueryError&&         error,
                        cbe::util::Context&&                context) {
    {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  };

public:
  // implementation of delegates
  cbe::QueryResult queryResult{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for query" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
    // Reset called flag, so current delegate instance can be reused
    called = false;
  }

  /** SDK requests, user defined functionality: 
   *  Feel free to implement more functionality using the library as you please.
  */

  /** Example of how to implement the library call to query. */
  void query(cbe::Container container, std::string name);

  /** Temporarily saving the name of the container that we want to create, to see if it is already existing on the account.  */
  std::string _queryName;
}; // class QueryJoinDelegate

//------------------------------------------------------------------------------

class MyCreateContainerDelegate :  public cbe::delegate::CreateContainerDelegate
{
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};

  bool                    called = false;

  void onCreateContainerSuccess(cbe::Container&& container) {
    {      
      std::lock_guard<std::mutex> lock(mutex);
      this->container = std::move(container); 
      called = true;
    }           
    conditionVariable.notify_one();      
  }
  void onCreateContainerError(Error&& error, cbe::util::Context&& context) {
    {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  }

public:
  // implementation of delegates
  cbe::Container container{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for create container" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
    // Reset called flag, so current delegate instance can be reused
    called = false;
  }
}; // class MyCreateContainerDelegate

//------------------------------------------------------------------------------

class MyCreateObjectDelegate :  public cbe::delegate::CreateObjectDelegate 
{

  std::mutex              mutex{};
  std::condition_variable conditionVariable{};

  bool                    called = false;

  void onCreateObjectSuccess(cbe::Object&& object) final {
    {      
      std::lock_guard<std::mutex> lock(mutex);
      this->object = std::move(object); 
      called = true;
    }           
    conditionVariable.notify_one();      
  }
  void onCreateObjectError(Error&& error, cbe::util::Context&& context) final {
    {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  }

public:
  // implementation of delegates
  cbe::Object object{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for create container" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
    // Reset called flag, so current delegate instance can be reused
    called = false;
  }
}; // class MyCreateObjectDelegate

//------------------------------------------------------------------------------

class RemoveDelegate : public cbe::delegate::container::RemoveDelegate 
{
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};
  bool                    called = false;
 
    /**
     * Called upon successful Remove.
     * @param objectId Instance of object that is being removed.
     * @param name Name of object that is being removed.
     */
    void onRemoveSuccess(cbe::ItemId containerId, std::string name) {
      {      
      std::lock_guard<std::mutex> lock(mutex);
      called = true;
      }           
      conditionVariable.notify_one();
    };
    
    /**
     * Called upon a failed remove() call.
     * @param error   Error information passed from %CloudBackend SDK.
     * @param context Additional context information about the original service
     *                call that has failed.
     */

    void onRemoveError(cbe::delegate::Error&& error, cbe::util::Context&& context) {
      {
        std::lock_guard<std::mutex> lock(mutex);
        errorInfo = ErrorInfo{std::move(context), std::move(error)};
        called = true;
      }
      conditionVariable.notify_one();
    };

    public:
    // implementation of delegates
    ErrorInfo errorInfo{};

    void waitForRsp() {
      std::unique_lock<std::mutex> lock(mutex);
      // std::cout << "Waiting, to be logged in" << std::endl;
      conditionVariable.wait(lock, [this] { return called; });
      // std::cout << "Now we have waited: " << called << std::endl;
      // Reset called flag, so current delegate instance can be reused
      called = false;
    }
  }; // class RemoveDelegate
