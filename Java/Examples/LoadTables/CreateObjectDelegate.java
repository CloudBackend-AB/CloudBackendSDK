import com.cbe.delegate.*;

public class CreateObjectDelegate extends com.cbe.delegate.CreateObjectDelegate {

  CreateObjectDelegate() {}
  private boolean        finished = false;
  private String         errorInfo;
  private com.cbe.Object returnObject;



  /**
   * Called upon successful CreateObject.<br>
   * 
   */
  @Override
  synchronized public void onCreateObjectSuccess(com.cbe.Object object) {
    returnObject = object;
    this.finished = true;
    // If delegate is reused, clear possibly error state
    errorInfo = null;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  synchronized public void onCreateObjectError(com.cbe.delegate.Error error, com.cbe.util.Context context) {
    errorInfo = "Login error: code=\"" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    this.finished = true;
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
    if (errorInfo != null) {
      throw new RuntimeException(errorInfo);
    }
    return returnObject; 
  }

}