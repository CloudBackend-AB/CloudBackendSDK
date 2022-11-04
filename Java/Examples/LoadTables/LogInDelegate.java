import com.cbe.*;
import com.cbe.delegate.*;


public class LogInDelegate extends com.cbe.delegate.LogInDelegate {


  LogInDelegate() {}
  private boolean              finished = false;
  private String               errorInfo;
  private com.cbe.CloudBackend cbObj;

      /**
   * Called upon successful log in.<br>
   * @param cloudBackend Instance of a CloudBackend holding the session.
   */
  @Override
  synchronized public void onLogInSuccess(com.cbe.CloudBackend cloudBackend) {
    cbObj = cloudBackend;
    this.finished = true;
    // If delegate is reused, clear possibly error state
    errorInfo = null;
    notify();
  }

  /**
   * Called upon failed log in.<br>
   * @param error   Error information passed from %CloudBackend SDK.<br>
   * @param context Additional context information about the original service<br>
   *                call that has failed.
   */
  @Override
  synchronized public void onLogInError(com.cbe.delegate.Error error, com.cbe.util.Context context) {
    errorInfo = "Login error: code=\"" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\""; 
    this.finished = true;
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
    if (errorInfo != null) {
      throw new RuntimeException(errorInfo);
    }
    return cbObj; 
  }
}