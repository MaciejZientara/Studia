// Przyznaje, ze wzoru nie wymyśliłem tylko uslyszałem
// jak jeden ze studentów zapytał Pana w przerwie na wykładzie
// o rozwiązanie i zapamiętałem wzór jaki zapisał na tablicy...
// ale przeszło testy, więc chyba jest dobrze :P
module GrayToBinary(output [31:0] o, input [31:0] i);
  reg [31:0] A,B,C,D;
  assign A = i ^ (i>>16);
  assign B = A ^ (A>>8);
  assign C = B ^ (B>>4);
  assign D = C ^ (C>>2);
  assign o = D ^ (D>>1);
endmodule
