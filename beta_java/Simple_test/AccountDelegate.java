import java.util.Scanner;
import com.cbe.*;

public class AccountDelegate extends AccountEventProtocol {

    public boolean finished = false;

    AccountDelegate() {
        // return AccountEventProtocol();
    }
    
    @Override
    public void onLogin(long atState, CloudBackend cloudbackend) {
        System.out.println("You have reached onlogin in Java, congrats\t" + cloudbackend.account().username() + " / id: " + cloudbackend.account().rootContainer().id());
        this.finished = true;
    }
}