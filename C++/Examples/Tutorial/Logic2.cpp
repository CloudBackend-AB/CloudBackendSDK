#include "Logic.h"

#include <algorithm>  // std::find_if_not
#include <cstring>
#include <cctype>     // std:isspace, std::tolower, std::toupper
#include <iterator>
#include <ios>        // std::left
#include <iomanip>    // std::setw
#include <sstream>    // std::ostringstream
#include <string>
#include <stdexcept>

#include "cbe/QueryChain.h"
#include "cbe/QueryResult.h"

// - - - - - - - - - - - - - - - - - DELEGATES - - - - - - - - - - - - - - - - - 

class MyLogInDelegate :  public cbe::delegate::LogInDelegate
{
  // Mutex lock and condition variable are used to verify that operation has 
  // completed.
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};

  // Used to see if callback is reached
  bool                    called = false;

  // User implemented callback, return value is cbe::CloudBackend&&  
  void onLogInSuccess(cbe::CloudBackend&& cloudBackend) final {
    {
      // Lock thread during operation 
      std::lock_guard<std::mutex> lock(mutex);
      this->cloudBackend = std::move(cloudBackend); 
      called = true;
    }
    // Notify when operation is done to unlock
    conditionVariable.notify_one();
  }

  // User implemented error callback
  void onLogInError(cbe::delegate::Error&&  error, 
                    cbe::util::Context&&    context) final {
    {
      std::lock_guard<std::mutex> lock(mutex);
      // Set member variable errorInfo with information about fail
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  }
public:
  // Member variables for the delegate, will later be accessed through the 
  // delegate defined in the code
  cbe::CloudBackend cloudBackend{cbe::DefaultCtor{}}; 
  ErrorInfo errorInfo{};

  // Wait function is called on the delegate when it is being used 
  void wait() {
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "Waiting, to be logged in" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    std::cout << "Now we have waited: " << called << std::endl;
  }

}; // class MyLogInDelegate 

class MyQueryDelegate :  public cbe::delegate::QueryDelegate
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

public:
  /*implementation of delegates */
  cbe::QueryResult  queryResult{cbe::DefaultCtor{}};
  ErrorInfo         errorInfo{};

  void wait() {
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "Waiting, for query" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    std::cout << "Now we have waited: " << called << std::endl;
  }
}; // class MyQueryDelegate

class MyCreateContainerDelegate :  public cbe::delegate::CreateContainerDelegate
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

public:
  /*implementation of delegates */
  cbe::Container container{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void wait() {
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "Waiting, for create container" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    std::cout << "Now we have waited: " << called << std::endl;
  }
}; // class MyCreateContainerDelegate

class MyCreateObjectDelegate :  public cbe::delegate::CreateObjectDelegate
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

public:
  /*implementation of delegates */
  cbe::Object object{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void wait() {
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "Waiting, for create container" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    std::cout << "Now we have waited: " << called << std::endl;
  }
}; // class MyCreateObjectDelegate

// - - - - - - - - - - - - - - - - - FUNCTIONS - - - - - - - - - - - - - - - - - 

