/*
      Copyright © CloudBackend AB 2022.
*/

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <stdio.h>
#include <thread>
#include <string>

#include "user_credentials.cpp"  // file is located in this folder

#include "cbe/Account.h"
#include "cbe/CloudBackend.h"
#include "cbe/Container.h"
#include "cbe/QueryChain.h"
#include "cbe/Types.h"

#include "cbe/delegate/LogInDelegate.h"
#include "cbe/delegate/QueryDelegate.h"
#include "cbe/delegate/UploadDelegate.h"
#include "cbe/delegate/CreateContainerDelegate.h"
#include "cbe/delegate/TransferError.h"

#include "cbe/delegate/container/RemoveDelegate.h"

  //----------------------------------------------------------------------------

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
  /*implementation of delegates */
  cbe::CloudBackend cloudBackend{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void wait() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, to be logged in" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
  }
};  // class LogInDelegate

  //----------------------------------------------------------------------------

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
  /*implementation of delegates */
  cbe::QueryResult queryResult{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void wait() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for query" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
  }

  /** SDK requests, user defined functionallity: 
   *  Feel free to implement more functionallity using the library as you please.
  */

  /** Example of how to implement the library call to query. */
  void query(cbe::Container container, std::string name);
};  // class QueryDelegate

  //----------------------------------------------------------------------------

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

  void wait() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for query" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
  }

  /** Example of how to implement the library call to upload. */
  void uploadToContainer(std::string path, std::string name, cbe::Container container);
};  // class UploadDelegate

  //----------------------------------------------------------------------------

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

  void wait() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for query" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
  }
};  // class CreateContainerDelegate

  //----------------------------------------------------------------------------

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
  /*implementation of delegates */
  ErrorInfo errorInfo{};

  void wait() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for deletion" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
  }
};  // class RemoveContainerDelegate


  //----------------------------------------------------------------------------
  //============================================================================
  //----------------------------------------------------------------------------

