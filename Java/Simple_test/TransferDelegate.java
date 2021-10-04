import java.util.Scanner;
import com.cbe.*;

public class TransferDelegate extends TransferEventProtocol {
    
    @Override
    public void onObjectUploaded(com.cbe.Object object) {
        this.finished = true;
    }
    
    @Override
    public void onObjectUploadFailed(String name, long objectId, long parentId, long atState, long status) {}
    
    // @Override
    // public void onChunkSent(Object object, long sent, long total) {}
    
    // @Override  
    // public void onChunkReceived(Object object, long received, long total) {}
    
    @Override
    public void onObjectDownloaded(com.cbe.Object object, String path) {}
    
    @Override
    public void onObjectBinaryDownloaded(com.cbe.Object object, byte[] data) {}
    
    @Override
    public void onObjectDownloadFailed(com.cbe.Object object, long status) {}
    
    public boolean finished = false;
    // public QueryResult qR = null;
}