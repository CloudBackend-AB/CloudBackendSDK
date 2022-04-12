package cbe.tutorial;

import java.io.IOException;
import java.util.Map.Entry;

import com.cbe.CloudBackend;
import com.cbe.Container;
import com.cbe.Filter;
import com.cbe.Item;
import com.cbe.Obj_KV_Map;
import com.cbe.Obj_VI_Pair;
import com.cbe.Object;
import com.cbe.QueryResult;

class AccountEventDelegate extends com.cbe.AccountEventProtocol {
  boolean finished = false;
  @Override
  public synchronized void onLogin(long atState, CloudBackend cloudbackend) {
    System.out.println("Login success");
    finished = true;
    notify();
  }

  @Override
  public synchronized void onError(long failedAtState, long code, String reason,
                                   String message) {
    System.out.println("Login failed");
    finished = true;
    notify();
  }  
} // class AccountEventDelegate

class ItemEventDelegate extends com.cbe.ItemEventProtocol {
  private boolean           finished = false;
  private QueryResult       resultSet = null;
  private com.cbe.Container container = null;
  private com.cbe.Object    object = null;

  @Override
  public synchronized  void onLoadError(Filter filter, long operation,
                                        long code, String reason,
                                        String message) {
    System.out.println("onLoadError(...,\"" + reason + "\", \"" + message + "\"");
    finished = true;
    notify();
  }

  @Override
  public synchronized  void onQueryLoaded(QueryResult resultSet) {
    this.resultSet = resultSet;
    finished = true;
    notify();
  }
  
  @Override
  public synchronized void onItemError(Item container, int type,
                                       long operation, long failedAtState,
                                       long code, String reason,
                                       String message) {
    System.out.println("onItemError(...,\"" + reason + "\", \"" + message + "\"");
    finished = true;
    notify();
  }

  @Override
  public synchronized void onContainerAdded(Container container) {
    this.container = container;
    finished = true;
    notify();
  }

  public synchronized  QueryResult waitResult() {
    while (!finished) {
      try {
        wait();
      } catch (Exception e) {
        System.out.println(e);
      }
    }
    return resultSet;
  }
  
  @Override
  public synchronized void onObjectAdded(com.cbe.Object object) {
    this.object = object;
    finished = true;
    notify();
  }

  public synchronized com.cbe.Container waitContainerAdded() {
    while (!finished) {
      try {
        wait();
      } catch (Exception e) {
        System.out.println(e);
      }
    }
    return container;
  }
  public synchronized com.cbe.Object waitObjectAdded() {
    while (!finished) {
      try {
        wait();
      } catch (Exception e) {
        System.out.println(e);
      }
    }
    return object;
  }
} // class ItemEventDelegate

public class Main {
  static  final Inquiry inquiry = new Inquiry();

  public static void main(String[] args) {
    System.out.println("Program start");
    AccountEventDelegate accountDelegate = new AccountEventDelegate();
    System.out.println("About to log in");
    CloudBackend cloudBackend = CloudBackend.logIn("gitHubTester1",
                                          "gitHubTester1password",
                                          "cbe_girhubtesters",
                                          accountDelegate);
    synchronized (accountDelegate) {
      while (!accountDelegate.finished) {
        try {
          accountDelegate.wait();
        } catch (Exception e) {
          System.out.println(e);
        }
      }
    }
    if (cloudBackend.account().userId()>0) {
      System.out.println("Login: " + cloudBackend.account().username() + " " +
                         cloudBackend.account().firstName() + " " +
                         cloudBackend.account().lastName());
    } else {
      System.out.println("Check the credentials.");
      System.out.println("Terminating SDK " + cloudBackend.version());
      System.out.println("Exit program.");
      return;
    }
    // step 1
    ItemEventDelegate itemDelegate = new ItemEventDelegate();
    cloudBackend.query(cloudBackend.account().rootContainer().id(),
                       itemDelegate);
    QueryResult resultSet =
        loadContainerContents(cloudBackend,
                              cloudBackend.account().rootContainer());
    if (resultSet != null) {
      printContainerContents(resultSet);
    }
    
    com.cbe.Container container;
    if (inquiry.inquireBool("Do you want to create a new container", false)) {
      final String name = inquiry.inquireString("Set name for new container");
      container = createContainer(name, cloudBackend.account().rootContainer());
    } else {
      container =
        selectContainerFromResultSet(
          resultSet,
          "In which Container do you want to create the new object",
          cloudBackend);
    }
    createObject(container);
    final com.cbe.QueryResult containerObjects = loadContainerObjects(container);
    printObjects(containerObjects);
    
    System.out.println("Terminating SDK " + cloudBackend.version());
    System.out.println("Program end.");
  } // main()
  
