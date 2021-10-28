import java.util.*;
import javax.swing.*;

public class zad{
    JFrame f;
    JPanel panel;
    JLabel info;
    zad(){//init
        f = new JFrame("Solitaire");

        // JMenuBar - menu gry
        panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.PAGE_AXIS));
        info = new JLabel();
        info.setBounds(50,50,100,30);

        f.add(panel);

        f.add(info);

        f.setSize(800,600);
        f.setLayout(null);
        f.setVisible(true);
        f.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
    }
    void mainLoop(){
        info.setText("hello");
        // System.out.println();
        do{
            int wd = f.getSize().width, ht = f.getSize().height;
            info.setBounds(wd/2,ht/2,100,30);
        }
        while(true);
        //System.exit(0);
    }

    public static void main(String[] args){
        zad z = new zad();
        z.mainLoop();
    }
}