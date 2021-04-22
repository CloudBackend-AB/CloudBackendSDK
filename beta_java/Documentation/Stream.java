/**
 * Copyright © CloudBackend AB 2021.
 */
package com.cbe;

public class Stream {

  public synchronized void delete() {}

  /** Do not set stream id or length, these will be retrived by getStreams in class Object. Future releases will only expose getters. */
  public void set_streamId(java.math.BigInteger value) {}
  public java.math.BigInteger get_streamId() {}
  public void set_length(long value) {}
  public long get_length() {}

}
