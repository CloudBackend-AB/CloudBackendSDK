import com.cbe.delegate.container.*;

public class RemoveContainerDelegate extends com.cbe.delegate.container.RemoveDelegate {

  RemoveContainerDelegate() {}
  private boolean                                   finished = false;
  private String                                    errorInfo;
  private com.cbe.delegate.container.RemoveSuccess  removeSuccess;

  /**
   * Called upon successful Download.<br>
   * 
   */
  @Override
  synchronized public void onRemoveSuccess(long containerId, String name) {
    removeSuccess.setContainerId(containerId);
    removeSuccess.setName(name);
    this.finished = true;
    // If delegate is reused, clear possibly error state
    errorInfo = null;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  synchronized public void onRemoveError(com.cbe.delegate.Error error, com.cbe.util.Context context) {
    errorInfo = "Login error: code=\"" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    this.finished = true;
    notify();
  }

  synchronized public com.cbe.delegate.container.RemoveSuccess waitForRsp() {
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
    return removeSuccess; 
  }
}