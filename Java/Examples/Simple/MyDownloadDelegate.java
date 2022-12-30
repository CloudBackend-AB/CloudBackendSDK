import com.cbe.delegate.*;
import com.cbe.*;

public class MyDownloadDelegate extends com.cbe.delegate.DownloadDelegate {

  MyDownloadDelegate() {}
  private boolean        finished = false;
  private String         errorInfo;
  private com.cbe.Object returnObject;

  /**
   * Called upon successful download.<br>
   * 
   */
  @Override
  synchronized public void onDownloadSuccess(com.cbe.Object object, String path) {
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
  synchronized public void onDownloadError(com.cbe.delegate.TransferError error,
                                           com.cbe.util.Context       context) {
    errorInfo = "Download error: code=" + error.getErrorCode() + 
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