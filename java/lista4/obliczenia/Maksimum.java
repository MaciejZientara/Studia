package obliczenia;

import java.util.*;

public class Maksimum extends Binarne{
    public Maksimum(Wyrazenie ar1,Wyrazenie ar2){
        arg1 = ar1;
        arg2 = ar2;
    }
    public int oblicz(){
        int a = arg1.oblicz(), b = arg2.oblicz();
        return (a<b) ? b : a;
    }
    @Override
    public String toString(){
        return "max("+arg1+", "+arg2+")";
    }
    @Override
    public boolean equals(Object o){
        if(!(o instanceof Maksimum))
            return false;
        
        return toString().equals(o.toString());
    }
}