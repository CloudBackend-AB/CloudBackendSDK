#ifndef CBE__delegate__ICloudBackendListener_h__
#define CBE__delegate__ICloudBackendListener_h__

#include "CBE/QueryResult.h"
#include "CBE/Types.h"

#include "CBE/delegate/Error.h"

#include "CBE/util/Context.h"
#include "CBE/util/ErrorInfo.h"
#include "CBE/util/Exception.h"

#include <memory>

namespace cbe {
  namespace delegate {
class ICloudBackendListener {
  public:
    /**
     * Called upon successful create of an @ref cbe::Object "object".
     * @param object Instance of object that is being created.
     */
    virtual void onRemoteObjectAdded(cbe::Object&& object) = 0;

    /**
     * Called upon successful move of an @ref cbe::Object "object".
     * @param object Instance of object that is being moved.
     */
    virtual void onRemoteObjectMoved(cbe::Object&& object) = 0;
    
    /**
     * Called upon successful removal of an @ref cbe::Object "object".
     * @param objectId Instance of the object that is being Removed.
     * @param name Name of the object that is being Removed.
     */
    virtual void onRemoteObjectRemoved(cbe::ItemId objectId, 
                                       std::string name) = 0;
    
    /**
     * Called upon successful renaming of an @ref cbe::Object "object".
     * @param object Instance of object that is being renamed.
     */
    virtual void onRemoteObjectRenamed(cbe::Object&& object) = 0;
    
    /**
     * Called upon successful Create.
     * @param conatiner Instance of container that is being created.
     */
    virtual void onRemoteContainerAdded(cbe::Container&& container) = 0;

    /**
     * Called upon successful Move.
     * @param container Instance of container that is being moved.
     */
    virtual void onRemoteContainerMoved(cbe::Container&& container) = 0;

    /**
     * Called upon successful Remove.
     * @param containerId Instance of the container that is being Removed.
     * @param name Name of the container that is being removed.
     */
    virtual void onRemoteContainerRemoved(cbe::ItemId containerId, 
                                          std::string name) = 0;
    
    /**
     * Called upon successful Rename.
     * @param container Instance of container that is being Renamed.
     */
    virtual void onRemoteContainerRenamed(cbe::Container&& container) = 0;

    virtual ~ICloudBackendListener();
  }; // struct ICloudBackendListener
/**
 * Pointer to ICloudBackendListener.
 */
using ICloudBackendListenerPtr = std::shared_ptr<ICloudBackendListener>;
  } // namespace delegate
} // namespace cbe

#endif // !CBE__delegate__ICloudBackendListener_h__