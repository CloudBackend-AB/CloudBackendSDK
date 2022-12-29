import com.cbe.*;
import com.cbe.delegate.*;

public class MyListSharesDelegate extends com.cbe.delegate.ListSharesDelegate {

  MyListSharesDelegate() {}
  private boolean     finished = false;
  private String      errorInfo;
  private QueryResult qr;

   /**
   * Called upon successful listing of shares.<br>
   * @param qResult Instance of cbe::QueryResult containing list of shares.
   */
  @Override
  synchronized public void onListSharesSuccess(com.cbe.QueryResult qResult) {
    qr = qResult;
    // If delegate is reused, clear possible error state
    errorInfo = null;
    finished = true;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  synchronized public void onListSharesError(com.cbe.delegate.Error error,
                                             com.cbe.util.Context   context) {
    errorInfo = "List shares error: code=" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    finished = true;
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
    // Reset finished flag, so current delegate instance can be reused
    finished = false; 
    if (errorInfo != null) {
      throw new RuntimeException(errorInfo);
    }
    return qr; 
  }
}
