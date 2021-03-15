/*
     Copyright © CloudBackend AB 2020.
*/

#ifndef INCLUDE_CBE_CLOUDBACKEND_H_
#define INCLUDE_CBE_CLOUDBACKEND_H_

#include <string>
#include <map>
#include <iostream>

#include "CBE/Account.h"
#include "CBE/Item.h"
#include "CBE/Types.h"
#include "CBE/Filter.h"
#include "CBE/Object.h"

#include "CBE/ShareManager.h"
#include "CBE/GroupManager.h"

namespace CBE
{
/** Forward declarations */
class Container;

class CloudBackend
{
public:
   /**
          * This should return a instance of cloudbackend giving the user access to the root contianer and logIng in the user
          * @param username of the user to be signed in
          * @param password of the user to be signed in
          * @param source is the identifier for the tenant
          * @param delegate. Delegate is a shared pointer to the class AccountEventProtocol that the user implements to recieve the callback on function completion. See Types.h for decleration.
    */
   static CloudBackendPtr logIn(const std::string &username, const std::string &password, const std::string &source, CBE::AccountDelegatePtr delegate);

   /**
    * adds a listner that will recieve updates as changes occur on the account.  removeListener should always be called when you stop using the delegate.
    * @param delegate: Delegate is a shared pointer to the class ItemEventProtocol that the user has implemented. The pointer is defined in Types.h.
    */
   void addListener(CBE::ItemDelegatePtr delegate);

   /**
    * removes a listner that will recieve updates as changes occur on the account
    * @param delegate: Delegate is the previously passed in shared pointer to an ItemEventProtocol that the user has implemented to recieve the callback on function completion. The pointer is defined in Types.h.
    */
   void removeListener(CBE::ItemDelegatePtr delegate);


   /**
       *  Call to get a list of items in the container. Implement onQueryLoaded to from CBE::ItemDelegatePtrto recieve the callback.  Requires you already signedIn.
       * @param containerId
       * @param delegate is a shared pointer to the class in which you implement CBE::ItemDelegatePtr to recieve the callback on function completion.       
    */
   virtual void query(uint64_t containerId, CBE::ItemDelegatePtr delegate);

   /**
       *  Call to get a list of items in the container using a filter. Implement onQueryLoaded to from CBE::ItemEventProtocol to recieve the callback. Requires you already signedIn
       *  Does not require a containerId due to having the filter.
       * @param filter
       * @param delegate is a shared pointer to the class in which you implement CBE::ItemEventProtocol to recieve the callback on function completion.
    */
   virtual void query(CBE::container_id_t containerId, CBE::Filter filter, CBE::ItemDelegatePtr delegate); // add a Container Id

   /**
     * Queries with a given path to the container or object given in the path
     * @param path, e.g /documents/pictures/pic.jpg or /documents/pictures.
     * @param delegate, is a shared pointer to the class in which you implement CBE::ItemEventProtocol to recieve the callback on function completion.
    */
   virtual void queryWithPath(std::string path, CBE::ItemDelegatePtr delegate);

   /** 
     * casts an item to a container.
    */
   static CBE::ContainerPtr castContainer(CBE::ItemPtr item);

   /**
     * casts an item to an object
    */
   static CBE::ObjectPtr castObject(CBE::ItemPtr item);

   //retunrs an account objct with information on the user
   CBE::AccountPtr account();

   //returns the version number
   std::string version();
  
   CBE::GroupManager *groupManager(); //Group - NYI
   CBE::ShareManager* shareManager(); // Shares 
   
   virtual ~CloudBackend();

protected:
   CloudBackend();

   CBE::ShareManager* _shareManager;
   CBE::GroupManager* _groupManager;
   CBE::AccountPtr _account;


   bool listening;

   std::vector<CBE::ItemDelegatePtr> _event_listeners;
};

} // namespace CBE
// namespace CBE

#endif // INCLUDE_CBE_CLOUDBACKEND_H_
