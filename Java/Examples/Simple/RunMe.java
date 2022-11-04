import com.cbe.*;
import com.std.*;
import com.cbe.util.*;
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
    CloudBackend cloudBackend = null;
    try (InputStream input = new FileInputStream("../../resources/config.properties")) {

      Properties prop = new Properties();

      // load a properties file
      prop.load(input);

      // get the property value and set the login credentials
      String username = prop.getProperty(credentials + ".username");
      String password = prop.getProperty(credentials + ".password");
      String tenant   = prop.getProperty(credentials + ".tenant");
      MyLogInDelegate delegate = new MyLogInDelegate();
      System.out.println("call CloudBackend.login");
      cloudBackend = CloudBackend.logIn(username, password, tenant, delegate);
      return delegate.waitForRsp();
    } catch (IOException ex) {
      throw new RuntimeException(ex);
    }
    finally {
      cloudBackend.terminate();
    }
  }
  
  public QueryResult query(Container container) {
    MyQueryDelegate delegate = new MyQueryDelegate();
    QueryChain chain = container.query(delegate);
    return delegate.waitForRsp();
  }

  public Container createContainer(Container container) {
    MyCreateContainerDelegate delegate = new MyCreateContainerDelegate();
    Container tempCont = container.createContainer("javaContainer", delegate);
    return delegate.waitForRsp();
  }

  public com.cbe.Object uploadBinary(Container container) throws IOException {
    try (InputStream input = new FileInputStream("../../resources/config.properties")) {
      Properties prop = new Properties();
      byte[] data;
      String name;
      
      // load a properties file
      prop.load(input);
      data = Files.readAllBytes(Paths.get(prop.getProperty("binaryPath")));
      name = prop.getProperty("binaryFileName");    
      MyUploadDelegate delegate = new MyUploadDelegate();
      container.uploadBinary(name, data, delegate);
      return delegate.waitForRsp();
    } catch (IOException ex) {
      throw new RuntimeException(ex);
    }
  }

  public void downloadObject(com.cbe.Object object, String path) {
    MyDownloadDelegate delegate = new MyDownloadDelegate();
    object.download(path , delegate);
  }

  public com.cbe.Object uploadObject(Container container) {
    try (InputStream input = new FileInputStream("../../resources/config.properties")) {
      Properties prop = new Properties();
      String path;
      String name;
        
      // load a properties file
      prop.load(input);
      path = prop.getProperty("path");
      name = prop.getProperty("filename");
      MyUploadDelegate delegate = new MyUploadDelegate();
      container.upload(name, path, delegate);
      return delegate.waitForRsp();
    } catch (IOException ex) {
      throw new RuntimeException(ex);
    }
  }

  // maybe only return the metadata key value map instead look at logic in main function.
  public com.cbe.Object createObject(Container container) {
    MyCreateObjectDelegate delegate = new MyCreateObjectDelegate();
    Obj_VI_Pair metaDataValue = new Obj_VI_Pair("testMetadata", true);

    Obj_KV_Map metadata = new Obj_KV_Map();
    metadata.put("testKey", metaDataValue);

    com.cbe.Object tempObj = container.createObject("JavaTestObject", delegate, metadata);
    delegate.waitForRsp();
    return tempObj;
  }


  public void removeContainer(Container container) {
    MyRemoveContainerDelegate delegate = new MyRemoveContainerDelegate();
    container.remove(delegate);
    delegate.waitForRsp();
    return;
  }

  public void setContainerACL(Container container , com.std.Acl_Map permissionsMap) {
    MyAclDelegate delegate = new MyAclDelegate();
    container.setAcl(permissionsMap, delegate);
    delegate.waitForRsp();
    return;
  }

  public AbstractMap<Long,com.cbe.Permissions> getContainerACL(Container container) {
    MyAclDelegate delegate = new MyAclDelegate();
    container.getAcl(delegate);
    return delegate.waitForRsp();
  }

  public long share(long userGroupId, Container container) {
    MyShareDelegate delegate = new MyShareDelegate();
    container.share(userGroupId, "javaShare", delegate);
    return delegate.waitForRsp();
  }

  public QueryResult listAvailableShares(com.cbe.CloudBackend cloudbackend) {
    MyListSharesDelegate delegate = new MyListSharesDelegate();
    cloudbackend.shareManager().listAvailableShares(delegate);
    return delegate.waitForRsp();
  }

  public void unshareContainer(long shareId, Container container) {
    MyUnShareDelegate delegate = new MyUnShareDelegate();
    container.unShare(shareId, delegate);
    delegate.waitForRsp();
  }

  public static void main(String[] argv) {
    System.out.println("RunMe program start");
    RunMe inst = new RunMe();
    CloudBackend cbobj = inst.login("cr1");
    Account a = cbobj.account();

    System.out.println("Main Java container.uname: " + a.username());
    if (cbobj.account().userId()>0) {
      System.out.println("Login: " + cbobj.account().username() + " " + cbobj.account().firstName() + " " + cbobj.account().lastName());
    } else {
      System.out.println("Login failed.");
      cbobj.terminate();
      System.out.println("RunMe program end.");
      return;
    }

    Filter filter = new Filter();
    filter.setAscending(true);
    System.out.println("RunMe::query ");   
    QueryResult qR = inst.query(cbobj.account().rootContainer());
    List<Item> items = qR.getItemsSnapshot();
    if(qR == null) {
      System.out.println("Query failed null. ");
    } else {
      System.out.println("RunMe::Loop 1 ");
      items = qR.getItemsSnapshot();
      System.out.println("RunMe::Loop 2 ");
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

    com.cbe.Object object = inst.uploadObject(testContainer);
    if (!object.idLoaded()) {
      System.out.println("Object creation failed!");
      return;
    } else {
      System.out.println("Object created id: " + object.id());
    }

    String dPath = "/tmp/";
    inst.downloadObject(object, dPath);
    if (!object.idLoaded()) {
      System.out.println("Object download failed!");
      return;
    } else {
      System.out.println("1'st Object downloaded: " + object.name() + " to " + dPath);
    }
    
    qR = inst.query(testContainer);
    List<Item> items2 = qR.getItemsSnapshot();
    for(Item item : items2) {
      System.out.println("Item: " + item.name());
    }

    com.cbe.Object binaryObject = null;
    try {
      binaryObject = inst.uploadBinary(testContainer);
    }
    catch(IOException e) {
      e.printStackTrace();
    }

    inst.downloadObject(binaryObject, dPath);
    if (!object.idLoaded()) {
      System.out.println("Binary Object download failed!");
      return;
    } else {
      System.out.println("Binary Object downloaded: " + binaryObject.name()  + " to " + dPath);
    }
    
    inst.removeContainer(testContainer);

    cbobj.terminate();
    System.out.println("RunMe program end.");
  }

}

