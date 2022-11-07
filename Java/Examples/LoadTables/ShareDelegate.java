import com.cbe.delegate.*;

public class ShareDelegate extends com.cbe.delegate.ShareDelegate {

  ShareDelegate() {}
  private boolean  finished = false;
  private String   errorInfo;
  private long     returnShareId;

  /**
   * Called upon successful share.<br>
   * @param shareId Id of the share.
   */
  @Override
  synchronized public void onShareSuccess(long shareId) {
    returnShareId = shareId;
    this.finished = true;
    // If delegate is reused, clear possibly error state
    errorInfo = null;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  synchronized public void onShareError(com.cbe.delegate.Error error, com.cbe.util.Context context) {
    errorInfo = "Login error: code=\"" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    this.finished = true;
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
    if (errorInfo != null) {
      throw new RuntimeException(errorInfo);
    }
    return returnShareId; 
  }

}