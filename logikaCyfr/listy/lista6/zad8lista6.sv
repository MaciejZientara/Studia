module d_latch(output q, nq, input en, d);
  logic nr, ns;
  nand gq(q, nr, nq), gnq(nq, ns, q),
  gr(nr, d, en), gs(ns, nr, en);
endmodule

module t_latch(output q, nq, input c, t);
  d_latch latch(q,nq,c,((t&nq)^(!t&q)));
endmodule

module zad8(output nq,input clk);
  wire Q1, Q2, nQ1, nQ2;
  t_latch latch1(Q1,nQ1,clk,(Q1|Q2));
  t_latch latch2(Q2,nQ2,clk,(nQ1|Q2));
  assign nq = nQ2;//tylko zeby wygenerowal sie uklad
endmodule

