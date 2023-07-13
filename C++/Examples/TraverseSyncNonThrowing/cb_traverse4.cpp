#include "cbe/Account.h"
#include "cbe/CloudBackend.h"
#include "cbe/QueryChainSync.h"
#include "cbe/ShareManager.h"

#include <cassert>
#include <chrono>
#include <condition_variable>
#include <exception>          // std::set_terminate
#include <functional>         // std::function
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>            // std::ostringstream
#include <thread>

// - - - - - - - - - - - - - - - - - - MAIN  - - - - - - - - - - - - - - - - - - 

int main(void) {
  std::set_terminate([]() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  });

  std::cout << "about to log in" << std::endl;
  cbe::CloudBackend::LogInError logInError;
  auto cloudBackend = cbe::CloudBackend::logIn("githubtester1", 
                                               "gitHubTester1password", 
                                               "cbe_githubtesters", 
                                               "linux_desktop",
                                               logInError);
  if (logInError) {
    std::cout << "Error! " << logInError << std::endl;
  }
  auto account = cloudBackend.account();
  std::cout << "firstName=\"" << account.firstName()
            << "\", lastName=\"" << account.lastName() << "\"" 
            << "\troot=" << account.rootContainer().id()
            << std::endl;

  auto query = [](cbe::Container container) {
    cbe::Container::QueryError queryError;
    auto queryResult = container.query(queryError);
    if (queryError) {
      std::cout << "Error! " << queryError << std::endl;
    }
    return queryResult;
  };

  struct Container {
    cbe::Container cbeContainer;
    Container* const  parentContainer;

    std::string path() const {
      std::ostringstream oss;
      std::function<void(const Container&)> buildPath =
        [&oss, &buildPath](const Container& container) {
          if (container.parentContainer) {
            buildPath(*container.parentContainer);
            oss << '/';
          }
          oss << container.cbeContainer.name();
        }; // lambda buildPath
      buildPath(*this);
      return oss.str();
    }
  };  // struct Container

  using ContainerMap = std::map<cbe::ContainerId, Container>;
  struct Object {
    const std::string name;
    Container*        parentContainer;
    std::string path() const {
      return parentContainer? (parentContainer->path() + '/' + name) : name;
    }
  };  // struct Object

  using ObjectMap = std::map<cbe::ItemId, Object>;
  ContainerMap containerMap;
  ObjectMap objectMap;

  std::function<void(cbe::Container,Container*)> processContainer;
  std::function<void(cbe::Item, Container*)> processItem =
      [&objectMap, &processContainer, &cloudBackend](cbe::Item  item,
                                      Container*    parentContainer) {
    if (item.type() == cbe::ItemType::Container) {
      auto childCbeContainer = cbe::CloudBackend::castContainer(item);
      processContainer(childCbeContainer, parentContainer);
    } else if (item.type() == cbe::ItemType::Object) {
      objectMap.emplace(item.id() /* key */,
                        Object{item.name(), parentContainer });
    }
  };  // function processItem lambda

  processContainer = [&containerMap, &objectMap, &processContainer, 
                      &query, &cloudBackend, &processItem](
        cbe::Container cbeContainer,
        Container*        parentContainer) {
    std::cout << '.' << std::flush;
    auto p = containerMap.emplace(cbeContainer.id() /* key */,
                                  Container{cbeContainer, parentContainer});
    assert(p.second); // entry inserted, conainer not encountered before
    auto& /* Container* */ currentContainer = p.first->second;
    auto queryResult = query(cbeContainer);
    for (auto& item : queryResult.getItemsSnapshot()) {
      processItem(item, &currentContainer /* parentContainer */);
    }
  }; // function processContainer() lambda

  std::cout << "***** listing shares ***** ";
  auto shareManager = cloudBackend.shareManager();
  cbe::ShareManager::ListSharesError listSharesError;
  auto shares = *shareManager.listAvailableShares(listSharesError);
  if (listSharesError) {
    std::cout << "Error! " << listSharesError << std::endl;
  }
  for (auto& item : shares.getItemsSnapshot()) {
    std::cout << std::endl;
    std::cout << item.name() << ", " << item.description()
              << ",\t" << item.parentId() 
              << ", " << item.ownerId()
              << "."  << std::endl;
    processItem(item, nullptr /* parentContainer */);
  }

  std::cout << "\n***** Shares sorted by id *****\n";
  for (const auto& elem : containerMap) {
    std::cout << elem.first << " " << elem.second.path() << '\n';
  }

  std::cout << "\n***** traversing containers *****" << std::endl;
  processContainer(account.rootContainer(),
                   nullptr /* parentContainer, nullptr implies root container */);

  std::cout << "\n***** Containers sorted by id numbers *****\n";
  for (const auto& elem : containerMap) {
    std::cout << elem.first << " " << elem.second.path() << '\n';
  }

  std::cout << "\n+++++ Objects sorted by id +++++\n";
  for (const auto& elem : objectMap) {
    std::cout << elem.first << " " << elem.second.path() << '\n';
  }
  cloudBackend.terminate();
}
