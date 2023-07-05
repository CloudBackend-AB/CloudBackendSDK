#include "Logic.h"

#include <algorithm>  // std::find_if_not
#include <cctype>     // std:isspace, std::tolower, std::toupper
#include <cstring>
#include <iomanip>    // std::setw
#include <ios>        // std::left
#include <sstream>    // std::ostringstream
#include <stdexcept>
#include <string>

#include "MyDelegates.cpp"
#include "MyGenericFunctions.cpp"

// - - - - - - - - - - - - - - - - - FUNCTIONS - - - - - - - - - - - - - - - - - 

// Exit function that reports how program was terminated
void Exercise::exitProgram(const int exitCode) {
  std::ostringstream oss;
  oss << "Exiting program with exit code: " << exitCode << ".";
  if (myCloudBackend) {
    myCloudBackend.terminate();
  }
  throw std::invalid_argument( oss.str() );
}

/* Exercise 2 */

// Creates a new container in the parentContainer argument that is passed in
void Exercise::createContainer(cbe::Container parentContainer) {
  std::cout << "Logic: Create container" << std::endl;
  // Generic function to get string from user input
  const auto name = inquireString("Set name for Container");

  // Create a delegate that is going to be used for the createContainer method
  std::shared_ptr<MyCreateContainerDelegate> createContainerDelegate = 
                                  std::make_shared<MyCreateContainerDelegate>();
  parentContainer.createContainer(name, createContainerDelegate);
  
  // Calling wait for the delegate to finish
  createContainerDelegate->waitForRsp();

  // Check if error, can also check if a container was constructed.
  if (createContainerDelegate->errorInfo){ 
    std::cout << "Error!" << std::endl;
    std::cout << "Container creation failed." << std::endl;
    
    // Print info about error 
    std::cout << createContainerDelegate->errorInfo << std::endl;
    
    // Exit program with unique code 
    exitProgram(21);
  }

  // Get container from the delegate
  this->myContainer = createContainerDelegate->container;
}

// Loads container contents to be able to see what is inside the container that 
// is passed in
void Exercise::loadContainerContents(cbe::Container container) {
  // Generic function to print information about container
  std::cout << "Getting sub-container of " << containerName(container) 
            << std::endl;
  
  // Create a filter that limits the query to only return containers 
  cbe::Filter containerFilter;
  containerFilter.setDataType(cbe::ItemType::Container);
  std::shared_ptr<MyQueryDelegate> queryDelegate = 
                                            std::make_shared<MyQueryDelegate>();

  // Call query function with filter and delegate
  container.query(containerFilter, queryDelegate);

  // Wait for delegate to finish
  queryDelegate->waitForRsp();
  
  // Check if error 
  if (queryDelegate->errorInfo){ // Could also check if qResult was constructed
    std::cout << "Error!" << std::endl;
    std::cout << "Could not load container contents." << std::endl;

    // Print info about error 
    std::cout << queryDelegate->errorInfo << std::endl;

    // Exit program with unique code
    exitProgram(30);
  }
  
  // Get queryResult from the delegate
  this->qResult = queryDelegate->queryResult;
}

// Function that prints container contents
void Exercise::printContainerContents(cbe::QueryResult qResult) {  
  // Put queryResult in vector of cbe::Item 
  std::vector<cbe::Item> items = qResult.getItemsSnapshot();
  std::cout << "------------------------------------------------------------" 
            << std::endl;
  // Loop through items and print them
  for (const auto& item : items) {
    // Generic print function 
    printItem(item);
  }
  std::cout << "------------------------------------------------------------" 
            << std::endl;
}

/* Exercise 3 */

