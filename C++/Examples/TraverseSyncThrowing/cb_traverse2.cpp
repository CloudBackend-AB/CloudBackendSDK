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
  cbe::CloudBackend cloudBackend{cbe::DefaultCtor{}};
  try
  {
    cloudBackend = cbe::CloudBackend::logIn("githubtester2", 
                             "gitHubTester2password", 
                             "cbe_githubtesters");
  }
  catch (const cbe::CloudBackend::LogInException& e)
  {
    std::cout << "error, login failed! \nerror info="
              << e.what() << std::endl;
  }
  auto account = cloudBackend.account();
  std::cout << "firstName=\"" << account.firstName()
            << "\", lastName=\"" << account.lastName() << "\"" 
            << "\troot=" << account.rootContainer().id()
            << std::endl;

  auto query = [](cbe::Container container) {
    return container.query().getQueryResult();
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

  using Map = std::map<cbe::ContainerId, Container>;
  Map map;
  std::function<void(cbe::Container, Container*)> processContainer =
    [&map, &processContainer, &query, &cloudBackend](
        cbe::Container    cbeContainer,
        Container*        parentContainer) {
    std::cout << '.' << std::flush;
    auto p = map.emplace(cbeContainer.id() /* key */,
                         Container{cbeContainer, parentContainer});
    assert(p.second); // entry inserted
    auto currentContainer = &p.first->second;
    auto queryResult = query(cbeContainer);

    for (auto& item : queryResult.getItemsSnapshot()) {
      if (item.type() == cbe::ItemType::Container) {
        auto childCbeContainer = cbe::CloudBackend::castContainer(item);
        processContainer(childCbeContainer,
                         currentContainer /* parentContainer */);
      }
    }
  };  // function processContainer

  std::cout << "***** listing shares *****" << std::endl;

  cbe::ShareManager shareManager = cloudBackend.shareManager();
  cbe::QueryResult shares{cbe::DefaultCtor{}};
  try
  {
    shares = shareManager.listAvailableShares();
  }
  catch (const cbe::ShareManager::ListSharesException& e)
  {
    std::cout << "Error!" << std::endl << e.what() << std::endl;
  }
  std::vector<cbe::Item> items = shares.getItemsSnapshot();
  for (auto& item : items) {
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
  std::cout << "\n***** Shares sorted id number *****\n";
  for (const auto& elem : map) {
    std::cout << elem.first << " " << elem.second.path() << '\n';
  }

  std::cout << "\n***** traversing containers *****" << std::endl;
  processContainer(account.rootContainer(), nullptr /* parentContainer */);

  std::cout << "\n***** Containers sorted id number *****\n";
  for (const auto& elem : map) {
    std::cout << elem.first << " " << elem.second.path() << '\n';
  }
  cloudBackend.terminate();
}  // main
