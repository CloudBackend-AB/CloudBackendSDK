import com.cbe.delegate.*;
import com.std.Members_Vec;

class MyListMembersDelegate extends com.cbe.delegate.ListMembersDelegate {

  MyListMembersDelegate() {}

  private boolean     finished = false;
  private String      errorInfo;
  private Members_Vec myMembersVec = null;

  /**
   * Called upon successful ListMembers.<br>
   * @param members Vector of cbe::Member holding the members<br>
   * of a group.
   */
  @Override
  public synchronized void onListMembersSuccess(com.std.Members_Vec members) {
    this.myMembersVec = members;
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
  public synchronized void onListMembersError(com.cbe.delegate.Error error,
                                              com.cbe.util.Context context) {
    errorInfo = "ListMembers error: code=" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    finished = true;
    notify();
  }

  synchronized public com.std.Members_Vec waitForRsp() {
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
    return myMembersVec;
  }
} // class MyListMembersDelegate
