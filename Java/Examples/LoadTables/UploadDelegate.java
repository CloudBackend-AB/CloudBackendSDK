import com.cbe.delegate.*;
import com.cbe.*;

public class UploadDelegate extends com.cbe.delegate.UploadDelegate {

  UploadDelegate() {}
  public boolean         finished = false;
  private com.cbe.Object returnObject;
  private String         errorInfo;

    /**
   * Called upon successful Upload.<br>
   * @param object Instance of object that is being Uploaded.
   */
  @Override
  synchronized public void onUploadSuccess(com.cbe.Object object) {
    returnObject = object;
    // If delegate is reused, clear possibly error state
    errorInfo = null;
    this.finished = true;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  synchronized public void onUploadError(com.cbe.delegate.TransferError error, com.cbe.util.Context context) {
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
    // Reset finished flag, so current delegate instance can be reused
    finished = false; 
    if (errorInfo != null) {
      throw new RuntimeException(errorInfo);
    }
    return returnObject; 
  }
}