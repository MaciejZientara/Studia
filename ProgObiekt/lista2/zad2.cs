using System;

class array{
	public class node{
		public int ind;
		public int val;
		public node next;
		public node prev;
	}
	public node x;
	public array(){
		this.x=new node();
	}
	public array(int a, int b){
		this.x=new node();
		for(int i=0; i<(b-a); i++){
			if(i==0)
				this.x.prev=null;
			this.x.ind=i;
			this.x.val=i;
			if(i==(b-a-1)){
				this.x.next=null;
				break;
			}
			this.x.next = new node();
			this.x.next.prev=this.x;
			//Console.WriteLine("init: x.val={0}, i={1}",this.x.val,i);
			this.x=this.x.next;
		}
		//Console.WriteLine("x.ind={0},x.val={1},x.prev.ind={2},x.prev.val={3}",x.ind,x.val,x.prev.ind,x.prev.val);
	}
	public void set(int i, int a){
		while(this.x.ind!=i){
			if(this.x.ind<i)
				this.x=this.x.next;
			if(this.x.ind>i)
				this.x=this.x.prev;
		}
		this.x.val=a;
	}
	public int get(int i){
		//Console.WriteLine("x.ind={0}",x.ind);
		while(this.x.ind!=i){
			//Console.WriteLine("get: ind={0},i={1}",this.x.ind,i);
			if(this.x.ind<i)
				this.x=this.x.next;
			if(this.x.ind>i)
				this.x=this.x.prev;
			//Console.WriteLine("while");
		}
		return this.x.val;
	}
}

class test{
	public static void Main(){
		array a1 = new array(0,100);
		array a2 = new array(0,100);
		array a3 = new array(0,100);
		
		for(int i=0; i<100; i++){
			a3.set(i,a1.get(i)+a2.get(i));
			//Console.WriteLine("for: {0}",a3.get(i));
		}
	//	for(int i=0; i<100; i++)
	//		Console.WriteLine("{0}",a3.get(i));
	}
}
