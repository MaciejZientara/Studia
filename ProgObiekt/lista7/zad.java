import java.util.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;

class A implements Serializable,ActionListener{
	String a;
	String b;
	String c;
/*	public A(){}
	public A(A x){
		a=x.a;
		b=x.b;
		c=x.c;
	}
*/	public String toString(){
		String tmp = super.toString();
		return tmp + "(" + a + "," + b + "," + c + ")";	
	}
	private JTextField text1;
	private JTextField text2;
	private JTextField text3;
	private String FNAME;
	void run(String FILE){
		FNAME=FILE;
		System.out.println("A");
		JFrame f = new JFrame("edytuj A");
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		Container cont = f.getContentPane();
		GridLayout lay = new GridLayout(4,2);
		cont.setLayout(lay);
		JLabel name1 = new JLabel("a");
		cont.add(name1);
		text1 = new JTextField(a,40);
		cont.add(text1);
		JLabel name2 = new JLabel("b");
		cont.add(name2);	
		text2 = new JTextField(b,40);
		cont.add(text2);
		JLabel name3 = new JLabel("c");
		cont.add(name3);
		text3 = new JTextField(c,40);
		cont.add(text3);
		
		JButton save = new JButton("zapisz");
		save.addActionListener(this);
		cont.add(save);

		f.pack();
		f.setVisible(true);

	}
	public void actionPerformed(ActionEvent e){
		//System.out.println(e.getActionCommand());
		//System.out.println(text1.getText());
		//System.out.println(text2.getText());
		//System.out.println(text3.getText());
		a = text1.getText();
		b = text2.getText();
		c = text3.getText();
		try{
			File FI = new File(FNAME);
			ObjectOutputStream o = new ObjectOutputStream(new BufferedOutputStream(
			new FileOutputStream(FI)));
			o.writeObject(this);
			o.flush();
			o.close();
		}
		catch(FileNotFoundException ex){}
		catch(IOException ex){}
	}
}
class B extends A implements Serializable,ActionListener{
	String d;
	String e;
	String f;
	public String toString(){
		String tmp = super.toString();
		return tmp + "(" + d + "," + e + "," + f + ")";
	}
	void run(){
		System.out.println("B");
	}
}
class C extends A implements Serializable,ActionListener{
	String g;
	String h;
	String i;
	public String toString(){
		String tmp = super.toString();
		return tmp + "(" + g + "," + h + "," + i + ")";
	}
	void run(){
		System.out.println("C");
	}
}


public class zad{
	public static void main(String[] args){
		if(args.length>1){
			switch(args[1]){
				case "A": 
					try{
						FileInputStream FL = new FileInputStream(new File(args[0]));
						System.out.println("w try");
						ObjectInputStream in = new ObjectInputStream(new BufferedInputStream(
						FL));
						System.out.println("weszlo");
						A a = (A)in.readObject();
						System.out.println("weszlo1");
						if(a!=null)
							a.run(args[0]);
						System.out.println("weszlo2");
						in.close();
					}
					catch(FileNotFoundException e){
						System.out.println("file not found");
						A a = new A();
						a.run(args[0]);
					}
					catch(IOException e){System.out.println("IO");}
					catch(ClassNotFoundException e){System.out.println("class not found");}
					break;
				case "B":
					B b = new B();
					//b.run();
					break;
				case "C":
					C c = new C();
					//c.run();
					break;
				default: break;
			}
		}
	}
}

