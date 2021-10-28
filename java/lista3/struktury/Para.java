package struktury;

import java.util.*;

public class Para{
	public final String klucz;
	private double wartosc;

	public Para(String k, double w){
		if(k == null || k.equals(""))
			throw new IllegalArgumentException("pusty klucz\n");
		for(int i=0; i<k.length(); i++)
			if(!Character.isLowerCase(k.charAt(i)))
				throw new IllegalArgumentException("zly format klucza\n");
		klucz = k;
		wartosc = w;
	}

	public double getWartosc(){
		return wartosc;
	}
	
	public void setWartosc(double wart){
		wartosc = wart;
	}

	@Override
	public String toString(){
		return "(" + klucz + ", " + wartosc + ")";
	}

	@Override
	public boolean equals(Object o){
		if(!(o instanceof Para))
			return false;

		if(((Para)o).klucz.equals(klucz))
			return true;

		return false;
	}

}

