#ifndef CBE_MEMBER_H
#define CBE_MEMBER_H

#include "CBE/Types.h"

namespace CBE {
    
  struct Request {
      uint64_t userId{0};
      std::string alias{""};
      std::string applicationComment{""};
      uint64_t date{0}; //unix time
  };

//this might be better to have just as a string on the member. also if so then list members should call list roles.
  struct Role {
    std::string role;
  };

  class Member {
    public:

    virtual void kick(std::string kickComment, CBE::GroupDelegatePtr delegate);
    virtual void ban(std::string banComment, CBE::GroupDelegatePtr delegate);
    virtual void unBan(CBE::GroupDelegatePtr delegate);
    virtual uint64_t memberId() const;

    /** Returns the member name.*/
    virtual std::string name() const;
    virtual CBE::Visibility visibility() const;
    virtual CBE::group_id_t groupId() const;
    virtual std::map<std::pair<uint64_t,uint64_t>,std::pair<uint64_t,std::string>> getMemberBanInfo();

  };


}

#endif //CBE_MEMBER_H