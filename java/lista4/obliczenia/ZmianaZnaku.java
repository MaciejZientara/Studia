package obliczenia;

import java.util.*;

public class ZmianaZnaku extends Unarne{
    public ZmianaZnaku(Wyrazenie ar1){
        arg1 = ar1;
    }
    public int oblicz(){
        return -1*arg1.oblicz();
    }
    @Override
    public String toString(){
        return "-"+arg1;
    }
    @Override
    public boolean equals(Object o){
        if(!(o instanceof ZmianaZnaku))
            return false;
        
        return toString().equals(o.toString());
    }
}