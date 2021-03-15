/*
      Copyright © CloudBackend AB 2020.
*/

#include "SimpleEx.h"
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "user_credentials.cpp"  // The user_credentials.cpp is located whitin this folder



bool finished;

/** Keeps the main thread alive while running the program and terminates the main thread when programFinished() sets the bool finished to true. */
void waitUntilFinished()
{
  finished = false;

  while (!finished)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }
}

/** Called when we wish to terminate the main thread and quit the program. */
void programFinished() {
  std::cout << "program complete\n";
  finished = true;
}


/** ENTRY POINT for this example code! 
 *  Remember the main function is not whithin the CBEExample namespace therefor we need to call the delegate and the cloudbackend object by cbeEx-> 
*/
int main(void) {
  printf("Hello! This is a test program.\n");

  /** Creates a new instance of this class which we need to access the cloudBackend object from.*/
  std::shared_ptr<CBEExample> cbeEx = std::make_shared<CBEExample>();
  CBE::AccountDelegatePtr accountDelegate(cbeEx);
  /** Loging in to the account that you where provided with when downloading this example code. 
   *  username and password can be found in the scripts and binary folder in user_credentials.cpp if you wish to change the credentials.
   *  The API call to logIn can be found in CloudBackend.h in the include folder. The respond will either be onLogin located down below in callbacks,
   *  or if the account does not exists onError(CBE::persistence_t failedAtState, uint32_t code, std::string reason, std::string message). */
  cbeEx->cloudBackend = CBE::CloudBackend::logIn(username, password, tenant, accountDelegate);
  
  /** Keeps the thread alive between API requests. */
  waitUntilFinished();
  /** Handle your allocations of memory and clean up! */
  
  /** Exit the program.*/
  return 0;
}

//Server requests

/** call query on the container of your choice, try implement query on the container after the upload as a new method in this class where you simply query for all items in the container.*/
void CBEExample::query(CBE::ContainerPtr container, std::string name) {

  /** To see if the Container that we want to add is not already added on the account we create a filter where we want to search specifically for that container.
   *  Another way to do this would be to check onItemError if the respond states 409 conflict, container already exists then we could remove the container and create it again. 
   *  That would probably be the smarter and easier way, but to show the variety of the SDK we use the filter and query insted of handling this by error statements.
  */

  /** Save the name to be able to compare in the result. */
  _queryName = name;

  /** Query has the callback onQueryLoaded and if an error occures then onLoadError is called. To make the program complete implement both. */
  CBE::ItemDelegatePtr itemDelegate = getPtr();
  container->query(itemDelegate);
}

/*  Upload file to a given container */
void CBEExample::uploadToContainer(std::string path, std::string name, CBE::ContainerPtr container) {
  /** Upload to the container with the file name + extension and from the folder path.
   *  transfer delegate defined in the .h file.
   *  upload has the callback onObjectUploaded and if file is missing from folder or other failure occurs then onObjectUploadFailed() is where the respond is.
   */
  CBE::TransferDelegatePtr transferDelegate = getPtr();
  container->upload(name, path, transferDelegate);
}

void CBEExample::createContainer(CBE::ContainerPtr container, std::string name) {
  std::cout << "Create container: " << name << " located in container: " << container->name() << " with id: " << container->id() << std::endl;
  /** Create found in Container.h creates a container in the container that calls this SDK method / API.
   *  
   *  You can call it as below or if it is a container you want to have a pointer to in your 
   *  program by defining CBE::ContainerPtr newNameContainer = container->create(..).
   *  
   *  Create has the corresponding callback onContainerAdded(CBE::ContainerPtr container) seen down below in callbacks.
   */
  CBE::ItemDelegatePtr itemDelegate = getPtr();
  container->create(name, itemDelegate);
}

// Callbacks

/** onLogin is the callback to logIn and can be found in AccountEventProtocol. */
void CBEExample::onLogin(uint32_t atState, CBE::CloudBackendPtr cbe)
{
  printf("onLogin reached \n");

  /** add input to name a company specific container. */
  std::cout << "Name for a new Company Container: ";
  std::string name;
  std::getline(std::cin, name);

  query(cbe->account()->rootContainer(), name);

}

