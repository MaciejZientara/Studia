package obliczenia;

import java.util.*;

public class Zmienna extends Wyrazenie{
    public static final HashMap<String, Integer> hm;
    private String name;

    static{
        hm = new HashMap<String,Integer>();
    }

    public Zmienna(String s){
        name = s;
        //hm.put(s,v);
    }
    public int oblicz(){
        return hm.get(name);
    }
    @Override
    public String toString(){
        return name;
    }
    @Override
    public boolean equals(Object o){
        if(!(o instanceof Zmienna))
            return false;
        
        return toString().equals(o.toString());
    }
}