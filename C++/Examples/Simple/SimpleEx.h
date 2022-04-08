/*
      Copyright © CloudBackend AB 2020.
*/

#ifndef INCLUDE_CBE_TESTBODY_H_
#define INCLUDE_CBE_TESTBODY_H_


#include "CBE/Types.h"
#include "CBE/CloudBackend.h"
#include "CBE/Account.h"
#include "CBE/Protocols/AccountEventProtocol.h"
#include "CBE/Protocols/TransferEventProtocol.h"
#include "CBE/Protocols/ItemEventProtocol.h"
#include <memory>

class CBEExample :  public std::enable_shared_from_this<CBEExample>,
                    public CBE::AccountEventProtocol,
                    public CBE::TransferEventProtocol,
                    public CBE::ItemEventProtocol
{
public:
  /*implementation of delegates */
  CBE::CloudBackendPtr cloudBackend;
  // CBE::ItemDelegatePtr itemDelegate{shared_from_this()};
  // CBE::TransferDelegatePtr transferDelegate{shared_from_this()};
  
  std::shared_ptr<CBEExample> getPtr() {
    return shared_from_this();
  }

  /** From AccountEventProtocol: */

  /**
   * onLogin is virtually defined in the includePublic->CBE->Protocols->AccountEventProtocol.
   * In implementations such as this example code we need to define what happens in the callback from the server / public cloud / edge / node.
   * Hence we need to redefine the call here and implement in the .cpp file. 
  */
  void onLogin(uint32_t atState, CBE::CloudBackendPtr cbe);

  /** Gets called when the account status has changed (required). */
  void onError(CBE::persistence_t failedAtState, uint32_t code, std::string reason, std::string message);

  
  /** From ItemEventProtocol: */
  
  /** Gets called when a query has been performed. 
   * @param qr is the result of a query and contains the items of the query and also the filter that was used for the query.
  */
  void onQueryLoaded(CBE::QueryResultPtr qr);

  /** Gets called if the query fails look at message and reason for information. */
  void onLoadError(CBE::Filter filter, uint32_t operation, uint32_t code, std::string reason, std::string message);

  /** Gets called when a Container has been added. */
  void onContainerAdded(CBE::ContainerPtr container);

  /** Gets called when a Container has been removed. */
  void onContainerRemoved(CBE::item_id_t containerId, std::string name);

  /** Gets called when an error occurred. */
  void onItemError(CBE::ItemPtr container, CBE::item_t type, uint32_t operation, uint32_t failedAtState, uint32_t code, std::string reason, std::string message);
  

  /** From TransferEventProtocol: */

  /** */
  void onObjectUploaded(CBE::ObjectPtr object);

  /** Gets called when a error has occured in the upload stream. */
  void onObjectUploadFailed(std::string name, CBE::object_id_t objectId, CBE::container_id_t parentId, CBE::persistence_t atState, CBE::failed_status_t status);


  /** SDK requests, user defined functionallity: 
   *  Feel free to implement more functionallity using the library as you please.
  */

  /** Example of how to implement the library call to query. */
  void query(CBE::ContainerPtr container, std::string name);

  /** Example of how to implement the library call to createContainer. */
  void createContainer(CBE::ContainerPtr container, std::string name);

  /** Example of how to implement the library call to upload. */
  void uploadToContainer(std::string path, std::string name, CBE::ContainerPtr container);

  /** Temporarily saving the name of the container that we want to create, to see if it is already existing on the account.  */
  std::string _queryName;


};

#endif
