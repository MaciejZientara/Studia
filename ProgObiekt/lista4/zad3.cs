using System;

interface Igraph{
	void print();
	void rm(string a, string b);
	void ad(string a, string b);
	void pathfind(string a, string b);
}

public class graph_array : Igraph{
	public class elem{
		public string first;
		public string second;
		public elem next;
	
		public elem(string a, string b){
			first=a;
			second=b;
		}
	}	
	
	elem root;

	public graph_array(){
		root = null;
	}

	public void print(){
		elem tmp=root;
		while(tmp!=null){
			Console.Write("({0},{1}) ",tmp.first,tmp.second);
			tmp=tmp.next;
		}	
		Console.WriteLine("");
		return;
	}
	
	public void rm(string a, string b){
		if(root==null)
			return;

		if	((root.first.Equals(a) && root.second.Equals(b)) ||
			(root.first.Equals(b) && root.second.Equals(a))){
			root=root.next;
			return;
		}

		if(root.next==null)
			return;
		
		elem tmp=root;
		while((!tmp.next.first.Equals(a) || !tmp.next.second.Equals(b)) &&
				(!tmp.next.first.Equals(b) || !tmp.next.second.Equals(a))){
			if(tmp.next.next==null)
				break;
			tmp=tmp.next;
		}
		if(tmp.next.next!=null)
			tmp.next=tmp.next.next;
		else
			if((tmp.next.first.Equals(a) && tmp.next.second.Equals(b)) ||
				(tmp.next.first.Equals(b) && tmp.next.second.Equals(a)))
				tmp.next=null;
			
		return;
	}

	public void ad(string a, string b){
		if(root==null){
			root=new elem(a,b);
			return;
		}
		elem tmp=root;		
		while((!tmp.first.Equals(a) || !tmp.second.Equals(b)) &&
				(!tmp.first.Equals(b) || !tmp.second.Equals(a))){
			if(tmp.next==null){
				tmp.next = new elem(a,b);
				break;
			}
			tmp=tmp.next;
		}
		return;
	}

	private int length(){
		int i=0;
		elem tmp=root;
		while(tmp!=null){
			i=i+1;
			tmp=tmp.next;
		}	
		return i;
	}

	private void gen(int n, int m){
		Random rnd = new Random();
		while(length()!=m){
			string a = rnd.Next(n).ToString();
			string b = rnd.Next(n).ToString();
			while(a.Equals(b))
				b = rnd.Next(n).ToString();
			ad(a,b);
		}
		return;
	}
	public graph_array(int n, int m){
		root= null;
		gen(n,m);
	}

	private string[]name;
	private int[] visited;
	
	private int vercount(){
		return name.Length;
	}

	private void pathrec(string x, int ile){
		for(int i=0; i<vercount(); i++)
			if(name[i].Equals(x)){
				visited[i]=ile;
				break;
			}

		elem tmp = root;
		while(tmp!=null){
			if(tmp.first.Equals(x)){
				int k=0;
				while(!name[k].Equals(tmp.second))
					k=k+1;
				if(ile+1<visited[k])
					pathrec(tmp.second,ile+1);
			}
			if(tmp.second.Equals(x)){
				int k=0;
				while(!name[k].Equals(tmp.first))
					k=k+1;
				if(ile+1<visited[k])
					pathrec(tmp.first,ile+1);
			}
			tmp=tmp.next;
		}
		return;
	}

	public void pathfind(string a, string b){
		name = new string[length()];
		elem tmp=root;
		int i=0;
		while(tmp!=null){
			bool czy=true;
			for(int j=0; j<i; j++)
				if(name[j].Equals(tmp.first)){
					czy=false;
					break;
				}
			if(czy){
				name[i]=tmp.first;
				i=i+1;
			}
			czy=true;
			for(int j=0; j<i; j++)
				if(name[j].Equals(tmp.second)){
					czy=false;
					break;
				}
			if(czy){
				name[i]=tmp.second;
				i=i+1;
			}
			tmp=tmp.next;
		}

		for(int w=0; w<length(); w++)	
			if(name[w]==default(string)){
				Array.Resize<string>(ref name, w);
				break;
			}

		visited= new int[vercount()];
		for(int q=0; q<vercount(); q++)
			visited[q]=length()+2;
		
		bool exa=false, exb=false;
		for(int k=0; k<vercount(); k++){
			if(name[k].Equals(a))
				exa=true;
			if(name[k].Equals(b))
				exb=true;
		}
		if(!exa || !exb){
			Console.WriteLine("no connection");
			return;
		}

		pathrec(a,0);
		for(int k=0; k<vercount(); k++)
			if(name[k].Equals(b)){
				if(visited[k]==length()+2)
					Console.WriteLine("no connection");
				else
					Console.WriteLine("{0}",visited[k]);
				break;
			}

		return;
	}
}

public class graph_list : Igraph{
	public class node{
		public string neighbour;
		public node next;

		public node(string b){
			neighbour = b;
		}
	}
	
	public class NODE{
		public string ver;
		public NODE next;
		public node root;

		public NODE(string a){
			ver = a;
		}
		
