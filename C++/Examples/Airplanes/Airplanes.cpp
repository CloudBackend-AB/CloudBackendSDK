#include "Airplanes.h"

// #include "QueryDelegate.h"

#include "cbe/CloudBackend.h"
#include "cbe/QueryChain.h"

#include <algorithm>
#include <condition_variable>
#include <iomanip>
#include <mutex>
#include <stdexcept>
#include <sstream>


Airplanes::Airplanes(const std::string& credentials) : 
                                               cloudBackend{cbe::DefaultCtor{}}, 
                                               account{cbe::DefaultCtor{}} {
  constexpr const auto  username = "githubtester2";
  constexpr const auto  password = "gitHubTester2password";
  constexpr const auto  tenant   = "cbe_githubtesters";
  
  std::shared_ptr<LogInDelegate> logInDelegate = 
                               std::make_shared<LogInDelegate>();

  logInDelegate->cloudBackend = cbe::CloudBackend::logIn(username, 
                                                         password, 
                                                         tenant, 
                                                         logInDelegate);
  logInDelegate->waitForRsp();
  cloudBackend = logInDelegate->cloudBackend;
  account = cloudBackend.account();
} // Airplanes ctor

namespace {


} // Anonymous namespace

cbe::QueryResult Airplanes::query(cbe::Filter         filter,
                                  cbe::ContainerId containerId) {
  
  std::shared_ptr<QueryDelegate> queryDelegate = 
                                              std::make_shared<QueryDelegate>();
  cloudBackend.query(containerId, filter, queryDelegate);
  queryDelegate->waitForRsp();

  return queryDelegate->queryResult;
}

namespace {


} // Anonymous namespace

cbe::Container Airplanes::createContainer(cbe::Container     parentContainer,
                                          const std::string& name) {
  std::shared_ptr<MyCreateContainerDelegate> createContainerDelegate = 
                                  std::make_shared<MyCreateContainerDelegate>();

  parentContainer.createContainer(name, createContainerDelegate);
  createContainerDelegate->waitForRsp();
  return createContainerDelegate->container; 
} 

cbe::Object Airplanes::createObject(cbe::Container     parentContainer,
                                       const std::string&    name,
                                       cbe::KeyValues&&  keyValues) {
 
  std::for_each(std::begin(keyValues), std::end(keyValues),
       [](const cbe::KeyValues::value_type& entry) {
         std::cout << std::setw(14) << entry.second.first;
       });
  std::cout << std::endl;
  std::shared_ptr<MyCreateObjectDelegate> createObjectDelegate = 
                                     std::make_shared<MyCreateObjectDelegate>();
  parentContainer.createObject(name, createObjectDelegate, keyValues);
  createObjectDelegate->waitForRsp();

  // std::cout << "Created object \"" << createObjectDelegate->object.name() 
  //           << "\" with id " << createObjectDelegate->object.id() << '\n';
  return createObjectDelegate->object;
}
void Airplanes::removeContainer(cbe::Container container) {
  std::shared_ptr<RemoveDelegate> removeDelegate = 
                                  std::make_shared<RemoveDelegate>();
  container.remove(removeDelegate);
  removeDelegate->waitForRsp();
}
