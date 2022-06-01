import java.util.Scanner;
import com.cbe.*;

public class TransferUploadDelegate extends TransferUploadEventProtocol {
  public boolean finished = false;

 /**
   * Gets called when a upload has been completed. 
   */
  @Override
  public void onObjectUploaded(com.cbe.Object object) {
    System.out.println("ObjectUploaded: name=\"" + object.name() + "\"");
    this.finished = true;
  }

  /**
   *  Gets called when a error has occured in the upload stream. 
   */
  @Override
  public void onObjectUploadFailed(String name, long objectId, long parentId, long atState, long status) {
    System.out.println("ObjectUploadFailed: name=\"" + name + "\"");
    this.finished = true;
  }

  /**
   *  Gets called when a chunk of data has successfully been recieved. 
   */
  @Override
  public void onChunkSent(com.cbe.Object object, long sent, long total) {
    System.out.println("onChunkSent: sent=\"" + sent + "\"");
  }
}