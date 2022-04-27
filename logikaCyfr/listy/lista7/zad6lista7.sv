module tff(output q, nq, input t, clk, nrst);
logic ns, nr, ns1, nr1, j, k;
nand n1(ns, clk, j), n2(nr, clk, k), n3(q, ns, nq),
n4(nq, nr, q, nrst), n5(ns1, !clk, t, nq),
n6(nr1, !clk, t, q), n7(j, ns1, k), n8(k, nr1, j, nrst);
endmodule

//pojawia sie glitch kiedy wysoki zegar i t zmiana z H na L
//poza tym zegar o okresie 50 ns, wypelnienie 2/5
module circuit(output o, input clk, nrst);
  wire q1,q2,q3;
  tff tf1(q1,,1,clk,nrst);
  tff tf2(q2,,q1,clk,nrst);
  tff tf3(q3,,q1,!clk,nrst);
  assign o = q2 & q3;
endmodule


