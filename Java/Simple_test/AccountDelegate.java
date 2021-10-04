import java.util.Scanner;
import com.cbe.*;

public class AccountDelegate extends AccountEventProtocol {

    public boolean finished = false;

    AccountDelegate() {}
    
    @Override
    public void onLogin(long atState, CloudBackend cloudbackend) {
        System.out.println("You've reached onlogin in Java congrats " + cloudbackend.account().username() + " " + cloudbackend.account().rootContainer().id());
        this.finished = true;
    }
}