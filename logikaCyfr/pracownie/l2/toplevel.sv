// W tym rozwiązaniu sprawdzam czy jakieś 2 bity są zapalone.
// Jeżeli są zapalone 3 bity to dwie bramki AND zwrócą prawde.
// Ponieważ mamy zwrócić 0 dla 4 zapalonych bitów, 
// to mam dodatkowo negacje na AND wszystkich bitów.

module circuit(output o, input [3:0] i);
  assign o = (i[3]&i[0] | i[3]&i[1] | i[2]&i[0] | &i[3:2] | &i[2:1] | &i[1:0]) & !&i;
endmodule