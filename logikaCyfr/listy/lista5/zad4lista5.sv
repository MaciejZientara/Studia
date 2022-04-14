module decoder_2_to_4(output [3:0] o, input [1:0] i);
  assign o = 4'b1 << i;
endmodule

module decoder_3_to_8(output [7:0] o, input [2:0] i);
  reg [3:0] vec1;
  reg [3:0] vec2;
  decoder_2_to_4 d1(vec1,i[1:0]);
  decoder_2_to_4 d2(vec2,i[1:0]);//mozna tego nie robic, bo vec1 = vec2
  assign o = {(vec1 & {4{i[2]}}),(vec2 & {4{!i[2]}})};
endmodule