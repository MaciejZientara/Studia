using System;

public class slownik<K,T>{
	protected class node{
		public K key;
		public T val;
		public node next;
	}
	
	protected node root;
	
	public slownik(){
		root = new node();
	}
	
	public void ad(K k, T t){
		node tmp = new node();
		tmp.val=t;
		tmp.key=k;
		if(root==null)
			root=tmp;
		else{
			tmp.next=root;
			root=tmp;
		}
	}

	public T find(K k){
		node tmp=root;
		while(tmp!=null){
			if(k.Equals(tmp.key))
				break;
			else
				tmp=tmp.next;
		}
		if(tmp!=null)
			return tmp.val;
		else
			return default(T);
	}

	public void rm(K k){
		node tmp = root;
		if(!k.Equals(tmp.key)){
			while(tmp.next!=null){
				//Console.WriteLine("{0}",tmp.next.key);
				if(k.Equals(tmp.next.key))
					break;
				else
					tmp=tmp.next;
			}
			if(tmp.next!=null)
				tmp.next=tmp.next.next;
		}
		else
			root=root.next;
	}

};

class main{
	public static void Main(){
		slownik<int,string> s1 = new slownik<int,string>();
		Console.WriteLine("{0}",s1.find(1));
		s1.ad(0,"zero");
		s1.ad(1,"jeden");
		s1.ad(2,"dwa");
		Console.WriteLine("{0}",s1.find(0));
		Console.WriteLine("{0}",s1.find(1));
		Console.WriteLine("{0}",s1.find(2));
		s1.rm(1);
		Console.WriteLine("{0}",s1.find(0));
		Console.WriteLine("{0}",s1.find(1));
		Console.WriteLine("{0}",s1.find(2));
	}
}

