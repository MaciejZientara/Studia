using System;

class lista{
	class node{
		public int ind;
		public int val;
		public node next;
	}
	node root;
	
	void init(node x, int a){
		//from x.ind to a
		for(int i=x.ind+1; i<=a; i++){
			x.next=new node();
			int tmp=x.val;
			x=x.next;
			x.ind=i;
			x.val=next(tmp);
			//Console.WriteLine("x.val={0}",x.val);
		}
	}

	protected virtual int next(int i){
		return i+1;
	}
	
	public int element(int a){
		if(this.root==null){
			this.root=new node();
			init(this.root,a);
			return a;
		}
		node x=root;
		while(x.ind<a){
			if(x.next==null)
				init(x,a);
			x=x.next;
		}
		return x.val;
	}
	public int size(){
		node x=root;
		if(root==null)
			return 0;
		while(x.next!=null)
			x=x.next;
		return x.ind;
	}
}

class pierwsze:lista{
	bool pierwszy(int x){
		if(x==0 || x==1)
			return false;
		for(int i=2; i*i<=x; i++)
			if(x%i==0)
				return false;
		return true;
	}
	protected override int next(int i){
		i++;
		while(!pierwszy(i))
			i++;
		return i;
	}
}


class program{
	public static void Main(){
		lista a= new lista();
		Console.WriteLine("size={0}",a.size());
		Console.WriteLine("element={0}",a.element(40));
		Console.WriteLine("size={0}",a.size());
		Console.WriteLine("element={0}",a.element(35));
		Console.WriteLine("size={0}",a.size());
		Console.WriteLine("element={0}",a.element(50));
		Console.WriteLine("size={0}",a.size());
		Console.WriteLine("element={0}",a.element(52));
		Console.WriteLine("size={0}",a.size());
		lista b= new pierwsze();
		Console.WriteLine("size={0}",b.size());
		Console.WriteLine("element={0}",b.element(5));
		Console.WriteLine("size={0}",b.size());
		Console.WriteLine("element={0}",b.element(4));
		Console.WriteLine("size={0}",b.size());
		Console.WriteLine("element={0}",b.element(7));
		Console.WriteLine("size={0}",b.size());
		Console.WriteLine("element={0}",b.element(9));
		Console.WriteLine("size={0}",b.size());
	}
}
