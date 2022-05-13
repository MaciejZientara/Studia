module circuit(output bell, light, heat,
               input finish, start, door, nrst, clk);
  logic [4:0] q;
  // kody stanów automatu (one-hot)
  const logic [4:0] 
  CLOSED = 5'b00001, 
  COOK = 5'b00010, 
  PAUSE = 5'b00100, 
  BELL = 5'b01000, 
  OPEN = 5'b10000;
  //funkcja wyjsciowa
  assign bell = q[3];
  assign heat = q[1];
  assign light = q[4] | q[2] | q[1];
  //funkcja przejscia
  always_ff @(posedge clk, negedge nrst) begin
    if (!nrst) q <= 5'b00001;
    else unique case(q)
      CLOSED: begin q <= (door ? OPEN : (start ? COOK : CLOSED)); end
      COOK: begin q <= (door ? PAUSE : (finish ? BELL : COOK)); end
      PAUSE: begin q <= (!door ? COOK : PAUSE); end
      BELL: begin q <= (door ? OPEN : BELL); end
      OPEN: begin q <= (!door ? CLOSED : OPEN); end
    endcase
  end
endmodule