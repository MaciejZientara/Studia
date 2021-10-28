import java.util.*;

class multi_merge extends Thread{
	int[] tab;	

	void merge(int B, int E){
		int[] tmp = new int[E-B+1];
		int M=(B+E)/2;
		int l = B, r = M+1;
		int i=0;
		while(l<=M && r<=E){
			if(tab[l]<tab[r])
				tmp[i++]=tab[l++];
			else
				tmp[i++]=tab[r++];	
		}
		while(l<=M){
			tmp[i++]=tab[l++];
		}
		while(r<=E){
			tmp[i++]=tab[r++];
		}
		for(i=0;i<E-B+1;i++)
			tab[i+B]=tmp[i];
	}
	
	void mergesort(int B, int E){
		if(B<E){
			int M=(B+E)/2;
			mergesort(B,M);
			mergesort(M+1,E);
			merge(B,E);
		}	
	}

	void multi(int B, int E, int available){
		if(B<E){
			if(available<=1)
				mergesort(B,E);
			else{
				int M =(B+E)/2;

				Thread L = new Thread(){
					public void run(){
						multi(B,M,available-1);
					}
				};
				Thread R = new Thread(){
					public void run(){
						multi(M+1,E,available-1);
					}
				};

				if(B<M)
					L.start();
				if(M+1<E)
					R.start();

				try{
					L.join();
					R.join();
				}
				catch(InterruptedException ie){
					ie.printStackTrace();
				}

				merge(B,E);
			}
		}
	}

	int[] sort(int[] t){
		tab = new int[t.length];
		for(int i=0; i<t.length; i++)
			tab[i]=t[i];
		multi(0,t.length-1,(Runtime.getRuntime().availableProcessors())*2);
		return tab;
	}
}

public class zad5{
	public static void main(String[] argc){
		multi_merge mm = new multi_merge();
		int[] t = mm.sort(new int[]{8,3,5,4,3,1,4,6,7,8,4,3,0,9,2,1,3,7,5});
		for(int i=0; i<t.length; i++)
			System.out.print(t[i] + " ");
		System.out.println("");
	}
}
