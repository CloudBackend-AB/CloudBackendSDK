import java.io.IOException;
import java.sql.ResultSet;
import java.util.Map.Entry;

import com.cbe.CloudBackend;
import com.cbe.Container;
import com.cbe.Filter;
import com.cbe.Group;
import com.cbe.GroupManager;
import com.cbe.Groups_Vec;
import com.cbe.Item;
import com.cbe.Member;
import com.cbe.Members_Vec;
import com.cbe.Obj_KV_Map;
import com.cbe.Obj_VI_Pair;
import com.cbe.Object;
import com.cbe.QueryResult;


class AccountEventDelegate extends com.cbe.AccountEventProtocol {
  boolean finished = false;
  @Override
  public synchronized void onLogin(long atState, CloudBackend cloudbackend) {
    // System.out.println("Login success");
    finished = true;
    notify();
  }

  @Override
  public synchronized void onError(long failedAtState, long code, String reason,
                                   String message) {
    System.out.println("Login failed:");
    System.out.println("failedAtState: " + failedAtState);
    System.out.println("         code: " + code);
    System.out.println("       reason: " + reason);
    System.out.println("      message: " + message);
    finished = true;
    notify();
  }  
} // class AccountEventDelegate -----------------------------------------------

class GroupEventDelegate extends com.cbe.GroupEventProtocol {
  private boolean finished = false;
  private Groups_Vec myGroupsVec = null;
  private Members_Vec myMembersVec = null;
  private long newPersistenceState = 0;

  @Override
  public synchronized void onListGroups​(Groups_Vec groups) {
    // System.out.println("on List Group");
    this.myGroupsVec = groups;
    finished = true;
    notify();
  }

  public synchronized com.cbe.Groups_Vec waitListGroups() {
    while (!finished) {
      try {
        wait();
      } catch (Exception e) {
        System.out.println(e);
      }
    }
    return myGroupsVec;
  }

  @Override
  public synchronized void onListMembers​(long newPersistenceState, Members_Vec members) {
    // System.out.println("on List Members");
    this.myMembersVec = members;
    this.newPersistenceState = newPersistenceState;
    finished = true;
    notify();
  }

  public synchronized com.cbe.Members_Vec waitListMembers() {
    while (!finished) {
      try {
        wait();
      } catch (Exception e) {
        System.out.println(e);
      }
    }
    return myMembersVec;
  }

  @Override
  public synchronized void onGroupError​(long operationId, long operation, 
                                        long failedAtState, long code, 
                                        java.lang.String reason, java.lang.String message) {
    System.out.println("Group failed:");
    System.out.println("  operationId: " + operationId);
    System.out.println("    operation: " + operation);
    System.out.println("failedAtState: " + failedAtState);
    System.out.println("         code: " + code);
    System.out.println("       reason: " + reason);
    System.out.println("      message: " + message);
    finished = true;
    notify();
  }  

} // class GroupEventDelegate -----------------------------------------------

class ItemEventDelegate extends com.cbe.ItemEventProtocol {
  private boolean           finished = false;
  private QueryResult       resultSet = null;
  private com.cbe.Container container = null;
  private com.cbe.Object    object = null;

  @Override
  public synchronized  void onQueryLoaded(QueryResult resultSet) {
    this.resultSet = resultSet;
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
  public synchronized void onContainerAdded(Container container) {
    this.container = container;
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

  @Override
  public synchronized void onObjectAdded(com.cbe.Object object) {
    this.object = object;
    finished = true;
    notify();
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

  @Override
  public synchronized  void onLoadError(Filter filter, long operation,
                                        long code, String reason,
                                        String message) {
    System.out.println("onLoadError(...,\"" + reason + "\", \"" + message + "\"");
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

} // class ItemEventDelegate --------------------------------------------------

public class MyGroups {
  static  final Inquiry inquiry = new Inquiry();

  public static void main(String[] args) {
    System.out.println("Program main start.");
    AccountEventDelegate accountDelegate = new AccountEventDelegate();
    // System.out.println("About to log in");
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
      accountDelegate.delete();
      System.out.println("Check the credentials.");
      try {
        Thread.sleep(100); // tidy gc clean up
      } catch (InterruptedException e) {
        e.printStackTrace();
      }
      System.out.println("Program end after failing to login.");
      return;
    }
    // step 4
    
    System.out.println("List my groups with their respective members:");
    GroupManager groupManager = cloudBackend.groupManager();
    Groups_Vec theGroups = listMyGroups(groupManager);
    for (final com.cbe.Group group : theGroups) {
      String groupName = group.name();
      if (!groupName.equals("cbe")) {
        System.out.println();
        System.out.println(groupName + "\t(" + group.id() + ")");
        System.out.println("-------------------------------------");
        Members_Vec theMembers = listMyMembers(group);
        for (final com.cbe.Member member : theMembers) {
          System.out.println("  " + member.name());
        }
        System.out.println();
        QueryResult resultSet =
            loadContainerContents(cloudBackend, group.groupContainer());
        if (resultSet != null) {
          printContainerContents(resultSet);
        }
    
      }
    }
    System.out.println();


    System.out.println("Terminating SDK " + cloudBackend.version());
    Runtime.getRuntime().runFinalization();
    try {
      Thread.sleep(100); // tidy gc clean up
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
    System.out.println("Program main end.");
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
    itemDelegate.delete();
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

  static com.cbe.Groups_Vec listMyGroups(com.cbe.GroupManager theGroupManager) {
    GroupEventDelegate groupDelegate = new GroupEventDelegate();
    theGroupManager.listGroups(groupDelegate);
    return groupDelegate.waitListGroups();
  }

  static com.cbe.Members_Vec listMyMembers(com.cbe.Group theGroup) {
    GroupEventDelegate groupDelegate = new GroupEventDelegate();
    theGroup.listMembers(groupDelegate);
    return groupDelegate.waitListMembers();
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
}  // MyGroups --------------------------------------------------------------------
