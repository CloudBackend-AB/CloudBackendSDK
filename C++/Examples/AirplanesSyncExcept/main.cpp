#include "Airplanes.h"

#include "cbe/Account.h"
#include "cbe/CloudBackend.h"
#include "cbe/Container.h"
#include "cbe/QueryChain.h"
#include "cbe/QueryChainSync.h"
#include "cbe/Types.h"

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>

#include "MyGenericFunctions.cpp"

namespace {

  using Items = std::vector<cbe::Item>;
  void printItems(Items&& items, const std::string& headline);

  struct Entry {
    const char* const code;
    const char* const country;
    const char* const location;
  };
  static const Entry entries[] = {
                                  {"AMS", "Netherlands"  , "Amsterdam" },
                                  {"ARN", "Sweden"       , "Stockholm" },
                                  {"ATH", "Greece"       , "Athens"    },
                                  {"BER", "Germany"      , "Berlin"    },
                                  {"BRU", "Belgium"      , "Brussels"  },
                                  {"BTS", "Slovakia"     , "Bratislava"},
                                  {"CDG", "France"       , "Paris"     },
                                  {"CPH", "Denmark"      , "Copenhagen"},
                                  {"DUB", "Ireland"      , "Dublin"    },
                                  {"FCO", "Italy"        , "Rome"      },
                                  {"FRA", "Germany"      , "Frankfurt" },
                                  {"GOT", "Sweden"       , "Gothenburg"},
                                  {"GVA", "Switzerland"  , "Geneva"    },
                                  {"HAD", "Sweden"       , "Halmstad"  },
                                  {"HEL", "Finland"      , "Helsinki"  },
                                  {"KEF", "Iceland"      , "Reykjavik" },
                                  {"KRN", "Sweden"       , "Kiruna"    },
                                  {"LHR", "UK"           , "London"    },
                                  {"LUX", "Luxembourg"   , "Luxembourg"},
                                  {"MAD", "Spain"        , "Madrid"    },
                                  {"OSL", "Norway"       , "Oslo"      },
                                  {"PRG", "Czechia"      , "Prague"    },
                                  {"RIX", "Latvia"       , "Riga"      },
                                  {"TLL", "Estonia"      , "Tallinn"   },
                                  {"VBY", "Sweden"       , "Visby"     },
                                  {"VIE", "Austria"      , "Vienna"    },
                                  {"VNO", "Lithuania"    , "Vilnius"   },
                                  {"WAW", "Poland"       , "Warsaw"    },
                                };

  constexpr auto entriesSize = sizeof(entries) / sizeof(entries[0]);

} // Anonymous namespace


