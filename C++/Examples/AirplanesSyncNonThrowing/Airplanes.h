#ifndef Airplanes_h__
#define Airplanes_h__

#include "cbe/Account.h"
#include "cbe/CloudBackend.h"
#include "cbe/Filter.h"
#include "cbe/Types.h"

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <random>

class Airplanes
{
public:
  cbe::CloudBackend cloudBackend;
  cbe::Account account;

  Airplanes();
  Airplanes(const Airplanes &) = delete;

  cbe::QueryResult query(cbe::Filter      filter,
                         cbe::ContainerId containerId);

  cbe::Container createContainer(cbe::Container    parentContainer,
                                 const std::string &name);

  cbe::Object createObject(cbe::Container    parentContainer,
                           const std::string &name,
                           cbe::KeyValues    &&keyValues);

  void removeContainer(cbe::Container container);
};


class Random {
  std::random_device  rd{};
  std::mt19937        generator{rd()};
public:
  using Int = std::uint64_t;

  Int nextInt() {
    return nextInt(Int{1} << 48 /* bound */);
  }
  Int nextInt(Int bound) {
    std::uniform_int_distribution<Int>  distribution{0, bound - 1};
    return distribution(generator);
  }
};


#endif
