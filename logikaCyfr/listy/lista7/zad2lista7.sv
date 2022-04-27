module tff(output q, nq, input t, clk, nrst);
  logic ns, nr, ns1, nr1, j, k;
  nand n1(ns, clk, j), n2(nr, clk, k), n3(q, ns, nq),
  n4(nq, nr, q, nrst), n5(ns1, !clk, t, nq),
  n6(nr1, !clk, t, q), n7(j, ns1, k), n8(k, nr1, j, nrst);
endmodule

module circuit(output [2:0] out, input down, nrst, clk);
  logic [2:0] nq;
  wire t0, t1, t2;
  assign t0 = 1;
  assign t1 = (down==1 ? nq[0] : out[0]);
  assign t2 = (down==1 ? nq[1] : out[1]) & t1;
  tff T0(out[0],nq[0],t0,clk,nrst);
  tff T1(out[1],nq[1],t1,clk,nrst);
  tff T2(out[2],nq[2],t2,clk,nrst);
endmodule