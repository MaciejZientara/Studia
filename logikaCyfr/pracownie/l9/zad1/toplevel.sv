module circuit(output bell, light, heat,
               input finish, start, door, nrst, clk);
  logic [4:0] q;
  /* kodowanie one-hot
  00001 - closed
  00010 - cook
  00100 - pause
  01000 - bell
  10000 - open
  */
  assign bell = q[3];
  assign heat = q[1] & !door; 
  //wylaczenie przy otwarciu drzwi wyniknie ze stanu ale jest zalezne od zegara
  //w celach bezpieczenstwa lepiej miec krancowke odcinajaca doplyw pradu
  //kiedy otworzy sie drzwi (tak jest w sprzedawanych mikrofalach)
  assign light = q[4] | q[2] | q[1];

  always_ff @(posedge clk, negedge nrst) begin
    if (!nrst) q <= 5'b00001;
    else begin
      q[0] <= !door & (q[4] | (!start & q[0]));
      q[1] <= !door & ((start & q[0]) | q[2] | (!finish & q[1]));
      q[2] <= door & (q[1] | q[2]);
      q[3] <= !door & ((finish & q[1]) | q[3]);
      q[4] <= door & (q[0] | q[3] | q[4]);
    end
  end
endmodule