// The same as loadContainerContents but with different filter
void Exercise::loadContainerObjects(cbe::Container container) {
  std::cout << "Getting objects in container " << containerName(container) 
            << std::endl;
  std::shared_ptr<MyQueryDelegate> queryDelegate = 
                                            std::make_shared<MyQueryDelegate>();

  // Create a filter that limits the query to only return objects 
  cbe::Filter objectFilter;
  objectFilter.setDataType(cbe::ItemType::Object);
  
  // Call query function with filter and delegate
  container.query(objectFilter, queryDelegate);

  // Wait for delegate to finish
  queryDelegate->waitForRsp();
   // Check if error 
  if (queryDelegate->errorInfo){ // Can check if qResult was constructed.
    std::cout << "Error!" << std::endl;
    std::cout << "Could not load container contents." << std::endl;

    // Print info about fail
    std::cout << queryDelegate->errorInfo << std::endl;

    // Exit program with unique code 
    exitProgram(33);
  }

  // Get queryResult from the delegate
  this->qResult = queryDelegate->queryResult;
}

void Exercise::printObjects(cbe::QueryResult queryResult) {
  cbe::Object tempObject{cbe::DefaultCtor{}}; // nullptr;
  std::cout << "Printing Objects from query result: " << std::endl;
  std::cout << "------------------------------------------------------------" 
            << std::endl;
  
  // Loop through objects from queryResult
  for (const auto& item : queryResult.getItemsSnapshot()) {
    if (item.type() == cbe::ItemType::Object) {
      printItem(item);
      tempObject = cbe::CloudBackend::castObject(item);

      // Get key/value information to print
      const auto keyValues = tempObject.keyValues();
      if (!keyValues.empty()) {

        // Loop through key/values for each object
        for (const auto& keyValue : keyValues) {
          const auto& key      = keyValue.first;
          const auto& sdkValue = keyValue.second;
          const auto  indexed  = std::get<bool>(sdkValue);
          std::cout << "    " << key << " = " << "'"
                    << std::get<std::string>(sdkValue)
                    << "'" << (indexed? " \t\t(indexed)" : "") << std::endl;
        }
      }  // if (!keyValues.empty())
    }   // if (item.type() == cbe::ItemType::Object)
  }
  std::cout << "------------------------------------------------------------" 
            << std::endl;

  // If container does not contain any objects
  if (!tempObject) {
    std::cout << "Sorry, no objects found in the Container!" << std::endl;
  }
}

cbe::Container Exercise::selectContainer(const std::string& prompt) {
  std::cout << "Select Container" << std::endl;
  const auto items = this->qResult.getItemsSnapshot();
  
  // Do the following until user has entered a container that exists 
  while (true) {
    // Generic function to get string from user input
    const auto containerName = inquireString(prompt);

    // Loop through and look for container that user has requested
    for (const auto& item : items) {

      // If container is found, return it 
      if (item.name() == containerName) {
        return cbe::CloudBackend::castContainer(item);
      }
    }

    // If container is not fount an error is printed
    std::cout << "Error: the container you asked for, \"" << containerName
              << "\", was not found; following are the options:" << std::endl;
    // Loops through available containers to select 
    for (const auto& item : items) {
      printItem(item);
    }
  }
}

cbe::Object Exercise::createObject(cbe::Container inContainer) {
  std::cout << "Create Object" << std::endl;

  // Generic functions to get string from user input
  const std::string name = inquireString("Set name for Object");

  // Set the number of key/values the user wants to input 
  const int numOfTags =
                       inquireInt("Set the number of Key/Value pairs you want");
  
  // Create variable for key/values
  cbe::KeyValues keyValues;

  for (int i = 1; i <= numOfTags; i++) {
    
    // String that will contain the key name 
    std::string keyName{};

    // Set keyName and check if it is valid
    while(true) {
      keyName = inquireString("Name of Key #" + std::to_string(i));
      if(!isdigit(keyName[0])){
        break;
      }
      // Error message if user input starts with a number
      std::cout << "Key can not start with numerical! " << std::endl;
    }

    // Generic function to get string from user input
    const auto value =
        inquireString("Value for #" + std::to_string(i) + " '" + keyName + "'");
    
    // Generic function to get bool from user input
    // Indexed key/values are quicker to access through searching
    const auto indexed =
                       inquireBool("Make KeyValue pair #" + std::to_string(i) +
                                   " indexed or not (y indexed, n not indexed",
                                   true /* defaultVal */);
    // Insert value for key
    keyValues[keyName] = std::pair<std::string, bool>(value, indexed);
  }

  // Delegate for createObject
  std::shared_ptr<MyCreateObjectDelegate> createObjectDelegate = 
                                    std::make_shared<MyCreateObjectDelegate>();  
  
  // Check if user has created any key/values
  if (numOfTags > 0) {
    // Create object with name, delegate and key/values
    inContainer.createObject(name, keyValues, createObjectDelegate);
  } else {
    // Create object without key/values
    inContainer.createObject(name, createObjectDelegate);
  }
  
  // Wait for delegate to finish
  createObjectDelegate->waitForRsp();
  
  // Check if error 
  if (createObjectDelegate->errorInfo){ 
    std::cout << "Error!" << std::endl;
    std::cout << "Could not create object." << std::endl;

    // Print info about error
    std::cout << createObjectDelegate->errorInfo << std::endl;

    // Exit with unique code
    exitProgram(31);
  }
  // Return object from delegate
  return createObjectDelegate->object;    
}

