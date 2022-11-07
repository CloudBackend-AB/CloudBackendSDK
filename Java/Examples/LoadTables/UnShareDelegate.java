import com.cbe.delegate.*;

public class UnShareDelegate extends com.cbe.delegate.UnShareDelegate {

  UnShareDelegate() {}
  private boolean finished = false;
  private String  returnMessage;
  private String  errorInfo;

  /**
   * Called upon successful UnShare.<br>
   * 
   */
  synchronized public void onUnShareSuccess(String message) {
    returnMessage = message;
    this.finished = true;
    // If delegate is reused, clear possibly error state
    errorInfo = null;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  synchronized public void onUnShareError(com.cbe.delegate.Error error, com.cbe.util.Context context) {
    errorInfo = "Login error: code=\"" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    this.finished = true;
    notify();
  }
  synchronized public String waitForRsp() {
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
    return returnMessage; 
  }

}