module circuit(output out, input a, clk, nrst);
  logic [1:0] q;
  assign out = &q;
  always_ff @(posedge clk or negedge nrst) begin
    if(!nrst) q <= 2'b00;
    else
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
  end
endmodule