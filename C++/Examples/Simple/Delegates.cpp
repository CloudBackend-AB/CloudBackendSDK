/*
  Copyright © CloudBackend AB 2022 - 2023.
*/

#include <condition_variable>
#include <iostream>
#include <mutex>

#include "cbe/Account.h"
#include "cbe/CloudBackend.h"
#include "cbe/Container.h"
#include "cbe/QueryChain.h"
#include "cbe/Types.h"

#include "cbe/delegate/CreateContainerDelegate.h"
#include "cbe/delegate/LogInDelegate.h"
#include "cbe/delegate/QueryDelegate.h"
#include "cbe/delegate/TransferError.h"
#include "cbe/delegate/UploadDelegate.h"
#include "cbe/delegate/container/RemoveDelegate.h"

//------------------------------------------------------------------------------

class LogInDelegate :  public cbe::delegate::LogInDelegate
{
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};
  bool                    called = false;

  void onLogInSuccess(cbe::CloudBackend&& cloudBackend) final {
    {
    std::lock_guard<std::mutex> lock(mutex);
    this->cloudBackend = std::move(cloudBackend); 
    called = true;
    }
    conditionVariable.notify_one();
  }

  void onLogInError(cbe::delegate::Error&& error, cbe::util::Context&& context) final {
    {
    std::lock_guard<std::mutex> lock(mutex);
    errorInfo = ErrorInfo{std::move(context), std::move(error)};
    called = true;
    }
    conditionVariable.notify_one();
  }

  public:
  // Implementation of delegate
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
};  // class LogInDelegate

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
  void onQuerySuccess(cbe::QueryResult&& queryResult) final {
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
  void onQueryError(cbe::delegate::QueryError&&         error,
                    cbe::util::Context&&                context) final{
    {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  };

  public:
  // Implementation of delegate
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
};  // class QueryDelegate

//------------------------------------------------------------------------------

class UploadDelegate :  public cbe::delegate::UploadDelegate
{
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};
  bool                    called = false;

  /**
   * Called upon successful Upload.
   * @param object Instance of object that is being Uploadd.
   */
  void onUploadSuccess(cbe::Object&& object){
    {      
    std::lock_guard<std::mutex> lock(mutex);
    this->object = std::move(object); 
    called = true;
    }           
    conditionVariable.notify_one();
  };

  void onUploadError(cbe::delegate::TransferError&& error, 
                      cbe::util::Context&&         context){
    {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  };

  public:
  // Implementation of delegate
  cbe::Object object{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for query" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
    // Reset called flag, so current delegate instance can be reused
    called = false;
  }
};  // class UploadDelegate

//------------------------------------------------------------------------------

class CreateContainerDelegate :  public cbe::delegate::CreateContainerDelegate
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
  };
  void onCreateContainerError(Error&& error, cbe::util::Context&& context) {
    {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  };

  public:
  // Implementation of delegate
  cbe::Container container{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for query" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
    // Reset called flag, so current delegate instance can be reused
    called = false;
  }
};  // class CreateContainerDelegate

//------------------------------------------------------------------------------

class RemoveContainerDelegate :  public cbe::delegate::container::RemoveDelegate
{
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};
  bool                    called = false;

  void onRemoveSuccess(cbe::ItemId containerId, std::string name) {
    {      
    std::lock_guard<std::mutex> lock(mutex);
    called = true;
    }           
    conditionVariable.notify_one();      
  };
  void onRemoveError(Error&& error, cbe::util::Context&& context) {
    {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  };

  public:
  // Implementation of delegate
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for deletion" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
    // Reset called flag, so current delegate instance can be reused
    called = false;
  }
};  // class RemoveContainerDelegate

//------------------------------------------------------------------------------
