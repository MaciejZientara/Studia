// Chyba pomylilem kierunki z L i R, dlatego zamiast przepisywac caly 
// modul rejestr i poprawiajac kierunki, to po prostu zamienilem w 
// switch case b10 z b01 i testy przeszly :)

module d_latch1(output q, nq, input en, d);
  logic nr, ns;
  nand gq(q, nr, nq), gnq(nq, ns, q),
  gr(nr, d, en), gs(ns, nr, en);
endmodule

module d_latch(output q, nq, input clk, d);
logic r, s, nr, ns;
nand gq(q, nr, nq), gnq(nq, ns, q),
gr(nr, clk, r), gs(ns, nr, clk, s),
gr1(r, nr, s), gs1(s, ns, d);
endmodule

module controledBit(output Q, input [1:0] lr, input Dl,Dr,Dd, input clock);
  logic NQ;
  logic D;
  always_comb begin
    case (lr)
      2'b00: D = Q;
      2'b10: D = Dl;
      2'b01: D = Dr;
      default: D = Dd;//2'b11
    endcase
  end
  d_latch latch(Q, NQ, clock, D);
endmodule

// lr input
// 00 output
// 01 right (i/q[])
// 10 left (i/q[])
// 11 d[]

module rejestr(output [7:0] q, input [7:0] d, input i,c,l,r);
  controledBit b7(q[7],{l,r},i,   q[6],d[7],c);
  controledBit b6(q[6],{l,r},q[7],q[5],d[6],c);
  controledBit b5(q[5],{l,r},q[6],q[4],d[5],c);
  controledBit b4(q[4],{l,r},q[5],q[3],d[4],c);
  controledBit b3(q[3],{l,r},q[4],q[2],d[3],c);
  controledBit b2(q[2],{l,r},q[3],q[1],d[2],c);
  controledBit b1(q[1],{l,r},q[2],q[0],d[1],c);
  controledBit b0(q[0],{l,r},q[1],i,   d[0],c);  
endmodule