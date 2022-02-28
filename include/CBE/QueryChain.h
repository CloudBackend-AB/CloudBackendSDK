/*
      Copyright © CloudBackend AB 2020.
 */

#ifndef INCLUDE_CBE_QUERYCHAIN_H_
#define INCLUDE_CBE_QUERYCHAIN_H_

#include <string>
#include <vector>

#include "CBE/Types.h"
#include "CBE/Filter.h"


namespace CBE
{

class QueryChain
{
public:
 /**
  * Performs a join requet in conjunction with a query to get related items from another container.  If chained with a query or other joins the delegate will be called once with the complete joined query result.
  * @param containerToQuery: The container the join should be done with.
  * @param key1: The key from the previous query on which to perform the join.
  * @param key2: The key on objects in containerToQuery on which to perform the join
  * @param delegate: If this is null and join is chained with query the delegate from the query will be used.  If join is called on a querychain where a query has completed a delegate should be passed in to the final join in the chain.  
  */
 virtual QueryChainPtr join(ContainerPtr containerToQuery, std::string key1, std::string key2, CBE::ItemDelegatePtr delegate = nullptr);

 /**
  * Performs a join requet in conjunction with a query to get related items from another container
with additional constraints.  If chained with a query or other joins the delegate will be called
once with the complete joined query result.
  * @param containerToQuery: The container the join should be done with.
  * @param key1: The key from the previous query on which to perform the join.
  * @param key2: The key on objects in containerToQuery on which to perform the join
  * @param constraints: A filter that provides any constraints for the query.
  * @param delegate: If this is null and join is chained with query the delegate from the query will
be used.  If join is called on a querychain where a query has completed a delegate should be passed
in to the final join in the chain.
\  */
 virtual QueryChainPtr join(ContainerPtr containerToQuery, std::string key1, std::string key2,
                            CBE::Filter constraints, CBE::ItemDelegatePtr delegate = nullptr);

 /**
  * Performs a join requet in conjunction with a query using items in another container to adjust
  * results. If chained with a query or other joins the delegate will be called once with the
  * complete joined query result.
  * @param containerToQuery: The container the join should be done with.
  * @param key1: The key from the previous query on which to perform the join.
  * @param key2: The key on objects in containerToQuery on which to perform the join
  * @param containerForResults: Can be the conainer from the previous query if desired items should
  * be from that container.
  * @param delegate: If this is null and join is chained with query the delegate from the query will
  * be used.  If join is called on a querychain where a query has completed a delegate should be
  * passed in to the final join in the chain.
  */
 virtual QueryChainPtr join(ContainerPtr containerToQuery, std::string key1, std::string key2,
                            ContainerPtr containerForResults, CBE::ItemDelegatePtr delegate = nullptr);

 /**
  * Performs a join requet in conjunction with a query to adjust resluts with additional constraints
  * on the joined container.If chained with a query or other joins the delegate will be called once
  * with the complete joined query result.
  * @param containerToQuery: The container the join should be done with.
  * @param key1: The key from the previous query on which to perform the join.
  * @param key2: The key on objects in containerToQuery on which to perform the join
  * @param constraints: A filter that provides any constraints for the query.
  * @param containerForResults: Can be the conainer from the previous query if desired items should
  * be from that container.
  * @param delegate: If this is null and join is chained with query the delegate from the query will
  * be used.  If join is called on a querychain where a query has completed a delegate should be
  * passed in to the final join in the chain.
  */
 virtual QueryChainPtr join(ContainerPtr containerToQuery, std::string key1, std::string key2,
                            CBE::Filter constraints, ContainerPtr containerForResults, CBE::ItemDelegatePtr delegate = nullptr);

 /// Default the destructor for Account.
 virtual ~QueryChain() {}

//will contain the result of the last query or join after the delegate has been called.
 virtual CBE::QueryResultPtr getQueryResult();

protected:

  CBE::QueryResultPtr _queryResultPtr;

  QueryChain() {}
};
} // namespace CBE
// namespace CBE

#endif // INCLUDE_CBE_ACCOUNT_H_
