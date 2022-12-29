import com.cbe.CloudBackend;
import com.cbe.Container;
import com.cbe.Filter;
import com.cbe.Item;
import com.cbe.Object;
import com.cbe.QueryResult;

import com.std.Obj_KV_Map;
import com.std.Obj_VI_Pair;

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


public class Tutorial {
  static String programName = "Tutorial";
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

  public com.cbe.Object createObject(com.cbe.Container container, String title, Obj_KV_Map metadata) {
    MyCreateObjectDelegate delegate = new MyCreateObjectDelegate();
    com.cbe.Object tempObj = null;
    try
    {
      System.out.println("saving " + title);
      tempObj = container.createObject(title, delegate, metadata);
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
  }  //  Inquiry
  
  //////////////////////////////////////////////////////////////////////////////
  
  public static void main(String[] argv) {
    System.out.println(programName + " program start.");
    Tutorial inst = new Tutorial();

    com.cbe.CloudBackend cbobj = null;
    List<Item> items;
    try
    {
      // Exercise 1 here


      // Exercise 2 here


      // Exercise 3 here

    }
    catch (RuntimeException e) {
      System.out.println("Caught runtime error:");
      e.printStackTrace();
      System.out.println();
    }

    System.out.println("Disconnecting.");
    cbobj.terminate();
    System.out.println(programName + " program end.");
  }  // main
}   // class Tutorial
