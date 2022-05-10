module PWM(input clk,
           input [15:0] d,
           input [1:0] sel,
           output out,
           output logic [15:0] cnt,
           output logic [15:0] cmp,
           output logic [15:0] top);

  assign out = cnt < cmp;
  always_ff @(posedge clk) begin
    cnt <= cnt + 1;
    if (cnt >= top)
      cnt <= 0;
    if (sel == 2'b01)
      cmp <= d;
    if (sel == 2'b10)
      top <= d;
    if (sel == 2'b11)
      cnt <= d;
  end
endmodule