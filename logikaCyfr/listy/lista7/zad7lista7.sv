module dff(output q, input clk, d);
  logic r, s, nr, ns;
  nand gq(q, nr, nq), gnq(nq, ns, q), gr(nr, clk, r),
  gs(ns, nr, clk, s), gr1(r, nr, s), gs1(s, ns, d);
endmodule
module syncnt_load#(parameter N = 4)(
  output [N-1:0] q, input [N-1:0] d, input en, clk, load);
  genvar n;
  logic [N-1:0] t;
  assign t = {q[2] & t[2], q[1] & t[1], q[0] & t[0], en};
  for (n = 0; n < N; n = n + 1)
    dff df(q[n], clk, load ? d[n] : q[n] ^ t[n]);
endmodule 

module circuit(output o, input start,clk);
  reg [3:0] q;
  wire en = |q;
  syncnt_load sl(q,4'd4,en,clk,start);
  assign o = en;
endmodule
