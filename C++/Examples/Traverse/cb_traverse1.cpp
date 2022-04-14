
#include "CBE/Account.h"
#include "CBE/CloudBackend.h"
#include "CBE/Protocols/ItemEventProtocol.h"
#include "CBE/Protocols/AccountEventProtocol.h"

#include <chrono>
#include <condition_variable>
#include <exception>          // std::set_terminate
#include <functional>         // std::function
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

int main(void) {
  std::set_terminate([]() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  });

  class AccountEventProtocol : public CBE::AccountEventProtocol {
    std::mutex              mutex{};
    std::condition_variable conditionVariable{};
    CBE::CloudBackendPtr    cloudbackend{};
    bool  responseReceived = false;
    public:
      void onLogin(uint32_t atState, CBE::CloudBackendPtr cloudbackend) final {
        std::cout << "Account Login complete"
                  << " - name: "<< cloudbackend->account()->username()
                  << " - id: "<< cloudbackend->account()->userId() 
                  << std::endl;
        {
          std::lock_guard<std::mutex> lock(mutex);
          this->cloudbackend = cloudbackend;
          responseReceived = true;
        }
        conditionVariable.notify_one();
      }
      void onError(CBE::persistence_t failedAtState, uint32_t code,
                                      std::string reason,
                                      std::string message) final {
        std::cerr << "Account Event Error: Login failed" << '\n' ;
        {
          std::lock_guard<std::mutex> lock(mutex);
          responseReceived = true;
        }
        conditionVariable.notify_one();
      }
      CBE::CloudBackendPtr wait() {
        std::unique_lock<std::mutex> lock(mutex);
        conditionVariable.wait(lock, [this]{ return responseReceived; } );
        return cloudbackend;
      }
  }; // class AccountEventProtocol

  std::cout << "about to log in" << std::endl;
  auto accountDelegate = std::make_shared<AccountEventProtocol>();
  CBE::CloudBackend::logIn("githubtester1", "gitHubTester1password", "cbe_githubtesters",
                           accountDelegate);
  auto cloudBackend = accountDelegate->wait();
  if (!cloudBackend) {
    std::cerr << "Failed to login" << std::endl;
    return -1;
  }
  auto account = cloudBackend->account();
  std::cout << "firstName=\"" << account->firstName()
            << "\", lastName=\"" << account->lastName() << "\"" 
            << "\troot=" << account->rootContainer()->id()
            << std::endl;

  class ItemEventProtocol : public CBE::ItemEventProtocol {
    CBE::ContainerPtr       container{};
    std::mutex              mutex{};
    std::condition_variable conditionVariable{};
    CBE::QueryResultPtr     queryResult{};
    bool                    responseReceived = false;
    public:
      ItemEventProtocol(CBE::ContainerPtr container) : container{container} {}
      CBE::QueryResultPtr  wait() {
        std::unique_lock<std::mutex> lock(mutex);
        conditionVariable.wait(lock, [this]{ return responseReceived; } );
        return queryResult;
      }
    private:
      void onQueryLoaded(CBE::QueryResultPtr dir) final {
        {
          std::lock_guard<std::mutex> lock(mutex);
          queryResult = dir;
          responseReceived = true;
        }
        conditionVariable.notify_one();
      }

      void onLoadError(CBE::Filter filter, uint32_t operation, uint32_t code,
                      std::string reason, std::string message) final {
        std::cerr << "Failed to query container \""
                  << container->path() << container->name() <<'\n' ;
        {
          std::lock_guard<std::mutex> lock(mutex);
          responseReceived = true;
        }
        conditionVariable.notify_one();
      }
  }; // class ItemEventProtocol

  using Map = std::map<CBE::container_id_t, std::string>;
  //  Map map;
  auto query = [](CBE::ContainerPtr container) {
    auto itemDelegate = std::make_shared<ItemEventProtocol>(container);
    container->query(itemDelegate);
    return itemDelegate->wait();
  };

  unsigned cnt = 0;
  std::function<Map(CBE::ContainerPtr,const std::string&,Map)> processContainer =
      [&](CBE::ContainerPtr container, const std::string& parentPath, Map map) {
    auto queryResult = query(container);
    for (auto& itemPtr : queryResult->getItemsSnapshot()) {
      if (itemPtr->type() == CBE::ItemType::Container) {
        auto container = CBE::CloudBackend::castContainer(itemPtr);
        auto path = parentPath + container->name() + '/';
        auto id = container->id();
        std::cout << std::setw(16) << id
                  << std::string(cnt* 2, ' ') << ' ' << path << '\n';
        map.emplace(id, path);
        map = std::move(processContainer(container, path /* parentPath */, std::move(map)));
      }
    }
    return map;
  };  // function processContainer lambda

  std::cout << std::endl;
  std::cout << "***** traversing containers *****" << std::endl;
  std::cout << std::setw(16) << account->rootContainer()->id()
            << std::string(cnt* 2, ' ') << ' ' << "/" << '\n';
  auto map = processContainer(account->rootContainer(), "/" /* parentPath */, Map{});

  map.emplace(account->rootContainer()->id(), "/");

  std::cout << std::endl;
  std::cout << "***** containers sorted by id number *****" << std::endl;
  for (const auto& elem : map) {
    std::cout << elem.first << " " << elem.second << '\n';
  }

}  // main
