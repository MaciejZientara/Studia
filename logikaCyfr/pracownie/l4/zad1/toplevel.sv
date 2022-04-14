// Rozpisanie z tabelki, cos na styl multipleksera
module shifter(output [3:0] o, input l, input r, input [3:0] i);
  assign o[3] = (0 		& r & !l) | (i[3] & !r & !l) | (i[2] & !r & l);
  assign o[2] = (i[3] 	& r & !l) | (i[2] & !r & !l) | (i[1] & !r & l);
  assign o[1] = (i[2] 	& r & !l) | (i[1] & !r & !l) | (i[0] & !r & l);
  assign o[0] = (i[1] 	& r & !l) | (i[0] & !r & !l) | (0	 & !r & l);
endmodule