/**
 * Copyright © CloudBackend AB 2021.
 */
package com.cbe;

public class QueryResult {

  public synchronized void delete() {}

  public QueryResult() {}

  /** Returns a copy of the filter used for query.*/
  public Filter filter() {}

  /**
    * 
    * Returns a copy of a vector containing the items for the queryResult.    
    * The queryResult will update when new data comes in but the copy will not.
    * If iterating make sure to create a variable for a local copy.
    * @return vector<CBE::ItemPtr> contains the items matching the query.
  */
  public Items_Vec getItemsSnapshot() {}

  /** items loaded in the queryResult. */ 
  public java.math.BigInteger itemsLoaded() {}

  /**  total items in the cloud matching the query result.  This may be more than loaded. */
  public java.math.BigInteger totalCount() {}

  /** Out of the total Items loaded this returns the number of specific types of Items loaded */
  public java.math.BigInteger objectsLoaded() {}
  public java.math.BigInteger containersLoaded() {}

  /** Checks the QueryResult / cache to find if item is loaded locally in to the result. */
  public boolean containsItem(java.math.BigInteger itemId) {}

}
