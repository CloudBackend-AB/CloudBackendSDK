import java.util.Scanner;
import com.cbe.*;

public class ItemDelegate extends ItemEventProtocol {

    ItemDelegate() {}

    @Override
    public void onObjectAdded(com.cbe.Object object) {
        System.out.println("Object Added: " + object.name());
        this.finished = true;
    }
    
    @Override
    public void onMetadataAdded(com.cbe.Object object) {
        System.out.println("Metadata Added: " + object.name());
        this.finished = true;
    }
    
    @Override
    public void onObjectMoved(com.cbe.Object object) {
        System.out.println("Object Moved: " + object.name());
        this.finished = true;
    }
    
    @Override
    public void onObjectRemoved(long objectId, String name) {
        System.out.println("Object Removed: " + name + "\t(" + objectId + ")");
        this.finished = true;
    }
    
    @Override
    public void onObjectRenamed(com.cbe.Object object) {
        System.out.println("Object Renamed: " + object.name());
        this.finished = true;
    }
    
    @Override
    public void onObjectUpdated(com.cbe.Object object) {
        System.out.println("Object Updated: " + object.name());
        this.finished = true;
    }
    
    @Override
    public void onStreamsLoaded(com.cbe.Object object) {
        System.out.println("Streams Loaded: " + object.name());
        this.finished = true;
    }    

    @Override
    public void onContainerAdded(Container container) {
        System.out.println("Container Added: " + container.name());
        this.finished = true;
    }
    
    @Override
    public void onContainerMoved(Container container) {
        System.out.println("Container Moved: " + container.name());
        this.finished = true;
    }
    
    @Override
    public void onContainerRemoved(long containerId, String name) {  
        System.out.println("Container Removed: " + name + "\t(" + containerId + ")" );
        this.finished = true;
    }
    
    @Override
    public void onContainerRenamed(Container container) {
        System.out.println("Container Renamed: " + container.name());
        this.finished = true;
    }

    @Override
    public void onQueryLoaded(QueryResult dir) {
        System.out.println("Query Loaded: " + dir.itemsLoaded());
        this.qR = dir;
        this.finished = true;
    }

    @Override
    public void onLoadError(Filter filter, long operation, long code, String reason, String message) {
        System.out.println("LoadError: code=" + code + ", reson=\"" + reason + "\", message=\"" + message + "\"");
        this.finished = true;
    }

    @Override
    public void onItemError(Item container, int type, long operation, long failedAtState, long code, String reason, String message) {
        System.out.println("ItemError: code=" + code + ", reson=\"" + reason + "\", message=\"" + message + "\"");
        this.finished = true;
    }
    public boolean finished = false;
    public QueryResult qR = null;

    // Container _container;
}