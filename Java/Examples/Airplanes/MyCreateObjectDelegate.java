import com.cbe.delegate.*;
import com.cbe.*;

public class MyCreateObjectDelegate extends com.cbe.delegate.CreateObjectDelegate {

  private boolean        finished = false;
  private String         errorInfo;
  private com.cbe.Object returnObject;
  MyCreateObjectDelegate() {}

  /**
   * Called upon successful creation object.<br>
   * 
   */
  @Override
  synchronized public void onCreateObjectSuccess(com.cbe.Object object) {
    returnObject = object;
    // If delegate is reused, clear possible error state
    errorInfo = null;
    finished = true;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  synchronized public void onCreateObjectError(com.cbe.delegate.Error error,
                                               com.cbe.util.Context   context) {
    errorInfo = "Create error: code=" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    finished = true;
    notify();
  }

  synchronized public com.cbe.Object waitForRsp() {
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
    return returnObject; 
  }
}