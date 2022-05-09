module gcd#(parameter BYTES = 2)
  (output [(8*BYTES)-1:0] o,
   output fin,
   input [(8*BYTES)-1:0] a,b,
   input load, clk);
  logic [(8*BYTES)-1:0] A,B;
  assign fin = A==B;
  assign o = A;
  always_ff @(posedge clk or posedge load) begin
    if(load) begin
      A <= a;
      B <= b;
    end else if (A > B) begin
      A <= A - B;
    end
    else begin
      B <= A;
      A <= B;
    end    
  end
endmodule