
#include "CBE/Account.h"
#include "CBE/CloudBackend.h"
#include "CBE/Protocols/ItemEventProtocol.h"
#include "CBE/Protocols/AccountEventProtocol.h"
#include "CBE/Protocols/ShareEventProtocol.h"

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


template <typename ResultT>
class Sync {
  std::mutex                mutex{};
  std::condition_variable   conditionVariable{};
  bool                      responseReceived = false;

  using ResultPtr = std::shared_ptr<ResultT>;
  ResultPtr                 resultPtr{};
public:
  void onSuccess(ResultPtr resultPtr) {
    {
      std::lock_guard<std::mutex> lock(mutex);
      this->resultPtr = resultPtr;
      responseReceived = true;
    }
    conditionVariable.notify_one();
  }
  void onError() {
    {
      std::lock_guard<std::mutex> lock(mutex);
      responseReceived = true;
    }
    conditionVariable.notify_one();
  }
  ResultPtr wait() {
    std::unique_lock<std::mutex> lock(mutex);
    conditionVariable.wait(lock, [this]{ return responseReceived; } );
    return resultPtr;
  }
}; // class Sync

int main(void) {
  std::set_terminate([]() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  });

  class AccountEventProtocol : public CBE::AccountEventProtocol {
    Sync<CBE::CloudBackend> sync{};
  public:
    void onLogin(uint32_t atState, CBE::CloudBackendPtr cloudbackend) final {
      std::cout << "Account Login complete"
                << " - name: "<< cloudbackend->account()->username()
                << " - id: "<< cloudbackend->account()->userId() << std::endl;
      sync.onSuccess(cloudbackend);
    }
    void onError(CBE::persistence_t failedAtState, uint32_t code,
                                    std::string reason,
                                    std::string message) final {
      std::cerr << "Account Event Error: Login failed" << '\n' ;
      sync.onError();
    }
    CBE::CloudBackendPtr wait() {
      return sync.wait();
    }
  }; // class AccountEventProtocol

  std::cout << "about to log in" << std::endl;
  auto accountDelegate = std::make_shared<AccountEventProtocol>();
  CBE::CloudBackend::logIn("githubtester2", "gitHubTester2password", "cbe_githubtesters",
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
    Sync<CBE::QueryResult>  sync{};
  public:
    ItemEventProtocol(CBE::ContainerPtr container) : container{container} {}
    CBE::QueryResultPtr  wait() {
      return sync.wait();
    }
  private:
    void onQueryLoaded(CBE::QueryResultPtr dir) final {
      sync.onSuccess(dir);
    }

    void onLoadError(CBE::Filter filter, uint32_t operation, uint32_t code,
                     std::string reason, std::string message) final {
      std::cerr << "\nFailed to query container:\n"
                << "container->name=\"" << container->name() << "\"\n"
                << "operation=\"" << operation << "\"\n"
                << "code=\"" << code << "\"\n"
                << "reason=\"" << reason << "\"\n"
                << "message=\"" << message << "\"\n"
                ;
      sync.onError();
    }
  }; // class ItemEventProtocol

  auto query = [](CBE::ContainerPtr container) {
    auto itemDelegate = std::make_shared<ItemEventProtocol>(container);
    container->query(itemDelegate);
    return itemDelegate->wait();
  };

  class ShareEventProtocol : public CBE::ShareEventProtocol {
    CBE::ContainerPtr       container{};
    Sync<CBE::QueryResult>  sync{};
  public:
    ShareEventProtocol(CBE::ContainerPtr container) : container{container} {}
    CBE::QueryResultPtr  wait() {
      return sync.wait();
    }
  private:
    void onListAvailableShares(CBE::QueryResultPtr result) final {
      sync.onSuccess(result);
    }

    void onShareError(CBE::item_t type, CBE::persistence_t operation,
                      uint32_t code,
                      std::string reason, std::string message) final {
      std::cerr << __PRETTY_FUNCTION__ << "\nFailed to query share:"
                << "container->name=\"" << container->name() << "\"\n"
                << "operation=\"" << operation << "\"\n"
                << "code=\"" << code << "\"\n"
                << "reason=\"" << reason << "\"\n"
                << "message=\"" << message << "\"\n"
                ;
      sync.onError();
    }
    void onACLError(CBE::item_t type, CBE::persistence_t operation,
                    uint32_t code,
                    std::string reason, std::string message) final {
      std::cerr << __PRETTY_FUNCTION__ << "\nFailed to query share:"
                << "container->name=\"" << container->name() << "\"\n"
                << "operation=\"" << operation << "\"\n"
                << "code=\"" << code << "\"\n"
                << "reason=\"" << reason << "\"\n"
                << "message=\"" << message << "\"\n"
                ;
      sync.onError();
    }
  }; // class ShareEventProtocol


  struct Container {
    CBE::ContainerPtr cbeContainer;
    Container* const  parentContainer;

    std::string path() const {
      std::ostringstream oss;
      std::function<void(const Container&)> buildPath =
        [&oss, &buildPath](const Container& container) {
          if (container.parentContainer) {
            buildPath(*container.parentContainer);
            oss << '/';
          }
          oss << container.cbeContainer->name();
        }; // lambda buildPath
      buildPath(*this);
      return oss.str();
    }
  };  // struct Container

  using ContainerMap = std::map<CBE::container_id_t, Container>;
  struct Object {
    const std::string name;
    Container*        parentContainer;
    std::string path() const {
      return parentContainer? (parentContainer->path() + '/' + name) : name;
    }
  };  // struct Object

  using ObjectMap = std::map<CBE::item_id_t, Object>;
  ContainerMap containerMap;
  ObjectMap objectMap;

  std::function<void(CBE::ContainerPtr,Container*)> processContainer;
  std::function<void(CBE::ItemPtr,Container*)> processItem =
      [&objectMap, &processContainer, &cloudBackend](CBE::ItemPtr  itemPtr,
                                      Container*    parentContainer) {
    if (itemPtr->type() == CBE::ItemType::Container) {
      auto childCbeContainer = CBE::CloudBackend::castContainer(itemPtr);
      processContainer(childCbeContainer, parentContainer);
    } else if (itemPtr->type() == CBE::ItemType::Object) {
      objectMap.emplace(itemPtr->id() /* key */,
                        Object{itemPtr->name(), parentContainer });
    }
  };  // function processItem lambda
  
  processContainer =
    [&containerMap, &objectMap, &processContainer, &query, &cloudBackend, &processItem](
        CBE::ContainerPtr cbeContainer,
        Container*        parentContainer) {
    std::cout << '.' << std::flush;
    auto p = containerMap.emplace(cbeContainer->id() /* key */,
                                  Container{cbeContainer, parentContainer});
    assert(p.second); // entry inserted, conainer not encountered before
    auto& /* Container* */ currentContainer = p.first->second;
    auto queryResult = query(cbeContainer);
    for (auto& itemPtr : queryResult->getItemsSnapshot()) {
      processItem(itemPtr, &currentContainer /* parentContainer */);
    }
  }; // function processContainer() lambda

  std::cout << "***** listing shares *****";
  auto shareManager = cloudBackend->shareManager();
  auto shareDelegate =
      std::make_shared<ShareEventProtocol>(account->rootContainer());
  shareManager->listAvailableShares(shareDelegate);
  auto shares = shareDelegate->wait();
  for (auto& itemPtr : shares->getItemsSnapshot()) {
    std::cout << std::endl;
    std::cout << itemPtr->name() << ", " << itemPtr->description()
              << ",\t" << itemPtr->parentId() 
              << ", " << itemPtr->ownerId()
              << "."  << std::endl;
    processItem(itemPtr, nullptr /* parentContainer */);
  }

  std::cout << "\n***** Shares sorted by id *****\n";
  for (const auto& elem : containerMap) {
    std::cout << elem.first << " " << elem.second.path() << '\n';
  }

  std::cout << "\n***** traversing containers *****" << std::endl;
  processContainer(account->rootContainer(),
                   nullptr /* parentContainer, nullptr implies root container */);

  std::cout << "\n***** Containers sorted by id numbers *****\n";
  for (const auto& elem : containerMap) {
    std::cout << elem.first << " " << elem.second.path() << '\n';
  }

  std::cout << "\n+++++ Objects sorted by id +++++\n";
  for (const auto& elem : objectMap) {
    std::cout << elem.first << " " << elem.second.path() << '\n';
  }

}
