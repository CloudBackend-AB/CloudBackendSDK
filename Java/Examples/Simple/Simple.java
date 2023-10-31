import com.cbe.*;
import com.std.*;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.AbstractMap;
import java.util.List;
import java.util.Properties;


public class Simple {
  static String programName = "Simple";

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
    }
    catch (RuntimeException e)
    {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return queryResult;
  }

  public com.cbe.QueryResult getItems(com.cbe.Container container) {
    Filter filter = new Filter();
    filter.setAscending(true);
    filter.setContainerFirst(true);
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

  public com.cbe.QueryResult getObjects(com.cbe.Container container) {
    Filter filter = new Filter();
    filter.setAscending(true);
    filter.setDataType(com.cbe.ItemType.Object);
    // filter.setQuery("*");
    // System.out.println("Filter on name/title:" + filter.getQuery());
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

  public boolean downloadObject(com.cbe.Object object, String path) {
    boolean succeeded = false;
    MyDownloadDelegate delegate = new MyDownloadDelegate();
    try
    {
      object.download(path, delegate);
      if (!object.idLoaded()) {
        succeeded = false;
      } else {
        succeeded = true;
      }
    }
    catch (RuntimeException e)
    {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return succeeded;
  }

  public com.cbe.Object uploadBinary(com.cbe.Container container) throws IOException {
    com.cbe.Object returnObject = null;
    byte[] data;
    String name;
    String fullPath;
    try (InputStream input = new FileInputStream("../../resources/config.properties")) {
      Properties myProperties = new Properties();
      // load a properties file
      myProperties.load(input);
      fullPath = myProperties.getProperty("binaryPath");
      data = Files.readAllBytes(Paths.get(myProperties.getProperty("binaryPath")));
      name = myProperties.getProperty("binaryFileName");
    } catch (IOException ex) {
      throw new RuntimeException(ex);
    }
    try
    {
      MyUploadDelegate delegate = new MyUploadDelegate();
      System.out.println("upload " + fullPath);
      container.uploadBinary(name, data, delegate);
      returnObject = delegate.waitForRsp();
    }
    catch (RuntimeException e)
    {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return returnObject;
  }

  public com.cbe.Object uploadObject(com.cbe.Container container) {
    com.cbe.Object returnObject = null;
    String path;
    String name;
    try (InputStream input = new FileInputStream("../../resources/config.properties")) {
      Properties myProperties = new Properties();
      // load a properties file
      myProperties.load(input);
      path = myProperties.getProperty("path");
      name = myProperties.getProperty("filename");
    } catch (IOException ex) {
      throw new RuntimeException(ex);
    }
    try
    {
      MyUploadDelegate delegate = new MyUploadDelegate();
      System.out.println("upload " + path + name);
      container.upload(name, path, delegate);
      returnObject = delegate.waitForRsp();
    }
    catch (RuntimeException e)
    {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return returnObject;
  }

  public com.cbe.Object createObject(com.cbe.Container container, String title, Obj_KV_Map metadata) {
    MyCreateObjectDelegate delegate = new MyCreateObjectDelegate();
    com.cbe.Object tempObj = null;
    try
    {
      System.out.println("create " + title);
      tempObj = container.createObject(title, metadata, delegate);
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

  public void setContainerACL(com.cbe.Container container , com.std.Acl_Map permissionsMap) {
    MyAclDelegate delegate = new MyAclDelegate();
    try
    {
      container.setAcl(permissionsMap, delegate);
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

  public AbstractMap<Long,com.std.PermissionAclScopeEnum> getContainerACL(com.cbe.Container container) {
    MyAclDelegate delegate = new MyAclDelegate();
    AbstractMap<Long, com.std.PermissionAclScopeEnum> myMap = null;
    try
    {
      container.getAcl(delegate);
      myMap = delegate.waitForRsp();
    }
    catch (RuntimeException e)
    {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return myMap;
  }

  public long share(long userGroupId, com.cbe.Container container) {
    MyShareDelegate delegate = new MyShareDelegate();
    long myShare = 0;
    try
    {
      container.share(userGroupId, "javaShare", delegate);
      myShare = delegate.waitForRsp();
    }
    catch (RuntimeException e)
    {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return myShare;
  }

  public com.cbe.QueryResult listAvailableShares(com.cbe.CloudBackend cloudbackend) {
    MyListSharesDelegate delegate = new MyListSharesDelegate();
    com.cbe.QueryResult myResult = null;
    try
    {
      cloudbackend.shareManager().listAvailableShares(delegate);
      myResult = delegate.waitForRsp();
    }
    catch (RuntimeException e)
    {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
    return myResult;
  }

  public void unshareContainer(long shareId, com.cbe.Container container) {
    MyUnShareDelegate delegate = new MyUnShareDelegate();
    try
    {
      container.unShare(shareId, delegate);
      delegate.waitForRsp();
    }
    catch (RuntimeException e)
    {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    };
  }

  public static void execThis(String myCommand) {
    try
    {
      Process process = Runtime.getRuntime().exec(myCommand);
      BufferedReader reader = new BufferedReader(
                              new InputStreamReader(process.getInputStream()));
      String aline;
      while ((aline = reader.readLine()) != null) {
        System.out.println(aline);
      }
      reader.close();
    }
    catch (IOException e)
    {
      e.printStackTrace();
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  
  public static void main(String[] argv) {
    System.out.println(programName + " program start.");
    Simple inst = new Simple();
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
    String toFolder = "/tmp/";
    String file1 = "";
    String file2 = "";
    try
    {
      List<Item> items;
      String containerName = "zSimple";
      System.out.println(programName + "::getContainers");   
      QueryResult qR = inst.getContainers(cbobj.account().rootContainer());
      items = qR.getItemsSnapshot();
      if(qR == null) {
        System.out.println("Query got null. ");
      } else {
        System.out.println("Loaded: " + qR.containersLoaded());
        items = qR.getItemsSnapshot();
        System.out.println("Content of: /");
        for(Item item : items) {
          if (item.type() == com.cbe.ItemType.Container) {
            System.out.println(" <CNT>  " + item.name() + "/");
          } else {
            System.out.println("\t" + item.name());
          }
        }
      }
  
      Container testContainer = inst.createContainer(cbobj.account().rootContainer(), containerName);
      if (testContainer == null) {
        System.out.println("Failed creating container /" + containerName);
        cbobj.terminate();
        System.out.println(programName + " program stops.");
        return;
      } else {
        System.out.println("Container created: " + testContainer.name() + " (" + testContainer.id() + ")");
      }
  
      System.out.println(programName + "::uploadObject");   
      com.cbe.Object object = inst.uploadObject(testContainer);
      if (!object.idLoaded()) {
        System.out.println("Object creation failed!");
        return;
      } else {
        System.out.println("Object created id: " + object.id());
      }
  
      System.out.println(programName + "::uploadBinary");   
      com.cbe.Object binaryObject = null;
      binaryObject = inst.uploadBinary(testContainer);
  
      Container subContainer = inst.createContainer(testContainer, "my-sub");
      if (testContainer == null) {
        System.out.println("Failed creating sub-container in /" + testContainer.name() + "/");
        cbobj.terminate();
        System.out.println(programName + " program stops.");
        return;
      } else {
        System.out.println("Container created: " + subContainer.name() + " (" + subContainer.id() + ")");
      }
  
      qR = inst.getItems(testContainer);
      items = qR.getItemsSnapshot();
      if(qR == null) {
        System.out.println("Query got null. ");
      } else {
        items = qR.getItemsSnapshot();
        System.out.println("Loaded: " + qR.itemsLoaded());
        System.out.println("Content of: /" + testContainer.name() + "/");
        for(Item item : items) {
          if (item.type() == com.cbe.ItemType.Container) {
            System.out.println(" <CNT>  " + item.name() + "/");
          } else {
            System.out.println("\t" + item.name());
          }
        }
        System.out.println("Count: " + qR.objectsLoaded() + " objects\t" + qR.containersLoaded() + " containers");
      }
  
      file1 = toFolder + object.name();
      if (inst.downloadObject(object, toFolder)) {
        System.out.println("Downloaded: " + object.name() + " to " + toFolder);
      } else {
        System.out.println("Object download failed!");
        return;
      }
  
      file2 = toFolder + binaryObject.name();
      if (inst.downloadObject(binaryObject, toFolder)) {
        System.out.println("Downloaded: " + binaryObject.name()  + " to " + toFolder);
      } else {
        System.out.println("Binary Object download failed!");
        return;
      }
  
      System.out.println("Remove container /" + testContainer.name() + "/");
      inst.removeContainer(testContainer);

      String command = "ls -l " + file1 + " " + file2;
      System.out.println("Downloaded");
      System.out.println("----------");
      execThis(command);
    }
    catch (IOException e) {
      System.out.println("Caught i/o error:");
      e.printStackTrace();
      System.out.println();
    }
    catch (RuntimeException e) {
      System.out.println("Caught runtime error:");
      e.printStackTrace();
      System.out.println();
    }

    System.out.println("Disconnecting.");
    if (cbobj != null) {
      cbobj.terminate();
    }
    System.out.println(programName + " program end.");
  }  // main
}   // class Simple
