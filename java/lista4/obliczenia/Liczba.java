package obliczenia;

import java.util.*;

public class Liczba extends Wyrazenie{
    private int val;
    public Liczba(int v){
        val=v;
    }
    public int oblicz(){
        return val;
    }
    @Override
    public String toString(){
        return String.valueOf(val);
    }
    @Override
    public boolean equals(Object o){
        if(!(o instanceof Liczba))
            return false;
        
        return toString().equals(o.toString());
    }
}