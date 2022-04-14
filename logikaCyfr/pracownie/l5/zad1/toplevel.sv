  // Na podstawie: https://en.wikipedia.org/wiki/Sorting_network
  // Dla i bedacym liczbami {4,3,2,1}
  // bede wykonywal nastepne porownania i swap:
  // 4-2
  // 3-1
  // 4-3
  // 2-1
  // 3-2
module sort4(output [15:0] o, input [15:0] i);
  reg [15:0] A,B,C,D,E;
  always_comb begin;
    if (i[4+:4] > i[12+:4])//4-2
      A = {i[4+:4],i[8+:4],i[12+:4],i[0+:4]};
  	else
      A = i;
    
    if (A[0+:4] > A[8+:4])//3-1
      B = {A[12+:4],A[0+:4],A[4+:4],A[8+:4]};
  	else
      B = A;
    
    if (B[8+:4] > B[12+:4])//4-3
      C = {B[8+:4],B[12+:4],B[4+:4],B[0+:4]};
  	else
      C = B;
    
    if (C[0+:4] > C[4+:4])//2-1
      D = {C[12+:4],C[8+:4],C[0+:4],C[4+:4]};
  	else
      D = C;

    if (D[4+:4] > D[8+:4])//3-2
      E = {D[12+:4],D[4+:4],D[8+:4],D[0+:4]};
  	else
      E = D;

  end
  assign o = E;
endmodule