/*
 SDK Utility definitions.
 Copyright © CloudBackend AB 2020-2023.
 */
#ifndef INCLUDE_SDK_UTILITY_H_
#define INCLUDE_SDK_UTILITY_H_

#include <stdint.h>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <utility>

namespace cbe {

/**
 * This is the data used from shares(shareId) 
 * to keep track of user/group-id relating to a shareid
 */
struct ShareData {
  template <class ShareDataT>
    ShareData(ShareDataT&& rh) : id{rh.id}, isUserId{rh.isUserId} {}
    ShareData(uint64_t id, bool isUserId) : id{id}, isUserId{isUserId} {}
  //DEFAULT = 0, ID for userId or groupId
  uint64_t id;
  //DEFAULT == TRUE, UserId = True if userID and false if groupId
  bool isUserId;
};

/**
 * @brief Search lists with ID.
 * 
 * Example:
 * \code bool alreadyExists = std::find_if(vector.begin(), vector.end(), 
 *                                    cbe::MatchesID<uint64_t>(id)) != vector.end();
 * \endcode
 */
template<typename IdT>
class MatchesID {
  IdT _id;

public:
  MatchesID(const IdT& id) :
      _id(id) {
  }

  template<class ItemT>
  bool operator()(const ItemT& item) const {
    return item.id == _id;
  }
};
}
#endif //INCLUDE_SDK_UTILITY_H_
