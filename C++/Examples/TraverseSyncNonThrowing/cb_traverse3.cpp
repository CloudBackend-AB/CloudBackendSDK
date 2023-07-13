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
  auto cloudBackend = cbe::CloudBackend::logIn("githubtester3", 
                                               "gitHubTester3password", 
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
    auto queryResult = container.query(queryError).getQueryResult();
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
  };

  using ContainerMap = std::map<cbe::ContainerId, Container>;
  struct Object {
    const std::string name;
    Container&        parentContainer;
  };
  using ObjectMap = std::map<cbe::ItemId, Object>;
  ContainerMap containerMap;
  ObjectMap objectMap;
  std::function<void(cbe::Container,Container*)> processContainer =
    [&containerMap, &objectMap, &processContainer, &query, &cloudBackend](
        cbe::Container cbeContainer,
        Container*        parentContainer) {
    std::cout << '.' << std::flush;
    auto p = containerMap.emplace(cbeContainer.id() /* key */,
                                  Container{cbeContainer, parentContainer});
    assert(p.second); // entry inserted
    auto currentContainer = &p.first->second;
    auto queryResult = query(cbeContainer);
    for (auto& item : queryResult.getItemsSnapshot()) {
      if (item.type() == cbe::ItemType::Container) {
        auto childCbeContainer = cbe::CloudBackend::castContainer(item);
        processContainer(childCbeContainer,
                         currentContainer /* parentContainer */);
      } else if (item.type() == cbe::ItemType::Object) {
        objectMap.emplace(item.id() /* key */, 
                                       Object{item.name(), *currentContainer });
      }
    }
  }; // processContainer() lambda

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
    if (item.type() == cbe::ItemType::Container) {
      auto shareContainer = cbe::CloudBackend::castContainer(item);
      processContainer(shareContainer, nullptr /* parentContainer */);
    }
  }
  std::cout << "\n***** Shares sorted *****\n";
  for (const auto& elem : containerMap) {
    std::cout << elem.first << " " << elem.second.path() << '\n';
  }

  std::cout << "\n***** traversing containers *****" << std::endl;
  processContainer(account.rootContainer(), nullptr /* parentContainer */);
  std::cout << "\n***** Containers sorted by id number *****\n";
  for (const auto& elem : containerMap) {
    std::cout << elem.first << " " << elem.second.path() << '\n';
  }

  std::cout << "\n+++++ Objects sorted +++++\n";
  for (const auto& elem : objectMap) {
    std::cout << elem.first << " "
              << elem.second.parentContainer.path() << '/' << elem.second.name
              << '\n';
  }
  cloudBackend.terminate();
}  // main
