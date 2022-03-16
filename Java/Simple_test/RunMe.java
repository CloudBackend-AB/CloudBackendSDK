import com.cbe.*;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.Properties;


public class RunMe {

  public CloudBackend login(String credentials) {
    CloudBackend tempObj = null;
    try (InputStream input = new FileInputStream("../resources/config.properties")) {

      Properties prop = new Properties();

      // load a properties file
      prop.load(input);

      // get the property value and set the login credentials
      String username = prop.getProperty(credentials + ".username");
      String password = prop.getProperty(credentials + ".password");
      String tenant = prop.getProperty(credentials + ".tenant");

      AccountDelegate delegate = new AccountDelegate();
      tempObj = CloudBackend.logIn(username, password, tenant, delegate);
      while (!delegate.finished) {
        try
        {
            Thread.sleep(10);
        }
        catch(Exception e)
        {
             System.out.println(e);
        }
      }   
     
    } catch (IOException ex) {
    ex.printStackTrace();
    }
    // If login happened then return the cloudbackend object.
    return tempObj;
  }
  
  public QueryResult queryR(CloudBackend obj, Filter filter, long containerId) {
    ItemDelegate delegate = new ItemDelegate();
    // obj.account().rootContainer().query(delegate);
    obj.query(containerId, filter, delegate);
    while (!delegate.finished) {
      try
      {
        Thread.sleep(10);
      }
      catch(Exception e)
      {
        System.out.println(e);
      }
    }
    return delegate.qR;
  }

  public QueryResult query(Container container) {
    ItemDelegate delegate = new ItemDelegate();
    container.query(delegate);
    while (!delegate.finished) {
      try
      {
        Thread.sleep(10);
      }
      catch(Exception e)
      {
        System.out.println(e);
      }
    }
    return delegate.qR;
  }

  public Container createContainer(Container container) {
    ItemDelegate delegate = new ItemDelegate();
    Container tempCont = container.create("javaContainer", delegate);
    System.out.println("tempContainer id" + tempCont.id());
    while (!delegate.finished) {
      try
      {
        Thread.sleep(10);
      }
      catch(Exception e)
      {
        System.out.println(e);
      }
    }
    return tempCont;
  }

  public Container createContainer2(Container container, String name) {
    ItemDelegate delegate = new ItemDelegate();
    Container tempCont = container.create(name, delegate);
    System.out.println("tempContainer id" + tempCont.id());
    while (!delegate.finished) {
      try
      {
        Thread.sleep(10);
      }
      catch(Exception e)
      {
        System.out.println(e);
      }
    }
    return tempCont;
  }

  public com.cbe.Object uploadBinary(Container container) throws IOException {
    
    com.cbe.Object tempObj = null;

    try (InputStream input = new FileInputStream("../resources/config.properties")) {
      Properties prop = new Properties();
      
      // load a properties file
      prop.load(input);

      TransferDelegate delegate = new TransferDelegate();
      byte[] data = Files.readAllBytes(Paths.get(prop.getProperty("binaryPath")));
      tempObj = container.uploadBinary(prop.getProperty("binaryFileName"), data, delegate);
      System.out.println("tempObject id" + tempObj.id());


      while (!delegate.finished) {
        try
        {
            Thread.sleep(10);
        }
        catch(Exception e)
        {
             System.out.println(e);
        }
      }   
     
    } catch (IOException ex) {
    ex.printStackTrace();
    }
    
    return tempObj;
  }

  public com.cbe.Object uploadObject(Container container) {
    
    com.cbe.Object tempObj = null;

    try (InputStream input = new FileInputStream("../resources/config.properties")) {
      Properties prop = new Properties();
      
      // load a properties file
      prop.load(input);

      TransferDelegate delegate = new TransferDelegate();
      tempObj = container.upload(prop.getProperty("filename"), prop.getProperty("path") , delegate);
      System.out.println("tempObject id" + tempObj.id());    

      while (!delegate.finished) {
        try
        {
          Thread.sleep(10);
        }
        catch(Exception e)
        {
          System.out.println(e);
        }
      }
    } catch (IOException ex) {
      ex.printStackTrace();
    }

    return tempObj;
  }

