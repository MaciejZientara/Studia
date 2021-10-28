import java.util.*;

class queue<T>{
	class node{
		public T elem;
		public node next;

		public node(T e){
			elem = e;
		}
	}

	int max;
	int am;	
	node root;

	public queue(int m){
		max=m;
		am=0;
		root=null;
	}

	public void push(T e){
		if(root==null){
			root = new node(e);
			am++;
			return;
		}
		node tmp = root;
		while(tmp.next!=null)
			tmp = tmp.next;
		tmp.next = new node(e);
		am++;
	}
	
	public void pop(){
		if(root==null)
			return;
		root=root.next;
		am--;
	}

	public boolean isFull(){
		return (am>=max);
	}

}

class buff<T> extends Thread{
	queue<T> q;

	public buff(int n){
		q = new queue<>(n);
	}

	public void work(){
		Thread prod = new	Thread(){
			public void run(){
				try{
					synchronized(this){
						while(true){
							if(q.isFull()){
								System.out.println("full wait");
								wait();
							}
							System.out.println("push");
							q.push(null);
							
							Thread.sleep(5);
						}
					}
				}
				catch(InterruptedException e){
					System.out.println("Thread interrupted " + e);
				}
			}
		};
	
		Thread cons = new	Thread(){
			public void run(){
				try{
					synchronized(this){
						while(true){
							if(q.isFull()){
								System.out.println("full pop");
								q.pop();
								notifyAll();
							}
							System.out.println("pop");
							q.pop();
							
							Thread.sleep(5);
						}
					}
				}
				catch(Exception e){
					System.out.println("Thread interrupted " + e);
				}
			}
		};

		prod.start();
		cons.start();	
	}
}


public class zad3{
	public static void main(String[] argc){
		buff<Integer> b = new buff<>(2);
		b.work();
	}

}
