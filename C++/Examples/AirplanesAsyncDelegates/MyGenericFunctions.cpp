/*
  Copyright © CloudBackend AB 2022 - 2023.
*/

#include <algorithm>  // std::find_if_not
#include <cctype>     // std:isspace, std::tolower, std::toupper
#include <iterator>
#include <iomanip>    // std::setw
#include <ios>        // std::left
#include <iostream>
#include <sstream>    // std::ostringstream

// - - - - - - - - - - - - - - GENERIC FUNCTIONS - - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - GENERIC INPUT FUNCTIONS - - - - - - - - - - - - - -

namespace {

  const bool  noBoolDefaultVal{}; // = false
  bool inquireBool(const std::string& prompt,
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
  } // bool inquireBool

  const int noIntDefaultVal{}; // = 0
  int inquireInt(const std::string& prompt,
                        const int&  defaultVal) {

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
  } // int inquireInt

  std::string trimString(const std::string& str) {
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
  } // std::string trimString

  const std::string noStringDefaultVal{}; // ""
  std::string inquireString(const std::string&  prompt,
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
  } // std::string inquireString

  // - - - - - - - - - - - - - GENERIC PRINT FUNCTIONS - - - - - - - - - - - - - -

  const char* itemTypeString(cbe::ItemType itemType) {
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
  } // const char* itemTypeString

  void printItem(const cbe::Item& item, bool printParentId) {
    std::cout << std::left << std::setw(32) << item.name()
              << std::left << std::setw(10) << itemTypeString(item.type())
              << std::left << std::setw(3)  << "id:"
              << std::left << std::setw(16) << item.id();
    if (printParentId) {
      std::cout << std::left << std::setw(2)  << "p:" << item.parentId();
    }
    std::cout << std::endl;
  } // void printItem

  std::string containerName(cbe::Container container,
                                      bool           temporary) {
    std::ostringstream oss;
    oss << container.name() <<  " (" << (temporary? "(" : "")
        << container.id() << (temporary? ")" : "") << ")";
    return oss.str();
  } // std::string containerName

  std::string objectName(cbe::Object object,
                                bool           temporary) {
    std::ostringstream oss;
    oss << object.name() <<  " (" << (temporary? "(" : "")
        << object.id() << (temporary? ")" : "") << ")";
    return oss.str();
  } // std::string objectName

}  //  namespace
