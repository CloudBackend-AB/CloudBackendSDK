import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.AbstractMap;
import java.util.Properties;
import java.util.Random;

import com.cbe.ACL_Map;
import com.cbe.Account;
import com.cbe.CloudBackend;
import com.cbe.Container;
import com.cbe.Filter;
import com.cbe.Item;
import com.cbe.Obj_KV_Map;
import com.cbe.Obj_VI_Pair;
import com.cbe.QueryResult;

public class Airplanes {

  private final CloudBackend  cloudBackend;
  public Account              account;

  public Airplanes(String credentials) {
    try (final InputStream inputStream =
                     new FileInputStream("../../resources/config.properties")) {
      final var prop = new Properties();

      // load a properties file
      prop.load(inputStream);

      // get the property value and set the login credentials
      final var username = prop.getProperty(credentials + ".username");
      final var password = prop.getProperty(credentials + ".password");
      final var tenant = prop.getProperty(credentials + ".tenant");

      final var accountDelegate = new AccountDelegate();
      CloudBackend.logIn(username, password, tenant, accountDelegate);
      cloudBackend = accountDelegate.waitForRsp();
      if (cloudBackend == null) {
        throw new RuntimeException("Login failed. Exit program.");
      }
      account = cloudBackend.account();
    } catch (IOException ex) {
      // Soften the IOException
      throw new RuntimeException("Failed to open resource file", ex);
    }
    
  }
  
  public QueryResult query(Filter filter, long containerId) {
    QueryDelegate delegate = new QueryDelegate();
    cloudBackend.query(containerId, filter, delegate);
    return delegate.waitForRsp();
  }

  private static class QueryDelegate extends com.cbe.ItemEventProtocol {
    private QueryResult queryResult; /* null */
    private String      errorInfo;
    private boolean     finished;  /* false */
    
    @Override
    synchronized public void onQueryLoaded(QueryResult dir) {
      queryResult = dir;
      finished = true;
      notify();
    }
    @Override
    synchronized public void onLoadError(Filter filter, long operation,
                                         long code,
                                         String reason, String message) {
      errorInfo = "LoadError: code=" + code + ", reson=\"" + reason +
                  "\", message=\"" + message + "\"";
      finished = true;
      notify();
    }
    synchronized public QueryResult waitForRsp() {
      while (!finished) {
        try {
          wait();
        } catch (InterruptedException e) {
          e.printStackTrace();
        }
      }
      if (queryResult == null) {
        throw new RuntimeException(errorInfo);
      }
      return queryResult; 
    }
  } // class QueryDelegate
  
  public Container createContainer(Container parentContainer, String name) {
    class CreateContainerDelegate extends com.cbe.ItemEventProtocol {
      Container       container;
      private String  errorInfo;
      private boolean finished = false;
      
      @Override
      synchronized public void onContainerAdded(Container container) {
        this.container = container;
        finished = true;
        notify();
      }
      @Override
    synchronized public void onItemError(Item parentContainer, int type,
                                         long operation, long failedAtState,
                                         long code, String reason,
                                         String message) {
        errorInfo = "Error: Failed to create sub container in \"" +
                    parentContainer.name() + "\": " +
                    "type="           + type +
                    ",operation="     + operation +
                    ",failedAtState=" + failedAtState +
                    "\",code="        + code +
                    ",reason=\""      + reason +
                    "\",message=\""   + message + "\"";
        finished = true;
        notify();
      }
      synchronized public Container waitForRsp() {
        while (!finished) {
          try {
            wait();
          } catch (InterruptedException e) {
            e.printStackTrace();
          }
        }
        if (container == null) {
          throw new RuntimeException(errorInfo);
        }
        return container; 
      }
    } // class CreateContainerDelegate

    final var delegate = new CreateContainerDelegate();
    parentContainer.create(name, delegate);
    final var container = delegate.waitForRsp();
    // System.out.println("Created with id " + container.id());
    return container;
  } // createContainer()

  public com.cbe.Object createObject2(Container parentContainer, Obj_KV_Map metadata, String name) {
    class CreateObjectDelegate extends com.cbe.ItemEventProtocol {
      com.cbe.Object  object;
      private String  errorInfo;
      private boolean finished = false;
      
      @Override
      synchronized public void onObjectAdded(com.cbe.Object object) {
        this.object = object;
        finished = true;
        notify();
      }
      @Override
      synchronized public void onItemError(Item parentContainer, int type,
                                           long operation, long failedAtState,
                                           long code, String reason,
                                           String message) {
        errorInfo = "Error: Failed to create sub container in \"" +
                    parentContainer.name() + "\": " +
                    "type="           + type +
                    ",operation="     + operation +
                    ",failedAtState=" + failedAtState +
                    "\",code="        + code +
                    ",reason=\""      + reason +
                    "\",message=\""   + message + "\"";
        finished = true;
        notify();
      }
      synchronized public com.cbe.Object waitForRsp() {
        while (!finished) {
          try {
            wait();
          } catch (InterruptedException e) {
            e.printStackTrace();
          }
        }
        if (object == null) {
          throw new RuntimeException(errorInfo);
        }
        return object; 
      }
    } // class CreateObjectDelegate

    metadata.forEach((K,V)->{
      System.out.printf("%14s, ", V.getFirst());
    });
    System.out.print("\t");
    
    final var delegate = new CreateObjectDelegate();
    parentContainer.createObject(name, delegate, metadata);
    final var object = delegate.waitForRsp();
    System.out.println("Created object \"" + object.name() + "\" with id " +
                       object.id());
    return object;
  }

