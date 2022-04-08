#include "Logic.h"

#include "AccountEventProtocol.h"
#include "ItemEventProtocol.h"

#include <algorithm>  // std::find_if_not
#include <cstring>
#include <cctype>     // std:isspace, std::tolower, std::toupper
#include <ios>        // std::left
#include <iomanip>    // std::setw
#include <sstream>    // std::ostringstream


/* Here we will logIn() */
void Logic::start() {
  const bool login = inquireBool("Do you want to login", true /* defaultVal*/);
  if (!login) {
    programFinished();
    return;
  }
  const auto username = inquireString("Type username", "githubtester2");
  const auto password = inquireString("Type password");
  const auto tenant =   inquireString("Enter  tenant", "cbe_githubtesters");

  CBE::AccountDelegatePtr accountDelegate =
                                  std::make_shared<AccountEventProtocol>(this);
  cloudBackend = CBE::CloudBackend::logIn(username, password, tenant,
                                          accountDelegate);
}

void Logic::programFinished() {
  std::cout << "program finished." << std::endl;
  finished = true;
}

void Logic::saveQueryResultContinue(CBE::QueryResultPtr qR) {
  std::cout << "continue program" << std::endl;
  qResult = qR;
  logic();
}


/* Exercise 2 */
CBE::ContainerPtr Logic::createContainer(CBE::ContainerPtr container) {
  std::cout << "Logic: Create container" << std::endl;
  const auto name = inquireString("Set name for Container");
  CBE::ItemDelegatePtr itemDelegate = std::make_shared<ItemEventProtocol>(this);
  return container->create(name, itemDelegate);
}


void Logic::loadContainerContents(CBE::ContainerPtr container) {
  std::cout << "Getting sub-container of " << containerName(container) << std::endl;
  CBE::ItemDelegatePtr itemDelegate = std::make_shared<ItemEventProtocol>(this);
  CBE::Filter filter1;
  filter1.setDataType(CBE::ItemType::Container);
  container->query(filter1, itemDelegate);
}


void Logic::printContainerContents(CBE::QueryResultPtr q) {
  for (const auto& itemPtr : q->getItemsSnapshot()) {
    printItem(*itemPtr);
  }
}


/* Exercise 3 */



/** Logic Exercise 1,2 */
void Logic::logic() {

  std::unique_lock<std::recursive_mutex> lock{logicMutex};

  bool continueToNextStep;
  const int thisInstance = ++logicInstances;
  std::cout << "logic instance: " << thisInstance << std::endl;
  do {
    continueToNextStep = false;
    std::cout << "step: " << step << std::endl;
    switch (step) {
      /* after login; load the top container list of items in cache */
      case 1: {
        loadContainerContents(rootContainer);
        ++step;
        break;
      }

      /* Given the existing containers, should we create a new ? */
      case 2: {
        printContainerContents(qResult);
        if (inquireBool("Do you want to create a new container", false)) {
          auto newContainer = createContainer(rootContainer);
          std::cout << "Container name: "
                    << containerName(newContainer, true /* temporary */)
                    << std::endl;
          container = newContainer;
        } else {
          continueToNextStep = true;
        }
        ++step;
        break;
      }

      /* Exercise 3 */
      
      /* the end */
      default: {
        std::cout << "Exercise completed!" << std::endl;
        programFinished();
        break;
      }
    }
  } while(continueToNextStep);
  std::cout << "end of do-while loop;"
            << " instance: " << thisInstance
            << " next step: " << step 
            << std::endl;
}


/*----- generic functions -----*/
/*-- generic input functions --*/
const bool  Logic::noBoolDefaultVal{}; // = false
bool Logic::inquireBool(const std::string& prompt,
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
}

const int Logic::noIntDefaultVal{}; // = 0
int Logic::inquireInt(const std::string& prompt,
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
      std::cout << "Failed to convert answer \"" << answer << "\" to an integer."
                << "Got exception with what()=\"" << e.what() << "\"" << std::endl;
    }
  }
}

const std::string Logic::noStringDefaultVal{}; // ""
std::string Logic::inquireString(const std::string&  prompt,
                                 const std::string&  defaultVal) {
  const bool hasDefaultVal = (&defaultVal != &noStringDefaultVal);
  while(true) {
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
}

std::string Logic::trimString(const std::string& str) {
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
}

/*-- generic print functions --*/
const char* Logic::itemTypeString(CBE::item_t itemType) {
  static const struct ItemTypeValue {
    CBE::item_t itemType;
    const char* str;
  } ItemTypeValues[] {
      { CBE::ItemType::Unapplicable , "Unapplicable"  },
      { CBE::ItemType::Unknown      , "Unknown"       },
      { CBE::ItemType::Object       , "Object"        },
      { CBE::ItemType::Container    , "Container"     },
      { CBE::ItemType::Tag          , "Tag"           },
      { CBE::ItemType::Group        , "Group"         }
};
  const auto it = std::find_if(std::begin(ItemTypeValues) /* first */,
                               std::end(ItemTypeValues)   /* last */,
                               [itemType](const ItemTypeValue& ItemTypeValue) {
                                 return ItemTypeValue.itemType == itemType;
                               } /* p */);
  return (it != std::end(ItemTypeValues))? it->str : "unknown-item-type";
}

void Logic::printItem(const CBE::Item& item, bool printParentId) {
  std::cout << std::left << std::setw(31) << item.name()
            << std::left << std::setw(10) << itemTypeString(item.type())
            << std::left << std::setw(3)  << "id:"
            << std::left << std::setw(16) << item.id();
  if (printParentId) {
    std::cout << std::left << std::setw(2)  << "p:" << item.parentId();
  }
  std::cout << std::endl;
}

std::string Logic::containerName(CBE::ContainerPtr  container,
                                 bool               temporary) {
  std::ostringstream oss;
  oss << container->name() <<  " (" << (temporary? "(" : "")
      << container->id() << (temporary? ")" : "") << ")";
  return oss.str();
}

std::string Logic::objectName(CBE::ObjectPtr object,
                              bool           temporary) {
  std::ostringstream oss;
  oss << object->name() <<  " (" << (temporary? "(" : "")
      << object->id() << (temporary? ")" : "") << ")";
  return oss.str();
}
