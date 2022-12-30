import com.std.*;
import com.cbe.*;
import com.cbe.delegate.*;

public class MyAclDelegate extends com.cbe.delegate.AclDelegate {

  private boolean  finished = false;
  private String   errorInfo;
  private Acl_Map  permissionsMap = new Acl_Map();
  MyAclDelegate() {}

  /**
   * Called upon successful ACL operation.<br>
   * 
   */
  @Override
  synchronized public void onAclSuccess(com.std.Acl_Map aclMap) {
    permissionsMap = aclMap;
    // If delegate is reused, clear possible error state
    errorInfo = null;
    finished = true;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  synchronized public void onAclError(com.cbe.delegate.Error error,
                                      com.cbe.util.Context   context) {
    errorInfo = "ACL error: code=" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    finished = true;
    notify();
  }

  synchronized public com.std.Acl_Map waitForRsp() {
    while (!finished) {
      try {
        wait();
      } catch (InterruptedException e) {
        e.printStackTrace();
      }
    }
    // Reset finished flag, so current delegate instance can be reused
    finished = false; 
    if (errorInfo != null) {
      throw new RuntimeException(errorInfo);
    }
    return permissionsMap; 
  }
}