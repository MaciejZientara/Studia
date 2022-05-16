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
!E!Q - !e, !q
E!Q - e, !q
EQ - e, q
!EQ - !e, q

przejscia przy posedge clk
Start = !E!Q

!E!Q -(a+b)-> E!Q

E!Q -((a+b)c)-> EQ
E!Q -(!(a+b)c)-> !EQ
E!Q -(!(a+b)!c)-> !E!Q

EQ -((a+b)!c)-> E!Q
EQ -(!(a+b)c)-> !EQ

!EQ -(!(a+b))-> !E!Q
!EQ -(a+b)-> E!Q

nie oznaczone, ale sa oczywiscie tez petelki na kazdym ze stanow
*/
