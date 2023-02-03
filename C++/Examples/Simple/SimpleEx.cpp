/*
  Copyright © CloudBackend AB 2022 - 2023.
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

#include "Delegates.cpp"


int main(void) {
  printf("Starting SimplEx program.\n");
  cbe::CloudBackend cloudBackend{cbe::DefaultCtor{}};

  /**
   * Creates a new instance of this class which we need to access the 
   * cloudBackend object from.
   */
  std::shared_ptr<LogInDelegate> logInDelegate = 
                                              std::make_shared<LogInDelegate>();
  /** 
   * Login in to the account that you where provided with when downloading 
   * this example code. 
   * username and password can be found in the scripts and binary folder in 
   * user_credentials.cpp and can be changed as appropriate.
   * The API call to logIn() can be found in CloudBackend.h
   * in the C++/include folder.
   * The response will either be 
   * onLogInSuccess(cbe::CloudBackend&& cloudBackend)
   * located in the delegate callback,
   * or if the account does not exists 
   * onLogInError(cbe::delegate::Error&& error, cbe::util::Context&& context). 
   */

  logInDelegate->cloudBackend = cbe::CloudBackend::logIn(username, 
                                                         password,
                                                         tenant, 
                                                         logInDelegate);
  /** Keeps the thread alive between API requests. */
  // Waiting for the cloud response
  logInDelegate->waitForRsp();

  // Store a local copy
  cloudBackend = logInDelegate->cloudBackend;

  // Check if login was without error
  if (!logInDelegate->errorInfo) {
    // Yes, success!  
    std::cout << "Logged in:" << std::endl;
    std::cout << "SDK version = "
              << cloudBackend.version() << std::endl;
    std::cout << "   username = "
              << cloudBackend.account().username() << std::endl;
    std::cout << "     userId = "
              << cloudBackend.account().userId() << std::endl;
    std::cout << "     tenant = "
              << cloudBackend.account().source() << std::endl;
    std::cout << "      first = "
              << cloudBackend.account().firstName() << std::endl;
    std::cout << "       last = "
              << cloudBackend.account().lastName() << std::endl;
  } 
  else {
    // No, error...
    std::cout << "Exiting: Login failed!" << std::endl;
    std::cout << "errorInfo = " << logInDelegate->errorInfo << std::endl;
    cloudBackend.terminate();
    return 1; // Bail out - Due to failed login
  }

  //----------------------------------------------------------------------------

  std::shared_ptr<QueryDelegate> queryDelegate = 
                                              std::make_shared<QueryDelegate>();
  // Logged in users root container
  cbe::Container container = cloudBackend.account().rootContainer();

  // We want to check if the container the user wants to create already exist in 
  // the parent container.
  std::cout << "Name for a new Container to be created: ";
  std::string containerName;
  std::getline(std::cin, containerName);
  cloudBackend.query(container.id(), queryDelegate);
  // Waiting for the cloud response
  queryDelegate->waitForRsp();

  cbe::QueryResult::ItemsSnapshot resultSet = 
                                  queryDelegate->queryResult.getItemsSnapshot();  
  // Look through the parent container to check if the name has already been 
  // used.
  std::cout << "Content of /" << std::endl;
  std::cout << "---------------------------" << std::endl;
  for (cbe::Item& item : resultSet)
  {
    std::cout << item.name() << std::endl;
    if(item.name() == containerName){
      std::cout << "Exiting: Container already exist! " << item.name() << " (" 
                << item.id() << ")" << std::endl;
      cloudBackend.terminate();
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
  createContainerDelegate->waitForRsp();

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
  uploadDelegate->waitForRsp();

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
      removeContainerDelegate->waitForRsp();
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
  cloudBackend.terminate();
  return 0;
}  // main
