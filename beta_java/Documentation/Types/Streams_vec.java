/**
 * Copyright © CloudBackend AB 2021.
 */
package com.cbe;

public class Streams_vec extends java.util.AbstractList<Stream> implements java.util.RandomAccess {

  public synchronized void delete() {}

  public Streams_vec(Stream[] initialElements) {
    this();
    reserve(initialElements.length);

    for (Stream element : initialElements) {
      add(element);
    }
  }

  public Streams_vec(Iterable<Stream> initialElements) {
    this();
    for (Stream element : initialElements) {
      add(element);
    }
  }

  public Stream get(int index) {
    return doGet(index);
  }

  public Stream set(int index, Stream e) {
    return doSet(index, e);
  }

  public boolean add(Stream e) {
    modCount++;
    doAdd(e);
    return true;
  }

  public void add(int index, Stream e) {
    modCount++;
    doAdd(index, e);
  }

  public Stream remove(int index) {
    modCount++;
    return doRemove(index);
  }

  protected void removeRange(int fromIndex, int toIndex) {
    modCount++;
    doRemoveRange(fromIndex, toIndex);
  }

  public int size() {
    return doSize();
  }

  public Streams_vec() {}

  /** Copy operator/constructor */
  public Streams_vec(Streams_vec other) {}

  public long capacity() {}

  public void reserve(long n) {}

  public boolean isEmpty() {}

  public void clear() {}

  public Streams_vec(int count, Stream value) {}

  private int doSize() {}

  private void doAdd(Stream x) {}

  private void doAdd(int index, Stream x) {}

  private Stream doRemove(int index) {}

  private Stream doGet(int index) {}

  private Stream doSet(int index, Stream val) {}

  private void doRemoveRange(int fromIndex, int toIndex) {}

}
