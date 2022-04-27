module dff(output q, input clk, D, nrst);
  wire d = D & nrst;
  logic r, s, nr, ns;
  nand gq(q, nr, nq), gnq(nq, ns, q), gr(nr, clk, r),
  gs(ns, nr, clk, s), gr1(r, nr, s), gs1(s, ns, d);
endmodule
module tff(output q, nq, input t, clk, nrst);
  logic ns, nr, ns1, nr1, j, k;
  nand n1(ns, clk, j), n2(nr, clk, k), n3(q, ns, nq),
  n4(nq, nr, q, nrst), n5(ns1, !clk, t, nq),
  n6(nr1, !clk, t, q), n7(j, ns1, k), n8(k, nr1, j, nrst);
endmodule

// https://verilogpractice.wordpress.com/2018/01/29/divide-by-5/
module oneFifth(output o, input clk, nrst);
  wire d1,d2,d3,d4;
  wire q1,q2,q3,q4;
  assign d4 = q2;
  assign d2 = q1 ^ q2;
  assign d3 = q1 & q2;
  assign d1 = !q1 & !q3;
  dff df1(q1,clk,d1,nrst);
  dff df2(q2,clk,d2,nrst);
  dff df3(q3,clk,d3,nrst);
  dff df4(q4,!clk,d4,nrst);
  assign o = d4 | q4;
endmodule

module twoFifth(output o, input nrst, clk);
  wire nq, onef;
  oneFifth of(onef,clk,nrst);
  tff div2(o,nq,onef,clk,nrst);
endmodule