  public com.cbe.Object createObject(Container container) {
    ItemDelegate delegate = new ItemDelegate();
    Obj_VI_Pair metaDataValue = new Obj_VI_Pair("testMetadata", true);

    Obj_KV_Map metadata = new Obj_KV_Map();
    metadata.put("testKey", metaDataValue);

    com.cbe.Object tempObj = container.createObject("JavaTestObject", delegate, metadata);

    while (!delegate.finished) {
      try
      {
        Thread.sleep(10);
      }
      catch(Exception e)
      {
        System.out.println(e);
      }
    }
    return tempObj;
  }

  public com.cbe.Object createObject2(Container container, Obj_KV_Map metadata, String name) {
    ItemDelegate delegate = new ItemDelegate();
    // Obj_VI_Pair metaDataValue = new Obj_VI_Pair("testMetadata", true);

    // Obj_KV_Map metadata = new Obj_KV_Map();
    // metadata.put("testKey", metaDataValue);
    metadata.forEach((K,V)->{
      System.out.printf("%14s, ", V.getFirst());
    });
    System.out.print("\t");
    com.cbe.Object tempObj = container.createObject(name, delegate, metadata);

    while (!delegate.finished) {
      try
      {
        Thread.sleep(10);
      }
      catch(Exception e)
      {
        System.out.println(e);
      }
    }
    return tempObj;
  }

  public void removeContainer(Container container) {
    ItemDelegate delegate = new ItemDelegate();
    container.remove(delegate);
    while (!delegate.finished) {
      try
      {
        Thread.sleep(10);
      }
      catch(Exception e)
      {
        System.out.println(e);
      }
    }
    return;
  }

  public void setContainerACL(Container container , ACL_Map permissionsMap) {
    ShareDelegate delegate = new ShareDelegate();
    container.setACL(permissionsMap, delegate);
    while (!delegate.finished) {
      try
      {
        Thread.sleep(10);
      }
      catch(Exception e)
      {
        System.out.println(e);
      }
    }
    return;
  }

  public AbstractMap<Long,Integer> getContainerACL(Container container) {
    ShareDelegate delegate = new ShareDelegate();
    container.getACL(delegate);
    while (!delegate.finished) {
      try
      {
        Thread.sleep(10);
      }
      catch(Exception e)
      {
        System.out.println(e);
      }
    }
    return delegate.permissionsMap;

  }

  public long share(long userGroupId, Container container) {
    ShareDelegate delegate = new ShareDelegate();
    container.share(userGroupId, "javaShare", delegate);

    while (!delegate.finished) {
      try
      {
        Thread.sleep(10);
      }
      catch(Exception e)
      {
        System.out.println(e);
      }
    }
    return delegate.shareId;
  }

  public QueryResult listAvailableShares(com.cbe.CloudBackend cloudbackend) {
    ShareDelegate delegate = new ShareDelegate();
    cloudbackend.shareManager().listAvailableShares(delegate);
    while (!delegate.finished) {
      try
      {
        Thread.sleep(10);
      }
      catch(Exception e)
      {
        System.out.println(e);
      }
    }
    return delegate._qr;
  }

  public void unshareContainer(long shareId, Container container) {
    ShareDelegate delegate = new ShareDelegate();
    container.unShare(shareId, delegate);
    while (!delegate.finished) {
      try
      {
        Thread.sleep(10);
      }
      catch(Exception e)
      {
        System.out.println(e);
      }
    }
  }

