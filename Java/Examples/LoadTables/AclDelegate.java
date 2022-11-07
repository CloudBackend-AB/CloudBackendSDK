import com.std.*;
import com.cbe.*;
import com.cbe.delegate.*;

public class AclDelegate extends com.cbe.delegate.AclDelegate {

  AclDelegate() {}
  private boolean  finished = false;
  private String   errorInfo;
  private Acl_Map  permissionsMap = new Acl_Map();

  /**
   * Called upon successful Acl.<br>
   * 
   */
  @Override
  synchronized public void onAclSuccess(com.std.Acl_Map aclMap) {
    permissionsMap = aclMap;
    this.finished = true;
    // If delegate is reused, clear possibly error state
    errorInfo = null;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  synchronized public void onAclError(com.cbe.delegate.Error error, com.cbe.util.Context context) {
    errorInfo = "Login error: code=\"" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    this.finished = true;
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
    if (errorInfo != null) {
      throw new RuntimeException(errorInfo);
    }
    return permissionsMap; 
  }
}