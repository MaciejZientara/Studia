package obliczenia;

import java.util.*;

public class Stala extends Wyrazenie{
    private int val;
    private String name;
    public Stala(String s){
        switch (s) {
            case "JEDEN":
                val = 1;
                break;
            case "ZERO":
                val = 0;
                break;
            case "MINUSJEDEN":
                val = -1;
                break;
            default:
                break;
        }
        name = s;
    }
    public int oblicz(){
        return val;
    }
    @Override
    public String toString(){
        return name;
    }
    @Override
    public boolean equals(Object o){
        if(!(o instanceof Stala))
            return false;
        
        return toString().equals(o.toString());
    }
}