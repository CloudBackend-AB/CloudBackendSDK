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
    cloudBackend = cbe::CloudBackend::logIn(username, password, tenant, client);
    account = cloudBackend.account();
} // Airplanes ctor

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

cbe::QueryResult Airplanes::query(cbe::Filter      filter,
                                  cbe::ContainerId containerId) {
  return cloudBackend.query(containerId, filter).getQueryResult();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

cbe::Container Airplanes::createContainer(cbe::Container     parentContainer,
                                          const std::string& name) {
  return parentContainer.createContainer(name);
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
  return parentContainer.createObject(name, keyValues);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Airplanes::removeContainer(cbe::Container container) {
  container.remove();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
