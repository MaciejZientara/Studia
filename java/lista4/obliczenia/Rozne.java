package obliczenia;

import java.util.*;

public class Rozne extends Binarne{
    public Rozne(Wyrazenie ar1,Wyrazenie ar2){
        arg1 = ar1;
        arg2 = ar2;
    }
    public int oblicz(){
        int a = arg1.oblicz(), b = arg2.oblicz();
        return (a!=b) ? 1 : 0;
    }
    @Override
    public String toString(){
        return "("+arg1+" != "+arg2+")";
    }
    @Override
    public boolean equals(Object o){
        if(!(o instanceof Rozne))
            return false;
        
        return toString().equals(o.toString());
    }
}