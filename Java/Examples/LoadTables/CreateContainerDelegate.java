import com.cbe.*;
import com.cbe.util.*;
import com.cbe.delegate.*;

public class CreateContainerDelegate extends com.cbe.delegate.CreateContainerDelegate {

  CreateContainerDelegate() {}
  private boolean           finished = false;
  private String            errorInfo;
  private com.cbe.Container returnContainer;


  /**
   * Called upon successful CreateContainer.<br>
   * 
   */
  @Override
  synchronized public void onCreateContainerSuccess(com.cbe.Container container) {
    returnContainer = container;
    this.finished = true;
    // If delegate is reused, clear possibly error state
    errorInfo = null;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  synchronized public void onCreateContainerError(com.cbe.delegate.Error error, com.cbe.util.Context context) {
    errorInfo = "Login error: code=\"" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    this.finished = true;
    notify();
  }

    synchronized public Container waitForRsp() {
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
    return returnContainer; 
  }
}