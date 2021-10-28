import java.util.*;

class program{
	HashMap<String, Integer>HS;
	int iter;

	public class operation<T>{
		public String command;
		public String var;
		public T arg;
		
		public operation(String s){
			command = s;
		}
		public operation(String s, T o){
			command = s;
			arg = o;
		}
		public operation(String s, String v, T o){
			command = s;
			var = v;
			arg = o;
		}
		
		public boolean execute(){
			if(command.equals("print")){
				if(arg instanceof String && HS.get(arg)!=null)
					System.out.print(HS.get(arg));
				else
					System.out.print(arg);
			}
			if(command.equals("set")){
				if(arg instanceof formula){
					formula x = (formula)arg;
					HS.put(var, x.calcHS(HS));
				}
				else
					if(arg instanceof Integer)
						HS.put(var, (Integer)arg);
					else
						throw new IllegalArgumentException("argument not Integer or formula\n");
			}

			if(command.equals("==")){
				if(arg instanceof Integer)
					return HS.get(var)==(Integer)arg;
				else{
					if(arg instanceof String && HS.get(arg)!=null)
						return HS.get(var)==HS.get(arg);
					else
						throw new IllegalArgumentException("invalid argument\n");
				}
			}
			if(command.equals("!=")){
				if(arg instanceof Integer)
					return HS.get(var)!=(Integer)arg;
				else{
					if(arg instanceof String && HS.get(arg)!=null)
						return HS.get(var)!=HS.get(arg);
					else
						throw new IllegalArgumentException("invalid argument\n");
				}
			}
			if(command.equals("<")){
				if(arg instanceof Integer)
					return HS.get(var)<(Integer)arg;
				else{
					if(arg instanceof String && HS.get(arg)!=null)
						return HS.get(var)<HS.get(arg);
					else
						throw new IllegalArgumentException("invalid argument\n");
				}
			}
			if(command.equals("<=")){
				if(arg instanceof Integer)
					return HS.get(var)<=(Integer)arg;
				else{
					if(arg instanceof String && HS.get(arg)!=null)
						return HS.get(var)<=HS.get(arg);
					else
						throw new IllegalArgumentException("invalid argument\n");
				}
			}
			if(command.equals(">")){
				if(arg instanceof Integer)
					return HS.get(var)>(Integer)arg;
				else{
					if(arg instanceof String && HS.get(arg)!=null)
						return HS.get(var)>HS.get(arg);
					else
						throw new IllegalArgumentException("invalid argument\n");
				}
			}
			if(command.equals(">=")){
				if(arg instanceof Integer)
					return HS.get(var)>=(Integer)arg;
				else{
					if(arg instanceof String && HS.get(arg)!=null)
						return HS.get(var)>=HS.get(arg);
					else
						throw new IllegalArgumentException("invalid argument\n");
				}
			}



			return true;
		}
	}

	public class node{
		public boolean fi;
		public int number;
		public operation op;
		public node T;
		public node F;

		public node(operation o){
			fi = false;
			number = iter++;
			op = o;
			T = null;
			F = null;
		}
		public node(operation o, operation t, operation f){
			fi = true;
			number = iter++;
			op = o;
			T = new node(t);
			F = new node(f);
		}
		public node(operation o, node t, node f){
			fi = true;
			number = iter++;
			op = o;
			T = t;
			F = f;
		}
		public node(node x){
			fi = x.fi;
			number = x.number;
			op = x.op;
			T = x.T;
			F = x.F;
		}
		
		public node rec(int n){
			if(number==n)
				return this;
			if(!fi){
				if(T!=null){
					if(T.number<=n && number<T.number)
						return T.rec(n);
				}
				else{
					return null;
				}
				return null;
			}
			else{
				node t, f;
				if(T!=null){
					if(T.number<=n && number<T.number)
						t = T.rec(n);
					else
						t = null;
				}
				else
					t = null;
				if(F!=null){
					if(F.number<=n && number<F.number)
						f = F.rec(n);
					else
						f = null;
				}
				else
					f = null;
				if(t==null)
					return f;
				if(f==null)
					return t;
			}
			return null;
		}

		public void run(){
			if(op.execute()){
				if(T!=null)
					T.run();
			}
			else{
				if(F!=null)
					F.run();
			}
			return;
		}

		public void print(){
			System.out.println(number + " " + op.command);
			if(fi){
				System.out.println("TRUE");
				T.print();
				System.out.println("FALSE");
				F.print();
			}
			else{
				if(T!=null)
					T.print();
			}
			return;
		}

	}

	node root;

	public program(){
		HS = new HashMap<String, Integer>();
		root = null;
		iter = 0;
	}

	public void push(operation o){
		if(root==null){
			root = new node(o);
			return;
		}
		node tmp = root.rec(iter-1);

		if(tmp.T!=null){
			node tmpp = tmp.T;
			tmp.T = new node(o);
			tmp.T.T=tmpp;
		}
		else
			tmp.T = new node(o);
	}
	public void push(operation o, operation t, operation f){
		if(root==null){
			root = new node(o,t,f);
			return;
		}
		node tmp = root.rec(iter-1);
		if(tmp.T!=null)
			throw new IllegalArgumentException("can not push condition before a node\n");
		else
			tmp.T = new node(o,t,f);
	}
	public void push(operation o, node t, node f){
		if(root==null){
			root = new node(o,t,f);
			return;
		}
		node tmp = root.rec(iter-1);
		if(tmp.T!=null)
			throw new IllegalArgumentException("can not push condition before a node\n");
		else
			tmp.T = new node(o,t,f);	
	}
	public void push(int n, operation o){
		if(root==null){
			root = new node(o);
			return;
		}
		node tmp = root.rec(n);
		if(tmp.T!=null){
			node tmpp = tmp.T;
			tmp.T = new node(o);
			tmp.T.T=tmpp;
		}
		else
			tmp.T = new node(o);
	}
	public void push(int n, operation o, operation t, operation f){
		if(root==null){
			root = new node(o,t,f);
			return;
		}
		node tmp = root.rec(n);
		if(tmp.T!=null)
			throw new IllegalArgumentException("can not push condition before a node\n");
		else
			tmp.T = new node(o,t,f);
	}
	public void push(int n, operation o, node t, node f){
		if(root==null){
			root = new node(o,t,f);
			return;
		}
		node tmp = root.rec(n);
		if(tmp.T!=null)
			throw new IllegalArgumentException("can not push condition before a node\n");
		else
			tmp.T = new node(o,t,f);	
	}
	public void push(int n, int m){
		if(n<0 || m<0 || n>=iter || m>=iter)
			return;
		node tmpn = root.rec(n);
		node tmpm = root.rec(m);
		if(tmpn.T==null)
			tmpn.T = new node(tmpm);
		if(tmpn.T==null)
			System.out.println("NULL");
		return;
	}

	public void run(){
		root.run();
		return;
	}

	public void print(){
		root.print();
		return;
	}

}








public class zad3{
	public static void main(String[] args){
		program s = new program();
		s.push(s.new operation<Integer>("set","n",7));
		//s.print();
		s.push(s.new operation<Integer>("set","i",1));
		//s.print();
		s.push(s.new operation<Integer>("set","val",1));
		//s.print();
		s.push(s.new operation<String>(">=","n","i"),
				s.new operation<formula>("set","val",new formula("(val * i)")),
				s.new operation<String>("print","val"));
		s.push(4,s.new operation<formula>("set","i",new formula("(i + 1)")));
		s.push(6,3);
		//s.print();
		s.run();
	}
}
