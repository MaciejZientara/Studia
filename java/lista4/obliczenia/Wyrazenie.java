package obliczenia;

public abstract class Wyrazenie implements obliczalny{

    public static int suma (Wyrazenie... wyr){
        int ret = 0;
        for(int i = 0; i<wyr.length; i++)
            ret+=wyr[i].oblicz();
        return ret;
    }
    public static int iloczyn (Wyrazenie... wyr){
        int ret = 1;
        for(int i = 0; i<wyr.length; i++)
            ret*=wyr[i].oblicz();
        return ret;
    }
}