/**
 * Copyright © CloudBackend AB 2021.
 */
package com.cbe;

public final class FailedStatus {
  public final static FailedStatus FileNotFound = new FailedStatus("FileNotFound", cbeJNI.FileNotFound_get());
  public final static FailedStatus NotEnoughSpace = new FailedStatus("NotEnoughSpace", cbeJNI.NotEnoughSpace_get());
  public final static FailedStatus Aborted = new FailedStatus("Aborted", cbeJNI.Aborted_get());
  public final static FailedStatus Unknown = new FailedStatus("Unknown", cbeJNI.Unknown_get());

}