/** Gets called when the account status has changed (required). */
void CBEExample::onError(CBE::persistence_t failedAtState, uint32_t code, std::string reason, std::string message)
{
  printf("Account Fail ");
  programFinished();
}

void CBEExample::onQueryLoaded(CBE::QueryResultPtr qr) {
  /** qr holds both the filter which you used to query and the list of items that you asked for qould be a nullptr if there was'nt any items according to your request. */
  std::vector<CBE::ItemPtr> items = qr->getItemsSnapshot();
  CBE::ContainerPtr container = nullptr;
  for(auto item : items) {
    std::cout << "comparing " << item->name() << " " << _queryName << std::endl;
    if(item->name() == _queryName) {
      container = cloudBackend->castContainer(item);
    }
  }

  if(container != nullptr) {
    /** Since the container already exists on the account and we want to have a clean container, without sub containers that might have been created by other 
     * company personel that have tested the library, we delete the container and then create it again. 
     * When deleting a container all the sub containers and its objects are removed/deleted as well.
    */
    CBE::ItemDelegatePtr itemDelegate = getPtr();
    container->remove(itemDelegate);
  } else {
    /** Since we did not find the container on the account we can create it directly. */
    createContainer(cloudBackend->account()->rootContainer(), _queryName);
  }

}

/** If the query fails the error will be returned in onLoadError. */
void CBEExample::onLoadError(CBE::Filter filter, uint32_t operation, uint32_t code, std::string reason, std::string message){
  std::cout << "Failed query.\n";
}

/** onContainerAdded is related to create in container.h, the callback is found in the protocol ItemEventProtocol. */
void CBEExample::onContainerAdded(CBE::ContainerPtr container) {
  std::cout << "New container created: " << container->name() << " : " << container->id() << std::endl;
  /** path to where on you file ssytem you have the file, either absolute path or as in this case relative. */
  
  std::string path = "Upload_files/";
  /** File name with extension, only objects created in the cloud using createObject method does not need a file extension. */
  std::string uploadFilename = "w1.xml";
  uploadToContainer(path, uploadFilename, container);
}

/** onContainerRemoved is related to remove in container.h, the callback is found in the protocol ItemEventProtocol. */
void CBEExample::onContainerRemoved(CBE::item_id_t containerId, std::string name) {
  std::cout << name << " with id: " << containerId << " has been removed.\n";

  /** We removed the container since it was on the account after a person at you office done this lab since before and you named the container the same thing.
   * When removing a container all containers and objects within that container are removed which is important to remomber so that no important files are lost.
   */
  createContainer(cloudBackend->account()->rootContainer(), name);
}

/** onItemError gets called if the respond from calling an itemDelegate action fails, e.x container->move(...). */
void CBEExample::onItemError(CBE::ItemPtr container, CBE::item_t type, uint32_t operation, uint32_t failedAtState, uint32_t code, std::string reason, std::string message) {
  std::cout << "Error in: " << container->name() << ", operation: " << operation << " failed with code, reason, message: " << code << " , " << reason << " , " << message << std::endl;

  /** Program failed call to programFinished and debug what happend by reading print out in command line. */
  programFinished();
}


//  TransferDelegate respons

/** onObjectUploaded is called when an object has been uploaded by the call upload in container.h you find the call in TransferEventProtocol. */
void CBEExample::onObjectUploaded(CBE::ObjectPtr object)
{
  std::cout << "CBEExample::OnObjectUploaded\n";
  /** Program finished we can call to terminate the main thread and cleanup allocation of memory with new CBEExample and exit.
   *  When that happens the cloudbackend object gets its destructor called internally and starts cleaning up and terminates its threads. */
  programFinished();
}

/** Gets called when a error has occured in the upload. */
void CBEExample::onObjectUploadFailed(std::string name, CBE::object_id_t objectId, CBE::container_id_t parentId, CBE::persistence_t atState, CBE::failed_status_t status)
{
  std::cout << "CBEExample::OnObjectUploadFailed\n";
  /** Upload failed so we terminate and debug to find what was wrong, is the file in the path that you designated to the upload call, to upload from? */
  programFinished();
}
