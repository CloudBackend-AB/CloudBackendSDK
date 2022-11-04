import java.io.IOException;
import java.sql.ResultSet;
import java.util.Map.Entry;

import com.cbe.CloudBackend;
import com.cbe.Container;
import com.cbe.Filter;
import com.cbe.Group;
import com.cbe.GroupManager;
import com.cbe.Item;
import com.cbe.Member;
import com.cbe.Object;
import com.cbe.QueryResult;

import com.std.Groups_Vec;
import com.std.Members_Vec;
import com.std.Obj_KV_Map;
import com.std.Obj_VI_Pair;


class LogInDelegate extends com.cbe.delegate.LogInDelegate {
  public boolean       finished;     /* false */

  @Override
  public synchronized void onLogInSuccess(CloudBackend cloudBackend) {
    finished = true;
    notify();
  }

  @Override
  public synchronized void onLogInError(com.cbe.delegate.Error error, 
                                          com.cbe.util.Context context) {

    System.out.println("Login failed:");
    System.out.println("         code: " + error.getErrorCode());
    System.out.println("       reason: " + error.getReason());
    System.out.println("      message: " + error.getMessage()); 

    finished = true;
    notify();
  }
  
} // class LogInDelegate

class ListMyGroupsDelegate extends com.cbe.delegate.ListGroupsDelegate {
  private boolean finished = false;
  private Groups_Vec myGroupsVec = null;
    /**
   * Called upon successful listGroup<br>
   * @param groups Ref to vector of cbe::Group holding the joined groups.
   */
  @Override
  public synchronized void onListGroupsSuccess(com.std.Groups_Vec groups) {
    this.myGroupsVec = groups;
    finished = true;
    notify();
  }

 public synchronized com.std.Groups_Vec waitListGroups() {
    while (!finished) {
      try {
        wait();
      } catch (InterruptedException e) {
        System.out.println(e);
      }
    }
    return myGroupsVec;
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  public synchronized void onListGroupsError(com.cbe.delegate.Error error, com.cbe.util.Context context) {
    System.out.println("ListMyGroups failed:");
    System.out.println("         code: " + error.getErrorCode());
    System.out.println("       reason: " + error.getReason());
    System.out.println("      message: " + error.getMessage());
    finished = true;
    notify();
  }

} // class ListMyGroupsDelegate

class ListMyMembersDelegate extends com.cbe.delegate.ListMembersDelegate {
  private boolean finished = false;
  private Members_Vec myMembersVec = null;
  /**
   * Called upon successful ListMembers.<br>
   * @param members Vector of cbe::Member holding the members<br>
   * of a group.
   */
  @Override
  public synchronized void onListMembersSuccess(com.std.Members_Vec members) {
    this.myMembersVec = members;
    finished = true;
    notify();
  }

  public synchronized com.std.Members_Vec waitListMembers() {
    while (!finished) {
      try {
        wait();
      } catch (InterruptedException e) {
        System.out.println(e);
      }
    }
    return myMembersVec;
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  public synchronized void onListMembersError(com.cbe.delegate.Error error, com.cbe.util.Context context) {
    System.out.println("ListMyMembers failed:");
    System.out.println("         code: " + error.getErrorCode());
    System.out.println("       reason: " + error.getReason());
    System.out.println("      message: " + error.getMessage());
    finished = true;
    notify();
  }
} // ListMyMembersDelegate


class MyQueryDelegate extends com.cbe.delegate.QueryDelegate {
  private QueryResult queryResult; /* null */
  private String      errorInfo;
  private boolean     finished;  /* false */
  
  @Override
  public synchronized void onQuerySuccess(com.cbe.QueryResult qR) {
    queryResult = qR;
    finished = true;
    notify();
  }

  public synchronized  QueryResult waitResult() {
    while (!finished) {
      try {
        wait();
      } catch (InterruptedException e) {
        System.out.println(e);
      }
    }
    return queryResult;
  }

  @Override
  public synchronized void onQueryError(com.cbe.delegate.QueryError error, 
                                              com.cbe.util.Context context) {
    errorInfo = "LoadError: code=" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    finished = true;
    notify();
  }
} // class MyQueryDelegate


class CreateContainerDelegate 
                          extends com.cbe.delegate.CreateContainerDelegate {
  private Container   container;
  private String      errorInfo;
  private boolean     finished;  /* false */
  
  @Override
  public synchronized void onCreateContainerSuccess(Container _container) {
    container = _container;
    finished = true;
    notify();
  }

  public synchronized com.cbe.Container waitContainerAdded() {
    while (!finished) {
      try {
        wait();
      } catch (InterruptedException e) {
        System.out.println(e);
      }
    }
    return container;
  }

  @Override
  public synchronized void onCreateContainerError(com.cbe.delegate.Error error, 
                                                  com.cbe.util.Context context) {
    errorInfo = "CreateContainerError: code=" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    finished = true;
    notify();
  }
} // CreateContainerDelegate
  
class CreateObjectDelegate extends com.cbe.delegate.CreateObjectDelegate {
  private com.cbe.Object  object;     /* null */
  private String          errorInfo;
  private boolean         finished;  /* false */
  
  @Override
  public synchronized void onCreateObjectSuccess(com.cbe.Object _object) {
    object = _object;
    finished = true;
    notify();
  }

