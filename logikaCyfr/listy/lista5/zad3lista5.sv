module zadanie(
input [1:0] w,
input en,
output logic y0, y1, y2, y3//dodac logic
);
always_comb begin
y0 = 0;
y1 = 0;
y2 = 0;
y3 = 0;
if (en)
if (w == 0) y0 = 1;
else if (w == 1) y1 = 1;
else if (w == 2) y2 = 1;
else y3 = 1;//w==3
end
endmodule
//wektor y ma same 0 i jedna 1 na pozycji zaleznej od w
//to oznacza, ze przez uzywanie bardzo wielu zagniezdzonych if else
//wykonujemy dekoder 2 do 4