  static QueryResult loadContainerContents(CloudBackend cloudBackend,
                                           com.cbe.Container container) {
    System.out.println("Getting sub-containers in container \"" +
                        container.name() + "\"");
    ItemEventDelegate itemDelegate = new ItemEventDelegate();
    com.cbe.Filter filter1 = new com.cbe.Filter();
    filter1.setDataType(8 /* i.e., container */);
    cloudBackend.query(container.id(), filter1, itemDelegate);
    final QueryResult resultSet = itemDelegate.waitResult();
    return resultSet;
  }
  
  static void printContainerContents(QueryResult resultSet) {
    for (final com.cbe.Item item : resultSet.getItemsSnapshot()) {
      // item is container?
      if (item.type() == 8) {
        System.out.println("  " + item.name());
      }
    }
  }
  
  static com.cbe.Container createContainer(String             name,
                                           com.cbe.Container  parentContainer) {
    ItemEventDelegate itemDelegate = new ItemEventDelegate();
    parentContainer.create(name, itemDelegate);
    return itemDelegate.waitContainerAdded();
  }
  
  static com.cbe.Container
  selectContainerFromResultSet(com.cbe.QueryResult  resultSet,
                               String               prompt,
                               com.cbe.CloudBackend cloudBackend) {
    System.out.println("Select Container");
    final com.cbe.Items_Vec items = resultSet.getItemsSnapshot();
    while (true) {
      final String containerName = inquiry.inquireString(prompt);
      for (final com.cbe.Item item : items) {
        if(item.name().equals(containerName)) {
          return com.cbe.CloudBackend.castContainer(item);
        }
      }
      System.out.println("Error: the container you asked for, \"" +
                         containerName +
                         "\", was not found; following are the options:");
      printContainerContents(resultSet);
    }
    
  }
  
  static com.cbe.Object createObject(com.cbe.Container inContainer) {
    System.out.println("Create Object");
    final String name = inquiry.inquireString("Set name for Object");
    final int numOftags =
        inquiry.inquireInt("Set the number of Key/Value pairs you want");
    final com.cbe.Obj_KV_Map keyValues = new com.cbe.Obj_KV_Map();
    for(int i = 1; i <= numOftags; i++) {
      final String tag = inquiry.inquireString("Name of Key #" + i);
      final String value =
          inquiry.inquireString("Value for #" + i + " '" + tag + "'");
      final boolean indexed =
          inquiry.inquireBool("Make KeyValue pair #" + i +
                              " indexed or not (y indexed, n not indexed",
                              true /* defaultVal */);
      keyValues.put(tag, new com.cbe.Obj_VI_Pair(value, indexed));
    }
    ItemEventDelegate itemDelegate = new ItemEventDelegate();
    if (numOftags > 0) {
      inContainer.createObject(name, itemDelegate, keyValues);
    } else {
      inContainer.createObject(name, itemDelegate);
    }
    return itemDelegate.waitObjectAdded();
  }
  
  static QueryResult loadContainerObjects(com.cbe.Container container) {
    System.out.println("Getting objects in container \"" + container.name() + "\"");
    ItemEventDelegate itemDelegate = new ItemEventDelegate();
    com.cbe.Filter filter2 = new com.cbe.Filter();
    filter2.setDataType(4 /* i.e., object */);
    container.query(filter2, itemDelegate);
    return itemDelegate.waitResult();
  }

  static void printObjects(com.cbe.QueryResult resultSet) {
    com.cbe.Object tempObject = null; // nullptr;
    System.out.println("Printing Objects from query result:");
    for (final com.cbe.Item item : resultSet.getItemsSnapshot()) {
      if(item.type() == 4 /* object */) {
        System.out.println(item.name());
        tempObject = com.cbe.CloudBackend.castObject(item);
        final com.cbe.Obj_KV_Map keyValues = tempObject.keyValues();
        if(!keyValues.isEmpty()) {
          for (final java.util.Map.Entry<String, Obj_VI_Pair> entry :
                 keyValues.entrySet()) {
            final String key      = entry.getKey();
            final Obj_VI_Pair sdkValue = entry.getValue();
            final String  value  = sdkValue.getFirst();
            final boolean  indexed  = sdkValue.getSecond();
            System.out.print(String.format("%15s = '%s'" , key, value ));
            System.out.println((indexed? "\t(indexed)" : ""));
          }
        }  // if(!keyValues.isEmpty())
      }   // if(item.type() == Object)
    }
    if(tempObject == null) {
      System.out.println("Sorry, no objects found in the Container!");
    }
  }

  
  
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
  }
}
