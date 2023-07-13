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
#include "../user_credentials.cpp"

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
                                              tenant,
                                              client);
  }
  catch (cbe::CloudBackend::LogInException& e)
  {
    std::cout << "Error! Login failed..." << e.what() 
              << "\nType: "               << e.typeAsString() << std::endl; 
    return;
  }
  // Confirm login and SDK version
  std::cout << "\nLogged in as:\t" << myCloudBackend.account().username()
            << "\nSDK version:\t"  << myCloudBackend.version() << "\n"
            << std::endl;

  /* Exercise 2 */

  /* Exercise 3 */

  std::cout << "Exercise completed!" << std::endl;
  myCloudBackend.terminate();
} // void Exercise::logic
