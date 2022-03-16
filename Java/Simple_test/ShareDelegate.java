import java.util.Scanner;
import com.cbe.*;
import java.util.*;

public class ShareDelegate extends ShareEventProtocol {

    public long shareId = 0;
    public boolean finished = false;
    public QueryResult _qr = new QueryResult();
    public AbstractMap<Long, Integer> permissionsMap = new HashMap<Long, Integer>();

    ShareDelegate() {}
    
    @Override
    public void onListAvailableShares(QueryResult result) {
        System.out.println("onListAvailableShares");
        this._qr = result;
        this.finished = true;
    }
    
    @Override
    public void onListMyShares(QueryResult qResult) {
        this.finished = true;
    }
    
    @Override
    public void onContainerShared(long shareId) {
        this.finished = true;
        this.shareId = shareId;
    }
    
    @Override
    public void onContainerUnShared(String message) {
        System.out.println("onContainerUnShared message: " + message);
        this.finished = true;
    }
    
    @Override
    public void onObjectShared(long shareId) {
        this.finished = true;
    }
    
    @Override
    public void onObjectUnShared(String message) {
        this.finished = true;
    }
    
    @Override
    public void onShareError(int type, long operation, long code, String reason, String message) {
        System.out.println("ShareError: code=" + code + ", reson=\"" + reason + "\", message=\"" + message + "\"");
        this.finished = true;
    }
    
    @Override
    public void onACLError(int type, long operation, long code, String reason, String message) {
        this.finished = true;
    }

    @Override
    public void onContainerACLAdded(ACL_Map ACLs) {
        this.finished = true;
    }
    
    @Override
    public void onContainerAclLoaded(UserId_Vec userIds, ACL_Map ACLMap) {
        this.finished = true;
        // this.permissionsMap.put(ACLMap);
        this.permissionsMap = ACLMap;
    }
    
    
}