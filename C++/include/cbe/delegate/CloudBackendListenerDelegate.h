#ifndef CBE__delegate__CloudBackendListenerDelegate_h__
#define CBE__delegate__CloudBackendListenerDelegate_h__

#include "cbe/delegate/ICloudBackendListener.h"
#include "cbe/QueryResult.h"
#include "cbe/Types.h"

#include "cbe/delegate/Error.h"

#include "cbe/util/Context.h"
#include "cbe/util/ErrorInfo.h"
#include "cbe/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
class CloudBackendListenerDelegate : public ICloudBackendListener{
  public:
   
    virtual void onRemoteObjectAdded(cbe::Object&& object) override;
    
    virtual void onRemoteObjectMoved(cbe::Object&& object) override;
    
    virtual void onRemoteObjectRemoved(cbe::ItemId  objectId, 
                                       std::string  name) override;
  
    virtual void onRemoteObjectRenamed(cbe::Object&& object) override;

    virtual void onRemoteContainerAdded(cbe::Container&& container) override;     
    
    virtual void onRemoteContainerMoved(cbe::Container&& container) override;

    virtual void onRemoteContainerRemoved(cbe::ItemId containerId, 
                                          std::string name) override;
    
    virtual void onRemoteContainerRenamed(cbe::Container&& container) override;

    
    ~CloudBackendListenerDelegate();
  }; // struct CloudBackendListenerDelegate
/**
 * Pointer to CloudBackendListenerDelegate that is passed into: \n 
 * CloudBackend::addListener(CloudBackendListenerDelegatePtr).
 */
using CloudBackendListenerDelegatePtr = 
                                  std::shared_ptr<CloudBackendListenerDelegate>;
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__CloudBackendListenerDelegate_h__