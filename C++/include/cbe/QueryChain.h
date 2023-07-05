/*
 Copyright © CloudBackend AB 2020-2023.
 */

#ifndef INCLUDE_CBE_QUERYCHAIN_H_
#define INCLUDE_CBE_QUERYCHAIN_H_

#include "cbe/delegate/JoinDelegate.h"

#include "cbe/Types.h"
#include "cbe/Filter.h"

#include <string>

namespace CBI {
class QueryChain;
using QueryChainPtr = std::shared_ptr<CBI::QueryChain>;
} // namespace CBI

namespace cbe {

/**
 * @brief to do a search for Object combining more than one Container table.
 * 
 */
class QueryChain {
public:
  using JoinDelegatePtr = delegate::JoinDelegatePtr;
  /**
   * Performs a join request in conjunction with a query, or previous join, 
   * to get related items from another container.<br>
   * <b>Asynchronous</b> version of this service function.
   *
   * @param containerToQuery
   *          The container of which the join shall be done with.
   * @param key1
   *          The key from the previous query on which to perform the join.
   * @param key2
   *          The key on objects in \p containerToQuery on which to perform the
   *          join
   * @param joinDelegate
   *          Pointer to a @ref delegate::JoinDelegate "JoinDelegate"
   *          instance, implemented by the user, that receives the response of
   *          this query request.<br>
   *          I.e., either of the JoinDelegate callback functions
   *          @ref delegate::JoinDelegate::onJoinSuccess()
   *               "onJoinSuccess()" or
   *          @ref delegate::JoinDelegate::onJoinError()
   *               "onJoinError()"
   *          will be called.
   */
  QueryChain join(Container       containerToQuery,
                  std::string     key1,
                  std::string     key2,
                  JoinDelegatePtr joinDelegate);
  /**
   * Same as join(Container,std::string,std::string,delegate::JoinDelegatePtr),
   * but with an additional Filter parameter \p constraints .
   *
   * @param constraints A filter that provides any constraints for the query.
   */
  QueryChain join(Container       containerToQuery,
                  std::string     key1,
                  std::string     key2,
                  Filter          constraints,
                  JoinDelegatePtr joinDelegate);
  /**
   * Same as join(Container,std::string,std::string,delegate::JoinDelegatePtr),
   * but with an additional Container parameter \p containerForResults .
   *
   * @param containerForResults Can be the Container from the previous query if
   *                            desired items should be from that container.
   */
  QueryChain join(Container       containerToQuery,
                  std::string     key1,
                  std::string     key2,
                  Container       containerForResults,
                  JoinDelegatePtr joinDelegate);
  /**
   * Same as join(Container,std::string,std::string,Container,delegate::JoinDelegatePtr),
   * but with an additional Filter parameter \p constraints .
   *
   * @param constraints A filter that provides any constraints for the query.
   */
  QueryChain join(Container       containerToQuery,
                  std::string     key1,
                  std::string     key2,
                  Filter          constraints,
                  Container       containerForResults,
                  JoinDelegatePtr joinDelegate);

  /**
   * Returns the QueryResult provided to the delegate after the the last query()
   * or join() call.
   * If this method is called before the delegate call, an empty QueryResult is
   * returned.
   */
  QueryResult getQueryResult() const;

  QueryChain(cbe::DefaultCtor);
  ~QueryChain();

  explicit operator bool() const;
private:
  struct Impl;
  std::shared_ptr<Impl> pImpl{};

  friend class CloudBackend;
  friend class Container;
  friend class QueryChainSync;
  QueryChain(CBI::QueryChainPtr cbiPeerPtr);
  CBI::QueryChain& getCbiPeer() const;
}; // class QueryChain

/**
 * @brief Extension of class QueryChain
 * 
 * Extension of class QueryChain offering join() methods that do not require
 * a delegate.<br>
 * Instead, the delegate passed as argument in previous invocation in the
 * call chain - i.e.:
 * <ul>
 *   <li> CloudBackend::query(ContainerId,delegate::QueryJoinDelegatePtr) and
 *        overloads.
 *   <li> Container::query(delegate::QueryJoinDelegatePtr) and overloads.
 *   <li> QueryChain::join(join(Container,std::string,std::string,JoinDelegatePtr)
 *        and overloads.
 * </ul>
 * will be used.
 */
class QueryChainExt : public QueryChain {
public:
  using QueryChain::join; // Bring in the join()- methods from the base class

  /**
   * Performs a join request in line with function
   * @ref QueryChain::join(Container,std::string,std::string,delegate::JoinDelegatePtr)
   * "join()" in class QueryChain.<br>
   * If a call to this join function is chained with a previous call to a query,
   * only such a query functions accepting a
   * @ref delegate::QueryJoinDelegate "QueryJoinDelegate" can be used as:
   * <ul>
   * <li> CloudBackend::query(ContainerId,delegate::QueryJoinDelegatePtr)
   * <li> CloudBackend::query(ContainerId,Filter,delegate::QueryJoinDelegatePtr)
   * <li> Container::query(delegate::QueryJoinDelegatePtr)
   * <li> Container::query(Filter,delegate::QueryJoinDelegatePtr)
   * </ul>
   * <b>Asynchronous</b> version of this service function.
   * \see QueryChain::join(Container,std::string,std::string,delegate::JoinDelegatePtr)
   */
  QueryChainExt join(Container    containerToQuery,
                     std::string  key1,
                     std::string  key2);
  /**
   * \see QueryChain::join(Container,std::string,std::string,Filter,delegate::JoinDelegatePtr)
   */
  QueryChainExt join(Container    containerToQuery,
                     std::string  key1,
                     std::string  key2,
                     Filter       constraints);
  /**
   * \see QueryChain::join(Container,std::string,std::string,Container,delegate::JoinDelegatePtr)
   */
  QueryChainExt join(Container    containerToQuery,
                     std::string  key1,
                     std::string  key2,
                     Container    containerForResults);
  /**
   * \see QueryChain::join(Container,std::string,std::string,Filter,Container,delegate::JoinDelegatePtr)
   */
  QueryChainExt join(Container    containerToQuery,
                     std::string  key1,
                     std::string  key2,
                     Filter       constraints,
                     Container    containerForResults);
private:
  friend class CloudBackend;
  friend class Container;
  // Inherit base class' constructor
  using QueryChain::QueryChain;

  QueryChainExt(QueryChain&& rh);
}; // class QueryChainExt

} // namespace cbe

#endif // INCLUDE_CBE_ACCOUNT_H_
