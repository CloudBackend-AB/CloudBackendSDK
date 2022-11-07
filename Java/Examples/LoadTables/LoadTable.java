import java.io.BufferedReader;  // read csv
import java.io.File;            // exists()
import java.io.FileInputStream;
import java.io.FileReader;      // read csv
import java.io.InputStream;
import java.io.IOException;     // read csv
import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.time.Instant;
import java.util.Properties; 
import java.util.Scanner;
import java.util.*;
import java.math.*;
import com.cbe.*;
import com.std.*;



public class LoadTable {

  public CloudBackend login(String credentials) {
    CloudBackend cloudBackend = null;
    try (InputStream input = new FileInputStream("../../resources/config.properties")) {
      String username ;
      String password;
      String tenant;
      Properties prop = new Properties();
      // load a properties file
      prop.load(input);
      // get the property value and set the login credentials
      username = prop.getProperty(credentials + ".username");
      password = prop.getProperty(credentials + ".password");
      tenant   = prop.getProperty(credentials + ".tenant");
      LogInDelegate delegate = new LogInDelegate();
      cloudBackend = CloudBackend.logIn(username, password, tenant, delegate);
      cloudBackend = delegate.waitForRsp();
      return cloudBackend;
      // If login happened then return the cloudbackend object.
    } catch (IOException ex) {
      throw new RuntimeException(ex);
    }
    finally {
      cloudBackend.terminate();
    }
  }
 
  public QueryResult queryR(CloudBackend obj, Filter filter, long containerId) {
    QueryDelegate delegate = new QueryDelegate();
    QueryChain QC=obj.query(containerId, filter, delegate);
    return delegate.waitForRsp();
  }

  public QueryResult query(Container container) {
    QueryDelegate delegate = new QueryDelegate();
    container.query(delegate);
    return delegate.waitForRsp();
  }

  public Container createContainer(Container container, String containerName) {
    CreateContainerDelegate delegate = new CreateContainerDelegate();
    container.createContainer(containerName, delegate);
    return delegate.waitForRsp();
  }


  public com.cbe.Object uploadBinary(Container container) throws IOException {
    // Try fetch from relative path
      UploadDelegate delegate = new UploadDelegate();
    try (InputStream input = new FileInputStream("../../resources/config.properties")) {
      byte[] data;
      String name = "";
      Properties prop = new Properties();
      // load a properties file
      prop.load(input);

      data = Files.readAllBytes(Paths.get(prop.getProperty("binaryPath")));
      name = prop.getProperty("binaryFileName");
      container.uploadBinary(name, data, delegate);
    } catch (IOException ex) {
      throw new RuntimeException(ex);
    }
    return delegate.waitForRsp();
  }

  public com.cbe.Object uploadObject(Container container) {
    String name = "";
    String path = "";
    try (InputStream input = new FileInputStream("../../resources/config.properties")) {
      Properties prop = new Properties();
      
      // load a properties file
      prop.load(input);
      name = prop.getProperty("filename");
      path = prop.getProperty("path");

    } catch (IOException ex) {
      ex.printStackTrace();
    }

    UploadDelegate delegate = new UploadDelegate();
    container.upload(name, path, delegate);

    return delegate.waitForRsp();
  }

  public com.cbe.Object createObject(Container container, String title, Obj_KV_Map metadata) {
    CreateObjectDelegate delegate = new CreateObjectDelegate();
    container.createObject(title, delegate, metadata);
    return delegate.waitForRsp();
  }

  public void removeContainer(Container container) {
    RemoveContainerDelegate delegate = new RemoveContainerDelegate();
    container.remove(delegate);
    delegate.waitForRsp();
    return;
  }

  public void setContainerAcl(Container container, com.std.Acl_Map permissionsMap) {
    AclDelegate delegate = new AclDelegate();
    container.setAcl(permissionsMap, delegate);
    delegate.waitForRsp();
    return;
  }

  public com.std.Acl_Map getContainerAcl(Container container) {
    AclDelegate delegate = new AclDelegate();
    container.getAcl(delegate);
    return delegate.waitForRsp();

  }

  public long share(long userGroupId, Container container) {
    ShareDelegate delegate = new ShareDelegate();
    container.share(userGroupId, "javaShare", delegate);
    return delegate.waitForRsp();
  }

  public QueryResult listAvailableShares(com.cbe.CloudBackend cloudbackend) {
    ListSharesDelegate delegate = new ListSharesDelegate();
    cloudbackend.shareManager().listAvailableShares(delegate);
    return delegate.waitForRsp();
  }

  public void unshareContainer(long shareId, Container container) {
    UnShareDelegate delegate = new UnShareDelegate();
    container.unShare(shareId, delegate);
    delegate.waitForRsp();
  }


