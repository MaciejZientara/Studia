package structures;

import java.util.*;

public class OrderedList <T extends Comparable<T>> implements OrderedSequence<T>, Iterable<T>{
    protected class Node <T>{
        private Node<T> next;
        private T data;

        public T getData(){
            return data;
        }
        public Node<T> getNext(){
            return next;
        }
        public void setNext(Node<T> n){
            next = n;
        }

        public Node(T d){
            data = d;
        }
        public Node(T d, Node<T> n){
            data = d;
            next = n;
        }
    }

    private Node<T> start;

    public OrderedList(){
        start = null;
    }

    public void insert(T e) throws Exception{
        if(e == null)
            throw new NullPointerException();
        if(search(e))
            throw new Exception("element already in the list");
        if(start==null){
            start = new Node<T>(e);
            return;
        }//od min do max
        if(start.getData().compareTo(e)>0){
            start = new Node<T>(e,start);
            return;
        }
        Node<T> tmp = start;
        while(tmp.getNext()!=null){
            if(tmp.getNext().getData().compareTo(e)>0){
                tmp.setNext(new Node<T>(e,tmp.getNext()));
                return;
            }
            tmp = tmp.getNext();
        }
        tmp.setNext(new Node<T>(e));
    }

    public void remove(T e) throws Exception{
        if(e == null)
            throw new NullPointerException();
        if(start == null)
            return;
        if(start.getData()==e){
            start = start.getNext();
            return;
        }
        Node<T> tmp = start;
        while(tmp.getNext()!=null){
            if(tmp.getNext().getData()==e){
                tmp.setNext(tmp.getNext().getNext());
                return;
            }
            tmp = tmp.getNext();
        }
    }
    
    public T minimum() throws Exception{
        if(start == null)
            throw new Exception("empty list");
        return start.getData();
    }

    public T maximum() throws Exception{
        if(start == null)
            throw new Exception("empty list");
        Node<T> tmp = start;
        while(tmp.getNext()!=null){
            tmp = tmp.getNext();
        }
        return tmp.getData(); 
    }

    public T at(int pos) throws Exception{
        Node<T> tmp = start;
        int ret = 0;
        while(tmp!=null){
            if(ret==pos)
                return tmp.getData();
            ret++;
            tmp = tmp.getNext();
        }
        throw new Exception("out of bounds");
    }
    
    public boolean search(T e){
        Node<T> tmp = start;
        while(tmp!=null){
            if(tmp.getData()==e)
                return true;
            tmp = tmp.getNext();
        }
        return false;
    }
    
    public int index(T e) throws Exception{
        if(e == null)
            throw new NullPointerException();
        Node<T> tmp = start;
        int ret = 0;
        while(tmp!=null){
            if(tmp.getData()==e)
                return ret;
            ret++;
            tmp = tmp.getNext();
        }
        throw new Exception("element not found");
    }
    
    public int size(){
        Node<T> tmp = start;
        int ret = 0;
        while(tmp!=null){
            ret++;
            tmp = tmp.getNext();
        }
        return ret;
    }

    @Override
    public String toString (){
        String ret = "{ ";
        Node<T> tmp = start;
        while(tmp!=null){
            ret+=tmp.getData()+", ";
            tmp = tmp.getNext();
        }
        ret+="}";
        return ret;
    }
    
    
    
    
    protected Node<T> getStart(){
        return start;
    }

    public Iterator<T> iterator() { 
        return new ListIterator<T>(this); 
    } 

    private class ListIterator<T extends Comparable<T>> implements Iterator<T> { 
        OrderedList<T>.Node<T> current; 
          
        // initialize pointer to head of the list for iteration 
        public ListIterator(OrderedList<T> list) 
        { 
            current = list.getStart(); 
        } 
          
        // returns false if next element does not exist 
        public boolean hasNext() 
        { 
            return current != null; 
        } 
          
        // return current data and update pointer 
        public T next() 
        { 
            T data = current.getData(); 
            current = current.getNext(); 
            return data; 
        } 
          
        // // implement if needed 
        // public void remove() 
        // { 
        //     throw new UnsupportedOperationException(); 
        // } 
    } 
}



