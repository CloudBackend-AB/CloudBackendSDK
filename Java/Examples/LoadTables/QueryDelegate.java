import com.cbe.delegate.*;

public class QueryDelegate extends com.cbe.delegate.QueryDelegate {

  QueryDelegate() {}
  private boolean             finished = false;
  private String              errorInfo;
  private com.cbe.QueryResult qR;

   /**
   * Called upon successful query.<br>
   * @param queryResult Instance of a QueryResult containing the result set.
   */
  @Override
  synchronized public void onQuerySuccess(com.cbe.QueryResult queryResult) {
    qR = queryResult;
    this.finished = true;
    // If delegate is reused, clear possibly error state
    errorInfo = null;
    notify();
  }

  /**
   * Called upon a failed query() or join() call.<br>
   * @param error   Error information passed from %CloudBackend SDK.<br>
   * @param context Additional context information about the original service<br>
   *                call that has failed.
   */
  @Override
  synchronized public void onQueryError(com.cbe.delegate.QueryError error, com.cbe.util.Context context) {
    errorInfo = "Login error: code=\"" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    this.finished = true;
    notify();
  }

  synchronized public com.cbe.QueryResult waitForRsp() {
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
    return qR; 
  }
}