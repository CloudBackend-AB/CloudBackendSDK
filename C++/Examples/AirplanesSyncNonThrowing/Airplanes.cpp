#include "Airplanes.h"

#include "cbe/CloudBackend.h"
#include "cbe/QueryChain.h"
#include "cbe/QueryChainSync.h"

#include <algorithm>
#include <condition_variable>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <stdexcept>

#include "user_credentials.cpp"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Airplanes::Airplanes() : cloudBackend{cbe::DefaultCtor{}}, 
                         account{cbe::DefaultCtor{}} {

  cbe::CloudBackend::LogInError logInError;
  cloudBackend = cbe::CloudBackend::logIn(username, 
                                          password, 
                                          tenant, 
                                          client,
                                          logInError);
  if (logInError){
    std::cout << "Error, login failed! \nError info="
              << logInError << std::endl;
  } else {
    account = cloudBackend.account();
  }
} // Airplanes ctor
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

cbe::QueryResult Airplanes::query(cbe::Filter      filter,
                                  cbe::ContainerId containerId) {
  
  cbe::Container::QueryError queryError;
  auto result = cloudBackend.query(containerId, filter, queryError);
  if (queryError){
    std::cout << "Error!"   << std::endl 
              << queryError << std::endl;
    return cbe::QueryResult{cbe::DefaultCtor{}};
  }   
  return result.getQueryResult();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

cbe::Container Airplanes::createContainer(cbe::Container     parentContainer,
                                          const std::string& name) {
  cbe::Container::CreateContainerError createContainerError;
  auto result = parentContainer.createContainer(name, createContainerError);
  if (createContainerError){
    std::cout << "Error!"   << std::endl 
              << createContainerError << std::endl;
    return cbe::Container{cbe::DefaultCtor{}};
  }   
  return *result; 
} 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

cbe::Object Airplanes::createObject(cbe::Container     parentContainer,
                                    const std::string& name,
                                    cbe::KeyValues&&   keyValues) {
  std::cout << std::setw(14) << name;
  std::for_each(std::begin(keyValues), std::end(keyValues),
       [](const cbe::KeyValues::value_type& entry) {
         std::cout << std::setw(14) << entry.second.first;
       });
  std::cout << std::endl;

  cbe::Container::CreateObjectError createObjectError;
  auto result = parentContainer.createObject(name, keyValues, createObjectError);
  if (createObjectError){
    std::cout << "Error!"   << std::endl 
              << createObjectError << std::endl;
    return cbe::Object{cbe::DefaultCtor{}};
  }   
  return *result; 
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Airplanes::removeContainer(cbe::Container container) {
  cbe::Container::RemoveError removeError;
  container.remove(removeError);
  if (removeError){
    std::cout << "Error!"   << std::endl 
              << removeError << std::endl;
  }   
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
