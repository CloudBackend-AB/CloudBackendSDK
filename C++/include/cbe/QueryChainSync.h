#ifndef CBE__QueryChainSync_h__
#define CBE__QueryChainSync_h__

// #include "cbe/QueryResult.h"
#include "cbe/delegate/JoinDelegate.h"
#include "cbe/delegate/QueryJoinDelegate.h"

#include "cbe/util/Exception.h"

namespace cbe {
/**
 * @brief Synchronous version of querychain.
 * 
 * Provides the possibility to chain synchronous join() calls after either a
 * query() or join() in a synchronous call.
 */
class QueryChainSync : public QueryResult {
public:
  ~QueryChainSync();

  /*! \see delegate::JoinDelegate::Exception */
  using JoinException = delegate::JoinDelegate::Exception;
  /**
   * <b>Synchronous</b> version of
   * @ref QueryChain::join(Container,std::string,std::string,delegate::JoinDelegatePtr)
   * "QueryChain::query()", and throws
   * an exception, #JoinException, in case of a failed join.
   * \see QueryChain::join(Container,std::string,std::string,delegate::JoinDelegatePtr)
   *
   * @param containerToQuery
   *          The container the join shall be done with.
   * @param key1
   *          The key from the previous query on which to perform the join.
   * @param key2
   *          The key on objects in \p containerToQuery on which to perform the
   *          join
   *
   * @throws #JoinException
   *          Possible only if current instance of QueryChainSync has been
   *          retrieved by a call to any of the following methods:
   *          <ul>
   *          <li> CloudBackend::query(ContainerId)
   *          <li> CloudBackend::query(ContainerId,Filter)
   *          <li> Container::query()
   *          <li> Container::query(Filter)
   *          </ul>
   */
  QueryChainSync join(Container   containerToQuery,
                      std::string key1,
                      std::string key2);
  /**
   * Same as join(Container,std::string,std::string),
   * but with an additional Filter parameter \p constraints.
   * \see join(Container,std::string,std::string)
   *
   * @param constraints A Filter that provides any constraints for the query.
   */
  QueryChainSync join(Container   containerToQuery,
                      std::string key1,
                      std::string key2,
                      Filter      constraints);
  /**
   * Same as join(Container,std::string,std::string),
   * but with an additional Container parameter \p containerForResults.
   * \see join(Container,std::string,std::string)
   *
   * @param containerForResults The Container from the previous query, if
   *                            desired @ref cbe::Item "items" originate from
   *                            that container.
   */
  QueryChainSync join(Container   containerToQuery,
                      std::string key1,
                      std::string key2,
                      Container   containerForResults);
  /**
   * Same as join(Container,std::string,std::string,Container),
   * but with an additional Filter parameter \p constraints.
   * \see join(Container,std::string,std::string,Container)
   *
   * @param constraints A Filter that provides any constraints for the query.
   */
  QueryChainSync join(Container   containerToQuery,
                      std::string key1,
                      std::string key2,
                      Filter      constraints,
                      Container   containerForResults);

  /**
   * Returns the QueryResult provided to the delegate after the the last query()
   * or join() call.
   */
  QueryResult getQueryResult() const;

private:
  struct  Impl;
  std::shared_ptr<Impl> pImpl;

  friend CloudBackend;
  friend Container;

template <class ImplT, typename... QueryAsyncArgsTs>
friend
  QueryChainSync querySync(ImplT& impl,
                      delegate::QueryJoinDelegate::ErrorInfo*  queryJoinError,
                      const char*                               fnName,
                      QueryAsyncArgsTs&&...                     queryAsyncArgs);
  /*! \see delegate::JoinDelegate::ErrorInfo */
  using QueryJoinError = delegate::QueryJoinDelegate::ErrorInfo;
  QueryChainSync(QueryResult&&    queryResult,
                 QueryChain&&     queryChain,
                 QueryJoinError*  errorInfo);
  QueryChainSync(QueryResult&&    queryResult,
                 QueryChain&&     queryChain,
                 const Impl&      impl);
  QueryChainSync(std::size_t  joinCallErrorIndex);
}; // class QueryChainSync


} // namespace cbe


#endif // #ifndef CBE__QueryChainSync_h__