  public void removeContainer(Container container) {
    class RemoveContainerDelegate extends com.cbe.ItemEventProtocol {
      private String  errorInfo;
      private boolean finished = false;
      private String name;
      
      @Override
      synchronized public void onContainerRemoved(long    containerId,
                                                  String  name) {
        this.name = name;
        finished = true;
        notify();
      }
      @Override
      synchronized public void onItemError(Item parentContainer, int type,
                                           long operation, long failedAtState,
                                           long code, String reason,
                                           String message) {
        errorInfo = "Error: Failed to remove container \"" +
                    container.name() + "\": " +
                    "type="           + type +
                    ",operation="     + operation +
                    ",failedAtState=" + failedAtState +
                    "\",code="        + code +
                    ",reason=\""      + reason +
                    "\",message=\""   + message + "\"";
        finished = true;
        notify();
      }
      synchronized public String waitForRsp() {
        while (!finished) {
          try {
            wait();
          } catch (InterruptedException e) {
            e.printStackTrace();
          }
        }
        if (name == null) {
          throw new RuntimeException(errorInfo);
        }
        return name; 
      }
    } // class RemoveContainerDelegate

    final var delegate = new RemoveContainerDelegate();
    container.remove(delegate);
    delegate.waitForRsp();
  }

  public static void main(String[] argv) {
    System.out.println("Airplanes program start");
    final var airplaines = new Airplanes("cr1" /* credentials */);
    final var account = airplaines.account;
    System.out.println("Login: " + account.username() + " " +
                       account.firstName() + " " +
                       account.lastName());

    final var rootContainer = account.rootContainer();
    final var ascendingFilter = new Filter();
    ascendingFilter.setAscending(true);
    
    final var queryResult = airplaines.query(ascendingFilter,
                                             rootContainer.id());
    for(Item item : queryResult.getItemsSnapshot()) {
      System.out.println("Item: " + item.name());
    }
    final var rand = new Random();
    final var startContainer = "AirSimulation" + Integer.toString(rand.nextInt((1000000)));
    final var subContainer = airplaines.createContainer(rootContainer, startContainer);
    System.out.println("Created container " + subContainer.name() + " with id: " + subContainer.id());

    // Testing to create some data points for 2 different containers Airports and Planes.

    System.out.println("\nLoad airports");
    Container airports = airplaines.createContainer(subContainer, "Airports");

    class Entry {
      final String country;
      final String airportLocation;
      
      Entry(String country, String airportLocation) {
        this.country = country;
        this.airportLocation = airportLocation;
      }
      
      
    }
    final Entry entries[] = { new Entry("France"       , "Paris"),
                              new Entry("Spain"        , "Madrid"),
                              new Entry("UK"           , "London"    ),
                              new Entry("Netherlands"  , "Amsterdam" ),
                              new Entry("Denmark"      , "Copenhagen"),
                              new Entry("Norway"       , "Oslo"      ),
                              new Entry("Finland"      , "Helsinki"  ),
                              new Entry("Estonia"      , "Tallin"    ),
                              new Entry("Lithuania"    , "Vilnius"   ),
                              new Entry("Latvia"       , "Riga"      ),
                              new Entry("Iceland"      , "Reijkavik" ),
                              new Entry("Luxemburg"    , "Luxemburg" ),
                              new Entry("Belgium"      , "Antwerpen" ),
                              new Entry("Germany"      , "Berlin"    ),
                              new Entry("Poland"       , "Warsaw"    ),
                              new Entry("Italy"        , "Rome"      ),
                              new Entry("Czech Repulic", "Praha"     ),
                              new Entry("Slovakia"     , "Bratislava"),
                              new Entry("Austria"      , "Wien"      ),
                              new Entry("Switzerland"  , "Bern"      ),
                              new Entry("Slovenia"     , "Ljubljana" ),
                              new Entry("Greece"       , "Athens"    ),
                              new Entry("Irland"       , "Dublin"    ),
                      };

    final var metadata = new Obj_KV_Map();
    for (final var entry : entries) {
      metadata.put("Country", new Obj_VI_Pair(entry.country,
                                              true /* indexed */));
      metadata.put("Name", new Obj_VI_Pair(entry.airportLocation, true));
      airplaines.createObject2(airports, metadata, entry.airportLocation);
      metadata.clear();
    }

    final String swedishAirports[] = { "Gothenburg", "Sundsvall", "Linköping",
                                       "Örebro", "Norrköping", "Stockholm",
                                       "Malmö", "Luleå", "Kiruna" };
    
    metadata.put("Country", new Obj_VI_Pair("Sweden", true));
    for(final var swedishAirport : swedishAirports) {
      metadata.put("Name", new Obj_VI_Pair(swedishAirport, true));
      airplaines.createObject2(airports, metadata, swedishAirport);
    }
    metadata.clear();


    System.out.println("\nLoad planes");
    Container planes = airplaines.createContainer(subContainer, "Planes");

    final String aircraftModels[] = { "Boeing_737", "Airbus_A220", 
                                      "Boeing_747", "Airbus_A320", 
                                      "Boeing_777", "Airbus_A340",
                                      "Boeing_787", "Airbus_A350" };
    final String airLines[] = { "AF", "AY", "AZ", "BA", "DY", "IB", 
                                "KL", "LH", "LX", "SK", "TP", "UX" };
    for(int index=100; index<=160 /* !!! */; index++) {
      if((index%20) == 0) {
        metadata.put("Location",
                     new Obj_VI_Pair(
                       swedishAirports[rand.nextInt(swedishAirports.length)],
                       true));
      } else {
        metadata.put("Location",
                      new Obj_VI_Pair(
                        entries[rand.nextInt(entries.length)].airportLocation,
                        true));
      }
      metadata.put("Model",
                   new Obj_VI_Pair(aircraftModels[
                                     rand.nextInt(aircraftModels.length)],
                                   true));
      final var aircraftId = Integer.toString(rand.nextInt(9000)+1000);
      metadata.put("ACID", new Obj_VI_Pair(aircraftId, true));
      final var flight = airLines[rand.nextInt(airLines.length)] +
                                  Integer.toString(index);
      airplaines.createObject2(planes, metadata, flight);
    }
    
    // testing query.join for Airports with Planes.

    final var countryFilter = new Filter();
    countryFilter.setQuery("Country:Sweden");
    countryFilter.setDataType(4);    // ItemType::Object
    final var modelFilter = new Filter();
    modelFilter.setQuery("Model:Boeing*");
    modelFilter.setDataType(4);    // ItemType::Object
  
    System.out.println("\n\nFilter " + countryFilter.getQuery() + " and " +
                       modelFilter.getQuery());
    System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    final var flightsInCountryDelegate1 = new QueryDelegate();
    airports.query(countryFilter,
                   flightsInCountryDelegate1)
            .join(planes,"Name" /* key1 */, "Location" /* key2 */, modelFilter);  

    printItems(flightsInCountryDelegate1.waitForRsp().getItemsSnapshot(),
               "Boeing in Sweden");

    modelFilter.setQuery("Model:Airbus*");
    System.out.println("\n\nFilter " + countryFilter.getQuery() + " and "
                       + modelFilter.getQuery());
    System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    final var flightsInCountryDelegate2 = new QueryDelegate();
    airports.query(countryFilter, flightsInCountryDelegate2)
            .join(planes, "Name", "Location", modelFilter);  
    printItems(flightsInCountryDelegate2.waitForRsp().getItemsSnapshot(),
               "Airbus in Sweden");

    // cleanup
    airplaines.removeContainer(subContainer);
    System.out.println("\nRemoved " + subContainer.name());
    Runtime.getRuntime().runFinalization();
    System.out.println("Airplanes program end.");
    try {
      Thread.sleep(100);
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
  }

  private static void printItems(com.cbe.Items_Vec items, String headline) {
    System.out.printf("\n%s, count: %d\n", headline, items.size());
    for(final var item : items) {
      System.out.print("Name: " + item.name());
      System.out.print(" pid(" + item.parentId());
      System.out.println(") ts: " + item.updated());
      if(item.type() == 4 /* ItemType::Object */) {
        final var object = CloudBackend.castObject(item);
        if(object.keyValues().size() != 0) {
          System.out.println("-----------------------------------------------");
          final var keyValues = object.keyValues();
          keyValues.forEach((key, value)->{
            System.out.printf("%10s = %-15s ", key, value.getFirst());
            if (value.getSecond() /* indexed */) {
              System.out.println("(indexed)");
            } else {
              System.out.println();
            }
          } /* action */);
        }
        System.out.println("===============================================");
      }
    }
  }
}

