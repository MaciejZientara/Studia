module lfsr(
output logic [3:0] q,
input clk,
input nrst
);
always_ff @(posedge clk or negedge nrst)
if (!nrst) q <= 4'b0 ;
else q <= {q[2:0], q[3] ~^ q[1]};
endmodule

// 0000 -> 0001 -> 0011 -> 0110 -> 1100 -> 1000 -> 0000
// nie ma mozliwosci ustawic input, mozna tylko wyzerowac
// co oznacza, ze jest to jedyny cykl w tym liczniku