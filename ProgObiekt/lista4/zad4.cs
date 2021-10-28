using System;

public class gram{
	protected string word;
	protected string[] term;
	protected string[] non;
	protected string[] rule;
	
	private class node{
		public bool end;
		public string elem;
		public node[] next;
		
		public node(){
			end = false;
			elem = "S";
			next = new node[1];
		}

		public node(string s){
			end = false;
			elem = s;
			next = new node[s.Length];
		}

		public node(int i, string[] tab){
			end = true;
			elem = tab[i];
		}

	}

	node root;

	public gram(){
		root = new node();
		word=default(string);
		term = new string[] {"1","2","3","4","5","6","7","8","9","0"};
		non = new string[] {"S+S","S*S","(S)"};
		rule = new string[] {"((","))"};
	}
	
	public gram(string[] t, string[] n, string[] r){
		root = new node();
		word = default(string);
		term=t;
		non=n;
		rule=r;
	}

	private void rec(node x, int q, int end, Random rnd){
		for(int i=0; i<x.elem.Length; i++){
			if(x.elem[i]=='S'){
				if(q==end){
					x.next[i] = new node(rnd.Next(term.Length),term);
				}
				else{
					x.next[i] = new node(non[rnd.Next(non.Length)]);
					rec(x.next[i],q+1,end,rnd);
				}
			}
		}
	}

	private void dnf(node x){
		if(x.end){
			word = word + x.elem;
			return;
		}
		for(int i=0; i<x.elem.Length; i++)
			if(x.elem[i]!='S')
				word = word + x.elem[i];
			else
				dnf(x.next[i]);
		return;
	}

	private bool proper(string s){
		for(int i=0; i<s.Length; i++)
			if(rule.Length>0)
				foreach(string c in rule){
					bool ret = true;
					for(int j=0; j<c.Length; j++)
						if(i+j>=s.Length || c[j]!=s[i+j]){
							ret = false;
							break;
						}
					if(ret)
						return false;
				}
		return true;
	}

	public string gen(int end){
		Random rnd = new Random();
		rec(root,0,end,rnd);
		dnf(root);
		while(!proper(word)){
			word = default(string);
			rec(root,0,end,rnd);
			dnf(root);
		}
		return word;
	}

}


class main{
	public static void Main(){
		gram g = new gram();
		Console.WriteLine("{0}",g.gen(4));
		string[] t = new string[] {""};
		string[] n = new string[] {"aSb"};
		string[] r = new string[0];
		gram h = new gram(t,n,r);
		Console.WriteLine("{0}",h.gen(4));
	}
}
