
module circuit1(output [3:0] r1, input [3:0] r2, input clk,s1,s2,nrst);
  always_ff @(posedge clk or negedge nrst) begin
    if (!nrst) r1 <= 4'b0;
    else if (s1) r1 <= r1 + r2;
    else if (s2) r1 <= r1 + 1;
    else r1 <= r1;
  end
endmodule

module circuit2(output [3:0] r1, input [3:0] r2, input clk,s1,s2,nrst);
  always_ff @(posedge clk or negedge nrst) begin
    if (!nrst) r1 <= 4'b0;
    else r1 <= r1 + ((r2 & {4{s1}}) | (!s1&s2));
  end
endmodule

module compareOut(output [3:0] r11, output [3:0] r12, input [3:0] r2, input clk,s1,s2,nrst);
  circuit1 c1(r11,r2,clk,s1,s2,nrst);  
  circuit2 c2(r12,r2,clk,s1,s2,nrst);
endmodule

