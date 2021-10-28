package test;

import java.util.*;
import struktury.*;

public class testy{
	ZbiorNaTablicy T;
	ZbiorNaTablicyDynamicznej TD;

	{
		T = new ZbiorNaTablicy(4);
		TD = new ZbiorNaTablicyDynamicznej();
	}

	public void test1(){
		System.out.println("test1");
		try{
			System.out.println(T.ile());
			T.wstaw(new Para("jeden",1));
			T.wstaw(new Para("dwa",2));
			T.wstaw(new Para("trzy",3));
			System.out.println("ile 3 = " + T.ile());
			T.usun("dwa");
			System.out.println("ile 2 = " + T.ile());
			T.ustaw(new Para("jeden",4));
			System.out.println("jeden -> 4 = " + T.czytaj("jeden"));
		}catch(Exception e){
			System.out.print(e);
		}
		try{
			T.wstaw(new Para("jeden",1));
		}catch(Exception e){
			System.out.print(e);
		}
		try{
			T.wstaw(new Para("dwa",2));
			T.wstaw(new Para("cztery",4));
			T.wstaw(new Para("piec",5));
		}catch(Exception e){
			System.out.print(e);
		}
		try{
			System.out.println(T.czytaj("piec"));
		}catch(Exception e){
			System.out.print(e);
		}
		

		System.out.println("przed czysc = " + T.ile());
		T.czysc();
		System.out.println("po czysc = " + T.ile());
	}
	public void test2(){
		System.out.println("test2");
		try{
			System.out.println(TD.ile());
			TD.wstaw(new Para("jeden",1));
			TD.wstaw(new Para("dwa",2));
			TD.wstaw(new Para("trzy",3));
			System.out.println("ile 3 = " + TD.ile());
			TD.usun("dwa");
			System.out.println("ile 2 = " + TD.ile());
			TD.ustaw(new Para("jeden",4));
			System.out.println("jeden -> 4 = " + TD.czytaj("jeden"));
		}catch(Exception e){
			System.out.print(e);
		}
		try{
			TD.wstaw(new Para("jeden",1));
		}catch(Exception e){
			System.out.print(e);
		}
		try{
			TD.wstaw(new Para("dwa",2));
			TD.wstaw(new Para("cztery",4));
			TD.wstaw(new Para("piec",5));
		}catch(Exception e){
			System.out.print(e);
		}
		try{
			System.out.println(TD.czytaj("piec"));
			System.out.println(TD.czytaj("undefined"));
		}catch(Exception e){
			System.out.print(e);
		}
		

		System.out.println("przed czysc = " + TD.ile());
		TD.czysc();
		System.out.println("po czysc = " + TD.ile());
	}


	public void fullTest(){
		test1();
		System.out.print("\n\n");
		test2();
	}
}
