import java.util.Scanner;
import com.cbe.*;

public class TransferDownloadDelegate extends TransferDownloadEventProtocol {
    public boolean finished = false;
  /**
   *  Gets called when a chunk of data has successfully been recieved. 
   */
  @Override
  public void onChunkReceived(com.cbe.Object object, long received, long total) {
    System.out.println("onChunkReceived: recived=\"" + received + "\"");
  }
  
  /**
   *  Gets called when a download has completed. 
   */
  @Override
  public void onObjectDownloaded(com.cbe.Object object, String path) {
    System.out.println("ObjectDownloaded: name=\"" + object.name() + "\"");
    this.finished = true;
  }
  
  /**
   *  Gets called when a download has completed.  IMPORTANT!!   This data is on the heap and you are responsible for calling delete on it.  We may change data to a shared pointer in the future. 
   */
  @Override
  public void onObjectBinaryDownloaded(com.cbe.Object object, byte[] data) {
    System.out.println("ObjectBinaryDownloaded: name=\"" + object.name() + "\"");
    this.finished = true;
  }
  
  /**
   *  Gets called when a error has occured in the download stream 
   */
  @Override
  public void onObjectDownloadFailed(com.cbe.Object object, long status) {
    System.out.println("ObjectDownloadFailed: name=\"" + object.name() + "\"");
    this.finished = true;
  }
}
