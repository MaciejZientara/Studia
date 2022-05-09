module lfsr(
output logic [2:0] q,
input [2:0] d,
input load, clk
);
always_ff @(posedge clk)
if (load) q <= d;
else q <= {q[0], q[0] ^ q[2], q[1]};
endmodule

// 0 -> 0
// 001 -> 110 -> 011 -> 111 -> 101 -> 100 -> 010 -> 001
//   1	    6 	   3	  7	 	 5		4	   2	  1