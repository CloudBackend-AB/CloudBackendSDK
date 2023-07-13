/*
  Copyright © CloudBackend AB 2022 - 2023.
*/

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <stdio.h>
#include <string>
#include <thread>

#include "cbe/Account.h"
#include "cbe/CloudBackend.h"
#include "cbe/Container.h"
#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "Delegates.cpp"
#include "user_credentials.cpp"  // file is located in this folder


int main(void) {
  printf("Starting SimplEx program.\n");

  // The central object owning the login session.
  cbe::CloudBackend cloudBackend{cbe::DefaultCtor{}};

  //----------------------------------------------------------------------------
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

  // Create a new delegate callback instance of this class.
  std::shared_ptr<LogInDelegate> logInDelegate = 
                                              std::make_shared<LogInDelegate>();

  cloudBackend = cbe::CloudBackend::logIn(username, 
                                          password,
                                          tenant, 
                                          client,
                                          logInDelegate);
  // Wait until the delegate has received a response from the cloud service.
  logInDelegate->waitForRsp();

  // Check if login was without error
  if (!logInDelegate->errorInfo) {
    // Yes, no error, success!  
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
  } else {
    std::cout << "Warning: Login failed!" << std::endl;
    std::cout << "errorInfo = " << logInDelegate->errorInfo << std::endl;
    std::cout << "Exiting." << std::endl;
    cloudBackend.terminate();
    return 1; // Bail out - Due to failed login
  }

  //----------------------------------------------------------------------------
  // Ask for name of new container.
  std::string containerName;
  do {
    std::cout << "Name for a new Container to be created: ";
    std::getline(std::cin, containerName);
  } while (containerName < "A");

  //----------------------------------------------------------------------------
  // Check if the name the user wants to create
  // already exist in the parent container.
  std::shared_ptr<QueryDelegate> queryDelegate = 
                                              std::make_shared<QueryDelegate>();
  cbe::QueryResult::ItemsSnapshot resultSet;
  // My top root container to be used as parent - home://
  // in which to create the new sub-container
  cbe::Container parentContainer = cloudBackend.account().rootContainer();

  cloudBackend.query(parentContainer.id(), queryDelegate);
  // Wait until the delegate has received a response from the cloud service.
  queryDelegate->waitForRsp();

  // Check if error
  if (queryDelegate->errorInfo) {
    // Yes, error...
    std::cout << "Warning: Query failed!" << std::endl;
    std::cout << "errorInfo = " << queryDelegate->errorInfo << std::endl;
    std::cout << "Exiting." << std::endl;
    cloudBackend.terminate();
    return 3; // Bail out - Due to failed query
  } else {
    resultSet = queryDelegate->queryResult.getItemsSnapshot();  
    std::cout << "after getItemsSnapshot"  << std::endl;
    // Look through the parent container to check
    // if the name has already been used.
    for (cbe::Item& item : resultSet) {
      // std::cout << item.name() << std::endl;
      if (item.name() == containerName) {
        std::cout << "Warning: Container already exists! "
                  << item.name()
                  << " (" << item.id() << ")" 
                  << std::endl;
        std::cout << "Exiting." << std::endl;
        cloudBackend.terminate();
        return 2;
      }
    }
  }
  std::cout << "after if"  << std::endl;


  //----------------------------------------------------------------------------

  // Create a new sub-container in the parent container
  // using the name that the user provided
  std::shared_ptr<CreateContainerDelegate> createContainerDelegate = 
                                    std::make_shared<CreateContainerDelegate>();
  parentContainer.createContainer(containerName, createContainerDelegate);
  // Wait until the delegate has received a response from the cloud service.
  createContainerDelegate->waitForRsp();
  // Check if error
  if (createContainerDelegate->errorInfo) {
    // Yes, error... 
    std::cout << "Warning: CreateContainer failed!" << std::endl;
    std::cout << "errorInfo = " << createContainerDelegate->errorInfo 
              << std::endl;
    std::cout << "Exiting." << std::endl;
    cloudBackend.terminate();
    return 4; // Bail out - Due to failed CreateContainer
  }

  cbe::Container newContainer = createContainerDelegate->container;
  std::cout << "/" << newContainer.name()
            << "\t\tcreated" 
            << "\t\t(" << newContainer.id() << ")" 
            << std::endl;
 
  //----------------------------------------------------------------------------

  // Define which file to upload from the local system.
  std::shared_ptr<UploadDelegate> uploadDelegate = 
                                             std::make_shared<UploadDelegate>();
  std::string fileName = "w2.xml";
  // Define the file's relative path on the local system. 
  // Note: path must end with "/".
  std::string filePath = "Upload_files/";

  // Upload the file to the newly created container.
  newContainer.upload(fileName, filePath, uploadDelegate);
  // Wait until the delegate has received a response from the cloud service.
  uploadDelegate->waitForRsp();

  // Check if error
  if (uploadDelegate->errorInfo) {
    // Yes, error...
    std::cout << "Warning: Upload failed!" << std::endl;
    std::cout << "errorInfo = " << uploadDelegate->errorInfo << std::endl;
    std::cout << "Exiting." << std::endl;
    cloudBackend.terminate();
    return 5; // Bail out - Due to failed Upload
  }

  cbe::Object newObject = uploadDelegate->object;
  std::cout << "/" << newContainer.name() << "/" << newObject.name()
            << "\tuploaded"
            << "\t("
            << newObject.parentId()
            <<":"
            << newObject.id()
            << ")" 
            << std::endl;

  //----------------------------------------------------------------------------

  // Inspect content of the new container.
  // Note: Reusing the delegate.
  newContainer.query(queryDelegate);
  // Wait until the delegate has received a response from the cloud service.
  queryDelegate->waitForRsp();

  // Check if error
  if (queryDelegate->errorInfo) {
    // Yes, error...
    std::cout << "Warning: Query failed!" << std::endl;
    std::cout << "errorInfo = " << queryDelegate->errorInfo << std::endl;
    std::cout << "Exiting." << std::endl;
    cloudBackend.terminate();
    return 3; // Bail out - Due to failed query
  } else {
    std::cout << "Content of /" << newContainer.name() << std::endl;
    std::cout << "---------------------------" << std::endl;
    resultSet = queryDelegate->queryResult.getItemsSnapshot();
    for (cbe::Item& item : resultSet)
    {
      std::cout << item.name() << std::endl;
    }
    std::cout << "---------------------------" << std::endl;
  }

  //----------------------------------------------------------------------------

  // Clean up.
  std::string shouldBeDeleted{};
  std::cout << "Do you want to delete your newly created container called \"";
  std::cout << newContainer.name() << "\"?" << std::endl;
  
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
      // Wait until the delegate has received a response from the cloud service.
      removeContainerDelegate->waitForRsp();

      if (removeContainerDelegate->errorInfo) {
        std::cout << "errorInfo = "
                  << removeContainerDelegate->errorInfo << std::endl;
        std::cout << "Container could not be deleted!" << std::endl;
      } else {
        std::cout << "Container was deleted successfully!" << std::endl;
      }
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
