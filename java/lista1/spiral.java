import java.util.*;

public class spiral{
	int n;
	int[][] tab;
	int i, j;
	int side=0, length=1, lhelp=0, iter=0, val=1;
	
	void step(){
		val++;
		switch(side){
			case 0:
				j++;
				break;
			case 1:
				i--;
				break;
			case 2:
				j--;
				break;
			case 3:
				i++;
				break;
			default:
				break;
		}	
		
		//System.out.println("val = " + val);	
		//System.out.println("i = " + i + ", j = " + j);

		if(++iter==length){
			iter=0;
			side = (side+1)%4;
			if(++lhelp%2==0)
				length++;
		}
	}
	
	public void fillArray(){
		tab = new int[n][n];

		i = n/2;
		j = ((int)Math.ceil(n/2.0))-1;
		
		int end;
		if(n%2==0)
			end = 0;
		else
			end = n-1;

		while(i!=end || j!=end){
		//System.out.println("i = " + i + ", j = " + j);
			tab[i][j] = val;
			step();
		}
		tab[i][j]=val;
	}
	
	boolean isPrime(int x){
		if(x==0 || x==1)
			return false;
		int s = (int)Math.sqrt(x);
		for(int i=2; i<=s; i++)
			if(x%i==0)
				return false;
		return true;
	}

	public void printArray(){
		//System.out.print("print\n");
/*
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++)
				System.out.print(tab[i][j]);
			System.out.print("\n");
		}
*/
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++)
				if(isPrime(tab[i][j]))
					System.out.print("*");
				else
					System.out.print(" ");
			System.out.print("\n");
		}

	}
	
	public static void main(String[] argc) throws NumberFormatException {
		spiral spi = new spiral();
		try{
			spi.n = Integer.valueOf(argc[0]);
			if(spi.n<2 || spi.n>200)
				throw new IllegalArgumentException("parametr spoza zakresu 2..200\n");
			spi.fillArray();
			spi.printArray();
		}
		catch(NumberFormatException ex){
			System.err.println("nie liczba");
		}
	}
}
