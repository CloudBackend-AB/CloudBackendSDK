/**
 * Copyright © CloudBackend AB 2021.
 */
package com.cbe;

public final class FilterOrder {
  public final static FilterOrder Title = new FilterOrder("Title", cbeJNI.Title_get());
  public final static FilterOrder Relevance = new FilterOrder("Relevance", cbeJNI.Relevance_get());
  public final static FilterOrder Published = new FilterOrder("Published", cbeJNI.Published_get());
  public final static FilterOrder Updated = new FilterOrder("Updated", cbeJNI.Updated_get());
  public final static FilterOrder Length = new FilterOrder("Length", cbeJNI.Length_get());
  public final static FilterOrder S1 = new FilterOrder("S1", cbeJNI.S1_get());
  public final static FilterOrder S2 = new FilterOrder("S2", cbeJNI.S2_get());
  public final static FilterOrder S3 = new FilterOrder("S3", cbeJNI.S3_get());
  public final static FilterOrder S4 = new FilterOrder("S4", cbeJNI.S4_get());
}