  // program: loaddbtables
  // first parameter:  csv-file with input records
  // csv format: separator defined in csvSplitBy = ";"
  //
  // First line has to be the header
  // column 1: level1 = first container name
  // column 2: level2  = next subcontainer name
  // column 3: primarykey = object title name
  // column 4-: other column names = meta data key/values
  // E.g.:
  // Level1;Level2;Primarykey;Lat;Long;Spd
  //
  // Data is uploaded to the CloudBackend service
  // Database is per each login user id
  // Tables are organised as hierarchical tables
  // Top table at Level1 is referenced as /level1                   e.g. /unit1001
  // Subtable at next level is referenced /level1/level2            e.g. /unit1001/2022-01-19
  // The data records are stored in 
  // subtables and described as           /level1/level2/primarykey e.g. /unit1001/2022-01-19/1642609149
  // author: Anders Weister
  // company: CloudBackend AB
  // date: 2022-02-18, 2022-11-02
  
  public static void main(String[] argv) {
    System.out.println("LoadTable program start");

    int argvCount=argv.length;

    boolean foundHeader  = false;
    boolean foundItem    = false;
    boolean skipThisLine = false;
    boolean switchedDB   = false;
    int i = 0;
    int getItem = 0;
    int noSearchableKeyIndexFromColumn = 3;     // No Searchable Key/Value index after column
    String format = "%-38s%s%n";                // printf
    String expectHeaderColumn1 = "Level1";      // required as first column header
    String expectHeaderColumn2 = "Level2";      // required as second column header
    String expectHeaderColumn3 = "Primarykey";  // required as third column header

    String level1     = "";
    String level2     = "";
    String primaryKey = "";
    String tempString,tempString2;

    String term_RESET = "\033[0m";
    String term_RED   = "\033[31m";
    String Error_lbl  = term_RED + "Error: " + term_RESET;

    // for epoch timestamps
    Date date;
    DateFormat Dayformat;
    DateFormat DateTimeformat;
    long level1ContainerId, level2ContainerId;
    long timestamp,timestamp1,timestamp2;

    // for reading csv data file
    String csvLine = "";  
    String csvSplitBy = ";";  // separator
    String inputFileName="";
    String[] headerLabel={};
    List<String>labelList= new ArrayList<String>(); 

    // for adding column data to the record object
    boolean metaIndexed;
    String metaKey;
    String metaValue;
    Obj_KV_Map metadata1 = new Obj_KV_Map();
    Obj_VI_Pair metaDataValue;
    
    // CloudBackend Container filter
    Filter filterC = new Filter();
    filterC.setDataType(com.cbe.ItemType.Container);
    Filter filterCA = new Filter();
    filterCA.setAscending(true); // sorting name
    filterCA.setDataType(com.cbe.ItemType.Container);

    // CloudBackend Object filter
    Filter filterO = new Filter();
    filterO.setAscending(true); // sorting name
    filterO.setDataType(com.cbe.ItemType.Object);

    // for (i = 0; i < argv.length; i++) {
    //   System.out.println("argv[" + i + "]: " + argv[i]);
    // }

    if (argvCount>0) {
      inputFileName=argv[0];
    } else {
      System.out.println(Error_lbl + "input file name needed as parameter when calling this program.");
      System.exit(1);
    }
    File tempFile = new File(inputFileName);
    boolean fileExists = tempFile.exists();
    if (!fileExists) {
      System.out.print(Error_lbl + "file not found: ");
      System.out.println(inputFileName);
      System.exit(3);
    }

    timestamp1 = Instant.now().getEpochSecond();

    LoadTable inst = new LoadTable();
    CloudBackend cbobj = inst.login("cr1");  // referencing user credentials
    System.out.println("CloudBackend login as " + cbobj.account().username());
    Container level1Container=cbobj.account().rootContainer();
    Container level2Container=cbobj.account().rootContainer();
    QueryResult qR = inst.queryR(cbobj, filterC, cbobj.account().rootContainer().id());
    List<Item> items = qR.getItemsSnapshot();

    try   
    {  
      //parsing a CSV file into BufferedReader class constructor
      System.out.println("Using input file: " + inputFileName);
      BufferedReader br = new BufferedReader(new FileReader(inputFileName));  
      while ((csvLine = br.readLine()) != null)   //returns a Boolean value  
      {  
        String[] csvRead = csvLine.split(csvSplitBy);    // use semicolon as csv separator  
        if (!foundHeader) {
          foundHeader=true;
          skipThisLine=true;
          // System.out.println("Reading csv definition: " + csvRead.length + " columns");
          // System.out.print("[");
          i=0;
          for (String s : csvRead) {
            // System.out.print(i + "=" + s + " ");
            labelList.add(s);
            i++;
          }
          // System.out.println("]");

          headerLabel=labelList.toArray(headerLabel);
          // System.out.println(Arrays.toString(headerLabel));
        }

        level1=csvRead[0];                 // level1 name
        level2   =csvRead[1];              // level2 name
        primaryKey  =csvRead[2];           // primary key name
        for (i=3; i<csvRead.length; i++) {
          metaKey=headerLabel[i];          // Key
          metaValue=csvRead[i];            // Value
          if (i<noSearchableKeyIndexFromColumn) {
            metaIndexed=true;              // searchable index 
          } else {
            metaIndexed=false;             // no search index 
          }
          metaDataValue = new Obj_VI_Pair(metaValue, metaIndexed);
          metadata1.put(metaKey, metaDataValue);
        }

        if (skipThisLine) {
          System.out.print("Found header: ");
          System.out.println(Arrays.toString(headerLabel));
          if (!level1.equals(expectHeaderColumn1)) {
            System.out.println(Error_lbl + "First three column headers must contain: " + expectHeaderColumn1 + csvSplitBy + expectHeaderColumn2 + csvSplitBy + expectHeaderColumn3);
            System.out.println("Found: " + csvLine);
            System.out.println("'" + level1 + "'" + " vs " + "'" + expectHeaderColumn1 + "'");
            System.exit(4);
          }
          if (!level2.equals(expectHeaderColumn2)) {
            System.out.println(Error_lbl + "First three column headers must contain: " + expectHeaderColumn1 + csvSplitBy + expectHeaderColumn2 + csvSplitBy + expectHeaderColumn3);
            System.out.println("Found: " + csvLine);
            System.out.println("'" + level2 + "'" + " vs " + "'" + expectHeaderColumn2 + "'");
            System.exit(4);
          }
          if (!primaryKey.equals(expectHeaderColumn3)) {
            System.out.println(Error_lbl + "First three column headers must contain: " + expectHeaderColumn1 + csvSplitBy + expectHeaderColumn2 + csvSplitBy + expectHeaderColumn3);
            System.out.println("Found: " + csvLine);
            System.out.println("'" + primaryKey + "'" + " vs " + "'" + expectHeaderColumn3 + "'");
            System.exit(4);
          }
          skipThisLine=false;
        } else {
          if (!level1.equals(level1Container.name())) {
            // level1 container selection start
            // System.out.println("step: search for our level1 container");
            qR = inst.queryR(cbobj, filterC, cbobj.account().rootContainer().id());
            foundItem = false;
            i = 0;
            getItem = 0;
            if(qR == null) {
              System.out.println("Query failed null. ");
            } else {
              items = qR.getItemsSnapshot();
              for(Item item : items) {
                // System.out.println("Compare Item: " + item.name());
                if(item.name().equals(level1)) {
                  foundItem = true;
                  getItem = i;
                  level1ContainerId=item.id();
                  // System.out.println("reuse #" + getItem + " " + item.name() + "  (" + level1ContainerId + ")");
                  level1Container=com.cbe.CloudBackend.castContainer(item);
                }
              i++;
              }
            }
            if (foundItem) {
              // System.out.println("level1 container found: /" + level1Container.name() + "  (" + level1Container.id() + " Owner:" + level1Container.ownerId()+")");
            } else {  
              level1Container = inst.createContainer(cbobj.account().rootContainer(), level1);
              System.out.println("level1 container created: \t/" + level1Container.name() + "  (Id:" + level1Container.id() + " O:" + level1Container.ownerId()+")");
            }
            switchedDB = true;
            // level1 container selection done
          }

          if ((!level2.equals(level2Container.name())) || switchedDB) {
            // level2 container selection start
            qR = inst.queryR(cbobj, filterCA, level1Container.id());
            foundItem = false;
            i = 0;
            getItem = 0;
            if(qR == null) {
              System.out.println("Query failed null. ");
            } else {
              items = qR.getItemsSnapshot();
              for(Item item : items) {
                // System.out.println("Compare Item: " + item.name());
                if(item.name().equals(level2)) {
                  foundItem = true;
                  getItem = i;
                  level2ContainerId=item.id();
                  // System.out.println("reuse #" + getItem + " " + item.name() + "  (" + level2ContainerId + ")");
                  level2Container=com.cbe.CloudBackend.castContainer(item);
                }
                i++;
              }
            }
            if (foundItem) {
              // System.out.println("level2 subcontainer: /" + level1Container.name() + "/" + level2Container.name() + "  (" + level2Container.id() + " O:" + level2Container.ownerId()+")");
            } else {  
              level2Container = inst.createContainer(level1Container, level2);
              System.out.println("level2 subcontainer created: \t/" + level1Container.name() + "/" + level2Container.name() + "  (Id:" + level2Container.id() + " O:" + level2Container.ownerId()+")");
            }
            switchedDB = false;
            // level2 container selection done
          }
          // System.out.println("Creating record object");
          com.cbe.Object recordObject = inst.createObject(level2Container, primaryKey, metadata1);
          // System.out.println("Record object created: " + recordObject.name() + "  (Id:" + recordObject.id() + " P:" + recordObject.parentId()+")");
          System.out.println("/" + level1Container.name() + "/" + level2Container.name() + "/" + recordObject.name());
        }
      }  
    }   
    catch (IOException e)   
    {  
      e.printStackTrace();  
    }  
  
    timestamp2 = Instant.now().getEpochSecond();
    tempString = Long.toString(timestamp2);
    date = new Date(timestamp2 * 1000L);
    DateTimeformat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    DateTimeformat.setTimeZone(TimeZone.getTimeZone("Etc/UTC"));
    tempString2 = DateTimeformat.format(date);
    tempString = Long.toString(timestamp2-timestamp1);
    System.out.println("Run completed in: " + tempString + " s  @ " + tempString2 );
    cbobj.terminate();
    System.out.println("LoadTable program end");
  }  // main
}
