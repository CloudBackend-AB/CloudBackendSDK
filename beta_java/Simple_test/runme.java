import java.util.Scanner;
import java.util.*; 
import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.math.*;
import java.lang.*;
import com.cbe.*;


public class runme {

  public CloudBackend run(String username, String password, String tenant) {
    AccountDelegate delegate = new AccountDelegate();
    CloudBackend tempObj = CloudBackend.logIn(username, password, tenant, delegate);
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
  
  public QueryResult queryR(CloudBackend obj, Filter filter, java.math.BigInteger containerId) {
    ItemDelegate delegate = new ItemDelegate();
    obj.account().rootContainer().query(delegate);
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
    TransferDelegate delegate = new TransferDelegate();
      byte[] data = Files.readAllBytes(Paths.get("../testdata/sound1.mp3"));
      com.cbe.Object tempObj = container.uploadBinary("sound1.mp3", data, delegate);
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
      return tempObj;
  }

  public com.cbe.Object uploadObject(Container container) {
    TransferDelegate delegate = new TransferDelegate();
    com.cbe.Object tempObj = container.upload("home.xml", "../testdata/" , delegate);
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

  

  public static void main(String[] argv) {
    runme inst = new runme();
    String uname = "githubtester3";
    String pword = "gitHubTester3password";
    String tena  = "cbe_githubtesters";
    CloudBackend cbobj = inst.run(uname, pword, tena);
    
    System.out.println("You have reached main in Java, congratulations\t" + cbobj.account().username() );

    Filter filter = new Filter();
    filter.setAscending(true);
    
    System.out.println("\nContent listing of container: /");    
    QueryResult qR = inst.queryR(cbobj, filter, cbobj.account().rootContainer().id());
    if(qR == null) {
      System.out.println("Query failed - null. ");
    } else {
      AbstractList<Item> items = qR.getItemsSnapshot();
      for(Item item : items) {
        System.out.println("Item: " + item.name());
      }
    }

    System.out.println("\nCreating new container for uploading of objects.");
    Container testContainer = inst.createContainer(cbobj.account().rootContainer());
    System.out.println("Container created \tname: " + testContainer.name() + "\tid: " + testContainer.id());

    System.out.println("\nContent listing of container: /");    
    AbstractList<Item> items = qR.getItemsSnapshot();
      for(Item item : items) {
        System.out.println("Item: " + item.name());
      }
    
    System.out.println("\nUploading objects to /" + testContainer.name());  
    com.cbe.Object object = inst.uploadObject(testContainer);
    qR = inst.query(testContainer);
    items = qR.getItemsSnapshot();
    for(Item item : items) {
      System.out.println("Item: " + item.name());
    }
    try {
    com.cbe.Object binaryObject = inst.uploadBinary(testContainer);
    } catch(IOException e) {
        e.printStackTrace();
    }

    System.out.println("\nContent listing of container: " + testContainer.name());
    items = qR.getItemsSnapshot();
    for(Item item : items) {
      System.out.println("Item: " + item.name());
    }

    System.out.println("\nCleaning up.");
    BigInteger _parentId_ = testContainer.id();
    inst.removeContainer(testContainer);

    testContainer.delete();
    object.delete();

    System.out.println("Container and object have been removed  (" +  Integer.toHexString(System.identityHashCode(object)) + ")" );





  }
}
