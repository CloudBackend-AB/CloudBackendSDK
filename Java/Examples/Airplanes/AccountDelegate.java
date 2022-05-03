import com.cbe.CloudBackend;

public class AccountDelegate extends com.cbe.AccountEventProtocol {
    private CloudBackend cloudBackend = null;
    private boolean finished = false;

    @Override
    synchronized public void onLogin(long atState, CloudBackend cloudBackend) {
      this.cloudBackend = cloudBackend;
      finished = true;
      notify();
    }

    @Override
    synchronized public void onError(long failedAtState, long code,
                                     String reason, String message) {
      System.out.println("Login error: code=" + code +
                         ", reason=\"" + reason +
                         "\", message=\"" + message + "\"");
      finished = true;
      notify();
    }
    
    synchronized public CloudBackend waitForRsp() {
      while (!finished) {
        try {
          wait();
        } catch (InterruptedException e) {
          e.printStackTrace();
        }
      }
      return cloudBackend; 
    }
    
}
