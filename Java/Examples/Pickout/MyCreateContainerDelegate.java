import com.cbe.*;
import com.cbe.util.*;
import com.cbe.delegate.*;

public class MyCreateContainerDelegate extends com.cbe.delegate.CreateContainerDelegate {

  MyCreateContainerDelegate() {}
  private boolean           finished = false;
  private String            errorInfo;
  private com.cbe.Container returnContainer;


  /**
   * Called upon successful creation of container.<br>
   * 
   */
  @Override
  synchronized public void onCreateContainerSuccess(com.cbe.Container container) {
    returnContainer = container;
    // If delegate is reused, clear possible error state
    errorInfo = null;
    finished = true;
    notify();
  }

  /**
   * Called if an error is encountered.
   */
  @Override
  synchronized public void onCreateContainerError(com.cbe.delegate.Error error,
                                                  com.cbe.util.Context context) {
    errorInfo = "Create error: code=" + error.getErrorCode() + 
                ", reason=\"" + error.getReason() +
                "\", message=\"" + error.getMessage() + "\"";
    finished = true;
    notify();
  }

  synchronized public com.cbe.Container waitForRsp() {
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
    return returnContainer; 
  }
}