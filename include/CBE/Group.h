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
      virtual std::string name();
      virtual ~Group() {};

    protected:
      ///This function should not be called as it will not continue to be public and any call made on the object will fail.
      Group(){}
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
