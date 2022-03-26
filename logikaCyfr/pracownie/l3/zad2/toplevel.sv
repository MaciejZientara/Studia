// Write your modules here!
module Adder4(output [3:0] s, output G, output P, input [3:0] a, input [3:0] b, input c);
  function [1:0] makegp(input bitA, input bitB);
    makegp = ({bitA&bitB, bitA|bitB});
  endfunction
  logic g0, p0, c1;
  assign {g0, p0} = makegp(a[0],b[0]);
  assign s[0] = a[0]^b[0]^c;
  assign c1 = g0 | (p0 & c);
  
  logic g1, p1, c2;
  assign {g1, p1} = makegp(a[1],b[1]);
  assign s[1] = a[1]^b[1]^c1;
  assign c2 = g1 | (g0 & p1) | (p1 & p0 & c);
  
  logic g2, p2, c3;
  assign {g2, p2} = makegp(a[2],b[2]);
  assign s[2] = a[2]^b[2]^c2;
  assign c3 = g2 | (g1 & p2) | (g0 & p1 & p2) | (p2 & p1 & p0 & c);
  
  logic g3, p3;
  assign {g3, p3} = makegp(a[3],b[3]);
  assign s[3] = a[3]^b[3]^c3;
//  assign c4 = g3 | (g2 & p3) | (g1 & p2 & p3) | (g0 & p1 & p2 & p3) | (p3 & p2 & p1 & p0 & c);
  assign G = g3 | (g2 & p3) | (g1 & p2 & p3) | (g0 & p1 & p2 & p3);
  assign P = (p3 & p2 & p1 & p0);
endmodule

module Adder16(output [15:0] o, input [15:0] a, input [15:0] b);
  logic c0;
  assign c0 = 0;
  logic G0, P0, c4;
  Adder4 first(o[3:0], G0, P0, a[3:0], b[3:0], c0);
  assign c4 = G0 | (P0 & c0);

  logic G1, P1, c8;
  Adder4 second(o[7:4], G1, P1, a[7:4], b[7:4], c4);
  assign c8 = G1 | (G0 & P1) | (P1 & P0 & c0);
  
  logic G2, P2, c12;
  Adder4 third(o[11:8], G2, P2, a[11:8], b[11:8], c8);
  assign c12 = G2 | (G1 & P2) | (G0 & P1 & P2) | (P2 & P1 & P0 & c0);
  
  logic G3, P3, c16;
  Adder4 fourth(o[15:12], G3, P3, a[15:12], b[15:12], c12);
  //  assign c16 = G3 | (G2 & P3) | (G1 & P2 & P3) | (G0 & P1 & P2 & P3) | (P3 & P2 & P1 & P0 & c0);
  
endmodule
