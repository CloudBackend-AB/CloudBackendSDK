#include "Logic.h"

#include <algorithm>  // std::find_if_not
#include <cctype>     // std:isspace, std::tolower, std::toupper
#include <cstring>
#include <iomanip>    // std::setw
#include <ios>        // std::left
#include <sstream>    // std::ostringstream
#include <stdexcept>
#include <string>

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

  try
  {
    myContainer = parentContainer.createContainer(name);
  }
  catch (const cbe::Container::CreateContainerException& e)
  {
    std::cout << "Error!" << std::endl;
    std::cout << "Container creation failed." << std::endl;

    // Print info about error
    std::cout << e.what() << std::endl;

    // Exit program with unique code
    exitProgram(21);
  }
}

// Loads container contents to be able to see what is inside the container that
// is passed in
void Exercise::loadContainerContents(cbe::Container container) {
  // Generic function to print information about container
  std::cout << "Getting sub-container of " << containerName(container)
            << std::endl;
  try
  {
    // Create a filter that limits the query to only return containers
    cbe::Filter containerFilter;
    containerFilter.setDataType(cbe::ItemType::Container);

    // Call query function with filter
    qResult = container.query(containerFilter).getQueryResult();
  }
  catch (const cbe::Container::QueryException& e)
  {
    std::cout << "Error!" << std::endl;
    std::cout << "Could not load container contents." << std::endl;

    // Print info about error
    std::cout << e.what() << std::endl;

    // Exit program with unique code
    exitProgram(30);
  }
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

  try
  {
    // Call login and set member variable
    myCloudBackend = cbe::CloudBackend::logIn(username,
                                              password,
                                              tenant);
  }
  catch (const cbe::CloudBackend::LogInException& e)
  {
    // print info about error
    std::cout << "error, login failed! \nerror info="
              << e.what() << std::endl;

    // Exit program with unique code
    exitProgram(10);
  }

  // Confirm login and SDK version
  std::cout << "\nLogged in as:\t" << myCloudBackend.account().username()
            << "\nSDK version:\t"  << myCloudBackend.version() << "\n"
            << std::endl;

  /* Exercise 2 */
  // Put root container in member variable
  this->rootContainer = myCloudBackend.account().rootContainer();

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

  std::cout << "Exercise completed!" << std::endl;
  myCloudBackend.terminate();
} // void Exercise::logic
