import java.util.Scanner;
import com.cbe.*;

public class ItemDelegate extends ItemEventProtocol {

    ItemDelegate() {}

    @Override
    public void onObjectAdded(com.cbe.Object object) {
        // System.out.println("Object added";
        this.finished = true;
    }
    
    @Override
    public void onMetadataAdded(com.cbe.Object object) {}
    
    @Override
    public void onObjectMoved(com.cbe.Object object) {}
    
    @Override
    public void onObjectRemoved(long objectId, String name) {}
    
    @Override
    public void onObjectRenamed(com.cbe.Object object) {
        System.out.println("You've reached onObjectRenamed in ItemDelegate Java");
        this._listener = false;
    }
    
    @Override
    public void onObjectUpdated(com.cbe.Object object) {
        System.out.println("You've reached onObjectUpdated in ItemDelegate Java");
        this.finished = true;
    }
    
    @Override
    public void onStreamsLoaded(com.cbe.Object object) {}    

    @Override
    public void onContainerAdded(Container container) {
        // System.out.println("Container added");
        this.finished = true;
    }
    
    @Override
    public void onContainerMoved(Container container) {}
    
    @Override
    public void onContainerRemoved(long containerId, String name) {  
        System.out.println("Container removed: " + name + "  (" + containerId + ")" );
        this.finished = true;
    }
    
    @Override
    public void onContainerRenamed(Container container) {}

    @Override
    public void onQueryLoaded(QueryResult dir) {
        // System.out.println("Query loaded");
        this.qR = dir;
        this.finished = true;
    }

    @Override
    public void onLoadError(Filter filter, long operation, long code, String reason, String message) {
        System.out.println("onLoadError: " + "reason:" + reason + " msg:" + message);
        this.finished = true;
    }

    @Override
    public void onItemError(Item container, int type, long operation, long failedAtState, long code, String reason, String message) {
        System.out.println("onItemError: " + "reason:" + reason + " msg:" + message);
        this.finished = true;
    }
    public boolean finished = false;
    public boolean _listener = true;
    public QueryResult qR = null;

    // Container _container;
}