// - - - - - - - - - - - - - - LOGIC EXERCISE 1,2,3 - - - - - - - - - - - - - - 

void Exercise::logic() {
  /* Exercise 1 */
  // Generic function to get bool from user input  
  const bool login = inquireBool("Do you want to login", true /* defaultVal*/);
  
  // If user responds with a no then exit 
  if (!login) {
    // Exit program with unique code
    exitProgram(11);
  }
  // Generic function to get strings from user input
  // Second argument is "default value", user only has to press enter 
  const auto username = inquireString("Type username", "gitHubTester1");
  const auto password = inquireString("Type password");
  const auto tenant   = inquireString("Enter  tenant", "cbe_githubtesters");

  // Create log in delegate
  std::shared_ptr<MyLogInDelegate> logInDelegate = 
                                            std::make_shared<MyLogInDelegate>();
 
  // Call login and set member variable
  myCloudBackend = cbe::CloudBackend::logIn(username, 
                                            password, 
                                            tenant,
                                            logInDelegate);

  // Wait for delegate to finish
  logInDelegate->waitForRsp();
  
  // Check if error 
  if (logInDelegate->errorInfo){
    // Print info about error
    std::cout << "Error, login failed! \nError info=" 
              << logInDelegate->errorInfo << std::endl;
    
    // Exit program with unique code
    exitProgram(10);
  }

  // Confirm login and SDK version
  std::cout << "\nLogged in as:\t" << myCloudBackend.account().username()
            << "\nSDK version:\t"  << myCloudBackend.version() << "\n" 
            << std::endl; 
  
  /* Exercise 2 */
  // Put root container in member variable
  this->rootContainer = logInDelegate->cloudBackend.account().rootContainer();     
  
  // Load container contents ie. query it to get its contents
  loadContainerContents(this->rootContainer);
  
  // After loading the container, print its contents
  printContainerContents(this->qResult);
  
  // Given the existing containers, should we create a new?
  if (inquireBool("Do you want to create a new container", false)) {
    
    // Create a new container in the container referred container
    createContainer(rootContainer); 

    // Print some information about container
    std::cout << "Container name: "
              << containerName(this->myContainer, true /* temporary */)
              << std::endl;
  } 

  /* Exercise 3 */

  // Load container contents ie. query it to get its contents
  loadContainerContents(rootContainer);

  // After loading the container, print its contents
  printContainerContents(this->qResult);

  // Print the number of items in the container
  std::cout << "Item count: " << this->qResult.getItemsSnapshot().size() 
            << std::endl;
  
  // Prompts the user to select a container in which an object is to be created
  this->myContainer = 
     selectContainer("In which Container do you want to create the new object");

  // Print the container the user selected
  std::cout << "Container: " << containerName(this->myContainer) << std::endl;
  
  // Creates an object in the selected container
  object = createObject(this->myContainer);

  // Prints information about the object that has been created
  std::cout << "Designated name: " << objectName(object, true) << std::endl;
  std::cout << "Object name: " << objectName(object) << std::endl;

  // Finally after the object has been created, load its contents
  loadContainerObjects(this->myContainer);

  // Print all objects and its content  
  printObjects(this->qResult);
  
  std::cout << "Exercise completed!" << std::endl;
  myCloudBackend.terminate();
} // void Exercise::logic
