/*
  CloudBackend AB 2022 - 2023.
*/

#include <condition_variable>
#include <iostream>
#include <iterator>
#include <mutex>
#include <stdexcept>
#include <string>

#include "cbe/CloudBackend.h"
#include "cbe/Container.h"
#include "cbe/QueryResult.h"

#include "cbe/delegate/CreateContainerDelegate.h"
#include "cbe/delegate/LogInDelegate.h"
#include "cbe/delegate/QueryDelegate.h"

// - - - - - - - - - - - - - - - - - DELEGATES - - - - - - - - - - - - - - - - - 

class MyLogInDelegate :  public cbe::delegate::LogInDelegate
{
  // Mutex lock and condition variable are used to verify that operation has 
  // completed.
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};

  // Used to see if callback is reached
  bool                    called = false;

  // User implemented callback, return value is cbe::CloudBackend&&  
  void onLogInSuccess(cbe::CloudBackend&& cloudBackend) final {
    {
      // Lock thread during operation 
      std::lock_guard<std::mutex> lock(mutex);
      this->cloudBackend = std::move(cloudBackend); 
      called = true;
    }
    // Notify when operation is done to unlock
    conditionVariable.notify_one();
  }

  // User implemented error callback
  void onLogInError(cbe::delegate::Error&&  error, 
                    cbe::util::Context&&    context) final {
    {
      std::lock_guard<std::mutex> lock(mutex);
      // Set member variable errorInfo with information about fail
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  }
public:
  // Member variables for the delegate, will later be accessed through the 
  // delegate defined in the code
  cbe::CloudBackend cloudBackend{cbe::DefaultCtor{}}; 
  ErrorInfo errorInfo{};

  // Wait function is called on the delegate when it is being used 
  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "Waiting, to be logged in" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    std::cout << "Now we have waited: " << called << std::endl;
    // Reset called flag, so current delegate instance can be reused
    called = false;
  }

}; // class MyLogInDelegate 

//------------------------------------------------------------------------------

class MyQueryDelegate :  public cbe::delegate::QueryDelegate
{
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};
  bool                    called = false;

  /**
   * Called upon successful query.
   * @param queryResult Instance of a QueryResult containing the result set.
   */
  void onQuerySuccess(cbe::QueryResult&& queryResult) final {
    {      
      std::lock_guard<std::mutex> lock(mutex);
      this->queryResult = std::move(queryResult); 
      called = true;
    }           
    conditionVariable.notify_one();
  }

  /**
   * Called upon a failed query() or join() call.
   * @param error   Error information passed from %CloudBackend SDK.
   * @param context Additional context information about the original service
   *                call that has failed.
   */
  void onQueryError(cbe::delegate::QueryError&&         error,
                    cbe::util::Context&&                context) final {
    {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  }

public:
  /*implementation of delegates */
  cbe::QueryResult  queryResult{cbe::DefaultCtor{}};
  ErrorInfo         errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "Waiting, for query" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    std::cout << "Now we have waited: " << called << std::endl;
    // Reset called flag, so current delegate instance can be reused
    called = false;
  }
}; // class MyQueryDelegate

//------------------------------------------------------------------------------

class MyCreateContainerDelegate :  public cbe::delegate::CreateContainerDelegate
{
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};

  bool                    called = false;

  void onCreateContainerSuccess(cbe::Container&& container) final {
    {      
      std::lock_guard<std::mutex> lock(mutex);
      this->container = std::move(container); 
      called = true;
    }           
    conditionVariable.notify_one();      
  }
  void onCreateContainerError(Error&& error, cbe::util::Context&& context) final {
    {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  }

public:
  /*implementation of delegates */
  cbe::Container container{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "Waiting, for create container" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    std::cout << "Now we have waited: " << called << std::endl;
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
  /*implementation of delegates */
  cbe::Object object{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "Waiting, for create object" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    std::cout << "Now we have waited: " << called << std::endl;
    // Reset called flag, so current delegate instance can be reused
    called = false;
  }
}; // class MyCreateObjectDelegate

//------------------------------------------------------------------------------
