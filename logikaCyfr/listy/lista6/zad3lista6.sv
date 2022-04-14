//zwykly d_latch
module d_latch1(output q, nq, input en, d); 
  logic nr, ns;
  nand gq(q, nr, nq), gnq(nq, ns, q),
  gr(nr, d, en), gs(ns, nr, en);
endmodule

//d_latch aktywowany zboczem narastajacym = flip_flop
module d_latch(output q, nq, input en, d);
logic r, s, nr, ns;
nand gq(q, nr, nq), gnq(nq, ns, q),
  gr(nr, en, r), gs(ns, nr, en, s),
gr1(r, nr, s), gs1(s, ns, d);
endmodule

// https://www.quora.com/If-I-have-an-8-kHz-square-wave-clocks-and-a-5-bit-ripple-counter-what-is-the-frequency-of-the-last-flip-flop-What-is-the-duty-cycle-of-this-output-waveform
module freqDiv4(output o, input clk, input init);
  wire q1,nq1,q2,nq2;
  d_latch latch1(q1,nq1,clk,nq1|init);
  d_latch latch2(q2,nq2,q1,nq2|init);
  assign o=q2;
endmodule

