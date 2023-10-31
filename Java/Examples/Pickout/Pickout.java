import com.cbe.*;
import com.std.*;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.SimpleDateFormat;
import java.util.AbstractMap;
import java.util.Date;
import java.util.List;
import java.util.Properties;


public class Pickout {
  static String programName = "Pickout";

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
  
  public com.cbe.QueryResult getContainers(com.cbe.Container container) {
    Filter filter = new Filter();
    filter.setAscending(true);
    filter.setDataType(com.cbe.ItemType.Container);
    MyQueryDelegate delegate = new MyQueryDelegate();
    com.cbe.QueryResult queryResult = null;
    try
    {
      com.cbe.QueryChain chain = container.query(filter, delegate);
      queryResult = delegate.waitForRsp();
    } catch (RuntimeException e) {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return queryResult;
  }

  public com.cbe.QueryResult getItems(com.cbe.Container container) {
    Filter filter = new Filter();
    filter.setAscending(true);
    MyQueryDelegate delegate = new MyQueryDelegate();
    com.cbe.QueryResult queryResult = null;
    try
    {
      com.cbe.QueryChain chain = container.query(filter, delegate);
      queryResult = delegate.waitForRsp();
    } catch (RuntimeException e) {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return queryResult;
  }

  public com.cbe.QueryResult getObjects(com.cbe.Container container) {
    Filter filter = new Filter();
    filter.setAscending(true);
    filter.setDataType(com.cbe.ItemType.Object);
    filter.setQuery("*");
    System.out.println("Filter on name/title:" + filter.getQuery());
    MyQueryDelegate delegate = new MyQueryDelegate();
    com.cbe.QueryResult queryResult = null;
    try
    {
      com.cbe.QueryChain chain = container.query(filter, delegate);
      queryResult = delegate.waitForRsp();
    }
    catch (RuntimeException e)
    {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return queryResult;
  }

  public com.cbe.Container createContainer(com.cbe.Container parentContainer, String newName) {
    MyCreateContainerDelegate delegate = new MyCreateContainerDelegate();
    com.cbe.Container returnContainer = null;
    try
    {
      com.cbe.Container tempCont = parentContainer.createContainer(newName, delegate);
      returnContainer = delegate.waitForRsp();
    }
    catch (RuntimeException e)
    {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return returnContainer;
  }


  public com.cbe.Object createObject(com.cbe.Container container, String newName) {
    MyCreateObjectDelegate delegate = new MyCreateObjectDelegate();
    Obj_VI_Pair metaDataValue = new Obj_VI_Pair("testMetadata", true);
    Obj_KV_Map metadata = new Obj_KV_Map();
    metadata.put("testKey", metaDataValue);
    com.cbe.Object tempObj = null;
    try
    {
      tempObj = container.createObject(newName, metadata, delegate);
      delegate.waitForRsp();
    }
    catch (RuntimeException e)
    {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return tempObj;
  }

  public void removeContainer(com.cbe.Container container) {
    MyRemoveContainerDelegate delegate = new MyRemoveContainerDelegate();
    try
    {
      container.remove(delegate);
      delegate.waitForRsp();
    }
    catch (RuntimeException e)
    {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return;
  }

  public void removeObject(com.cbe.Object object) {
    MyRemoveObjectDelegate delegate = new MyRemoveObjectDelegate();
    try
    {
      object.remove(delegate);
      delegate.waitForRsp();
    }
    catch (RuntimeException e)
    {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return;
  }

  //////////////////////////////////////////////////////////////////////////////

  public static void main(String args[]) throws IOException {
    System.out.println(programName + " program start.");
    Pickout inst = new Pickout();
    com.cbe.CloudBackend cbobj = inst.myLogin("gh1");
    if (cbobj.account().userId()>0) {
      System.out.println("Authenticated as: " + cbobj.account().username() + "\t" + cbobj.account().firstName() + " " + cbobj.account().lastName());
      System.out.println("Version: " + cbobj.version());
    } else {
      System.out.println("Login failed.");
      cbobj.terminate();
      System.out.println(programName + " program stop.");
      return;
    }

    BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
    com.cbe.Container rootContainer = cbobj.account().rootContainer();
    com.cbe.Container myContainer = null;
    com.cbe.Object myNewObject;
    com.cbe.Object myObject;
    com.cbe.QueryResult qR = null;
    List<com.cbe.Item> items;
    String ans;
    String containerName = "zPickout";
    long i;
    qR = inst.getContainers(rootContainer);
    if (qR != null) {
      items = qR.getItemsSnapshot();
      for(com.cbe.Item item : items) {
        if (item.name().contentEquals(containerName)) {
          myContainer = com.cbe.CloudBackend.castContainer(item);
          System.out.println("Container found\t/" + myContainer.name() + " (" + myContainer.id() + ")");
        }
      }
    }
    if (myContainer == null) {
      myContainer = inst.createContainer(rootContainer, containerName);
      System.out.println("Container created\t/" + myContainer.name() + " (" + myContainer.id() + ")");
      System.out.println("Adding objects.");
      myNewObject = inst.createObject(myContainer, "data record");
      myNewObject = inst.createObject(myContainer, "my file name.txt");
    }

    if (myContainer != null) {
      qR = inst.getObjects(myContainer);
    }
    if (qR == null) {
      System.out.println("Query got null. ");
    } else {
      System.out.println("Content of: /" + myContainer.name());
      System.out.println("Objects loaded: " + qR.objectsLoaded());
      items = qR.getItemsSnapshot();
      for(com.cbe.Item item : items) {
        ans = "n";
        long epochTimestamp = item.updated();
        Date javaTimestamp = new Date(epochTimestamp * 1000L);
        SimpleDateFormat myDateformat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss z");
        String readableDate = myDateformat.format(javaTimestamp);
        System.out.print(readableDate + "  \t");
        System.out.print("(" + item.id() + ")" + "\t");
        System.out.println('"' + item.name() + '"');
        System.out.print("Delete object  (N/y) ? ");
        ans = reader.readLine();
        if (ans.contentEquals("y")) {
          System.out.println();
          myObject = com.cbe.CloudBackend.castObject(item);
          System.out.print(myObject.name() + "\t");
          System.out.println("to be removed.");
          System.out.println("title     = " + '"' + myObject.name() + '"');
          System.out.println("id        = " + myObject.id());
          System.out.println("created   = " + myObject.created());
          System.out.println("updated   = " + myObject.updated());
          System.out.println("parent    = " + myObject.parentId());
          System.out.println("length    = " + myObject.length());
          System.out.println("mime type = " + myObject.getMimeType());
          System.out.println("type      = " + myObject.type());
          try
          {
            inst.removeObject(myObject);
            System.out.println("ok");
          }
          catch (RuntimeException e)
          {
            System.out.println("Caught error:");
            e.printStackTrace();
            System.out.println();
          };
        } else {
          System.out.println("-");
        }
      }
    }
    if (myContainer != null) {
      System.out.println("");
      qR = inst.getItems(myContainer);
      i = qR.totalCount();
      System.out.println("Items left in container: " + i);
      System.out.print("Delete this container /" + myContainer.name() + "  (N/y) ? ");
      ans = reader.readLine();
      if (ans.contentEquals("y")) {
        if (i > 0) {
          System.out.print("Are you sure" + "  (N/y) ? ");
          ans = reader.readLine();
        }
        if (ans.contentEquals("y")) {
          try
          {
            inst.removeContainer(myContainer);
            System.out.println("ok");
          }
          catch (RuntimeException e)
          {
            System.out.println("Caught error:");
            e.printStackTrace();
            System.out.println();
          };
        } else {
          System.out.println("+");
        }
      } else {
        System.out.println("-");
      }
    }
    System.out.println("Disconnecting.");
    if (cbobj != null) {
      cbobj.terminate();
    }
    System.out.println(programName + " program end.");
  } // main
}  // class Pickout
