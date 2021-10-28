using System;

public class Lista<T>{
	protected class node{
		public T val;
		public node next;
		public node prev;
	}
	
	protected node start;
	protected node end;
	
	public Lista(){
		start=null;
		end=null;
	}

	public bool empty(){
		return (start==null && end==null);
	}
	

	public void add_front(T x){
		node tmp=new node();
		tmp.val=x;
		if(empty()){
			start=tmp;
			end=tmp;
		}
		else{
			tmp.next=start;
			start.prev=tmp;
			start=tmp;
		}
	}

	public void add_back(T x){
		node tmp=new node();
		tmp.val=x;
		if(empty()){
			start=tmp;
			end=tmp;
		}
		else{
			tmp.prev=end;
			end.next=tmp;
			end=tmp;
		}
	}

	public void pop_front(){
		Console.WriteLine("{0}",start.val);
		if(start==end){
			start=null;
			end=null;
		}
		else{
			start=start.next;
		}
	}

	public void pop_back(){
		Console.WriteLine("{0}",end.val);
		if(start==end){
			start=null;
			end=null;
		}
		else{
			end=end.prev;
		}
	}
	
	public T front(){
		return start.val;
	}
	public T back(){
		return end.val;
	}

};

class main{
	public static void Main(){
		Lista<int> a1 = new Lista<int>();
		Lista<char> a2 = new Lista<char>();
		if(a1.empty())
			Console.WriteLine("a1 pusta");
		if(a2.empty())
			Console.WriteLine("a2 pusta");
		a1.add_front(5);
		a2.add_back('v');
		if(a1.empty())
			Console.WriteLine("a1 pusta");
		if(a2.empty())
			Console.WriteLine("a2 pusta");
		Console.WriteLine("a1 front={0}",a1.front());
		Console.WriteLine("a1 back={0}",a1.back());
		Console.WriteLine("a2 front={0}",a2.front());
		Console.WriteLine("a2 back={0}",a2.back());


	}
}



