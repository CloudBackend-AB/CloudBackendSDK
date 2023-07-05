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

  try
  {
    cloudBackend = cbe::CloudBackend::logIn(username, password, tenant);
    account = cloudBackend.account();
  }
  catch (const cbe::CloudBackend::LogInException& e)
  {
    std::cout << "error, login failed! \nerror info="
              << e.what() << std::endl;
  }
} // Airplanes ctor

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

cbe::QueryResult Airplanes::query(cbe::Filter      filter,
                                  cbe::ContainerId containerId) {
  cbe::QueryResult tmp{cbe::DefaultCtor{}};
  try
  {
    tmp = cloudBackend.query(containerId, filter).getQueryResult();
  }
  catch (const cbe::Container::QueryException& e)
  {
    std::cout << "Error!" << std::endl << e.what() << std::endl;
  } 
  return tmp;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

cbe::Container Airplanes::createContainer(cbe::Container     parentContainer,
                                          const std::string& name) {
  cbe::Container tmp{cbe::DefaultCtor{}};
  try
  {
    tmp = parentContainer.createContainer(name);
  }
  catch (const cbe::Container::CreateContainerException& e)
  {
    std::cout << "Error!" << std::endl << e.what() << std::endl;
  }
  return tmp;
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
  cbe::Object tmp{cbe::DefaultCtor{}};
  try
  {
    tmp = parentContainer.createObject(name, keyValues);
  }
  catch (const cbe::Container::CreateObjectException& e)
  {
    std::cout << "Error!" << std::endl << e.what() << std::endl;
  }
  return tmp;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Airplanes::removeContainer(cbe::Container container) {
  try
  {
    container.remove();
  }
  catch (const cbe::Container::RemoveException& e)
  {
    std::cout << "Error!" << std::endl << e.what() << std::endl;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
