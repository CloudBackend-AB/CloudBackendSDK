import java.util.Scanner;
import java.util.*;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties; 
import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.math.*;
import java.lang.*;
import com.cbe.*;


public class runme {

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

    runme inst = new runme();
    CloudBackend cbobj = inst.login("cr1");
    
    System.out.println("You've reached main in Java congrats " + cbobj.account().username() + " " + cbobj.account().rootContainer().id());

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
    System.out.println("Container created id: " + testContainer.id());

    AbstractList<Item> items = qR.getItemsSnapshot();
      for(Item item : items) {
        System.out.println("Item: " + item.name());
      }
    
    com.cbe.Object object = inst.uploadObject(testContainer);
      
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
    runme inst2 = new runme();

    CloudBackend cbobj2 = inst2.login("cr2");

    ACL_Map permissions = new ACL_Map();
    permissions.put(cbobj2.account().userId(), 7);

    inst.setContainerACL(testContainer, permissions);

    long sId = inst.share(cbobj2.account().userId(), testContainer);

    AbstractMap<Long, Integer> queriedPermissionsMap = inst2.getContainerACL(testContainer);

    queriedPermissionsMap.forEach( (k,v) -> System.out.println("User Id: " + k + ": Permission: " + v));

    QueryResult shareQr = inst2.listAvailableShares(cbobj2);
    // To see if user2 has the permissions correct we test by query the testContainer with user2 and create an Object in the test container.
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
    // }
    Item tempItem = items.get(get);

    cont = cbobj2.castContainer(tempItem);
    com.cbe.Object cObj = inst2.createObject(cont);
   

    AbstractMap<String, Obj_VI_Pair> metadata = cObj.keyValues();

    metadata.forEach((K,V)->{
      System.out.println("MetaKey: " + K);
      System.out.println("MetaValue: " + V.getFirst() + " Indexed (true/false): " + V.getSecond());
    });
    
    inst.unshareContainer(sId, testContainer);

    inst2.removeContainer(testContainer);
  
  }
}
