import com.cbe.*;
import com.cbe.delegate.*;

public class MyLogInDelegate extends com.cbe.delegate.LogInDelegate {

  MyLogInDelegate() {}
  private boolean      finished = false;
  private String       errorInfo;
  private com.cbe.CloudBackend cbObj;

  /**
   * Called upon successful log in.<br>
   * @param cloudBackend Instance of a CloudBackend holding the session.
   */
  @Override
  synchronized public void onLogInSuccess(com.cbe.CloudBackend cloudBackend) {
    cbObj = cloudBackend;
    // If delegate is reused, clear possible error state
    errorInfo = null;
    finished = true;
    notify();
  }

  /**
   * Called upon failed log in.<br>
   * @param error   Error information passed from %CloudBackend SDK.<br>
   * @param context Additional context information about the original service<br>
   *                call that has failed.
   */
  @Override
  synchronized public void onLogInError(com.cbe.delegate.Error error,
                                        com.cbe.util.Context   context) {
    errorInfo = "Login error: code=" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\""; 
    finished = true;
    notify();
  }
    
  synchronized public CloudBackend waitForRsp() {
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
    return cbObj; 
  }
}