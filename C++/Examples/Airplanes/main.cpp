#include "Airplanes.h"

#include "cbe/Account.h"
#include "cbe/CloudBackend.h"
#include "cbe/Container.h"
#include "cbe/QueryChain.h"
#include "cbe/Types.h"

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>

namespace {

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

using Items = std::vector<cbe::Item>;
void printItems(Items&& items, const std::string& headline);

} // Anonymous namespace


int main() {
  std::cout << "Airplanes program start" << std::endl;
  Airplanes airplanes{"" /* credentials */};
  const auto account = airplanes.account;
  std::cout <<  "Login: " << account.username() << " " <<
                account.firstName() << " " << account.lastName() << '\n';
  const auto rootContainer = account.rootContainer();

  cbe::Filter ascendingFilter;
  ascendingFilter.setAscending(true);

  const auto queryResult = airplanes.query(ascendingFilter,
                                            rootContainer.id());
  for (const auto& item : queryResult.getItemsSnapshot()) {
    std::cout << "Item: " << item.name() << '\n';
  }

  Random rand;
  const auto startContainer = "AirSimulation" +
                              std::to_string(rand.nextInt());
  const auto subContainer = airplanes.createContainer(rootContainer,
                                                      startContainer);
  std::cout << "Created container " << subContainer.name()
            << " with id: " << subContainer.id() << '\n';
  try {
    // Testing to create some data points for 2 different containers Airports
    // and Planes.
    std::cout << "\nLoad airports" << '\n';
    /* const */ auto airports = airplanes.createContainer(subContainer, "Airports");

    struct Entry {
      const char* const country;
      const char* const airportLocation;
    };
    static const Entry entries[] = {{ "France"       , "Paris"     },
                                    { "Spain"        , "Madrid"    },
                                    { "UK"           , "London"    },
                                    { "Netherlands"  , "Amsterdam" },
                                    { "Denmark"      , "Copenhagen"},
                                    { "Norway"       , "Oslo"      },
                                    { "Finland"      , "Helsinki"  },
                                    { "Estonia"      , "Tallin"    },
                                    { "Lithuania"    , "Vilnius"   },
                                    { "Latvia"       , "Riga"      },
                                    { "Iceland"      , "Reijkavik" },
                                    { "Luxemburg"    , "Luxemburg" },
                                    { "Belgium"      , "Antwerpen" },
                                    { "Germany"      , "Berlin"    },
                                    { "Poland"       , "Warsaw"    },
                                    { "Italy"        , "Rome"      },
                                    { "Czech Repulic", "Praha"     },
                                    { "Slovakia"     , "Bratislava"},
                                    { "Austria"      , "Wien"      },
                                    { "Switzerland"  , "Bern"      },
                                    { "Slovenia"     , "Ljubljana" },
                                    { "Greece"       , "Athens"    },
                                    { "Irland"       , "Dublin"    },
                                  };
    constexpr auto entriesSize = sizeof(entries) / sizeof(entries[0]);
    for (const auto& entry : entries) {
      cbe::KeyValues keyValues{{"Country", {entry.country,  true}},
                              {"Name",    {entry.airportLocation, true}}};

      airplanes.createObject(airports, 
                             entry.airportLocation, 
                             std::move(keyValues));
    }

    static const char* swedishAirports[]{
                                "Gothenburg", "Sundsvall","Linköping",
                                "Örebro", "Norrköping", "Stockholm",
                                "Malmö", "Luleå", "Kiruna"
                              };
    constexpr auto swedishAirportsSize = sizeof(swedishAirports) /
                                          sizeof(swedishAirports[0]);
    {
      cbe::KeyValues keyValues{{"Country", { "Sweden",  true}}};
      for (auto swedishAirport : swedishAirports) {
        keyValues["Name"] = {swedishAirport, true};
        airplanes.createObject(airports, swedishAirport, std::move(keyValues));
      }
    }

    std::cout << "\nLoad planes" << '\n';
    const auto planes = airplanes.createContainer(subContainer, "Planes");

    static const char* aircraftModels[] = { "Boeing_737", "Airbus_A220",
                                            "Boeing_747", "Airbus_A320",
                                            "Boeing_777", "Airbus_A340",
                                            "Boeing_787", "Airbus_A350" };
    constexpr auto aircraftModelsSize = sizeof(aircraftModels) /
                                        sizeof(aircraftModels[0]);
    static const char* airLines[] = { "AF", "AY", "AZ", "BA", "DY", "IB",
                                      "KL", "LH", "LX", "SK", "TP", "UX" };
    constexpr auto airLinesSize = sizeof(airLines) / sizeof(airLines[0]);

    for (auto index=100U; index<=160U; index++) {
      const auto swedish = (index % 20) == 0U;
      const auto airportLocation =
          swedish? swedishAirports[rand.nextInt(swedishAirportsSize)]
                  : entries[rand.nextInt(entriesSize)].airportLocation;
      cbe::KeyValues keyValues{
        {"Location", {airportLocation,  true}},
        {"Model",    {aircraftModels[rand.nextInt(aircraftModelsSize)],
                      true}},
        {"ACID",     {std::to_string(rand.nextInt(9000)+1000), true}}};
      const auto flight = airLines[rand.nextInt(airLinesSize)] +
                          std::to_string(index);
      airplanes.createObject(planes, flight, std::move(keyValues));
    } // for ( ...
    // testing query.join for Airports with Planes.

    cbe::Filter countryFilter{};
    countryFilter.setQuery("Country:Sweden");
    countryFilter.setDataType(cbe::ItemType::Object);
    cbe::Filter modelFilter{};
    modelFilter.setQuery("Model:Boeing*");
    modelFilter.setDataType(cbe::ItemType::Object);

    std::cout << "\n\nFilter " << countryFilter.getQuery() << " and "
              << modelFilter.getQuery()
              << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    // const auto flightsInCountryDelegate1 = std::make_shared<QueryDelegate>();

    cbe::Filter containerFilter;
    containerFilter.setDataType(cbe::ItemType::Object);

    std::shared_ptr<QueryDelegate> queryDelegate = 
                                              std::make_shared<QueryDelegate>();
    airports.query(containerFilter, queryDelegate);
    queryDelegate->wait();

    printItems(queryDelegate->queryResult.getItemsSnapshot(), "Boeing in Sweden"); // Ska bort

    std::shared_ptr<QueryJoinDelegate> queryJoinDelegate = 
                                              std::make_shared<QueryJoinDelegate>();
    airports.query(countryFilter, queryJoinDelegate).join(planes, 
                                                          "Name", 
                                                          "Location", 
                                                          modelFilter);
    queryJoinDelegate->wait();
    printItems(queryJoinDelegate->queryResult.getItemsSnapshot(), 
               "Boeing in Sweden");

        

    modelFilter.setQuery("Model:Airbus*");
    std::cout << "\n\nFilter " << countryFilter.getQuery() << " and "
              << modelFilter.getQuery()
              << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    const auto flightsInCountryDelegate2 = std::make_shared<QueryDelegate>();

    airports.query(countryFilter, queryJoinDelegate).join(planes, 
                                                          "Name", 
                                                          "Location", 
                                                          modelFilter);
    queryJoinDelegate->wait();

    printItems(queryJoinDelegate->queryResult.getItemsSnapshot(), "Airbus in Sweden");
  }
  catch (... /* catch any exception */) {
    airplanes.removeContainer(subContainer);
    std::cout << "\nRemoved " + subContainer.name() << '\n';
    throw; // Re-throw exception
  }
  
  airplanes.removeContainer(subContainer);
  std::cout << "\nRemoved " + subContainer.name() << '\n';

  airplanes.cloudBackend.terminate();
}

namespace {

void printItems(Items&& items, const std::string& headline) {
  std::cout << headline << ", count: " << items.size() << '\n';
  for (const auto& item : items) {
    std::cout << "Name: " << item.name()
              << " pid(" << item.parentId()
              << ") ts: " << item.updated() << '\n';
    if (item.type() == cbe::ItemType::Object) {
      /* const */ auto object = cbe::CloudBackend::castObject(item);
      const auto keyValues = object.keyValues();

      if (!keyValues.empty()) {
        std::cout << "-----------------------------------------------\n";
        const auto flags = std::cout.flags(); // Save formatting state of cout
        std::for_each(std::begin(keyValues), std::end(keyValues),
                      [](const decltype(keyValues)::value_type& keyValue) {
                        std::cout << std::right << std::setw(10)
                                  << keyValue.first << " = "
                                  << std::left << std::setw(15)
                                  << keyValue.second.first
                                  << (keyValue.second.second? " (indexed)" : "")
                                  << '\n';
                      } /* f */);
        std::cout.flags(flags); // // Restore formatting state of cout
      }
      std::cout << "===============================================\n";
    }
  }
} // printItems

} // Anonymous namespace

// decltype(keyValues) == std::map<std::string, std::pair<std::string, bool>>
