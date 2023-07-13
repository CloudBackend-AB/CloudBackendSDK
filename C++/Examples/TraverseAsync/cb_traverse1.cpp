
#include "cbe/Account.h"
#include "cbe/CloudBackend.h"
#include "cbe/QueryChain.h"

#include <chrono>
#include <condition_variable>
#include <exception>          // std::set_terminate
#include <functional>         // std::function
#include <iomanip>
#include <iostream>
#include <mutex>
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
   void onLogInError(cbe::delegate::Error&& error, 
                     cbe::util::Context&& context) override {
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

  void waitForRsp() {
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

  void waitForRsp() {
    std::unique_lock<std::mutex> lock(mutex);
    // std::cout << "Waiting, for query" << std::endl;
    conditionVariable.wait(lock, [this] { return called; });
    // std::cout << "Now we have waited: " << called << std::endl;
  }
}; // class QueryDelegate

// - - - - - - - - - - - - - - - - - - MAIN  - - - - - - - - - - - - - - - - - - 

int main(void) {
  std::set_terminate([]() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  });

  std::cout << "about to log in" << std::endl;
  std::shared_ptr<LogInDelegate> logInDelegate = 
                                              std::make_shared<LogInDelegate>();

  cbe::CloudBackend::logIn("githubtester1", "gitHubTester1password", 
                           "cbe_githubtesters", "linux_desktop", logInDelegate);
  logInDelegate->waitForRsp();
  auto cloudBackend = logInDelegate->cloudBackend;
  
  if (!cloudBackend) {
    std::cerr << "Failed to login" << std::endl;
    return -1;
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
    std::shared_ptr<QueryDelegate> queryDelegate = 
                                              std::make_shared<QueryDelegate>();    
    container.query(queryDelegate);
    queryDelegate->waitForRsp();
    auto queryResult = queryDelegate->queryResult; 
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
  return 0;
}  // main
