import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.sql.ResultSet;
import java.util.Map.Entry;
import java.util.Properties;

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

public class GroupsMembers {
  static String programName = "GroupsMembers";

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


  public static void main(String[] args) {
    System.out.println(programName + " program start.");
    GroupsMembers inst = new GroupsMembers();
    com.cbe.CloudBackend cbobj = inst.myLogin("gh2");
    if (cbobj.account().userId()>0) {
      System.out.println("Authenticated as: " + cbobj.account().username() + "\t" + cbobj.account().firstName() + " " + cbobj.account().lastName());
      System.out.println("Version: " + cbobj.version());
    } else {
      System.out.println("Login failed.");
      cbobj.terminate();
      System.out.println(programName + " program stop.");
      return;
    }
    try
    {
      System.out.println("Group members and sub-containers:");
      com.cbe.GroupManager groupManager = cbobj.groupManager();
      Groups_Vec theGroups = listMyGroups(groupManager);
      for (final com.cbe.Group group : theGroups) {
        String groupName = group.name();
        System.out.println();
        System.out.println("=======================================");
        System.out.println("> " + group.id() + "\t" + groupName);
        System.out.println("=======================================");
        System.out.println("  ¤ Members");
        System.out.println("    -------");
        Members_Vec theMembers = listTheMembers(group);
        for (final com.cbe.Member member : theMembers) {
          System.out.println("    " + member.memberId() + "  " + member.name());
        }
        System.out.println();
        if (!groupName.equals("cbe")) {
          com.cbe.Container groupContainer = group.groupContainer();
          System.out.println("  ¤ Sub-containers in group container " + groupContainer.name());
          com.cbe.QueryResult resultSet =
              loadContainerContents(cbobj, groupContainer);
          if (resultSet != null) {
            System.out.println("    ------------------------------------------------------------");
            for (final com.cbe.Item item : resultSet.getItemsSnapshot()) {
              // item is container?
              if (item.type() == com.cbe.ItemType.Container) {
                System.out.println("    " + item.id() + " " + item.name());
              }
            }
        
          }
        }
        break;
      }
    } catch (RuntimeException e) {
      System.out.println("Caught error:");
      e.printStackTrace();
      System.out.println();
    }
    System.out.println();
    if (cbobj != null) {
      cbobj.terminate();
    }
    System.out.println(programName + " program end.");
  } // main()


  static com.cbe.QueryResult loadContainerContents(com.cbe.CloudBackend cloudBackend,
                                                   com.cbe.Container    container) {
    MyQueryDelegate queryDelegate = new MyQueryDelegate();
    com.cbe.Filter filter1 = new com.cbe.Filter();
    filter1.setDataType(com.cbe.ItemType.Container);
    cloudBackend.query(container.id(), filter1, queryDelegate);
    final com.cbe.QueryResult resultSet = queryDelegate.waitForRsp();
    queryDelegate.delete();
    return resultSet;
  }

  
  static com.cbe.QueryResult loadContainerObjects(com.cbe.Container container) {
    System.out.println("Getting objects in container \"" + container.name() + "\"");
    MyQueryDelegate queryDelegate = new MyQueryDelegate();
    com.cbe.Filter filter2 = new com.cbe.Filter();
    filter2.setDataType(com.cbe.ItemType.Object);
    container.query(filter2, queryDelegate);
    return queryDelegate.waitForRsp();
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
    MyListGroupsDelegate listGroupsDelegate = new MyListGroupsDelegate();
    theGroupManager.listGroups(listGroupsDelegate);
    return listGroupsDelegate.waitForRsp();
  }

  static com.std.Members_Vec listTheMembers(com.cbe.Group theGroup) {
    MyListMembersDelegate listMembersDelegate = new MyListMembersDelegate();
    theGroup.listMembers(listMembersDelegate);
    return listMembersDelegate.waitForRsp();
  }
}   // class GroupsMembers --------------------------------------------------------------------
