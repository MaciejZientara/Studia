package struktury;

import java.util.*;

public class ZbiorNaTablicyDynamicznej extends ZbiorNaTablicy{
	public ZbiorNaTablicyDynamicznej(){
		super(2);
	}

	@Override
	public void wstaw(Para p)throws Exception {
		for(int i=0; i<tab.length; i++)
			if(tab[i]!=null)
				if(tab[i].klucz.equals(p.klucz))
					throw new Exception("para juz istnieje\n");
		for(int i=0; i<tab.length; i++)
			if(tab[i]==null){
				tab[i]=p;
				return;
			}
		Para[] tmp = new Para[2*tab.length];
		for(int i=0; i<tab.length; i++)
			tmp[i]=tab[i];
		tmp[tab.length] = p;
		tab = tmp;
	}
	
	@Override
	public void usun(String k){
		for(int i=0; i<tab.length; i++)
			if(tab[i]!=null && tab[i].klucz.equals(k))
				tab[i]=null;
		for(int i=0; i<tab.length; i++)
			for(int j=i-1; j>=0; j--)
				if(tab[j]==null){
					if(tab[j+1]!=null){
						tab[j]=tab[j+1];
						tab[j+1]=null;
					}
				}
		for(int i=0; i<tab.length; i++)
			if(tab[i]==null && i<(tab.length/4)){
				Para[] tmp = new Para[tab.length/2];
				for(int j=0; j<tab.length; j++)
					tmp[j]=tab[j];
				tab = tmp;
				return;
			}
	}

}
