/*
      CloudBackend AB 2021.
*/

#ifndef INCLUDE_cbe_UPLOAD_DATA_H_
#define INCLUDE_cbe_UPLOAD_DATA_H_


#include "cbe/Types.h"
#include "cbe/CloudBackend.h"
#include "cbe/Account.h"

#include "cbe/delegate/LogInDelegate.h"
#include "cbe/delegate/TransferError.h"

#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>

class LogInDelegate : public cbe::delegate::LogInDelegate
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
  /*implementation of delegates */
  cbe::CloudBackend cloudBackend{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, to be logged in" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
  }  
};

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
  void onQueryError(cbe::delegate::QueryError&&         error,
                    cbe::util::Context&&                context) {
    {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  };

public:
  /*implementation of delegates */
  cbe::QueryResult queryResult{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for query" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
  }
};

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
  /*implementation of delegates */
  cbe::Object object{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for upload" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have uploaded: " << called << std::endl;
  }

  /** SDK requests, user defined functionallity: 
   *  Feel free to implement more functionallity using the library as you please.
  */

  /** Example of how to implement the library call to upload. */
  void uploadToContainer(std::string path, std::string name, cbe::Container container);

  /** Temporarily saving the name of the container that we want to create, to see if it is already existing on the account.  */
  std::string _queryName;
};

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
  /*implementation of delegates */
  cbe::Container container{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for create" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have created: " << called << std::endl;
  }

  /** Temporarily saving the name of the container that we want to create, to see if it is already existing on the account.  */
  std::string _queryName;
};

/** Example of how to implement the library call to query. */
void queryContainer(cbe::Container parentContainer, std::string name);

/** Example of how to implement the library call to createContainer. */
cbe::Container createContainer(cbe::Container parentContainer, std::string name);

/** Example of how to implement the library call to upload. */
void uploadToContainer(std::string path, std::string name, cbe::Container container);

/** Temporarily saving the name of the container that we want to create, to see if it is already existing on the account.  */
std::string _queryName;

void read_csv(cbe::Container this_container);

void upload_file(cbe::Container this_container);

void write_summary(cbe::Container this_container);


#endif
