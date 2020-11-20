/*
     Copyright © CloudBackend AB 2020.
*/

#ifndef INCLUDE_CBE_GROUP_MANAGER_H_
#define INCLUDE_CBE_GROUP_MANAGER_H_

#include <string>
#include "CBE/Protocols/GroupEventProtocol.h"
#include "CBE/Types.h"

namespace CBE {
  class GroupManager {
    public:
     virtual void createGroup(std::string name, std::string memberAlias, CBE::group_id_t parentGroup, CBE::GroupDelegatePtr delegate);
      virtual ~GroupManager(){};
    protected:
     ///This function should not be called as it will not continue to be public and any call made on the object will fail.
     GroupManager(){}
  };
}
// namespace CBE

#endif  // INCLUDE_CBE_GROUP_MANAGER_H_
