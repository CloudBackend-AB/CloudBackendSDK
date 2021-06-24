/*
     Copyright © CloudBackend AB 2020.
*/
#ifndef INCLUDE_CBE_GROUP_H_
#define INCLUDE_CBE_GROUP_H_

#include <string>
#include "CBE/Types.h"

namespace CBE {
  class Group  {
    public:
      /**
      * Returns the name of the group.
      */
      virtual std::string name() const;
      
      /**
      * Returns the id of the group.
      */
      virtual CBE::group_id_t id() const;
      
      /**
      * Returns the mutual container for the group.
      */
      virtual CBE::ContainerPtr groupContainer() const;

      virtual ~Group() {};

  };

  /** Struct with information about a given group member.
   * This struct contains information about a member in a group. **/
  class Member {
   public:
    Member(std::string name, CBE::user_id_t memberId) : name(name), memberId(memberId) { }

    CBE::user_id_t memberId;
    std::string name;
  };
}
// namespace CBE

#endif  // INCLUDE_CBE_GROUP_H_
