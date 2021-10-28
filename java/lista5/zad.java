import java.util.*;
import structures.*;

public class zad{
    public static void main(String[] args) {
        OrderedList<Integer> ol = new OrderedList<Integer>();        
        try{
            ol.insert(10);
            ol.insert(2);
            ol.insert(7);
            ol.insert(1);
            ol.insert(3);

            int sum = 0;
            for(int i : ol){ sum+=i; }
            System.out.println("sum = " + sum);

            System.out.println("min = " + ol.minimum());
            System.out.println("max = " + ol.maximum());
            System.out.println("size = " + ol.size());
            ol.remove(ol.minimum());
            System.out.println("min = " + ol.minimum());
            System.out.println("size = " + ol.size());

            System.out.println("2 = " + ol.index(ol.at(2)));

            System.out.println("false = " + ol.search(0));
            
            ol.at(10);

        }catch(Exception e){
            System.out.println(e);
        }
        OrderedList<String> ol1 = new OrderedList<String>();        
        try{
            ol1.insert("dlugie");
            ol1.insert("b");
            ol1.insert("ab");
            ol1.insert("krotkie");
            ol1.insert("aaa");

            String sum = "";
            for(String i : ol1){ sum+=i; }
            System.out.println("sum = " + sum);

            System.out.println("min = " + ol1.minimum());
            System.out.println("max = " + ol1.maximum());
            ol1.remove(ol1.minimum());
            System.out.println("min = " + ol1.minimum());
            System.out.println("size = " + ol1.size());

            System.out.println("2 = " + ol1.index(ol1.at(2)));

            System.out.println("false = " + ol1.search("0"));
            
            ol1.at(10);

        }catch(Exception e){
            System.out.println(e);
        }
        OrderedList<Calendar> ol2 = new OrderedList<Calendar>();        
        try{
            ol2.insert(Calendar.getInstance());
            ol2.insert(Calendar.getInstance());
            ol2.insert(Calendar.getInstance());
            ol2.insert(Calendar.getInstance());
            ol2.insert(Calendar.getInstance());

            int sum = 0;
            for(Calendar i : ol2){ sum+=i.get(Calendar.SECOND); }
            System.out.println("sum = " + sum);

            System.out.println("min = " + ol2.minimum().getTime());
            System.out.println("max = " + ol2.maximum().getTime());
            ol2.remove(ol2.minimum());
            System.out.println("min = " + ol2.minimum().getTime());
            System.out.println("size = " + ol2.size());

            System.out.println("2 = " + ol2.index(ol2.at(2)));

            System.out.println("false = " + ol2.search(Calendar.getInstance()));
            
            ol2.at(10);

        }catch(Exception e){
            System.out.println(e);
        }



    }
}