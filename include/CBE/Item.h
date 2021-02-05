/*
     Copyright © CloudBackend AB 2020.
*/

#ifndef INCLUDE_CBE_ITEM_H_
#define INCLUDE_CBE_ITEM_H_

#include <thread>
#include <mutex>
#include <map>
#include <string>
#include <vector>
#include "CBE/Types.h"

namespace CBE
{
/** Forward declarations */

/** Class for an Item.
   * This is class is the base class of Objects and Containers.
   */
class ObjectUpdater;
class Item {
 public:

  virtual std::map<uint64_t, std::vector<CBE::ShareData>> getShareIds() const;

    /** Get the shareId from userId */
  virtual CBE::share_id_t getShareFromUserId(user_id_t userId);

  /** Get the shareId from userId */
  virtual CBE::user_id_t getUserFromShareId(user_id_t shareId); 

  //virtual void addShareId(uint64_t shareId, uint64_t id, bool isUserId = true) const;

  virtual std::string aclTag() const;

  virtual std::string description() const; 

  /** Returns an Items id. */
  virtual CBE::item_id_t id() const;

  /** Returns the id of the Items parent. */
  virtual CBE::container_id_t parentId() const;
  virtual CBE::container_id_t oldParentId() const;

  /** Returns the name. */
  virtual std::string name() const;

  /** Returns the path */
  virtual std::string path() const;

  /** Returns the owner id. */
  virtual CBE::user_id_t ownerId() const;

  // virtual user_id_t userId() const;

  /** Returns which drive the container resides on. */
  virtual CBE::container_id_t driveId() const;

  /** Returns the username of the Containers owner. */
  virtual std::string username() const;

  //move or hide ?
  virtual bool idLoaded() const;
  virtual bool dataLoaded() const;

  /** Returns the creation date in Unix time. */
  virtual CBE::date_t created() const;

  /**Returns the updated date/time in Unix time*/
  virtual CBE::date_t updated() const;

  /** Returns the deleted date in Unix time*/
  virtual CBE::date_t deleted() const;

  /** Container or Object*/
  virtual CBE::item_t type() const;

  /** Returns the bit size of an object (ex file size). */
  // virtual uint64_t length() const;    move to object

  // virtual std::map<uint64_t, permission_status_t> ACLMap() const;
  
  virtual bool operator<(const CBE::Item &other) const;

  std::map<uint64_t, CBE::permission_status_t> ACLMap() const; 

  ///This function should not be called as it will not continue to be public and any call made on the item will fail.
  static CBE::item_id_t nextId();

  ///This function should not be called as it will not continue to be public and any call made on the item will fail.
  static void resetTempId();

  ///This function should not be called as it will not continue to be public and any call made on the item will fail.
  virtual ~Item(){};

protected:
  Item() {}
  // Item(CBE::item_id_t _tempId, CBE::item_id_t _id, CBE::container_id_t _parentId, CBE::container_id_t _oldParentId, std::string _name, CBE::user_id_t _userId, CBE::user_id_t _ownerId, CBE::container_id_t _driveId, std::string _username, CBE::date_t _created, CBE::date_t _updated, CBE::date_t _deleted, CBE::item_t _type);
  static CBE::item_id_t _currentTempId;
  

  static std::mutex _tempIdMutex;

};
} // namespace CBE
// namespace XAPI

#endif // INCLUDE_XAPI_ITEM_H_
