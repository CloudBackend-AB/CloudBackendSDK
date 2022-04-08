package cbe.tutorial;

import com.cbe.CloudBackend;
//import com.cbe.AccountEventProtocol;

class AccountEventProtocol extends com.cbe.AccountEventProtocol {
  boolean finished = false;
  @Override
  public synchronized void onLogin(long atState, CloudBackend cloudbackend) {
    System.out.println("Login success");
    finished = true;
    notify();
  }

  @Override
  public synchronized void onError(long failedAtState, long code, String reason, String message) {
    System.out.println("Login failed");
    finished = true;
    notify();
  }  
}

public class Main {

  public static void main(String[] args) {
    System.out.println("Program start");
    AccountEventProtocol delegate = new AccountEventProtocol();
    System.out.println("About to log in");
    CloudBackend cbobj = CloudBackend.logIn("gitHubTester1",
                                          "gitHubTester1password",
                                          "cbe_girhubtesters",
                                          delegate);
    synchronized (delegate) {
      while (!delegate.finished) {
        try
        {
            delegate.wait();
        }
        catch(Exception e)
        {
             System.out.println(e);
        }
      }      
    }
    if (cbobj.account().userId()>0) {
      System.out.println("Login: " + cbobj.account().username() + " " + cbobj.account().firstName() + " " + cbobj.account().lastName());
    } else {
      System.out.println("Check the credentials.");
      System.out.println("Terminating SDK " + cbobj.version());
      System.out.println("Exit program.");
      return;
    }

    System.out.println("Terminating SDK " + cbobj.version());
    System.out.println("Program end.");
  }
}
