#ifndef CBE_MEMBER_H
#define CBE_MEMBER_H

#include "CBE/Types.h"

namespace CBE {
  class Member {
    public:

    /** Returns the member id.*/
    virtual uint64_t memberId() const;

    /** Returns the member name.*/
    virtual std::string name() const;

    /** Returns the group id for the specific member object with this member id and name. */
    virtual CBE::group_id_t groupId() const;

  };


}

#endif //CBE_MEMBER_H