int main() {
  try {
  std::cout << "Airplanes main program start." << std::endl;
  Airplanes airplanes{};
  
  const auto account = airplanes.account;
  if (account.userId() == 0) {
    std::cout << "Login failed." << std::endl;
    std::cout << "Exiting." << std::endl;
    airplanes.cloudBackend.terminate();
    return 1;
  } else {
    std::cout << "Login as: "
              << account.username()  << " \t"
              << account.firstName() << " "
              << account.lastName()
              << std::endl;
  }
  cbe::Container rootContainer = account.rootContainer();
  cbe::Container demo{cbe::DefaultCtor{}};
  cbe::Container aero{cbe::DefaultCtor{}};
  cbe::Container airports{cbe::DefaultCtor{}};
  cbe::Container planes{cbe::DefaultCtor{}};
  cbe::QueryResult myQueryResult{cbe::DefaultCtor{}};
  cbe::QueryResult::ItemsSnapshot myItemsSnapshot;
  cbe::Filter containerFilter;
  containerFilter.setAscending(true);
  containerFilter.setDataType(cbe::ItemType::Container);
  std::string demoContainerName = "Demo";
  std::string aeroContainerName = "Aero";
  std::string airportsName = "Airports";
  std::string planesName = "Planes";
  Random rand;

    myQueryResult = airplanes.query(containerFilter, rootContainer.id());
    for (const auto& item : myQueryResult.getItemsSnapshot()) {
      if (item.name() == demoContainerName) {
        demo=cbe::CloudBackend::castContainer(item);
        break;
      }
    }

    if (demo) {
      std::cout << "Using:  \t/"
                << demo.name()
                << std::endl;
    } else {
      demo = airplanes.createContainer(rootContainer, demoContainerName);
      std::cout << "Created: \t/"
                << demo.name()
                << "\t\t(" << demo.id() << ")"
                << std::endl;
    }

    myQueryResult = airplanes.query(containerFilter, demo.id());
    for (const auto& item : myQueryResult.getItemsSnapshot()) {
      if (item.name() == aeroContainerName) {
        aero=cbe::CloudBackend::castContainer(item);
        break;
      }
    }

    if (aero) {
      std::cout << "Using:  \t/"
                << demo.name()
                << "/"
                << aero.name()
                << std::endl;
    } else {
      aero = airplanes.createContainer(demo, aeroContainerName);
      std::cout << "Created: \t/"
                << demo.name()
                << "/"
                << aero.name()
                << "\t(" << aero.id() << ")"
                << std::endl;
    }

    if (aero) {
      std::cout << "Using:  \t/"
                << demo.name()
                << "/"
                << aero.name()
                << std::endl;
    } else {
      aero = airplanes.createContainer(rootContainer, aeroContainerName);
      std::cout << "Created: \t/"
                << demo.name()
                << "/"
                << aero.name()
                << "\t(" << aero.id() << ")"
                << std::endl;
    }

    myQueryResult = airplanes.query(containerFilter, aero.id());
    myItemsSnapshot = myQueryResult.getItemsSnapshot();
    std::cout << "Content of: " << aero.name() << std::endl;
    for (const auto& item : myItemsSnapshot) {
      if (item.name() == airportsName) {
        std::cout << "Found: "
                  << item.name()
                  << "  ("
                  << item.id()
                  << ")"
                  << std::endl;
        airports=cbe::CloudBackend::castContainer(item);
      } else {
        // std::cout << "Item : " << item.name() << std::endl;
      }
    }

    if (airports) {
      std::cout << "Using:  \t/"
                << demo.name()
                << "/"
                << aero.name()
                << "/"
                << airports.name()
                << std::endl;
    } else {
      // Create some data points for airports
      std::cout << "\nLoad " << airportsName << std::endl;
      airports = airplanes.createContainer(aero, airportsName);
      std::cout << "Created: /" 
                << demo.name()
                << "/"
                << aero.name()
                << "/"
                << airports.name()
                << std::endl;

      for (const auto& entry : entries) {
        cbe::KeyValues keyValues{{"Country", {entry.country,  true}},
                                 {"Name",    {entry.location, true}}};
        airplanes.createObject(airports, 
                              entry.code, 
                              std::move(keyValues));
      }
    }  // Load airports

    // Planes
    int i = 0;
    int selection = 0;
    std::string pName;
    myQueryResult = airplanes.query(containerFilter, aero.id());
    myItemsSnapshot = myQueryResult.getItemsSnapshot();
    std::cout << "Content of: /" << demo.name() << "/" << aero.name() << std::endl;
    std::cout << "0 : - none - create a new set" << std::endl;
    for (const auto& item : myItemsSnapshot) {
      pName = item.name();
      if (pName.at(0) == 'P') {
        std::cout << i << " : " << item.name() << std::endl;
      }
      i++;
    }
    int maxVal = myItemsSnapshot.size();
    // std::cout << "max: " << maxVal << std::endl;
    do {
      selection = inquireInt("Which Planes do you want to reuse", 0);
    } while (selection >= maxVal);
    if (selection > 0) {
      planes = cbe::CloudBackend::castContainer(myItemsSnapshot[selection]);
      std::cout << "Using " << planes.name()
                << "  ("    << planes.id() << ")"
                << std::endl;
    } else {
      std::string numbers;
      while (numbers.length() < 12 ) {
        numbers = std::to_string(rand.nextInt());
      }
      std::string myPlanesName = planesName + "_" + numbers.substr(1,10);
      std::cout << "\nLoad planes" << '\n';
      planes = airplanes.createContainer(aero, myPlanesName);
      std::cout << "Created: /" 
                << aero.name()
                << "/"
                << planes.name()
                << std::endl;

      static const char* aircraftModels[] = { "Boeing_737", "Airbus_A220",
                                              "Boeing_747", "Airbus_A320",
                                              "Boeing_777", "Airbus_A340",
                                              "Boeing_787", "Airbus_A350" };
      constexpr auto aircraftModelsSize = sizeof(aircraftModels) /
                                          sizeof(aircraftModels[0]);
      static const char* airLines[] = { "AF", "AY", "AZ", "BA", "DY", "IB",
                                        "KL", "LH", "LX", "SK", "TP", "UX" };
      constexpr auto airLinesSize = sizeof(airLines) / sizeof(airLines[0]);

      for (auto index=100U; index<160U; index++) {
        const auto location = entries[rand.nextInt(entriesSize)].location;
        cbe::KeyValues keyValues{
          {"ACID",     {std::to_string(rand.nextInt(9000)+1000), true}},
          {"Location", {location, true}},
          {"Model",    {aircraftModels[rand.nextInt(aircraftModelsSize)], true}}};
        const auto flight = airLines[rand.nextInt(airLinesSize)] +
                            std::to_string(index);
        airplanes.createObject(planes, flight, std::move(keyValues));
      } // for ( ...
    }  // if

    // query.join for Airports with Planes.
    std::string queryFor;
    std::string queryPrefix = "Country:";
    cbe::Filter countryFilter{};
    cbe::Filter modelFilter{};

    do {
      queryFor = inquireString(queryPrefix,"Sweden");
      countryFilter.setDataType(cbe::ItemType::Object);
      countryFilter.setQuery(queryPrefix+queryFor);
      modelFilter.setDataType(cbe::ItemType::Object);
      modelFilter.setQuery("Model:Boeing*");
      std::cout << "\n  Filter " << countryFilter.getQuery() << " and "
                << modelFilter.getQuery()
                << "\n  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                << std::endl;

      cbe::Filter containerFilter;
      containerFilter.setDataType(cbe::ItemType::Object);

      // queries joining tables airports and planes
      auto result1 = airports.query(countryFilter).join(planes, 
                                                          "Name", 
                                                          "Location", 
                                                          modelFilter);
      printItems(result1.getItemsSnapshot(), 
                  "Boeing in "+queryFor);

      modelFilter.setQuery("Model:Airbus*");
      std::cout << "\n  Filter " << countryFilter.getQuery() << " and "
                << modelFilter.getQuery()
                << "\n  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                << std::endl;
      auto result2 = airports.query(countryFilter).join(planes, 
                                                        "Name",
                                                        "Location", 
                                                        modelFilter);
      printItems(result2.getItemsSnapshot(), 
                  "Airbus in "+queryFor);
    } while (inquireBool("\nAnother country search", "y"));

    std::cout << std::endl;

    if (inquireBool("Delete "+planes.name(),false)) {
      airplanes.removeContainer(planes);
      std::cout << "\nRemoved " + planes.name() << std::endl;
    }
    airplanes.cloudBackend.terminate();
    std::cout << "Airplanes program end." << std::endl;
  }
  catch (const cbe::util::Exception& e)
  {
    std::cout << "\nCBE specific exception! \nerror info=\"" 
              << e.what() << "\"\nType: '" << typeid(e).name() << '\'' << std::endl;
    return 1;
  }
  catch (std::invalid_argument& e) {
    std::cout << "\nException I caught after " << e.what() << " failed." << std::endl;
    return 2;
  }
  catch (...) {
    return 0;
    std::cout << "\nException O caught." << std::endl;
  }

}  //  main


namespace {
  void printItems(Items&& items, const std::string& headline) {
    std::cout << "  " << headline << ", count: " << items.size() << '\n';
    std::cout << "+--------+------------+------------+-------------+"
              << std::endl;
    for (const auto& item : items) {
      std::cout << "| " << item.name() << "  ";
      if (item.type() == cbe::ItemType::Object) {
        /* const */ auto object = cbe::CloudBackend::castObject(item);
        const auto keyValues = object.keyValues();
        if (!keyValues.empty()) {
          const auto flags = std::cout.flags(); // Save formatting state of cout
          std::for_each(std::begin(keyValues), std::end(keyValues),
                        [](const decltype(keyValues)::value_type& keyValue) {
                          std::cout << "| "
                                    << std::left << std::setw(11)
                                    << keyValue.second.first;
                        } /* f */);
          std::cout.flags(flags); // // Restore formatting state of cout
          std::cout << " |" << std::endl;
        }
      }
    }
    std::cout << "+--------+------------+------------+-------------+"
              << std::endl;
  } // printItems

} // Anonymous namespace