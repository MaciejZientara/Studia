module tff(output q, nq, input t, clk, nrst);
  logic ns, nr, ns1, nr1, j, k;
  nand n1(ns, clk, j), n2(nr, clk, k), n3(q, ns, nq),
  n4(nq, nr, q, nrst), n5(ns1, !clk, t, nq),
  n6(nr1, !clk, t, q), n7(j, ns1, k), n8(k, nr1, j, nrst);
endmodule

// Jesli chce zmieniac wartosc o 2 to po prostu zostawiam
// bit0 nie zmieniony i traktuje licznik jako mniejszy od bitu 1-3.

// Kierunek czy chce dodawac czy odejmowac to wybranie Q lub nQ
// z poprzednich bitow (slajdy 10,11 wyklad 7).

// Uklad zainspirowany z wykladu 7 slajdow 14,15.
module circuit(output [3:0] out, input down, step, nrst, clk);
  logic [3:0] nq;
  wire t0, t1, t2, t3;
  assign t0 = !step;
  assign t1 = (down==1 ? nq[0] : out[0]) | step;
  assign t2 = (down==1 ? nq[1] : out[1]) & t1;
  assign t3 = (down==1 ? nq[2] : out[2]) & t2;
  tff T0(out[0],nq[0],t0,clk,nrst);
  tff T1(out[1],nq[1],t1,clk,nrst);
  tff T2(out[2],nq[2],t2,clk,nrst);
  tff T3(out[3],nq[3],t3,clk,nrst);
endmodule