// Exit function that reports how program was terminated
void Exercise::exitProgram(const int exitCode) {
  std::ostringstream oss;
  oss << "Exiting program with exit code: " << exitCode << ".";
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
  createContainerDelegate->wait();

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
  queryDelegate->wait();
  
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
  const auto tenant =   inquireString("Enter  tenant", "cbe_githubtesters");

  // Create log in delegate
  std::shared_ptr<MyLogInDelegate> logInDelegate = 
                                            std::make_shared<MyLogInDelegate>();
 
  // Call login and set member variable 
  logInDelegate->cloudBackend = cbe::CloudBackend::logIn(username, 
                                                         password, 
                                                         tenant,
                                                         logInDelegate);

  // Wait for delegate to finish
  logInDelegate->wait();
  
  // Check if error 
  if (logInDelegate->errorInfo){
    // Print info about error
    std::cout << "Error, login failed! \nError info=" 
              << logInDelegate->errorInfo << std::endl;
    
    // Exit program with unique code
    exitProgram(10);
  }

  // Confirm login and SDK version
  cbe::CloudBackend myCloudBackend = logInDelegate->cloudBackend;
  std::cout << "\nLogged in as:\t" << myCloudBackend.account().username()
            << "\nSDK version:\t" << myCloudBackend.version() << "\n" 
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
    
    // Create a new container in the container refered container
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

// - - - - - - - - - - - - - GENERIC INPUT FUNCTIONS - - - - - - - - - - - - - -

const bool  Exercise::noBoolDefaultVal{}; // = false
bool Exercise::inquireBool(const std::string& prompt,
                        const bool&        defaultVal) {
  const bool hasDefaultVal = (&defaultVal != &noBoolDefaultVal);
  constexpr char  falseChar = 'n';
  const char      actualFalseStr =
      (hasDefaultVal && !defaultVal)? std::toupper(falseChar) : falseChar;
  constexpr char trueChar =  'y';
  const char      actualTrueChar =
      (hasDefaultVal && defaultVal)?  std::toupper(trueChar) : trueChar;
  while (true) {
    std::cout << prompt << " (" << actualTrueChar << '/' << actualFalseStr
              << ")? ";
    std::string answer;
    std::getline(std::cin, answer);
    if (answer.empty()) {
      if (hasDefaultVal) {
        return defaultVal;
      }
      continue;
    }
    switch (std::tolower(answer[0])) {
    case trueChar:  return true;
    case falseChar: return false;
    default: continue;
    }
  }
} // bool Exercise::inquireBool

const int Exercise::noIntDefaultVal{}; // = 0
int Exercise::inquireInt(const std::string& prompt,
                      const int&         defaultVal) {

  const bool hasDefaultVal = (&defaultVal != &noIntDefaultVal);
  while (true) {
    std::cout << prompt
              << (hasDefaultVal? (" (Default value is '" +
                                     std::to_string(defaultVal) + "')") : "")
              << "? ";
    std::string answer;
    std::getline(std::cin, answer);
    if (answer.empty()) {
      if (hasDefaultVal) {
        return defaultVal;
      }
      continue;
    }
    try {
      const int intVal = std::stoi(answer);
      return intVal;
    } catch (std::exception& e) {
      std::cout << "Failed to convert answer \"" << answer 
                << "\" to an integer." << "Got exception with what()=\"" 
                << e.what() << "\"" << std::endl;
    }
  }
} // int Exercise::inquireInt

const std::string Exercise::noStringDefaultVal{}; // ""
std::string Exercise::inquireString(const std::string&  prompt,
                                 const std::string&  defaultVal) {
  const bool hasDefaultVal = (&defaultVal != &noStringDefaultVal);
  while (true) {
    std::cout << prompt
              << (hasDefaultVal? (" (Default value is \"" + defaultVal + "\")")
                               : "") << "? ";
    std::string answer;
    std::getline(std::cin, answer);
    if (answer.empty() && hasDefaultVal) {
      answer = defaultVal;
    }
    const auto /* std::string */ trimmedAnswer = trimString(answer);
    if (trimmedAnswer.empty()) {
      std::cout << "Your input \"" << answer
                << "\" is not valid; please, input a response." << std::endl;
    } else {
      return trimmedAnswer;
    }
  }
} // std::string Exercise::inquireString

std::string Exercise::trimString(const std::string& str) {
   const auto /* std::string::const_iterator */ noSpaceFront =
       std::find_if_not(str.cbegin() /* first */, str.cend() /* last */,
                        [](int ch){
                          return std::isspace(ch);
                        } /* q */);
   const auto noSpaceBack =
       std::find_if_not(str.crbegin(),
                        std::string::const_reverse_iterator(noSpaceFront),
                        [](int ch){
                          return std::isspace(ch);
                        }).base();

   return std::string(noSpaceFront, noSpaceBack);
} // std::string Exercise::trimString

// - - - - - - - - - - - - - GENERIC PRINT FUNCTIONS - - - - - - - - - - - - - -

const char* Exercise::itemTypeString(cbe::ItemType itemType) {
  static const struct ItemTypeValue {
    cbe::ItemType itemType;
    const char* str;
  } ItemTypeValues[] {
      { cbe::ItemType::Unapplicable , "Unapplicable"  },
      { cbe::ItemType::Unknown      , "Unknown"       },
      { cbe::ItemType::Object       , "Object"        },
      { cbe::ItemType::Container    , "Container"     },
      { cbe::ItemType::Tag          , "Tag"           },
      { cbe::ItemType::Group        , "Group"         }
};
  const auto it = std::find_if (std::begin(ItemTypeValues) /* first */,
                               std::end(ItemTypeValues)   /* last */,
                               [itemType](const ItemTypeValue& ItemTypeValue) {
                                 return ItemTypeValue.itemType == itemType;
                               } /* p */);
  return (it != std::end(ItemTypeValues))? it->str : "unknown-item-type";
} // const char* Exercise::itemTypeString

void Exercise::printItem(const cbe::Item& item, bool printParentId) {
  std::cout << std::left << std::setw(32) << item.name()
            << std::left << std::setw(10) << itemTypeString(item.type())
            << std::left << std::setw(3)  << "id:"
            << std::left << std::setw(16) << item.id();
  if (printParentId) {
    std::cout << std::left << std::setw(2)  << "p:" << item.parentId();
  }
  std::cout << std::endl;
} // void Exercise::printItem

std::string Exercise::containerName(cbe::Container container,
                                    bool           temporary) {
  std::ostringstream oss;
  oss << container.name() <<  " (" << (temporary? "(" : "")
      << container.id() << (temporary? ")" : "") << ")";
  return oss.str();
} // std::string Exercise::containerName

std::string Exercise::objectName(cbe::Object object,
                              bool           temporary) {
  std::ostringstream oss;
  oss << object.name() <<  " (" << (temporary? "(" : "")
      << object.id() << (temporary? ")" : "") << ")";
  return oss.str();
} // std::string Exercise::objectName
