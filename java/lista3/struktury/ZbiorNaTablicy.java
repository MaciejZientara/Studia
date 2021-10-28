package struktury;

import java.util.*;

public class ZbiorNaTablicy extends zbior{
	protected Para[] tab;

	public ZbiorNaTablicy(int n){
		if(n<2)
			throw new IllegalArgumentException("za maly rozmiar tablicy\n");
		tab = new Para[n];
	}
	
	public Para szukaj(String k)throws Exception{
		for(int i=0; i<tab.length; i++)
			if(tab[i]!= null && tab[i].klucz.equals(k))
				return tab[i];
		throw new Exception("nie znalaziono klucza\n");
	}
	public void wstaw(Para p)throws Exception{
		for(int i=0; i<tab.length; i++)
			if(tab[i]!=null)
				if(tab[i].klucz.equals(p.klucz))
					throw new Exception("para juz istnieje\n");
		for(int i=0; i<tab.length; i++)
			if(tab[i]==null){
				tab[i]=p;
				return;
			}
		throw new Exception("brak miejsca w tablicy\n");
	}
	public void usun(String k){
		for(int i=0; i<tab.length; i++)
			if(tab[i]!= null && tab[i].klucz.equals(k)){
				tab[i]=null;
				return;
			}
	}
	public double czytaj(String k)throws Exception {
		for(int i=0; i<tab.length; i++)
			if(tab[i]!= null && tab[i].klucz.equals(k)){
				return tab[i].getWartosc();
			}
		throw new Exception("nie znalaziono klucza\n");	
	}
	public void ustaw(Para p)throws Exception {
		for(int i=0; i<tab.length; i++)
			if(tab[i]!= null && tab[i].equals(p)){
				tab[i] = p;
				return;
			}
		this.wstaw(p);
	}
	public void czysc(){
		tab = new Para[tab.length];
	}
	public int ile(){
		for(int i=0; i<tab.length; i++)
			for(int j=i-1; j>=0; j--)
				if(tab[j]==null){
					tab[j]=tab[j+1];
					tab[j+1]=null;
				}
		for(int i=0; i<tab.length; i++)
			if(tab[i]==null)
				return i;
		return tab.length;
	}
}


