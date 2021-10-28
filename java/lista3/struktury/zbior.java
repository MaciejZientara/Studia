package struktury;

import java.util.*;

public abstract class zbior{
	public abstract Para szukaj(String k)throws Exception;
	public abstract void wstaw(Para p)throws Exception;	
	public abstract void usun(String k);
	public abstract double czytaj(String k)throws Exception;
	public abstract void ustaw(Para p)throws Exception;
	public abstract void czysc();
	public abstract int ile();
}










