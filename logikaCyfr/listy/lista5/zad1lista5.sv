module multi(output logic [7:0] o, input [7:0] i, input [2:0] n);
  always_comb
    case(n)
      3'b1: o = i;
      3'b10: o = i<<1;
      3'b11: o = i + (i<<1);
      3'b100: o = i<<2;
      default: o = 8'bxxxxxxxx;
    endcase
endmodule
