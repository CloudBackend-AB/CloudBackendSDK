#include "cbe/Account.h"
#include "cbe/CloudBackend.h"
#include "cbe/QueryChain.h"
#include "cbe/ShareManager.h"

#include "cbe/delegate/LogInDelegate.h"
#include "cbe/delegate/QueryDelegate.h"
#include "cbe/delegate/ListSharesDelegate.h"

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

// - - - - - - - - - - - - - - - - - DELEGATES - - - - - - - - - - - - - - - - -
class LogInDelegate :  public cbe::delegate::LogInDelegate
{
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};

  bool                    called = false;

  void onLogInSuccess(cbe::CloudBackend&& cloudBackend) override {
     {
      std::lock_guard<std::mutex> lock(mutex);
      this->cloudBackend = std::move(cloudBackend); 
      called = true;
     }
     conditionVariable.notify_one();

   }
   void onLogInError(cbe::delegate::Error&& error, cbe::util::Context&& context) override {
     {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
     }
     conditionVariable.notify_one();
   }
public:
  /*implementation of delegates */
  cbe::CloudBackend cloudBackend{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void wait() {
    std::unique_lock<std::mutex> lock(mutex);
    std::cout << "Waiting, to be logged in" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    std::cout << "Now we have waited: " << called << std::endl;
  }

}; // class LogInDelegate

class QueryDelegate :  public cbe::delegate::QueryDelegate
{
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};
  bool                    called = false;

  /**
   * Called upon successful query.
   * @param queryResult Instance of a QueryResult containing the result set.
   */
  void onQuerySuccess(cbe::QueryResult&& queryResult) override {
    {      
      std::lock_guard<std::mutex> lock(mutex);
      this->queryResult = std::move(queryResult); 
      called = true;
    }           
    conditionVariable.notify_one();
  };

  /**
   * Called upon a failed query() or join() call.
   * @param error   Error information passed from %CloudBackend SDK.
   * @param context Additional context information about the original service
   *                call that has failed.
   */
  void onQueryError(cbe::delegate::QueryError&& error,
                    cbe::util::Context&&        context) override {
    {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one();
  };

public:
  /*implementation of delegates */
  cbe::QueryResult queryResult{cbe::DefaultCtor{}};
  ErrorInfo errorInfo{};

  void wait() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for query" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
  }
}; // class QueryDelegate

class ListSharesDelegate :  public cbe::delegate::ListSharesDelegate 
{
  std::mutex              mutex{};
  std::condition_variable conditionVariable{};
  bool                    called = false;
  
  void onListSharesSuccess(cbe::QueryResult&& qResult) override {
    {    
      std::lock_guard<std::mutex> lock(mutex);
      this->shares = std::move(qResult); 
      called = true;
    } 
    conditionVariable.notify_one(); 
  }

  void onListSharesError(cbe::delegate::Error&& error, 
                          cbe::util::Context&& context) override {
    {
      std::lock_guard<std::mutex> lock(mutex);
      errorInfo = ErrorInfo{std::move(context), std::move(error)};
      called = true;
    }
    conditionVariable.notify_one(); 
  }

  public:
    cbe::QueryResult shares{cbe::DefaultCtor{}};
    ErrorInfo errorInfo{};

    void wait() {
      std::unique_lock<std::mutex> lock(mutex);
      std::cout << "Waiting, loading shares" << std::endl;
      conditionVariable.wait(lock, [this] { return called; });
      std::cout << "Now we have waited: " << called << std::endl;
    }
}; // class ListSharesDelegate

// - - - - - - - - - - - - - - - - - - MAIN  - - - - - - - - - - - - - - - - - - 

int main(void) {
  std::set_terminate([]() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  });

  std::cout << "about to log in" << std::endl;
  std::shared_ptr<LogInDelegate> logInDelegate = 
                                              std::make_shared<LogInDelegate>();
  cbe::CloudBackend::logIn("githubtester2", "gitHubTester2password", 
                           "cbe_githubtesters", logInDelegate);
  logInDelegate->wait();

  auto cloudBackend = logInDelegate->cloudBackend;
  if (!cloudBackend) {
    std::cerr << "Failed to login" << std::endl;
    return -1;
  }
  auto account = cloudBackend.account();
  std::cout << "firstName=\"" << account.firstName()
            << "\", lastName=\"" << account.lastName() << "\"" 
            << "\troot=" << account.rootContainer().id()
            << std::endl;

  auto query = [](cbe::Container container) {
  std::shared_ptr<QueryDelegate> queryDelegate = 
                                              std::make_shared<QueryDelegate>();
    container.query(queryDelegate);
    queryDelegate->wait();
    return queryDelegate->queryResult; 
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
  std::function<void(cbe::Item,Container*)> processItem =
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
        Container*     parentContainer) {
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

  std::cout << "***** listing shares *****";
  auto shareManager = cloudBackend.shareManager();
  std::shared_ptr<ListSharesDelegate> listSharesDelegate = 
                                         std::make_shared<ListSharesDelegate>();
  shareManager.listAvailableShares(listSharesDelegate);
  listSharesDelegate->wait();
  auto shares = listSharesDelegate->shares; 
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
