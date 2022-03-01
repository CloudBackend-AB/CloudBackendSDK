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

   //This is currently used for test servers and is not secure.
   static CloudBackendPtr logIn(const std::string &username, const std::string &password,
                                const std::string &source, const std::string &host,
                                CBE::AccountDelegatePtr delegate);

   /// Source is not currently being used and should be fixed.
   /**
    *  Call to create an account. Implement onCreated to from CBE::AccountDelegatePtrto recieve the
    * callback. currently disabled
    * @param username
    * @param password
    * @param email
    * @param firstName
    * @param lastName
    * @param source
    * @param delegate is a shared pointer to the class in which you implement
    * CBE::AccountDelegatePtr
    */
   static CloudBackendPtr createAccount(const std::string& username, const std::string& password,
                                        const std::string& email, const std::string& firstName,
                                        const std::string& lastName, const std::string& source,
                                        CBE::AccountDelegatePtr delegate);

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
   virtual QueryChainPtr query(uint64_t containerId, CBE::ItemDelegatePtr delegate);

   /**
       *  Call to get a list of items in the container using a filter. Implement onQueryLoaded to from CBE::ItemEventProtocol to recieve the callback. Requires you already signedIn
       *  Does not require a containerId due to having the filter.
       * @param filter
       * @param delegate is a shared pointer to the class in which you implement CBE::ItemEventProtocol to recieve the callback on function completion.
    */
   virtual QueryChainPtr query(CBE::container_id_t containerId, CBE::Filter filter, CBE::ItemDelegatePtr delegate); // add a Container Id

   /**
     * Search the whole container for tags related to Objects in the container structure. 
     * EX: Key = Name, Value Contract/Object/Song => Name:Contract1.
     * 
     * Search handles tags in combination / conjunction of keys and/or key values seperated by |.
     * EX: Name:*|Country:Sweden|Country:Norway, this would search for objects with key Name but any value and where key Country is either Sweden or Norway.
     * @param tags is a string of key tags or key:value pairs that are seperated by |.
     * @param containerId is the uint64_t id of the rootContainer to start the search of Objects in. ex: if starting in the rootContainer, the whole account will be searched for matching tags, key:value's.
     * @param delegate is the callback pointer to when the API returns from either cache or Server.  
   */
   virtual QueryResultPtr search(std::string tags, CBE::container_id_t containerId, CBE::ItemDelegatePtr delegate);
   
   /**
     * Queries with a given path to the container or object given in the path
     * @param path, e.g /documents/pictures/pic.jpg or /documents/pictures.
     * @param delegate, is a shared pointer to the class in which you implement CBE::ItemEventProtocol to recieve the callback on function completion.
    */
   virtual CBE::QueryChainPtr queryWithPath(std::string path, CBE::ItemDelegatePtr delegate);

   
   /** 
     * casts an item to a container.
    */
   static CBE::ContainerPtr castContainer(CBE::ItemPtr item);

   /**
     * casts an item to an object
    */
   static CBE::ObjectPtr castObject(CBE::ItemPtr item);

   //Can be used to clear the cache if you are having memory issues.
   virtual bool clearCache();

   //retunrs an account objct with information on the user
   CBE::AccountPtr account();


   //returns the version number
   std::string version();


       CBE::GroupManager* groupManager();  // Group - NYI
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
