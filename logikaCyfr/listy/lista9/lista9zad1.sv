module circuit(output out, input a, clk, nrst);
  logic [1:0] q;
  assign out = &q;
  always_ff @(posedge clk or negedge nrst) begin
    if(!nrst) q <= 2'b00;
    else begin
      q[0] <= a^q[0]^q[1];
      q[1] <= (!q[0] & !q[1]) | (!a & q[1]);
    end
  end
endmodule

/* mapy karnaugh:
1. mapa stan + input -> q0
a\q1q0
  00 01 11 10
0  0  1  0  1
1  1  0  1  0

q0 = a^(q0^q1);

2. mapa stan + input -> q1
a\q1q0
  00 01 11 10
0  1  0  1  1
1  1  0  0  0

q1 = (!q0 & !q1) | (!a & q1);

*/

/*
q  a q_out
00 0 10
01 0 01
10 0 11
11 0 10
00 1 11
01 1 00
10 1 00
11 1 01

      unique case({q,a})
        3'b001: q <= 2'b11;
        3'b011: q <= 2'b00;
        3'b101: q <= 2'b00;
        3'b111: q <= 2'b01;            
        3'b000: q <= 2'b10;
        3'b010: q <= 2'b01;
        3'b100: q <= 2'b11;
        3'b110: q <= 2'b10;            
      endcase        
*/