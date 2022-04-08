import java.util.Scanner;
import com.cbe.*;

public class AccountDelegate extends AccountEventProtocol {

    public boolean finished = false;

    AccountDelegate() {}
    
    @Override
    public void onLogin(long atState, CloudBackend cloudbackend) {
        // System.out.println("onlogin as " + cloudbackend.account().username() + " /=" + cloudbackend.account().rootContainer().id());
        this.finished = true;
    }

    @Override
    public void onError(long failedAtState, long code, String reason, String message) {
        System.out.println("Login error: code=" + code + ", reson=\"" + reason + "\", message=\"" + message + "\"");
        this.finished = true;
    }
}