/**
 * Copyright © CloudBackend AB 2021.
 */
package com.cbe;

public class Filter {

  public synchronized void delete() {}

  public Filter() {}

  /** Returns the requested data type I.E CBE::ItemType::Container, Object, Tag, ... */
  public int getDataType() {}

  /** Returns the query string that was set on the filter. I.E key:value (name:*) would be an example. */
  public String getQuery() {}

  /** Returns the settings on how the data was sorted and displayed. */
  public boolean getAscending() {}
  public boolean getDeleted() {}
  public boolean getContainerFirst() {}

  /** Returns the offset that was used for the filter in the query. */
  public long getOffset() {}

  /** Returns the value of the count that was set for the query.*/
  public long getCount() {}

  /** Returns if skipping the cache was used or not. */
  public boolean getByPassCache() {}

  /** Returns the order the query was sorted for check enum FilterOrder to see options for sorting. */
  public FilterOrder getItemOrder() {}

  /** Returns the parent container / container that was queried if it is in the cache, if not nullptr will be returned. (this is after a query was done not before.) */
  public Container container() {}

  /** Set which data types to query for. */
  public void setDataType(int arg0) {}

  /** Set the query string, e.x: Name:* (would search for all objects with the metadata key of Name). 
    * Note* if used with rootContainer id as the queried container the search will be on the whole account. 
  */
  public void setQuery(String arg0) {}

  /** Sets the Order in which data should be displayed: Ascending, show deleted items and if Containers should be displayed top down first.  */
  public void setAscending(boolean arg0) {}
  public void setDeleted(boolean arg0) {}
  public void setContainerFirst(boolean arg0) {}

  /** Set the offset for paging, offset is the item offset where to start your query. i.e:
    *  There is already a query of the first 99 items and to get the rest you've setOffest to 100 to get the next set.
  */
  public void setOffset(long arg0) {}

  /** Set the Number of items you want to get from a container. 
    * So if a container has 50 items but you only want the 10 first in ascending order then set ascending to true and setCount to 10. 
  */
  public void setCount(long arg0) {}

  /** Set the order of how data will be shown by the enum of FilterOrder ex: Titel first, Relevance, published e.t.c */
  public void setItemOrder(FilterOrder order) {}

  /** Set bool to true, to skip the cache. Force update from Server. */
  public void setByPassCache(boolean arg0) {}

}