		public void rmv(string b){
			if(root==null)
				return;
			if(root.neighbour==b){
				root=root.next;
				return;
			}
			if(root.next==null)
				return;
			node tmp=root;
			while(tmp.next.next!=null){
				if(tmp.next.neighbour.Equals(b))
					break;
				tmp=tmp.next;
			}
			if(tmp.next.neighbour.Equals(b))
				tmp.next=tmp.next.next;
		}

	}
	
	NODE ROOT;

	public graph_list(){
		ROOT = null;
	}
	
	public void print(){
		Console.WriteLine("");
		for(NODE TMP=ROOT; TMP!=null; TMP=TMP.next){
			Console.Write("{0}: ", TMP.ver);
			for(node tmp=TMP.root; tmp!=null; tmp=tmp.next)
					Console.Write("{0} ", tmp.neighbour);
			Console.WriteLine("");
		}
		return;
	}

	public void rm(string a, string b){
		if(ROOT==null)
			return;
		if(ROOT.ver.Equals(a)){
			ROOT.rmv(b);
			if(ROOT.root==null)
				ROOT=ROOT.next;
			rm(b,a);
			return;
		}
		if(ROOT.next==null)
			return;
		NODE TMP = ROOT;
		while(TMP.next.next!=null){
			if(TMP.next.ver.Equals(a))
				break;
			TMP=TMP.next;
		}
		if(TMP.next.ver.Equals(a)){
			TMP.next.rmv(b);	
			if(TMP.next.root==null){
				TMP.next=TMP.next.next;
				rm(b,a);
			}
		}
		return;
	}
	
	public void ad(string a, string b){
		if(ROOT==null){
			ROOT = new NODE(a);
			ROOT.root = new node(b);
			ad(b,a);
			return;
		}

		NODE TMP = ROOT;
		while(!TMP.ver.Equals(a)){
			if(TMP.next==null){
				TMP.next = new NODE(a);
				TMP.next.root = new node(b);
				ad(b,a);
				break;
			}
			TMP=TMP.next;
		}
		if(TMP.ver.Equals(a)){
			node tmp = TMP.root;
			while(!tmp.neighbour.Equals(b)){
				if(tmp.next==null){
					tmp.next = new node(b);
					ad(b,a);
					break;
				}
				tmp=tmp.next;
			}
		}
		return;
	}

	private int length(){
		int i=0;
		NODE TMP=ROOT;
		while(TMP!=null){
			node tmp = TMP.root;
			while(tmp!=null){
				i=i+1;
				tmp=tmp.next;
			}
			TMP=TMP.next;
		}	
		return i/2;
	}
	private void gen(int n, int m){
		Random rnd = new Random();
		while(length()!=m){
			string a = rnd.Next(n).ToString();
			string b = rnd.Next(n).ToString();
			while(a.Equals(b))
				b = rnd.Next(n).ToString();
			ad(a,b);
		}
		return;
	}
	public graph_list(int n, int m){
		ROOT = null;
		gen(n,m);
	}
	
	private string[]name;
	private int[] visited;
	
	private int vercount(){
		return name.Length;
	}

	private void pathrec(string x, int ile){
		for(int i=0; i<vercount(); i++)
			if(name[i].Equals(x)){
				visited[i]=ile;
				break;
			}

		NODE TMP=ROOT;
		while(!TMP.ver.Equals(x))
			TMP=TMP.next;

		node tmp=TMP.root;
		while(tmp!=null){
			int i;
			for(i=0;i<vercount();i++)
				if(name[i].Equals(tmp.neighbour))
					break;
			if(ile+1<visited[i])
				pathrec(tmp.neighbour,ile+1);
			tmp=tmp.next;
		}
		return;
	}

	

	public void pathfind(string a, string b){
		int q=0;
		NODE TMP = ROOT;
		while(TMP!=null){
			q=q+1;
			TMP=TMP.next;
		}
		name = new string[q];
		visited = new int[q];
		TMP=ROOT;
		for(int i=0; i<q; i++){
			visited[i]=length()+2;
			name[i]=TMP.ver;
			TMP=TMP.next;
		}
		bool exa=false, exb=false;
		for(int i=0; i<q; i++){
			if(name[i].Equals(a))
				exa=true;
			if(name[i].Equals(b))
				exb=true;
		}
		if(!exa || !exb){
			Console.WriteLine("no connection");
			return;
		}
		
		pathrec(a,0);
		
		for(int i=0; i<q; i++)
			if(name[i].Equals(b)){
				if(visited[i]==length()+2)
					Console.WriteLine("no connection");
				else
					Console.WriteLine("{0}",visited[i]);
				break;
			}

		return;
	}
}


class main{
	public static void Main(){
		graph_array a1 = new graph_array(4,5);
		a1.print();
		a1.ad("4","5");
		a1.ad("3","4");
		a1.print();
		a1.pathfind("0","5");
		/*a1.rm("4","5");
		a1.rm("3","4");
		a1.print();
		a1.rm("0","1");
		a1.print();*/
		graph_list a2 = new graph_list(4,5);
		a2.print();
		a2.ad("4","5");
		a2.ad("3","4");
		a2.print();
		a2.pathfind("0","5");
		/*a2.rm("4","5");
		a2.rm("3","4");
		a2.print();
		a2.rm("0","1");
		a2.print();
		graph_list a3 = new graph_list();
		a3.ad("0","1");
		a3.ad("1","2");
		a3.rm("0","1");
		a3.print();*/
	}
}
