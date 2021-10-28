import java.util.*;
import obliczenia.*;

public class zad1_2{
    public static void main(String[] args){
        Zmienna.hm.put("x",2);
        Wyrazenie w = new Dodawanie(new Liczba(7), new Mnozenie(new Zmienna("x"), new Liczba(5)));
        System.out.println(w + " = " + w.oblicz());
    }
}