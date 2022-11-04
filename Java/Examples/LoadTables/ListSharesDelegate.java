import com.cbe.*;
import com.cbe.delegate.*;

public class ListSharesDelegate extends com.cbe.delegate.ListSharesDelegate {

  ListSharesDelegate() {}
  private boolean     finished = false;
  private String     errorInfo;
  private QueryResult qr;

   /**
   * Called upon successful Share.<br>
   * @param qResult Instance of cbe::QueryResult containing list of shares.
   */
  @Override
  synchronized public void onListSharesSuccess(com.cbe.QueryResult qResult) {
    qr = qResult;
    this.finished = true;
    // If delegate is reused, clear possibly error state
    errorInfo = null;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  synchronized public void onListSharesError(com.cbe.delegate.Error error, com.cbe.util.Context context) {
    errorInfo = "Login error: code=\"" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    this.finished = true;
    notify();
  }

  synchronized public QueryResult waitForRsp() {
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
    return qr; 
  }
}
