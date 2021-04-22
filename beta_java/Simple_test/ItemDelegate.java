import java.util.Scanner;
import com.cbe.*;

public class ItemDelegate extends ItemEventProtocol {

    @Override
    public void onObjectAdded(com.cbe.Object object) {}
    
    @Override
    public void onMetadataAdded(com.cbe.Object object) {}
    
    @Override
    public void onObjectMoved(com.cbe.Object object) {}
    
    @Override
    public void onObjectRemoved(java.math.BigInteger objectId, String name) {}
    
    @Override
    public void onObjectRenamed(com.cbe.Object object) {
        System.out.println("You have reached onObjectUpdated in ItemDelegate Java");
        this._listener = false;
    }
    
    @Override
    public void onObjectUpdated(com.cbe.Object object) {
        System.out.println("You have reached onObjectUpdated in ItemDelegate Java");
        this.finished = true;
    }
    
    @Override
    public void onContainerAdded(Container container) {
        System.out.println("You have reached onContainerAdded in ItemDelegate Java");
        this.finished = true;
    }
    
    @Override
    public void onContainerMoved(Container container) {}
    
    @Override
    public void onContainerRemoved(java.math.BigInteger containerId, String name) {  
        System.out.println("You have reached onContainerRemoved in ItemDelegate Java.");
        System.out.println("Container removed \tname: " + name);
        this.finished = true;
    }
    
    @Override
    public void onContainerRenamed(Container container) {}

    @Override
    public void onQueryLoaded(QueryResult dir) {
        System.out.println("You have reached onQueryLoaded in ItemDelegate Java");
        this.qR = dir;
        this.finished = true;
    }

    @Override
    public void onLoadError(Filter filter, long operation, long code, String reason, String message) {
        System.out.println("Error: query did not succeed but error message returned to JAVA " + reason + " " + message);
        this.finished = true;
    }

    @Override
    public void onItemError(Item container, int type, long operation, long failedAtState, long code, String reason, String message) {
        System.out.println("OnItemError?");
        this.finished = true;
    }
    public boolean finished = false;
    public boolean _listener = true;
    public QueryResult qR = null;

    // Container _container;
}