package obliczenia;

import java.util.*;

public class Dodawanie extends Binarne{
    public Dodawanie(Wyrazenie ar1,Wyrazenie ar2){
        arg1 = ar1;
        arg2 = ar2;
    }
    public int oblicz(){
        return arg1.oblicz() + arg2.oblicz();
    }
    @Override
    public String toString(){
        return "("+ arg1 +" + "+ arg2 +")";
    }
    @Override
    public boolean equals(Object o){
        if(!(o instanceof Dodawanie))
            return false;
        
        return toString().equals(o.toString());
    }
}