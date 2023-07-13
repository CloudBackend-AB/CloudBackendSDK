#include "Airplanes.h"

#include "cbe/CloudBackend.h"
#include "cbe/QueryChain.h"

#include <algorithm>
#include <condition_variable>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <stdexcept>

#include "MyDelegates.cpp"
#include "user_credentials.cpp"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Airplanes::Airplanes() : cloudBackend{cbe::DefaultCtor{}}, 
                         account{cbe::DefaultCtor{}} {

  std::shared_ptr<LogInDelegate> logInDelegate = 
                                              std::make_shared<LogInDelegate>();

  logInDelegate->cloudBackend = cbe::CloudBackend::logIn(username, 
                                                         password, 
                                                         tenant, 
                                                         client,
                                                         logInDelegate);
  logInDelegate->waitForRsp();

  cloudBackend = logInDelegate->cloudBackend;
  account = cloudBackend.account();
  if (logInDelegate->errorInfo) {
    std::cout << "Error info: " << std::endl;
    std::cout << logInDelegate->errorInfo << std::endl;
  }
} // Airplanes ctor

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

cbe::QueryResult Airplanes::query(cbe::Filter      filter,
                                  cbe::ContainerId containerId) {
  
  std::shared_ptr<QueryDelegate> queryDelegate = 
                                              std::make_shared<QueryDelegate>();
  cloudBackend.query(containerId, filter, queryDelegate);
  queryDelegate->waitForRsp();
  if (queryDelegate->errorInfo) {
    std::cout << "Error info: " << std::endl;
    std::cout << queryDelegate->errorInfo << std::endl;
    throw std::runtime_error("airport query");
  }
  return queryDelegate->queryResult;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

cbe::Container Airplanes::createContainer(cbe::Container     parentContainer,
                                          const std::string& name) {
  std::shared_ptr<MyCreateContainerDelegate> createContainerDelegate = 
                                  std::make_shared<MyCreateContainerDelegate>();

  parentContainer.createContainer(name, createContainerDelegate);
  createContainerDelegate->waitForRsp();
  if (createContainerDelegate->errorInfo) {
    std::cout << "Error info: " << std::endl;
    std::cout << createContainerDelegate->errorInfo << std::endl;
    throw std::runtime_error(name+" container create");
  }
  return createContainerDelegate->container; 
} 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

cbe::Object Airplanes::createObject(cbe::Container     parentContainer,
                                    const std::string& name,
                                    cbe::KeyValues&&   keyValues) {
  std::cout << std::setw(14) << name;
  std::shared_ptr<QueryDelegate> queryDelegate = 
                                              std::make_shared<QueryDelegate>();
  std::for_each(std::begin(keyValues), std::end(keyValues),
       [](const cbe::KeyValues::value_type& entry) {
         std::cout << std::setw(14) << entry.second.first;
       });
  std::cout << std::endl;
  std::shared_ptr<MyCreateObjectDelegate> createObjectDelegate = 
                                     std::make_shared<MyCreateObjectDelegate>();
  parentContainer.createObject(name, keyValues, createObjectDelegate);
  createObjectDelegate->waitForRsp();
  if (createObjectDelegate->errorInfo) {
    std::cout << "Error info: " << std::endl;
    std::cout << createObjectDelegate->errorInfo << std::endl;
    throw std::runtime_error(name+" object create");
  }

  // std::cout << "Created object \"" << createObjectDelegate->object.name() 
  //           << "\" with id " << createObjectDelegate->object.id() << '\n';
  return createObjectDelegate->object;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Airplanes::removeContainer(cbe::Container container) {
  std::shared_ptr<RemoveDelegate> removeDelegate = 
                                             std::make_shared<RemoveDelegate>();
  container.remove(removeDelegate);
  removeDelegate->waitForRsp();
  if (removeDelegate->errorInfo) {
    std::cout << "Error info: " << std::endl;
    std::cout << removeDelegate->errorInfo << std::endl;
    throw std::runtime_error(container.name()+" container delete");
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
