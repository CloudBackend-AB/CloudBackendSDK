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
#include "cbe/delegate/DownloadDelegate.h"
#include "cbe/delegate/DownloadSuccess.h"

// - - - - - - - - - - - - - - - - - DELEGATES - - - - - - - - - - - - - - - - - 

struct MyLogInDelegate : cbe::delegate::LogInDelegate
{
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};

  bool                    called = false;
  cbe::delegate::Error error{};

  void onLogInSuccess(cbe::CloudBackend&& cloudBackend) final {
    {
      std::lock_guard<std::mutex> lock(mutex);
      this->cloudBackend = std::move(cloudBackend); 
      error = cbe::delegate::Error{};
      called = true;
    }
    conditionVariable.notify_one();
  }

  void onLogInError(cbe::delegate::Error&&  error, 
                    cbe::util::Context&&    context) final {
    {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      this->error = std::move(error);
      called = true;
    }
    conditionVariable.notify_one();
  }
  cbe::CloudBackend cloudBackend{cbe::DefaultCtor{}}; 
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "Waiting, to be logged in" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    std::cout << "Now we have waited: " << called << std::endl;
    called = false;
  }

}; // struct MyLogInDelegate 

//------------------------------------------------------------------------------

struct MyQueryDelegate : cbe::delegate::QueryDelegate
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

  cbe::QueryResult  queryResult{cbe::DefaultCtor{}};
  ErrorInfo         errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "Waiting, for query" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    std::cout << "Now we have waited: " << called << std::endl;
    called = false;
  }
}; // struct MyQueryDelegate

//------------------------------------------------------------------------------

struct MyCreateContainerDelegate : cbe::delegate::CreateContainerDelegate
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

  /*implementation of delegates */
  cbe::Container container{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "Waiting, for create container" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    std::cout << "Now we have waited: " << called << std::endl;
    called = false;
  }
}; // struct MyCreateContainerDelegate

//------------------------------------------------------------------------------

struct MyCreateObjectDelegate : cbe::delegate::CreateObjectDelegate
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

  /*implementation of delegates */
  cbe::Object object{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "Waiting, for create object" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    std::cout << "Now we have waited: " << called << std::endl;
    called = false;
  }
}; // struct MyCreateObjectDelegate

//------------------------------------------------------------------------------
struct MyDownloadDelegate : cbe::delegate::DownloadDelegate {
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};
  bool                    called{};

  Success success{};

  void onDownloadSuccess(cbe::Object&& object,
                                std::string   path) final {
    {
      std::lock_guard<std::mutex> lock{mutex};
      success = Success{std::move(object), std::move(path)};
      called = true;
    }
    conditionVariable.notify_one();
  }

  void onDownloadError(cbe::delegate::TransferError&&        error,
                              cbe::util::Context&&           context) final {
    {
      std::lock_guard<std::mutex> lock{mutex};
      success = Success{};
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  }

  ErrorInfo errorInfo{};
  cbe::Object waitForRsp() {
    std::unique_lock<std::mutex> lock{mutex};
    conditionVariable.wait(lock, [this]{ return called; } );
    called = false;
    return success.object;
  }
}; // struct MyDownloadDelegate

//------------------------------------------------------------------------------
struct MyGetStreamsDelegate : cbe::delegate::GetStreamsDelegate {
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};
  bool                    called{};

  cbe::util::Optional<cbe::Streams> streams{};

  void onGetStreamsSuccess(cbe::Streams&& streams) final {
    {
      std::lock_guard<std::mutex> lock{mutex};
      this->streams = std::move(streams);
      called = true;
    }
    conditionVariable.notify_one();
  }

  void onGetStreamsError(cbe::delegate::Error&& error,
                          cbe::util::Context&&   context) final {
    {
      std::lock_guard<std::mutex> lock{mutex};
      streams.reset();
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  }
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock{mutex};
    conditionVariable.wait(lock, [this]{ return called; } );
    called = false;
  }
}; // struct MyGetStreamsDelegate
// - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - - - - - -
  struct MyUploadDelegate : cbe::delegate::UploadDelegate {
    std::mutex              mutex{};
    std::condition_variable conditionVariable{};
    bool                    called{};
    cbe::Object object{ cbe::DefaultCtor{} };

    void onUploadSuccess(cbe::Object&& object) final {
      {
        std::lock_guard<std::mutex> lock{mutex};
        this->object = std::move(object);
        called = true;
      }
      conditionVariable.notify_one();
    }

    void onUploadError(cbe::delegate::TransferError&& error,
                       cbe::util::Context&&           context) final {
      {
        std::lock_guard<std::mutex> lock{mutex};
        object = cbe::Object{ cbe::DefaultCtor{} };
        errorInfo = ErrorInfo{std::move(context), std::move(error)};
        called = true;
      }
      conditionVariable.notify_one();
    }

    ErrorInfo errorInfo{};

    cbe::Object waitForRsp() {
      std::unique_lock<std::mutex> lock{mutex};
      conditionVariable.wait(lock, [this]{ return called; } );
      called = false;
      return std::move(object);
    }
  }; // struct MyUploadDelegate
// - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - - - - - -
  struct MyUploadStreamDelegate : cbe::delegate::UploadDelegate {
    std::mutex              mutex{};
    std::condition_variable conditionVariable{};
    bool                    called{};
    cbe::Object object{ cbe::DefaultCtor{} };

    void onUploadSuccess(cbe::Object&& object) final {
      {
        std::lock_guard<std::mutex> lock{mutex};
        this->object = std::move(object);
        called = true;
      }
      conditionVariable.notify_one();
    }

    void onUploadError(cbe::delegate::TransferError&& error,
                       cbe::util::Context&&           context) final {
      {
        std::lock_guard<std::mutex> lock{mutex};
        object = cbe::Object{ cbe::DefaultCtor{} };
        errorInfo = ErrorInfo{std::move(context), std::move(error)};
        called = true;
      }
      conditionVariable.notify_one();
    }

    ErrorInfo errorInfo{};

    void waitForRsp() {
      std::unique_lock<std::mutex> lock{mutex};
      conditionVariable.wait(lock, [this]{ return called; } );
      called = false;
    }
  }; // struct MyUploadDelegate