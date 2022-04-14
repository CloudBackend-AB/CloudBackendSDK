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
void Logic::loadContainerObjects(CBE::ContainerPtr container) {
  std::cout << "Getting objects in container " << containerName(container) << std::endl;
  CBE::ItemDelegatePtr itemDelegate = std::make_shared<ItemEventProtocol>(this);
  CBE::Filter filter2;
  filter2.setDataType(CBE::ItemType::Object);
  container->query(filter2, itemDelegate);
}


void Logic::printObjects(CBE::QueryResultPtr q) {
  CBE::ObjectPtr tempObject{}; // nullptr;
  std::cout << "Printing Objects from query result: " << std::endl;
  for (const auto& itemPtr : q->getItemsSnapshot()) {
    if(itemPtr->type() == CBE::ItemType::Object) {
      printItem(*itemPtr);
      tempObject = CBE::CloudBackend::castObject(itemPtr);
      const auto keyValues = tempObject->keyValues();
      if(!keyValues.empty()) {
        for (const auto& keyValue : keyValues) {
          const auto& key      = keyValue.first;
          const auto& sdkValue = keyValue.second;
          const auto  indexed  = std::get<metadata_dataindex_indexed>(sdkValue);
          std::cout << "    " << key << " = " << "'"
                    << std::get<metadata_dataindex_value>(sdkValue)
                    << "'" << (indexed? " \t\t(indexed)" : "") << std::endl;
        }
      }  // if(!keyValues.empty())
    }   // if(itemPtr->type() == CBE::ItemType::Object)
  }
  if(!tempObject) {
    std::cout << "Sorry, no objects found in the Container!" << std::endl;
  }
}


CBE::ContainerPtr Logic::selectContainer(const std::string& prompt) {
  std::cout << "Select Container" << std::endl;
  const auto items = qResult->getItemsSnapshot();
  while (true) {
    const auto containerName =
      inquireString(prompt);
    for (const auto& item : items) {
      if(item->name() == containerName) {
        return CBE::CloudBackend::castContainer(item);
      }
    }
    std::cout << "Error: the container you asked for, \"" << containerName
              << "\", was not found; following are the options:" << std::endl;
    for(const auto& item : items) {
      printItem(*item);
    }
  }
}


CBE::ObjectPtr Logic::createObject(CBE::ContainerPtr inContainer) {
  std::cout << "Create Object" << std::endl;
  const std::string name = inquireString("Set name for Object");
  const int numOftags =
                      inquireInt("Set the number of Key/Value pairs you want");
  CBE::metadata_type keyValues;
  for(int i = 1; i <= numOftags; i++) {
    const auto tag = inquireString("Name of Key #" + std::to_string(i));
    const auto value =
            inquireString("Value for #" + std::to_string(i) + " '" + tag + "'");
    const auto indexed =
                       inquireBool("Make KeyValue pair #" + std::to_string(i) +
                                   " indexed or not (y indexed, n not indexed",
                                   true /* defaultVal */);
    keyValues[tag] = std::pair<std::string, bool>(value, indexed);
  }
  CBE::ItemDelegatePtr itemDelegate = std::make_shared<ItemEventProtocol>(this);
  if (numOftags > 0) {
    return inContainer->createObject(name, itemDelegate, keyValues);
  } else {
    return inContainer->createObject(name, itemDelegate);
  }
}



/** Logic Exercise 1,2,3 */
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

      /* Look in the target container */
      case 3: {
        ++step;
        loadContainerContents(rootContainer);
        break;
      }

      /* print list of items; select target container; create object */
      case 4: {
        printContainerContents(qResult);
        std::cout << "Item count: " << qResult->getItemsSnapshot().size()
                  << std::endl;
        container =
            selectContainer(
              "In which Container do you want to create the new object");
        std::cout << "Container: " << containerName(container) << std::endl;
        object = createObject(container);
        std::cout << "Designated name: "
                  << objectName(object, true /* temporary */) << std::endl;
        ++step;
        break;
      }

      /* object created; load the top container list of items in cache */
      case 5: {
        std::cout << "Object name: " << objectName(object) << std::endl;
        loadContainerObjects(container);
        ++step;
        break;
      }

      /* print list of objects and their key/value data */
      case 6: {
        printObjects(qResult);
        ++step;
        logic();
        break;
      }

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
