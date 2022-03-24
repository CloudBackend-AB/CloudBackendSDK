/*
     Copyright © CloudBackend AB 2020 - 2022..
*/
#ifndef CBE_SHAREEVENTPROTOCOL_H_
#define CBE_SHAREEVENTPROTOCOL_H_

#include "CBE/Types.h"
#include "CBE/Item.h"

namespace CBE {
    /** Protocol for implementing a Share delegate.
     * Abstract base class which is used when you want notification about
     * changes in folder and on documents. */


    class ShareEventProtocol {
    public:
        virtual ~ShareEventProtocol() {}

        /** Gets called when a query for available shares that have been shared to you. */
        virtual void onListAvailableShares(CBE::QueryResultPtr result){};
        
        /** Gets called when a query for shares that you have shared to other accounts or groups. */
        virtual void onListMyShares(CBE::QueryResultPtr qResult){};
        
        /** Gets called when an Access Control List (std::map with user_id and permission) has been added to a Container */
        virtual void onContainerACLAdded(std::map<CBE::user_id_t, CBE::permission_status_t> ACLs){};
        
        /** Gets called when an Access Control List (std::map with user_id and permission) has been added to an Object */
        virtual void onObjectACLAdded(std::map<CBE::user_id_t, CBE::permission_status_t> ACLs) {};
        
        /** Gets called when an Access Control List (std::map with user_id and permission) has been loaded for a Container */
        virtual void onContainerAclLoaded(std::vector<CBE::user_id_t> userIds, std::map<uint64_t, CBE::permission_status_t> ACLMap) {};
        
        /** Gets called when an Access Control List (std::map with user_id and permission) has been loaded for an Object */
        virtual void onObjectAclLoaded(std::vector<CBE::user_id_t> userIds, std::map<uint64_t, CBE::permission_status_t> ACLMap) {};
        
        /** Gets called when a Container has been shared
         * @param shareId is the id of the share and is needed when unSharing and can be used to locate which user is tied to that share. 
        */
        virtual void onContainerShared(uint64_t shareId) {};
        
        /** Gets called when a Container has been unShared, message will be OK else an shareError will be thrown.  */
        virtual void onContainerUnShared(std::string message){};
        
        /** Gets called when an Object has been shared. */
        virtual void onObjectShared(uint64_t shareId) {};
        
        /** Gets called when an Object has been unshared, massage will be OK, if not ok there will be a shareError thrown. */
        virtual void onObjectUnShared(std::string message){};
        
        /** Errors regarding share and list share calls onShareError. */
        virtual void onShareError(CBE::item_t type, CBE::persistence_t operation, uint32_t code,
                                  std::string reason, std::string message){};
        
        /** Gets called when errors regarding ACLs have occured, outcome of a get or setACL that has failed.
         * @param type is either Container or Object.
         * @param operation is set or getACL for either Container or Object.
         * @param code is according to w3 standards, for more information look at cloudbackend website or look at RFC 7231
         * @param reason is from server/edge why the error occured.
         * @param message of the error can be something like 403, forbidden, permission denied where permission denied is the message 
         * telling you that the ACL:s you've set is not allowing the other user to access the Object or Container. 
         */
        virtual void onACLError(CBE::item_t type, CBE::persistence_t operation,
                                uint32_t code, std::string reason,
                                std::string message){};
    };
}

#endif //CBE_SHAREEVENTPROTOCOL_H_
