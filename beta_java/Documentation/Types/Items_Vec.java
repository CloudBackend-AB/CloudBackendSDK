/**
 * Copyright © CloudBackend AB 2021.
 */
package com.cbe;

public class Items_Vec extends java.util.AbstractList<Item> implements java.util.RandomAccess {

  public synchronized void delete() {}

  public Items_Vec(Item[] initialElements) {
    this();
    reserve(initialElements.length);

    for (Item element : initialElements) {
      add(element);
    }
  }

  public Items_Vec(Iterable<Item> initialElements) {
    this();
    for (Item element : initialElements) {
      add(element);
    }
  }

  public Item get(int index) {
    return doGet(index);
  }

  public Item set(int index, Item e) {
    return doSet(index, e);
  }

  public boolean add(Item e) {
    modCount++;
    doAdd(e);
    return true;
  }

  public void add(int index, Item e) {
    modCount++;
    doAdd(index, e);
  }

  public Item remove(int index) {
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

  public Items_Vec() {}

  public Items_Vec(Items_Vec other) {}

  public long capacity() {}

  public void reserve(long n) {}

  public boolean isEmpty() {}

  public void clear() {}

  public Items_Vec(int count, Item value) {}

  private int doSize() {}

  private void doAdd(Item x) {}

  private void doAdd(int index, Item x) {}

  private Item doRemove(int index) {}

  private Item doGet(int index) {}

  private Item doSet(int index, Item val) {}

  private void doRemoveRange(int fromIndex, int toIndex) {}

}
