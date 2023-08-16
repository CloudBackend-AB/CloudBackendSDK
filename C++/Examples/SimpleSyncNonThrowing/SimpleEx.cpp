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
#include "cbe/QueryChainSync.h"
#include "cbe/Types.h"

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
   */

  cbe::CloudBackend::LogInError logInError;
  cloudBackend = cbe::CloudBackend::logIn(username, 
                                          password,
                                          tenant, 
                                          client,
                                          logInError);

  // Check if error
  if (logInError){
    // Print info about error
    std::cout << "Error, login failed! \nError info="
              << logInError << std::endl;
    return 1;
  } else {
    std::cout << "Logged in:" << std::endl;
    std::cout << "SDK version = " << cloudBackend.version()             << std::endl;
    std::cout << "   username = " << cloudBackend.account().username()  << std::endl;
    std::cout << "     userId = " << cloudBackend.account().userId()    << std::endl;
    std::cout << "     tenant = " << cloudBackend.account().source()    << std::endl;
    std::cout << "      first = " << cloudBackend.account().firstName() << std::endl;
    std::cout << "       last = " << cloudBackend.account().lastName()  << std::endl;
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
  cbe::QueryResult::ItemsSnapshot resultSet;
  // My top root container to be used as parent - home://
  // in which to create the new sub-container
  cbe::Container parentContainer = cloudBackend.account().rootContainer();

  cbe::CloudBackend::QueryError queryError;
  resultSet = cloudBackend.query(parentContainer.id(), queryError).getQueryResult().getItemsSnapshot();

  // Check if error
  if (queryError) {
    std::cout << "Error!" << std::endl << queryError << std::endl;
    return 2;
  } else {
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
        return 3;
      }
    }
  }
  //----------------------------------------------------------------------------

  // Create a new sub-container in the parent container
  // using the name that the user provided
  cbe::Container::CreateContainerError createContainerError;
  cbe::Container newContainer = *parentContainer.createContainer(containerName, createContainerError);

  // Check if error
  if (createContainerError) {
    std::cout << "Error!" << std::endl << createContainerError << std::endl;
    cloudBackend.terminate();
    return 4;
  } else {
    std::cout << "/" << newContainer.name()
              << "\t\tcreated" 
              << "\t\t(" << newContainer.id() << ")" 
              << std::endl;
  }
  //----------------------------------------------------------------------------
  // Define which file to upload from the local system.
  std::string fileName = "w2.xml";
  // Define the file's relative path on the local system. 
  // Note: path must end with "/".
  std::string filePath = "Upload_files/";

  // Upload the file to the newly created container.
  cbe::Container::UploadError uploadError; 
  cbe::Object newObject = *newContainer.upload(fileName, filePath, uploadError);

  // Check if error
  if (uploadError) {
    std::cout << "Error!" << std::endl << uploadError << std::endl;
    cloudBackend.terminate();
    return 5;
  } else {
    std::cout << "/" << newContainer.name() << "/" << newObject.name()
              << "\tuploaded"
              << "\t("
              << newObject.parentId()
              <<":"
              << newObject.id()
              << ")" 
              << std::endl;
  } 
  //----------------------------------------------------------------------------
  // Inspect content of the new container.
  // Note: Reusing the error variable.
  resultSet = newContainer.query(queryError).getQueryResult().getItemsSnapshot();

  // Check if error
  if (queryError) {
    std::cout << "Error!" << std::endl << queryError << std::endl;
    cloudBackend.terminate();
    return 6;
  } else {
    std::cout << "Content of /" << newContainer.name() << std::endl;
    std::cout << "---------------------------" << std::endl;
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
      cbe::Container::RemoveError removeError;
      newContainer.remove(removeError);

      if (removeError) {
        std::cout << "Error!" << std::endl << removeError << std::endl;
        cloudBackend.terminate();
        return 7;
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
