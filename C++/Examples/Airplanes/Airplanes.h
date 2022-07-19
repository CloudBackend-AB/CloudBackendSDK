#ifndef Airplanes_h__
#define Airplanes_h__

#include "CBE/Filter.h"
#include "CBE/Types.h"

class Airplanes {
  CBE::CloudBackendPtr cloudBackend;
public:
  CBE::AccountPtr      account;

  Airplanes(const std::string& credentials);
  Airplanes(const Airplanes&) = delete;

  CBE::QueryResultPtr query(CBE::Filter         filter,
                            CBE::container_id_t containerId);

  CBE::ContainerPtr createContainer(CBE::ContainerPtr   parentContainer,
                                    const std::string&  name);
  CBE::ObjectPtr    createObject(CBE::ContainerPtr    parentContainer,
                                 const std::string&   name,
                                 CBE::metadata_type&& metadata);
  void removeContainer(CBE::ContainerPtr   container);
};


#endif
