import com.cbe.delegate.*;

public class MyQueryDelegate extends com.cbe.delegate.QueryDelegate {

  MyQueryDelegate() {}
  private boolean             finished = false;
  private String              errorInfo;
  private com.cbe.QueryResult queryResult;

   /**
   * Called upon successful query.<br>
   * @param queryResult Instance of a QueryResult containing the result set.
   */
  @Override
  synchronized public void onQuerySuccess(com.cbe.QueryResult queryResult) {
    this.queryResult = queryResult;
    // If delegate is reused, clear possible error state
    errorInfo = null;
    finished = true;
    notify();
  }

  /**
   * Called upon a failed query() or join() call.<br>
   * @param error   Error information passed from %CloudBackend SDK.<br>
   * @param context Additional context information about the original service<br>
   *                call that has failed.
   */
  @Override
  synchronized public void onQueryError(com.cbe.delegate.QueryError error,
                                        com.cbe.util.Context        context) {
    errorInfo = "Query error: code=" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    finished = true;
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
    // Reset finished flag, so current delegate instance can be reused
    finished = false; 
    if (errorInfo != null) {
      throw new RuntimeException(errorInfo);
    }
    return queryResult; 
  }
}