int main(void) {
  printf("Starting SimplEx program.\n");

  /** Creates a new instance of this class which we need to access the 
   * cloudBackend object from.
   */
  std::shared_ptr<LogInDelegate> logInDelegate = 
                                              std::make_shared<LogInDelegate>();
  /** Login in to the account that you where provided with when downloading 
   *  this example code. 
   *  username and password can be found in the scripts and binary folder in 
   *  user_credentials.cpp if you wish to change the credentials.
   *  The API call to logIn can be found in CloudBackend.h in the include folder.
   *  The respond will either be onLoginSuccess located down below in callbacks,
   *  or if the account does not exists 
   *  onLogInError(cbe::delegate::Error&& error, cbe::util::Context&& context). 
   */

  logInDelegate->cloudBackend = cbe::CloudBackend::logIn(username, 
                                                         password,
                                                         tenant, 
                                                         logInDelegate);
  /** Keeps the thread alive between API requests. */
  // Waiting for the cloud response
  logInDelegate->wait();

  // Check if login was without error
  if (!logInDelegate->errorInfo) {
    // Yes, success!  
    std::cout << "Logged in:" << std::endl;
    std::cout << "SDK version = "
              << logInDelegate->cloudBackend.version() << std::endl;
    std::cout << "   username = "
              << logInDelegate->cloudBackend.account().username() << std::endl;
    std::cout << "     userId = "
              << logInDelegate->cloudBackend.account().userId() << std::endl;
    std::cout << "     tenant = "
              << logInDelegate->cloudBackend.account().source() << std::endl;
    std::cout << "      first = "
              << logInDelegate->cloudBackend.account().firstName() << std::endl;
    std::cout << "       last = "
              << logInDelegate->cloudBackend.account().lastName() << std::endl;
  } 
  else {
    // No, error...
    std::cout <<"Exiting: Login failed!" << std::endl;
    std::cout <<"errorInfo = " << logInDelegate->errorInfo << std::endl;
    return 1; // Bail out - Due to failed login
  }

  //----------------------------------------------------------------------------

  std::shared_ptr<QueryDelegate> queryDelegate = 
                                              std::make_shared<QueryDelegate>();
  // Logged in users root container
  cbe::Container container = 
                          logInDelegate->cloudBackend.account().rootContainer();

  // We want to check if the container the user wants to create already exist in 
  // the parent container.
  std::cout << "Name for a new Container to be created: ";
  std::string containerName;
  std::getline(std::cin, containerName);
  logInDelegate->cloudBackend.query(container.id(), queryDelegate);
  // Waiting for the cloud response
  queryDelegate->wait();

  cbe::QueryResult::ItemsSnapshot resultset = 
                                  queryDelegate->queryResult.getItemsSnapshot();  
  // Look through the parent container to check if the name has already been 
  // used.
  std::cout << "Content of /" << std::endl;
  std::cout << "---------------------------" << std::endl;
  for (cbe::Item& item : resultset)
  {
    std::cout << item.name() << std::endl;
    if(item.name() == containerName){
      std::cout << "Exiting: Container already exist! " << item.name() << " (" 
                << item.id() << ")" << std::endl;
      return 2;
    }
  }
  std::cout << "---------------------------" << std::endl;

  // Check if error
  if (queryDelegate->errorInfo) {
    // Yes, error...
    std::cout <<"Exiting: Query failed!" << std::endl;
    std::cout <<"errorInfo = " << queryDelegate->errorInfo << std::endl;
    return 3; // Bail out - Due to failed query
  }

  //----------------------------------------------------------------------------

  std::shared_ptr<CreateContainerDelegate> createContainerDelegate = 
                                    std::make_shared<CreateContainerDelegate>();
  // Create a container in the parent container using the name that the user
  // provided
  container.createContainer(containerName, createContainerDelegate);
  // Waiting for the cloud response
  createContainerDelegate->wait();

  // Check if error
  if (createContainerDelegate->errorInfo) {
    // Yes, error... 
    std::cout <<"Exiting: CreateContainer failed!" << std::endl;
    std::cout <<"errorInfo = " << createContainerDelegate->errorInfo 
              << std::endl;
    return 4; // Bail out - Due to failed CreateContainer
  }

  cbe::Container newContainer = createContainerDelegate->container;
  std::cout << "/" << newContainer.name() << " (" << newContainer.id()
            << ")\t created." << std::endl;
 
  //----------------------------------------------------------------------------

  std::shared_ptr<UploadDelegate> uploadDelegate = 
                                             std::make_shared<UploadDelegate>();
  // Here we define which file to upload from the local system.
  std::string fileName = "w2.xml";
  // Here we define the file's relative path on the local system. 
  // Note: path must end with "/".
  std::string filePath = "Upload_files/";

  // Upload the file to the newly created container.
  newContainer.upload(fileName, filePath, uploadDelegate);
  // Waiting for the cloud response
  uploadDelegate->wait();

  // Check if error
  if (uploadDelegate->errorInfo) {
    // Yes, error...
    std::cout <<"Exiting: Upload failed!" << std::endl;
    std::cout <<"errorInfo = " << uploadDelegate->errorInfo << std::endl;
    return 5; // Bail out - Due to failed Upload
  }

  cbe::Object newObject = uploadDelegate->object;
  std::cout << "/" << newContainer.name() << "/" << newObject.name()
            << "\t uploaded." << std::endl;

  //----------------------------------------------------------------------------


  std::cout << "Do you want to delete your newly created container called \"";
  std::cout << newContainer.name() << "\"?" << std::endl;
  
  std::string shouldBeDeleted{};
  while (true)
  { 
    shouldBeDeleted = "";
    std::cout << "(y/n): ";
    std::getline(std::cin, shouldBeDeleted);

    if (shouldBeDeleted == "y"){
      std::cout << "Deleting container." << std::endl;
      std::shared_ptr<RemoveContainerDelegate> removeContainerDelegate = 
                                      std::make_shared<RemoveContainerDelegate>();
      
      newContainer.remove(removeContainerDelegate);
      removeContainerDelegate->wait();
      std::cout << "Container was deleted successfully!" << std::endl;
      break;
    } else if (shouldBeDeleted == "n")
    {
      std::cout << "The container will be kept." << std::endl;
      break;
    }
  }

  //----------------------------------------------------------------------------
  std::cout << "End of program!" << std::endl;
  logInDelegate->cloudBackend.terminate();
  return 0;
}  // main
