
#include "cbe/Account.h"
#include "cbe/CloudBackend.h"
#include "cbe/QueryChainSync.h"

#include <chrono>
#include <condition_variable>
#include <exception>          // std::set_terminate
#include <functional>         // std::function
#include <iomanip>
#include <iostream>
#include <mutex>
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
    cloudBackend = cbe::CloudBackend::logIn("githubtester1", 
                                            "gitHubTester1password", 
                                            "cbe_githubtesters",
                                            "linux_desktop");
  }
  catch (const cbe::CloudBackend::LogInException& e)
  {
    std::cout << "error, login failed! \nerror info="
              << e.what() << std::endl;
    return 1;
  }
  
  auto account = cloudBackend.account();
  std::cout << "username=\"" << account.username() << std::endl;
  std::cout << "firstName=\"" << account.firstName()
            << "\", lastName=\"" << account.lastName() << "\"" 
            << "\troot=" << account.rootContainer().id()
            << std::endl;

  using Map = std::map<cbe::ContainerId, std::string>;
  unsigned cnt = 0;
  Map map;
  std::function<Map(cbe::Container,const std::string&,Map)> processContainer =
      [&](cbe::Container container, const std::string& parentPath, Map map) {

    try
    {
      auto queryResult = container.query().getQueryResult();
      for (auto& item : queryResult.getItemsSnapshot()) {
        if (item.type() == cbe::ItemType::Container) {
          auto container = cbe::CloudBackend::castContainer(item);
          auto path = parentPath + container.name() + '/';
          auto id = container.id();
          std::cout << std::setw(16) << id
                    << std::string(cnt* 2, ' ') << ' ' << path << '\n';
          map.emplace(id, path);
          map = std::move(processContainer(container, path /* parentPath */, 
                          std::move(map)));
        }
      }
    }
    catch (const cbe::util::Exception& e) {
      std::cout << "Error!"   << e.what() 
                << "\nType: " << e.typeAsString() << std::endl;
    }
    return map;
  };  // function processContainer lambda

  std::cout << std::endl;
  std::cout << "***** traversing containers *****" << std::endl;
  std::cout << std::setw(16) << account.rootContainer().id()
            << std::string(cnt* 2, ' ') << ' ' << "/" << '\n';
  
  map = processContainer(account.rootContainer(), "/" /* parentPath */, Map{});

  map.emplace(account.rootContainer().id(), "/");

  std::cout << std::endl;
  std::cout << "***** containers sorted by id number *****" << std::endl;
  for (const auto& elem : map) {
    std::cout << elem.first << " " << elem.second << '\n';
  }
  cloudBackend.terminate();
}  // main
