package obliczenia;

import java.util.*;

public class Silnia extends Unarne{
    public Silnia(Wyrazenie ar1){
        arg1 = ar1;
    }
    public int oblicz(){
        int tmp = arg1.oblicz();
        int ret = 1;
        for(int i=2; i<=tmp; i++)
            ret*=i;
        return ret;
    }
    @Override
    public String toString(){
        return "("+arg1+")!";
    }
    @Override
    public boolean equals(Object o){
        if(!(o instanceof Silnia))
            return false;
        
        return toString().equals(o.toString());
    }
}