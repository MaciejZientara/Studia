import java.util.*;

class stack<T>{
	private class node{
		public T elem;
		public node next;
		public node(T S, node N){
			elem=S;
			next=N;
		}
	}

	node root;

	public stack(){
		root = null;
	}
	
	public void push(T s){
		//System.out.println("push s =" + s);
		node tmp = new node(s,root);
		root=tmp;
	}
	
	public T pop(){
		if(root==null)
			return null;
		T tmp = root.elem;
		root = root.next;
		return tmp;
	}

	public int size(){
		node tmp = root;
		int i = 0;
		while(tmp!=null){
			i = i + 1;
			tmp = tmp.next;
		}
		return i;
	}
	
	public T top(){
		return root.elem;
	}

	public void fullprint(){
		for(node i=root; i!=null; i=i.next)
			System.out.print(i.elem + " ");
		System.out.print("\n");
	}
}



class formula{
	private class node{
		public boolean leaf;
		public String elem;
		public node left;
		public node right;
		public node(String s){
			leaf = true;
			elem = s;
		}
		public node(String s, node l, node r){
			leaf = false;
			elem = s;
			left = l;
			right = r;
		}
	}
	
	private node root;

	public formula(String s){
		stack<String> ST = new stack<String>();
		stack<node> ND = new stack<node>();
		for(int i=0; i<s.length(); i++){
			//ST.fullprint();
			if(s.charAt(i)=='(' || s.charAt(i)==' ') continue;
			if(s.charAt(i)==')'){
				String r=ST.pop(),m=ST.pop(),l=ST.pop();
				//System.out.println("l,m,r : " + l + " " + m + " " + r);
				if(l==null){
					if(r==null){
						node tmp = ND.pop();
						ND.push(new node(m,ND.pop(),tmp));
					}
					else
						ND.push(new node(m,ND.pop(),new node(r)));
					ST.push(null);
					continue;
				}
				if(r==null){
					ND.push(new node(m,new node(l),ND.pop()));
					ST.push(null);
					continue;
				}
				ND.push(new node(m,new node(l),new node(r)));
				ST.push(null);
				//i++;
				continue;
			}
			String t = "";
			while(i<s.length() && s.charAt(i)!=' ' && s.charAt(i)!=')')
				t = t + s.charAt(i++);
			if(s.charAt(i)==')')
				i--;
			if(t=="")
				ST.push(null);
			else
				ST.push(t);
		}
		while(ST.size()>3){
			String r=ST.pop(),m=ST.pop(),l=ST.pop();
			//ST.fullprint();
			//System.out.println("while: l,m,r : " + l + " " + m + " " + r);
			if(l==null){
				if(r==null){
					node tmp = ND.pop();
					ND.push(new node(m,ND.pop(),tmp));
				}
				else
					ND.push(new node(m,ND.pop(),new node(r)));
				ST.push(null);
				continue;
			}
			if(r==null){
				ND.push(new node(m,new node(l),ND.pop()));
				ST.push(null);
				continue;
			}
			ND.push(new node(m,new node(l),new node(r)));
			ST.push(null);
		}
		root = ND.pop();
	}


	private HashMap<String, Integer> HS;
	private int calc_rec(node x){
		if(x.leaf){
			if(HS.get(x.elem)!=null)
				return HS.get(x.elem);
			return Integer.parseInt(x.elem);
		}
		switch(x.elem){
			case "+":
				return calc_rec(x.left) + calc_rec(x.right);
			case "-":
				return calc_rec(x.left) - calc_rec(x.right);
			case "*":
				return calc_rec(x.left) * calc_rec(x.right);
			case "/":
				return calc_rec(x.left) / calc_rec(x.right);
			default:
				return 0;
		}
	}
	public int calc(){
		HS = new HashMap<String, Integer>();
		return calc_rec(root);
	}

	public int calcHS(HashMap<String, Integer> hs){
		HS = new HashMap<String, Integer>();
		HS = hs;
		return calc_rec(root);
	}

	private String eq;
	private void tS_rec(node x){
		if(!x.leaf){
			eq = eq + "(";
			tS_rec(x.left);
			eq = eq + x.elem;
			tS_rec(x.right);
			eq = eq + ")";
		}
		else
			eq = eq + x.elem;
	}
	public String toString(){
		eq = "";
		tS_rec(root);
		return eq;
	}

}


public class zad2{
	public static void main(String[] args){
		//String s = "(((0 * 7) * (71 - 23)) + ((3 - (4 * 5)) * (0 - 1)))";
		//System.out.println("s = " + s);
		//formula f = new formula(s);
		//System.out.println(f.toString());
		//System.out.println("f.calc() = " + f.calc());
		String S = "(2 * (x * x))";
		formula f = new formula(S);
		HashMap<String, Integer> hash = new HashMap<String, Integer>();
		hash.put("x",2);
		System.out.println(f.calcHS(hash));
	}
}
