// Wzorowalem sie na tych ukladach:
// https://www.chegg.com/homework-help/questions-and-answers/b-4-points-diagram-shows-design-one-digit-bcd-adder-subtractor-two-bcd-inputs-a3-0-b3-0-ca-q52579730
// https://www.eeeguide.com/wp-content/uploads/2016/09/4-Bit-BCD-Subtraction-using-9s-Complement-Method-0004.jpg
// https://electronics.stackexchange.com/questions/302155/bcd-subtractor-units-connections

module fullAdder(output S, output C, input a, input b, input c);
  assign S = a^b^c;
  assign C = a&b | a&c | b&c;
endmodule

module overflowAdder(output [3:0] s, output over, input [3:0] a, input [3:0] b, input c);
  logic c1, c2, c3;
  fullAdder add0 (s[0], c1, a[0], b[0], c);
  fullAdder add1 (s[1], c2, a[1], b[1], c1);
  fullAdder add2 (s[2], c3, a[2], b[2], c2);
  fullAdder add3 (s[3], over, a[3], b[3], c3);
endmodule

module minusIfSub(output [3:0] b, input [3:0] a, input sub);
  logic carry;
  overflowAdder add(b,carry,(a ^ {4{sub}}),0,sub);
  //assign b = (a ^ {4{sub}}) + sub;
endmodule//minus b jesli sub==1, else input=output 

module nineComplement(output [3:0] b, input [3:0] a, input sub);
  reg [3:0] A;
  minusIfSub minusA (A, a, sub);
  reg [3:0] compl;
  assign compl[3] = sub;
  assign compl[2] = 0;
  assign compl[1] = 0;
  assign compl[0] = sub;
  logic carry;
  overflowAdder add(b,carry,A,compl,0);
  //assign b = A + compl;
endmodule//wykonuje 9complement tylko kiedy sub == 1, else input=output

module BCDadder(output [3:0] s, output carry, input [3:0] a, input [3:0] b, input c, input sub);
  reg [3:0] complB;
  nineComplement ncompl (complB, b, sub);
  reg [3:0] Z;
  logic overflow;
  overflowAdder overAddAB(Z, overflow, a, complB, c);
  logic carry1;
  assign carry1 = Z[3]&Z[2] | Z[3]&Z[1] | overflow;
  reg [3:0] addTmp;
  assign addTmp[3] = 0;
  assign addTmp[2] = carry1;
  assign addTmp[1] = carry1;
  assign addTmp[0] = 0;
  logic carry2;
  overflowAdder overAdd6(s, carry2, addTmp, Z, 0);
  assign carry = carry1 | carry2;
endmodule

module doubleAdder(output [7:0] o, input [7:0] a, input [7:0] b, input sub);
  reg [3:0] first;
  logic carry1;
  BCDadder add1(first,carry1,a[3:0],b[3:0],sub,sub);
  logic carry2;
  reg [3:0] second;
  BCDadder add2(second,carry2,a[7:4],b[7:4],carry1,sub);
  assign o = {second, first};
endmodule
