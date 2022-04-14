module d_latch(output q, nq, input en, d);
logic r, s, nr, ns;
nand gq(q, nr, nq), gnq(nq, ns, q),
  gr(nr, en, r), gs(ns, nr, en, s),
gr1(r, nr, s), gs1(s, ns, d);
endmodule

module t_latch(output q, nq, input c, t);
  d_latch latch(q,nq,c,((t&nq)^(!t&q)));
endmodule

module jk_latch(output q, nq, input c, j, k);
  d_latch latch(q,nq,c,((j&nq)^(!k&q)));
endmodule

// https://www.allaboutcircuits.com/technical-articles/conversion-of-t-flip-flops-part-v/
module my_jk(output q, nq, input c, j, k);
  t_latch tl(q,nq,c,j^k);
endmodule


module compare(output q1,q2,nq1,nq2, input c,j,k);
  jk_latch jk1(q1,nq1,c,j,k);
  my_jk jk2(q2,nq2,c,j,k);
endmodule
