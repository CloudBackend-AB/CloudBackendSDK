import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;
import java.util.Random;

import javax.security.auth.login.AccountException;

import com.cbe.Account;
import com.cbe.CloudBackend;
import com.cbe.Container;
import com.cbe.Filter;
import com.cbe.Item;
import com.cbe.QueryResult;
import com.std.Obj_KV_Map;
import com.std.Obj_VI_Pair;

public class Airplanes {
  static String programName = "Airplanes";
  private CloudBackend   cloudBackend = null;
  static  final Inquiry  inquiry = new Inquiry();

  public com.cbe.CloudBackend myLogin(String credentials) {
    String username = "";
    String password = "";
    String tenant   = "";
    try (InputStream input = new FileInputStream("../../resources/config.properties")) {
      Properties myProperties = new Properties();
      // load a properties file
      myProperties.load(input);
      // get the property value and set the login credentials
      username = myProperties.getProperty(credentials + ".username");
      password = myProperties.getProperty(credentials + ".password");
      tenant   = myProperties.getProperty(credentials + ".tenant");
    } catch (IOException ex) {
      throw new RuntimeException(ex);
    }
    if (username == null) {
      System.out.println("Credentials not found: " + credentials);
      username = "?";
      password = "?";
      tenant   = "?";
    }
    com.cbe.CloudBackend cloudBackend = null;
    MyLogInDelegate delegate = new MyLogInDelegate();
    try
    {
      System.out.println("Connecting as " + username);
      cloudBackend = com.cbe.CloudBackend.logIn(username, password, tenant, delegate);
      System.out.println("Waiting.");
      cloudBackend = delegate.waitForRsp();
    }
    catch (RuntimeException e)
    {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return cloudBackend;
  }
  
  public QueryResult query(Filter filter, long containerId) {
    final var delegate = new MyQueryDelegate();
    cloudBackend.query(containerId, filter, delegate);
    return delegate.waitForRsp();
  }


  
  public Container createContainer(Container parentContainer, String name) {
    // Delegate using anonymous class
    final var delegate = new MyCreateContainerDelegate();
    parentContainer.createContainer(name, delegate);
    return delegate.waitForRsp();
  } // createContainer()

  public com.cbe.Object createObject(Container parentContainer, String name,
                                     Obj_KV_Map metadata) {
    final var delegate =  new MyCreateObjectDelegate();   
    metadata.forEach((K,V)->{
      System.out.printf("%-14s ", V.getFirst());
    });
    System.out.print("\t");
    parentContainer.createObject(name, metadata, delegate);
    final var object = delegate.waitForRsp();
    System.out.println("Created object \"" + object.name() + "\" with id " +
                       object.id());
    return object;
  }

  public void removeContainer(Container container) {
    final var delegate = new MyRemoveContainerDelegate();
    container.remove(delegate);
    delegate.waitForRsp();
  }

  private static void printItems(com.std.Items_Vec items, String headline) {
    System.out.printf("%s, count: %d\n", headline, items.size());
    for(final var item : items) {
      System.out.print("Name: " + item.name());
      System.out.print(" pid(" + item.parentId());
      System.out.println(") ts: " + item.updated());
      if(item.type() == com.cbe.ItemType.Object) {
        final var object = CloudBackend.castObject(item);
        final var keyValues = object.keyValues();
        if(!keyValues.isEmpty()) {
          System.out.println("-----------------------------------------------");
          keyValues.forEach((key, value)->{
            System.out.printf("%10s = %-16s%s\n",
                              key, value.getFirst(),
                              (value.getSecond()? "\t(indexed)" : ""));
          } /* action */);
        }
        System.out.println("===============================================");
      }
    }
  }  //  printItems


  public static void main(String[] argv) {
    System.out.println(programName + " program start.");
    Airplanes inst = new Airplanes();
    com.cbe.CloudBackend cbobj = inst.myLogin("gh2");
    if (cbobj.account().userId()>0) {
      System.out.println("Authenticated as: " + cbobj.account().username() + "\t" + cbobj.account().firstName() + " " + cbobj.account().lastName());
      System.out.println("Version: " + cbobj.version());
    } else {
      System.out.println("Login failed.");
      if (cbobj != null) {
        cbobj.terminate();
      }
      System.out.println(programName + " program stop.");
      return;
    }
    boolean foundPlanes = false;
    Container airports = null;
    Container planes = null;
    Container simulation = null;
    String    planesName = "";
    inst.cloudBackend = cbobj;
    final var account = cbobj.account();
    final var containerFilter = new Filter();
    final var rand = new Random();
    final var rootContainer = account.rootContainer();
    containerFilter.setDataType(com.cbe.ItemType.Container);
    

    record Entry(String country, String airportLocation) {}
    // record requires OpenJDK >= 16 (i.e., jdk >= 14)
    final Entry entries[] = { new Entry("France"       , "Paris"     ),
                              new Entry("Spain"        , "Madrid"    ),
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
    final String swedishAirports[] = { "Stockholm", "Gothenburg", "Sundsvall",
                                       "Malmö", "Linköping", "Norrköping",
                                       "Örebro", "Luleå", "Kiruna" };
    try {
      System.out.println("\nContent of: " + rootContainer.name());
      var queryResult = inst.query(containerFilter,
                                   rootContainer.id());
      for(final var item : queryResult.getItemsSnapshot()) {
        if (item.name().equals("Simulation")) {
          System.out.println("Found: " + item.name());
          simulation=com.cbe.CloudBackend.castContainer(item);
        } else {
          System.out.println("Item : " + item.name());
        }
      }
      if (simulation == null) {
        simulation = inst.createContainer(rootContainer, "Simulation");
        System.out.println("Created container " + simulation.name() +
                           " with id: " + simulation.id());
      }
      System.out.println("\nContent of: " + simulation.name());
      queryResult = inst.query(containerFilter, simulation.id());
      for(final var item : queryResult.getItemsSnapshot()) {
        if (item.name().equals("Airports")) {
          System.out.println("Found: " + item.name());
          airports=com.cbe.CloudBackend.castContainer(item);
        } else {
          foundPlanes = true;
          System.out.println("Item : " + item.name());
        }
      }
  
      // Testing to create some data points for 2 different containers 
      // Airports and Planes.
      System.out.println("\nLoad airports");
      if (airports == null) {
        airports = inst.createContainer(simulation, "Airports");
        System.out.println("Created container " + airports.name() +
                           " with id: " + airports.id());

        for (final var entry : entries) {
          final var metadata = new Obj_KV_Map() {
            // Anomyous default ctor
            {
              put("Country", new Obj_VI_Pair(entry.country, true /* indexed */));
              put("Name",    new Obj_VI_Pair(entry.airportLocation, true));
            }
          };
          inst.createObject(airports, entry.airportLocation, metadata);
        }
    
        {
          final var metadata = new Obj_KV_Map() {
            {
              put("Country", new Obj_VI_Pair("Sweden", true));
            }
          };
          for(final var swedishAirport : swedishAirports) {
            metadata.put("Name", new Obj_VI_Pair(swedishAirport, true));
            inst.createObject(airports, swedishAirport, metadata);
          }
        }
      } else {
        System.out.println("Using existing " + airports.name() + 
                           " (" + airports.id() + ")");
      }
      if (foundPlanes) {
        if (inquiry.inquireBool("Use an existing Planes", true)) {
          planesName = inquiry.inquireString("Select which Planes");
        }
        for(final var item : queryResult.getItemsSnapshot()) {
          if (item.name().equals(planesName)) {
            System.out.println("Using " + item.name() + " ("+item.id()+")");
            planes=com.cbe.CloudBackend.castContainer(item);
          }
        }
      }

      if (planes == null) {
        planesName = "Planes" + 
                     Long.toString(java.lang.Math.abs(rand.nextLong()));
        planes =  inst.createContainer(simulation, planesName);
        System.out.println("\nCreated container " + planes.name() +
                          " with id: " + planes.id());
        System.out.println("Load planes");
    
        final String aircraftModels[] = { "Boeing_737", "Airbus_A220", 
                                          "Boeing_747", "Airbus_A320", 
                                          "Boeing_777", "Airbus_A340",
                                          "Boeing_787", "Airbus_A350" };
        final String airLines[] = { "AF", "AY", "AZ", "BA", "DY", "IB", 
                                    "KL", "LH", "LX", "SK", "TP", "UX" };
        for (int index=100; index<=160; index++) {
          final var swedish = (index % 20) == 0;
          final var airportLocation =
              swedish? swedishAirports[rand.nextInt(swedishAirports.length)]
                    : entries[rand.nextInt(entries.length)].airportLocation;
          
          final var metadata = new Obj_KV_Map() {
            {
              put("Location", new Obj_VI_Pair( airportLocation, true) );
              put("Model",
                  new Obj_VI_Pair(aircraftModels[
                                    rand.nextInt(aircraftModels.length)],
                                  true ));
              final var aircraftId = Integer.toString(rand.nextInt(9000)+1000);
              put("ACID", new Obj_VI_Pair(aircraftId, true));
            }
          };
          final var flight = airLines[rand.nextInt(airLines.length)] +
                                      Integer.toString(index);
          inst.createObject(planes, flight, metadata);
        } // for (int index= ...
      }

      // testing query.join for Airports with Planes.
      final var countryFilter = new Filter();
      final var countryText = inquiry.inquireString("Country", "Sweden");
      countryFilter.setQuery("Country:" + countryText);
      // countryFilter.setQuery("Country:Spain");
      countryFilter.setDataType(com.cbe.ItemType.Object);
      final var modelFilter = new Filter();
      modelFilter.setQuery("Model:Boeing*");
      modelFilter.setDataType(com.cbe.ItemType.Object);
    
      System.out.println("\n\nFind planes on airports of a specific country.");
      System.out.println("\nFilter " + countryFilter.getQuery() + " and " +
                          modelFilter.getQuery());
      System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
      final var flightsInCountryDelegate1 = new MyQueryDelegate();
      final var queryJoinDelegate = new com.cbe.delegate.JoinDelegate();
      airports.query(countryFilter,
                     flightsInCountryDelegate1)
              .join(planes,"Name" /* key1 */, "Location" /* key2 */,
                    modelFilter, queryJoinDelegate);  
  
      printItems(flightsInCountryDelegate1.waitForRsp().getItemsSnapshot(),
                 "Boeing in the country");
  
      modelFilter.setQuery("Model:Airbus*");
      System.out.println("\n\nFilter " + countryFilter.getQuery() + " and "
                         + modelFilter.getQuery());
      System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
      final var flightsInCountryDelegate2 = new MyQueryDelegate();
      airports.query(countryFilter, flightsInCountryDelegate2)
              .join(planes, "Name", "Location", modelFilter, queryJoinDelegate);  
      printItems(flightsInCountryDelegate2.waitForRsp().getItemsSnapshot(),
                 "Airbus in the country");
    }
    finally {
      System.out.println();
      if (inquiry.inquireBool("Do you want to remove the planes", false)) {
        inst.removeContainer(planes);
        System.out.println("Removed " + planes.name());
      }
    }
    System.out.println();
    cbobj.terminate();
    System.out.println(programName + " program end.");
  }  //  main


  // -----------------------  Generic functions  -------------------------------

  static class Inquiry {
    private final java.util.Scanner scanner = new java.util.Scanner(System.in);

    int inquireInt(String  prompt, Integer defaultVal) {
      final boolean hasDefaultVal = defaultVal != null;
      while (true) {
        System.out.print(prompt + (hasDefaultVal? (" (Default value is '" +
                         defaultVal + "')") : "") +  "? ");
        final String answer = scanner.nextLine();
        if (answer.isEmpty()) {
          if (hasDefaultVal) {
            return defaultVal;
          }
          continue;
        }
        try {
          final int intVal = Integer.parseInt(answer);
          return intVal;
        }
        catch (NumberFormatException e) {
          System.out.println("Failed to convert answer \"" + answer +
                             "\" to an integer. " + "Got exception \"" + e +
                             "\"");
        }
      }
    }
    int inquireInt(String  prompt) {
      return inquireInt(prompt, null /* defaultVal */);
    }
    
    String inquireString(String  prompt, String  defaultVal) {
      final boolean hasDefaultVal = defaultVal != null;
      while (true) {
        System.out.print(
          prompt +
          (hasDefaultVal? (" (Default value is \"" + defaultVal + "\")") : "") +
          "? ");
        String answer = scanner.nextLine();
        if (answer.isEmpty() && hasDefaultVal) {
          answer = defaultVal;
        }
       final String trimmedAnswer = answer.trim();
        if (trimmedAnswer.isEmpty() && !(hasDefaultVal && defaultVal.isEmpty()))
        {
          System.out.println("Your input \"" + answer +
                             "\" is not valid; please, input a response.");
          continue;
        }
        return trimmedAnswer;
      }
    }
    String inquireString(String  prompt) {
      return inquireString(prompt, null /* defaultVal */);
    }
    
    boolean inquireBool(String  prompt, Boolean defaultVal) {
      final boolean hasDefaultVal = defaultVal != null;
      final char  falseChar = 'n';
      final char  actualFalseStr =
          (hasDefaultVal && !defaultVal)? Character.toUpperCase(falseChar)
                                        : falseChar;
      final char  trueChar = 'y';
      final char  actualTrueChar =
          (hasDefaultVal && defaultVal)? Character.toUpperCase(trueChar)
                                       : trueChar;
      while (true) {
        System.out.print(prompt + " (" + actualTrueChar + '/' + actualFalseStr
                         + ")? ");
        final String answer = scanner.nextLine();
        if (answer.isEmpty()) {
          if (hasDefaultVal) {
            return defaultVal;
          }
          continue;
        }
        switch (Character.toLowerCase(answer.charAt(0))) {
        case trueChar:
          return true;
        case falseChar:
          return false;
        default:
          continue;
        }
      }
    }
    boolean inquireBool(String  prompt) {
      return inquireBool(prompt, null /* defaultVal */);
    }
  }  //  Inquiry  - Generic functions -

}  //  class Airplanes
