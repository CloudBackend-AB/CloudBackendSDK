import com.cbe.delegate.*;

public class MyUnShareDelegate extends com.cbe.delegate.UnShareDelegate {

  private boolean finished = false;
  private String  returnMessage;
  private String  errorInfo;
  MyUnShareDelegate() {}

  /**
   * Called upon successful unsharing.<br>
   * 
   */
  @Override
  synchronized public void onUnShareSuccess(String message) {
    returnMessage = message;
    // If delegate is reused, clear possible error state
    errorInfo = null;
    finished = true;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  synchronized public void onUnShareError(com.cbe.delegate.Error error,
                                          com.cbe.util.Context   context) {
    errorInfo = "Unshare error: code=" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    finished = true;
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
    // Reset finished flag, so current delegate instance can be reused
    finished = false; 
    if (errorInfo != null) {
      throw new RuntimeException(errorInfo);
    }
    return returnMessage; 
  }
}