  public synchronized com.cbe.Object waitObjectAdded() {
    while (!finished) {
      try {
        wait();
      } catch (InterruptedException e) {
        System.out.println(e);
      }
    }
    return object;
  }

  @Override
  public synchronized void onCreateObjectError(com.cbe.delegate.Error error, 
                                                com.cbe.util.Context context) {
    errorInfo = "CreateObjectError: code=" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    finished = true;
    notify();
  }
}  // CreateObjectDelegate

public class MyGroups {
  static  final Inquiry inquiry = new Inquiry();

  public static void main(String[] args) {
    System.out.println("Program MyGroups start.");
    LogInDelegate loginDelegate = new LogInDelegate();
    // System.out.println("About to log in");
    CloudBackend cloudBackend = CloudBackend.logIn("gitHubTester1",
                                                   "gitHubTester1password",
                                                   "cbe_girhubtesters",
                                                   loginDelegate);
    synchronized (loginDelegate) {
      while (!loginDelegate.finished) {
        try {
          loginDelegate.wait();
        } catch (InterruptedException e) {
          System.out.println(e);
        }
      }
    }
    if (cloudBackend.account().userId()>0) {
      System.out.println("Login: " + 
                         cloudBackend.account().username()  + " " +
                         cloudBackend.account().firstName() + " " +
                         cloudBackend.account().lastName());
    } else {
      System.out.println("Program stops after failing to login.");
      cloudBackend.terminate();
      return;
    }
    
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
    System.out.println("SDK " + cloudBackend.version());
    cloudBackend.terminate();
    System.out.println("Program MyGroups end.");
  } // main()
  
  static QueryResult loadContainerContents(CloudBackend cloudBackend,
                                           com.cbe.Container container) {
    System.out.println("Getting sub-containers in container \"" +
                        container.name() + "\"");
    MyQueryDelegate queryDelegate = new MyQueryDelegate();
    com.cbe.Filter filter1 = new com.cbe.Filter();
    filter1.setDataType(com.cbe.ItemType.Container);
    cloudBackend.query(container.id(), filter1, queryDelegate);
    final QueryResult resultSet = queryDelegate.waitResult();
    queryDelegate.delete();
    return resultSet;
  }
  
  static void printContainerContents(QueryResult resultSet) {
    for (final com.cbe.Item item : resultSet.getItemsSnapshot()) {
      // item is container?
      if (item.type() == com.cbe.ItemType.Container) {
        System.out.println("  " + item.name());
      }
    }
  }
  
  static com.cbe.Container createContainer(String             name,
                                           com.cbe.Container  parentContainer) {
    CreateContainerDelegate createContainerDelegate = new CreateContainerDelegate();
    parentContainer.createContainer(name, createContainerDelegate);
    return createContainerDelegate.waitContainerAdded();
  }
  
  static com.cbe.Container
  selectContainerFromResultSet(com.cbe.QueryResult  resultSet,
                               String               prompt,
                               com.cbe.CloudBackend cloudBackend) {
    System.out.println("Select Container");
    final com.std.Items_Vec items = resultSet.getItemsSnapshot();
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
    final com.std.Obj_KV_Map keyValues = new com.std.Obj_KV_Map();
    for(int i = 1; i <= numOftags; i++) {
      final String tag = inquiry.inquireString("Name of Key #" + i);
      final String value =
          inquiry.inquireString("Value for #" + i + " '" + tag + "'");
      final boolean indexed =
          inquiry.inquireBool("Make KeyValue pair #" + i +
                              " indexed or not (y indexed, n not indexed",
                              true /* defaultVal */);
      keyValues.put(tag, new com.std.Obj_VI_Pair(value, indexed));
    }
    CreateObjectDelegate createObjectDelegate = new CreateObjectDelegate();
    if (numOftags > 0) {
      inContainer.createObject(name, createObjectDelegate, keyValues);
    } else {
      inContainer.createObject(name, createObjectDelegate);
    }
    return createObjectDelegate.waitObjectAdded();
  }
  
  static QueryResult loadContainerObjects(com.cbe.Container container) {
    System.out.println("Getting objects in container \"" + container.name() + "\"");
    MyQueryDelegate queryDelegate = new MyQueryDelegate();
    com.cbe.Filter filter2 = new com.cbe.Filter();
    filter2.setDataType(com.cbe.ItemType.Object);
    container.query(filter2, queryDelegate);
    return queryDelegate.waitResult();
  }

  static void printObjects(com.cbe.QueryResult resultSet) {
    com.cbe.Object tempObject = null; // nullptr;
    System.out.println("Printing Objects from query result:");
    for (final com.cbe.Item item : resultSet.getItemsSnapshot()) {
      if(item.type() == com.cbe.ItemType.Object) {
        System.out.println(item.name());
        tempObject = com.cbe.CloudBackend.castObject(item);
        final com.std.Obj_KV_Map keyValues = tempObject.keyValues();
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

  static com.std.Groups_Vec listMyGroups(com.cbe.GroupManager theGroupManager) {
    ListMyGroupsDelegate listGroupsDelegate = new ListMyGroupsDelegate();
    theGroupManager.listGroups(listGroupsDelegate);
    return listGroupsDelegate.waitListGroups();
  }

  static com.std.Members_Vec listMyMembers(com.cbe.Group theGroup) {
    ListMyMembersDelegate listMembersDelegate = new ListMyMembersDelegate();
    theGroup.listMembers(listMembersDelegate);
    return listMembersDelegate.waitListMembers();
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