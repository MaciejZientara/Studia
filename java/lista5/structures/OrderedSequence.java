package structures;

public interface OrderedSequence<T extends Comparable<T>>{
    public void insert(T e) throws Exception;
    public void remove(T e) throws Exception;
    public T minimum() throws Exception;
    public T maximum() throws Exception;
    public T at(int pos) throws Exception;
    public boolean search(T e);
    public int index(T e) throws Exception;
    public int size();
    //...
}