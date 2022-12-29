import com.cbe.delegate.*;

public class MyShareDelegate extends com.cbe.delegate.ShareDelegate {

  private boolean  finished = false;
  private String   errorInfo;
  private long     returnShareId;
  MyShareDelegate() {}

  /**
   * Called upon successful sharing.<br>
   * @param shareId Id of the share.
   */
  @Override
  synchronized public void onShareSuccess(long shareId) {
    returnShareId = shareId;
    // If delegate is reused, clear possible error state
    errorInfo = null;
    finished = true;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  synchronized public void onShareError(com.cbe.delegate.Error error,
                                        com.cbe.util.Context   context) {
    errorInfo = "Share error: code=" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    finished = true;
    notify();
  }

  synchronized public long waitForRsp() {
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
    return returnShareId; 
  }
}