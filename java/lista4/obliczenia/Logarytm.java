package obliczenia;

import java.util.*;

public class Logarytm extends Binarne{
    public Logarytm(Wyrazenie ar1,Wyrazenie ar2){
        arg1 = ar1;
        arg2 = ar2;
    }
    public int oblicz(){
        return (int)(Math.log(arg1.oblicz())/Math.log(arg2.oblicz()));
    }
    @Override
    public String toString(){
        return "log_" + arg1 + "(" + arg2 + ")";
    }
    @Override
    public boolean equals(Object o){
        if(!(o instanceof Logarytm))
            return false;
        
        return toString().equals(o.toString());
    }
}