  public static void main(String[] argv) {
    System.out.println("RunMe program start");
    RunMe inst = new RunMe();
    CloudBackend cbobj = inst.login("cr1");
    if (cbobj.account().userId()>0) {
      System.out.println("Login: " + cbobj.account().username() + " " + cbobj.account().firstName() + " " + cbobj.account().lastName());
    } else {
      System.out.println("Login failed. Exit program.");
      return;
    }

    Filter filter = new Filter();
    filter.setAscending(true);
    
    QueryResult qR = inst.queryR(cbobj, filter, cbobj.account().rootContainer().id());
    
    if(qR == null) {
      System.out.println("Query failed null. ");
    } else {
      AbstractList<Item> items = qR.getItemsSnapshot();
      for(Item item : items) {
        System.out.println("Item: " + item.name());
      }
    }

    Container testContainer = inst.createContainer(cbobj.account().rootContainer());
    if (!testContainer.idLoaded()) {
      System.out.println("Container creation failed!");
      return;
    } else {
      System.out.println("Container created id: " + testContainer.id());
    }


    AbstractList<Item> items = qR.getItemsSnapshot();
      for(Item item : items) {
        System.out.println("Item: " + item.name());
      }
    
    com.cbe.Object object = inst.uploadObject(testContainer);
    if (!object.idLoaded()) {
      System.out.println("Object creation failed!");
      return;
    } else {
      System.out.println("Object created id: " + object.id());
    }
    
    qR = inst.query(testContainer);
    items = qR.getItemsSnapshot();
    for(Item item : items) {
      System.out.println("Item: " + item.name());
    }

    try {
      com.cbe.Object binaryObject = inst.uploadBinary(testContainer);
    }
    catch(IOException e) {
      e.printStackTrace();
    }
    
    items = qR.getItemsSnapshot();
    for(Item item : items) {
      System.out.println("Item: " + item.name());
    }

    long _parentId_ = testContainer.id();
    RunMe inst2 = new RunMe();

    CloudBackend cbobj2 = inst2.login("cr2");
    if (cbobj2.account().userId() > 0) {
      System.out.println("Login: " + cbobj.account().username() + " " + cbobj.account().firstName() + " " + cbobj.account().lastName());
    } else {
      System.out.println("Login failed. Exit program.");
      return;
    }

    ACL_Map permissions = new ACL_Map();
    permissions.put(cbobj2.account().userId(), 7);

    inst.setContainerACL(testContainer, permissions);

    long sId = inst.share(cbobj2.account().userId(), testContainer);

    AbstractMap<Long, Integer> queriedPermissionsMap = inst2.getContainerACL(testContainer);

    queriedPermissionsMap.forEach( (k,v) -> System.out.println("User Id: " + k + ": Permission: " + v));

    QueryResult shareQr = inst2.listAvailableShares(cbobj2);
    // To see if user2 has the permissions correct we test by query the testContainer with user2
    // and create an Object in the test container.
    Container cont = null;
    int i = 0;
    int get = 0;
    items = shareQr.getItemsSnapshot();
    for(Item item : items) {
      System.out.println("share: " + item.name());
      if(item.name() == "javaContainer") {
        get = i;
      }
      i++;
    }
    Item tempItem = items.get(get);

    cont = cbobj2.castContainer(tempItem);
    com.cbe.Object cObj = inst2.createObject(cont);
   

    AbstractMap<String, Obj_VI_Pair> metadata = cObj.keyValues();
    metadata.forEach((K,V)->{
      if (V.getSecond()) {
        System.out.println(K + "=" + V.getFirst() + " \t(indexed)");
      } else {
        System.out.println(K + "=" + V.getFirst());
      }
    });
    
    inst.unshareContainer(sId, testContainer);

    inst2.removeContainer(testContainer);
  
    // Testing to create some data points for 2 different containers Airports and Planes.

    System.out.print("\nLoad airports\t");
    Container airports = inst.createContainer2(cbobj.account().rootContainer(), "Airports");

    for(int k = 1; k <= 22; k++) {
      final List<String> countries        = Collections.unmodifiableList(Arrays.asList("France", "Spain", "UK" , "Netherlands", "Denmark", "Norway", "Finland", "Estonia", "Lithuania", "Latvia", "Iceland", "Luxemburg", "Belgium", "Germany", "Poland", "Italy", "Czech Rebulic", "Slovakia", "Austria", "Switzerland", "Slovenia", "Greece", "Irland"));
      final List<String> airportLocations = Collections.unmodifiableList(Arrays.asList("Paris", "Madrid", "London", "Amsterdam", "Copenhagen", "Oslo", "Helsinki", "Tallin", "Vilnius", "Riga", "Reijkavik", "Luxemburg", "Antwerpen", "Berlin", "Warsaw", "Rome", "Praha", "Bratislava", "Wien", "Bern", "Ljubljana", "Athens", "Dublin"));
    
     Obj_KV_Map mdata = new Obj_KV_Map();
     mdata.put("Country", new Obj_VI_Pair(countries.get(k), true));
     mdata.put("Name", new Obj_VI_Pair(airportLocations.get(k), true));
     inst.createObject2(airports, mdata, airportLocations.get(k));
    }
  
    for(int n = 0; n <= 6; n++) {
      final List<String> airportLocations = Collections.unmodifiableList(Arrays.asList("Gothenburg", "Sundsvall", "Linköping", "Örebro", "Norrköping", "Stockholm", "Malmö"));

      Obj_KV_Map mdata = new Obj_KV_Map();
      mdata.put("Country", new Obj_VI_Pair("Sweden", true));
      mdata.put("Name", new Obj_VI_Pair(airportLocations.get(n), true));
      inst.createObject2(airports, mdata, airportLocations.get(n));
    }


    System.out.print("\nLoad planes\t");
    Container planes = inst.createContainer2(cbobj.account().rootContainer(), "Planes");

    for(int j=100; j<=200; j++) {
      
      if((j%20) == 0) {
        final List<String> airportLocations = Collections.unmodifiableList(Arrays.asList("Gothenburg", "Sundsvall", "Linköping", "Örebro", "Norrköping", "Stockholm", "Malmö"));
        final List<String> airplaneType = Collections.unmodifiableList(Arrays.asList("Boeing_737", "Airbus_A350", "Boeing_747", "Airbus_A220", "Boeing_777", "Boeing_787", "Airbus_A320"));
        final List<String> airLine = Collections.unmodifiableList(Arrays.asList("AY", "DY", "SK"));
        
        Obj_KV_Map mdata = new Obj_KV_Map();

        Random rand = new Random();
        mdata.put("Location", new Obj_VI_Pair(airportLocations.get(rand.nextInt((7))), true));
        mdata.put("Model", new Obj_VI_Pair(airplaneType.get(rand.nextInt((7))), true));
        mdata.put("APID", new Obj_VI_Pair(Integer.toString(rand.nextInt((10000))), true));
        final String name = airLine.get(rand.nextInt((3))) + Integer.toString(j);
        inst.createObject2(planes, mdata, name);
      
      } else {
        final List<String> airportLocations = Collections.unmodifiableList(Arrays.asList("Paris", "Madrid", "London", "Amsterdam", "Copenhagen", "Oslo", "Helsinki", "Tallin", "Vilnius", "Riga", "Reijkavik", "Luxemburg", "Antwerpen", "Berlin", "Warsaw", "Rome", "Praha", "Bratislava", "Wien", "Bern", "Ljubljana", "Athens", "Dublin"));
        final List<String> airplaneType = Collections.unmodifiableList(Arrays.asList("Boeing_737", "Airbus_A350", "Boeing_747", "Airbus_A220", "Boeing_777", "Boeing_787", "Airbus_A320"));
        final List<String> airLine = Collections.unmodifiableList(Arrays.asList("AF", "AY", "AZ", "BA", "DY", "KL", "LH", "LX", "SK", "TP", "UX"));

        Obj_KV_Map mdata = new Obj_KV_Map();

        Random rand = new Random();
        mdata.put("Location", new Obj_VI_Pair(airportLocations.get(rand.nextInt((22))), true));
        mdata.put("Model", new Obj_VI_Pair(airplaneType.get(rand.nextInt((7))), true));
        mdata.put("APID", new Obj_VI_Pair(Integer.toString(rand.nextInt((10000))), true));
        final String name = airLine.get(rand.nextInt((11))) + Integer.toString(j);
        inst.createObject2(planes, mdata, name);
      }
    }
    
    // testing query.join for Airports with Planes.

    Filter f1 = new Filter();
    f1.setQuery("Country:Sweden");
    f1.setDataType(4);    // ItemType::Object
    Filter f2 = new Filter();
    f2.setQuery("Model:Boeing*");
    f2.setDataType(4);    // ItemType::Object
  
    System.out.println("\nFilter " + f1.getQuery() + " and " + f2.getQuery());
    ItemDelegate delegate = new ItemDelegate();
    delegate.finished = false;
    airports.query(f1, delegate).join(planes, "Name", "Location", f2);  

    while(!delegate.finished) {
      try
      {
          Thread.sleep(1000);
      }
      catch(InterruptedException ex)
      {
          Thread.currentThread().interrupt();
      }
    }
    
    items = delegate.qR.getItemsSnapshot();
    System.out.println("\nResult count: " + items.size());
    
    for(Item item : items) {
      System.out.print("Name: " + item.name());
      System.out.print("\tpid(" + item.parentId());
      System.out.println(") ts: " + item.updated());
      if(item.type() == 4) {
        com.cbe.Object obj = cbobj.castObject(item);
        if(obj.keyValues().size() != 0) {
          // System.out.println("Key/Value data:");
          System.out.println("---------------------------------------------------");
          AbstractMap<String, Obj_VI_Pair> meta = obj.keyValues();
          meta.forEach((K,V)->{
            if (V.getSecond()) {
              System.out.printf("%10s = %-15s  (indexed) \n", K, V.getFirst());
            } else {
              System.out.printf("%10s = %-15s \n", K, V.getFirst());
            }
          });
        }
        System.out.println("===================================================");
      }
    }

    f2.setQuery("Model:Airbus*");
  
    System.out.println("\nFilter " + f1.getQuery() + " and " + f2.getQuery());
    ItemDelegate delegate1 = new ItemDelegate();
    delegate.finished = false;
    airports.query(f1, delegate1).join(planes, "Name", "Location", f2);  

    while(!delegate1.finished) {
      try
      {
          Thread.sleep(1000);
      }
      catch(InterruptedException ex)
      {
          Thread.currentThread().interrupt();
      }
    }
    
    items = delegate1.qR.getItemsSnapshot();
    System.out.println("\nResult count: " + items.size());
    
    for(Item item : items) {
      System.out.print("Name: " + item.name());
      System.out.print("\tpid(" + item.parentId());
      System.out.println(") ts: " + item.updated());
      if(item.type() == 4) {
        com.cbe.Object obj = cbobj.castObject(item);
        if(obj.keyValues().size() != 0) {
          // System.out.println("Key/Value data:");
          System.out.println("---------------------------------------------------");
          AbstractMap<String, Obj_VI_Pair> meta = obj.keyValues();
          meta.forEach((K,V)->{
            if (V.getSecond()) {
              System.out.printf("%10s = %-15s  (indexed) \n", K, V.getFirst());
            } else {
              System.out.printf("%10s = %-15s \n", K, V.getFirst());
            }
            // System.out.println("Key: " + K);
            // System.out.println("Value: " + V.getFirst() + " Indexed (true/false): " + V.getSecond());
          });
        }
        System.out.println("===================================================");
      }
    }
    

    // cleanup.
    inst.removeContainer(airports);
    inst.removeContainer(planes);
    System.out.println("RunMe program end.");
  }

}

