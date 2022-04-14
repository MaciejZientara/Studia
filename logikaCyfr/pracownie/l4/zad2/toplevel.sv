module multi9(output [8:0] out, input [3:0] n);
  //multiplekser na 9 wyjsc na podstawie n
  assign out[8] = n[3] & !n[2] & !n[1] & !n[0];
  assign out[7] = !n[3] & n[2] & n[1] & n[0];
  assign out[6] = !n[3] & n[2] & n[1] & !n[0];
  assign out[5] = !n[3] & n[2] & !n[1] & n[0];
  assign out[4] = !n[3] & n[2] & !n[1] & !n[0];
  assign out[3] = !n[3] & !n[2] & n[1] & n[0];
  assign out[2] = !n[3] & !n[2] & n[1] & !n[0];
  assign out[1] = !n[3] & !n[2] & !n[1] & n[0];
  assign out[0] = !n[3] & !n[2] & !n[1] & !n[0];
endmodule

module funnelShifter(output [7:0] o, input [7:0] a, input [7:0] b, input [3:0] n);
  reg [8:0] switch;
  multi9 plex(switch,n);
  reg [15:0] tmp;
  assign tmp = {a,b};//nie jestem pewien czy {a,b} czy {b,a} ...
  assign o = (tmp[15-:8] & {8{switch[8]}}) | 
             (tmp[14-:8] & {8{switch[7]}}) | 
             (tmp[13-:8] & {8{switch[6]}}) | 
             (tmp[12-:8] & {8{switch[5]}}) | 
             (tmp[11-:8] & {8{switch[4]}}) | 
             (tmp[10-:8] & {8{switch[3]}}) | 
             (tmp[9-:8] & {8{switch[2]}}) | 
             (tmp[8-:8] & {8{switch[1]}}) | 
             (tmp[7-:8] & {8{switch[0]}});
endmodule

module magicShifter(output [7:0] o, input [7:0] i, input [3:0] n, input ar, input lr , input rot);
  reg [7:0] helper;
  //przesuniecie arytmetyczne rozmazuje najbardziej znaczacy bit tylko kiedy przesuwamy w prawo
  //to znaczy rozmazuje dla ar & !lr, wpp wypelniam 0
  //jesli rot wysoki to helper = i, wpp to co wyzej
  assign helper = (i & {8{rot}}) | 
                  ({8{i[7] & ar & !lr}} & {8{!rot}});
  //jesli w lewo i po lewej(A), jesli w prawo i po prawej(B), helper z drugiej strony
  funnelShifter fS(o, 
                   ((i & {8{lr}}) | (helper & {8{!lr}})), 
                   ((i & {8{!lr}}) | (helper & {8{lr}})), 
                   ((n & {4{!lr}}) | ((8-n) & {4{lr}})));//w lewo przesuniecie o n, w prawo o 8-n
endmodule