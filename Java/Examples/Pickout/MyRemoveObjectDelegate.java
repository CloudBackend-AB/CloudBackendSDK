public class MyRemoveObjectDelegate extends 
                                    com.cbe.delegate.object.RemoveDelegate {

  private boolean                                   finished = false;
  private String                                    errorInfo;
  private com.cbe.delegate.object.RemoveSuccess  removeSuccess;
  MyRemoveObjectDelegate() {}

  /**
   * Called upon successful remove.<br>
   * 
   */
  @Override
  synchronized public void onRemoveSuccess(long objectId, String name) {
    removeSuccess = new com.cbe.delegate.object.RemoveSuccess(objectId,
                                                                 name);
    // If delegate is reused, clear possible error state
    errorInfo = null;
    finished = true;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  synchronized public void onRemoveError(com.cbe.delegate.Error error,
                                         com.cbe.util.Context   context) {
    errorInfo = "Remove error: code=" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    finished = true;
    notify();
  }

  synchronized public com.cbe.delegate.object.RemoveSuccess waitForRsp() {
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
    return removeSuccess; 
  }
}