module ckt(
  input clk, rst, a, b, c,
  output logic q
);
  logic e;
  always_ff @(posedge clk or posedge rst)
    if (rst) begin
      e <= 1'b0;
      q <= 1'b0;
    end else begin
      e <= a || b;
      q <= e && c;
    end
endmodule

/*
stany:
BEGIN - !e, !q
E - e, !q
Q - e, q

przejscia przy posedge clk
BEGIN -(a)-> E
BEGIN -(b)-> E

E -(!a!b)-> BEGIN
E -(c)-> Q

Q -(!a!b)-> BEGIN
Q -(!ce)-> E
*/