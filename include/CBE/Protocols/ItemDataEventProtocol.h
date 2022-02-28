/*
     Copyright © CloudBackend AB 2020.
*/


#ifndef CBE_ITEMDATAEVENTPROTOCOL_H_
#define CBE_ITEMDATAEVENTPROTOCOL_H_


#include "CBE/Item.h"
#include "CBE/QueryResult.h"
#include "CBE/Types.h"


namespace CBE {
  /** Protocol for implementing a Item delegate.
   * Abstract base class which is used when you want notification about
   * changes in folder and on documents. */


  class ItemDataEventProtocol {
    public:
      virtual ~ItemDataEventProtocol() {
      }

      /** Gets session for item. */
      virtual void onDocumentSessionLoaded(std::string url){};

      /** Gets called when an error occurred. */
      virtual void onItemError(CBE::ItemPtr container, int type, uint32_t operation, uint32_t failedAtState, uint32_t code, std::string reason, std::string message){};



  };
}

#endif //CBE_ITEMDATAEVENTPROTOCOL_H_
