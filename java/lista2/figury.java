import java.util.*;

class Wektor{
	public final double dx, dy;

	Wektor(double a, double b){
		dx = a;
		dy = b;
	}

	public static Wektor add(Wektor w, Wektor v){
		return new Wektor(v.dx+w.dx,v.dy+w.dy);
	}
}

class Prosta{
	public final double a, b, c;

	Prosta(double x, double y, double z){
		a = x;
		b = y;
		c = z;
	}

	public static Prosta przesun(Prosta p, Wektor v){
		return new Prosta(p.a,p.b,p.c - p.a*v.dx + p.b*v.dy);
	}

	public static boolean czyRownolegle(Prosta l, Prosta k){
		return (k.a*l.b == k.b*l.a);
	}
	public static boolean czyProstopadle(Prosta l, Prosta k){
		return (k.a*l.a + k.b*l.b == 0);
	}
	public static Punkt przeciecie(Prosta l, Prosta k){
		if(Prosta.czyRownolegle(l,k))
			throw new IllegalArgumentException("proste rownolegle, brak punktu przeciecia\n");
		double x, y;
	
		y = (l.c*k.a - k.c*l.a)/(k.b*l.a - k.a*l.b);
		if(k.a!=0.0)
			x = -(k.b*y+k.c)/k.a;
		else
			x = -(l.b*y+l.c)/l.a;


		return new Punkt(x,y);
	}

	@Override
	public String toString(){
		return a + "x + " + b + "y + " + c;
	}

}

class Punkt{
	private double x, y;
	
	Punkt(double a, double b){
		x = a;
		y = b;
	}

	public double getx(){
		return x;
	}
	public double gety(){
		return y;
	}

	@Override
	public boolean equals(Object obj){
		if(obj == null)
			return false;
		if(obj instanceof Punkt)
			return (x==((Punkt)obj).getx() && y==((Punkt)obj).gety());
		else
			return false;
	}
	
	@Override
	public String toString(){
		return "(" + x + ", " + y + ")";
	}

	public void przesun(Wektor v){
		x+=v.dx;
		y+=v.dy;
	}
	public void obroc(Punkt p, double kat){
		double snk = Math.sin(kat), csk = Math.cos(kat);
		double xp = x * csk - y * snk, yp = y * csk + x * snk;
		x = xp;
		y = yp;
	}
	public void odbij(Prosta p){
		double c = p.b * x - p.a * y;
		Punkt tmp = Prosta.przeciecie(p,new Prosta(-p.b,p.a,c));
		if(this.equals(tmp))
			return;
		Wektor d = new Wektor((tmp.x-x),(tmp.y-y));
		this.przesun(d);
		this.przesun(d);
	}
}

class Odcinek{
	private Punkt A, B;

	Odcinek(Punkt a, Punkt b){
		if(a.equals(b))
			throw new IllegalArgumentException("te same punkty w odcinku\n");
		A = a;
		B = b;
	}

	@Override
	public String toString(){
		return "(" + A + ", " + B + ")";
	}

	public void przesun(Wektor v){
		A.przesun(v);
		B.przesun(v);
	}
	public void obroc(Punkt p, double kat){
		A.obroc(p,kat);
		B.obroc(p,kat);
	}
	public void odbij(Prosta p){
		A.odbij(p);
		B.odbij(p);
	}
}


class Trojkat{
	private Punkt A, B, C;

	private boolean oneLine(Punkt a, Punkt b, Punkt c){
		return (a.gety() - b.gety()) * (a.getx() - c.getx()) ==
             (a.gety() - c.gety()) * (a.getx() - b.getx());
	}

	Trojkat(Punkt a, Punkt b, Punkt c){
		if(a.equals(b) || a.equals(c) || b.equals(c))
			throw new IllegalArgumentException("te same punkty w trojkacie\n");
		if(oneLine(a,b,c))
			throw new IllegalArgumentException("punkty na jednej prostej\n");
		A = a;
		B = b;
		C = c;
	}

	@Override
	public String toString(){
		return "(" + A + ", " + B + ", " + C + ")";
	}

	public void przesun(Wektor v){
		A.przesun(v);
		B.przesun(v);
		C.przesun(v);
	}
	public void obroc(Punkt p, double kat){
		A.obroc(p,kat);
		B.obroc(p,kat);
		C.obroc(p,kat);
	}
	public void odbij(Prosta p){
		A.odbij(p);
		B.odbij(p);
		C.odbij(p);	
	}
}




public class figury{

	public static void main(String[] argc){
		Punkt A = new Punkt(1,2);
		Punkt O = new Punkt(0,0);
		
		//Trojkat t = new Trojkat(A,O,new Punkt(2,4));
		Wektor v = new Wektor(-2,0);
		
		Prosta l = new Prosta(1,0,-2);
		Prosta k = Prosta.przesun(l,v);
	
		Punkt B = new Punkt(A.getx(),A.gety());
		B.przesun(Wektor.add(new Wektor(1,0),new Wektor(0,-2)));	
		System.out.println(O);
		System.out.println(A);
		System.out.println(B);
		Trojkat t = new Trojkat(O,A,B);
		System.out.println(k);
		System.out.println("przed: " + t);
		t.odbij(k);
		System.out.println("po: " + t);
		
		System.out.println(Prosta.przeciecie(new Prosta(1,-1,1),new Prosta(1,1,1)));
	
		Punkt C = new Punkt(2,0);
		System.out.print("obrot " + C);
		C.obroc(O,Math.atan(1)*2.0);
		System.out.print(" o PI/2 wzgl. (0,0) = " + C + "\n");
	}
}
