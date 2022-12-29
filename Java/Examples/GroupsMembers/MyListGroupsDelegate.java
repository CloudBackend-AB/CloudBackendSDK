import com.cbe.delegate.*;
import com.std.Groups_Vec;

class MyListGroupsDelegate extends com.cbe.delegate.ListGroupsDelegate {

  MyListGroupsDelegate() {}
  private boolean    finished = false;
  private String     errorInfo;
  private Groups_Vec myGroupsVec = null;

  /**
   * Called upon successful listGroup<br>
   * @param groups Ref to vector of cbe::Group holding the joined groups.
   */
  @Override
  synchronized public void onListGroupsSuccess(com.std.Groups_Vec groups) {
    this.myGroupsVec = groups;
    // If delegate is reused, clear possible error state
    errorInfo = null;
    finished = true;
    notify();
  }

  /**
   * Called upon a failed call.<br>
   * @param error   Error information passed from %CloudBackend SDK.<br>
   * @param context Additional context information about the original service<br>
   *                call that has failed.
   */
  @Override
  public synchronized void onListGroupsError(com.cbe.delegate.Error error,
                                             com.cbe.util.Context context) {
    errorInfo = "ListGroups error: code=" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    finished = true;
    notify();
  }

  synchronized public com.std.Groups_Vec waitForRsp() {
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
    return myGroupsVec;
  }
} // class MyListGroupsDelegate
