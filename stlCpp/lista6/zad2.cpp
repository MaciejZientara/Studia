#include <iostream>
#include <list>
#include <algorithm>
#include <string>

typedef unsigned char uc;

using namespace std;

class point{
	public:
	uc R, G, B;
	string name;
	double X, Y;
	point(uc r, uc g, uc b, string n, double x, double y):
	R(r),
	G(g),
	B(b),
	name(n),
	X(x),
	Y(y)
	{}
	
	double luminance(){
		return 0.3*(double)R + 0.59*(double)G + 0.11*(double)B;
	}
};
std::ostream &operator<<(std::ostream &os, point const &p) { 
    return os << p.name << " (" << p.X << ", " << p.Y << ") RGB: (" << (int)p.R << ", " << (int)p.G << ", " << (int)p.B << ")";
}


int main(){
	list<point> l{
		point(0xff,0xff,0xff,"alfa",100,100),
		point(0xfa,0xfa,0xfa,"beta",-100,100),
		point(0x1,0x1,0x1,"gamma",100,-100),
		point(0x7,0x2,0x5,"delta",-100,-100),
		point(0x0,0x0,0x0,"epsilon",32,-8.8),
		point(0xf0,0xf0,0xf0,"dzeta",-6.1,-5),
		point(0x88,0x88,0x88,"eta",-9,0.1),
		point(0x24,0x24,0x24,"teta",-18,-9.1),
		point(0x11,0x11,0x11,"jota",71,-71),
		point(0xaa,0xaa,0xaa,"kappa",-10,10.01),
		point(0x1,0x2,0x3,"lambda",100,100),
		point(0x10,0x20,0x30,"mi",100,100),
		point(0x77,0x77,0x77,"ni",100,100),
		point(0x18,0x18,0x18,"ksi",100,100),
		point(0x5,0x5,0x5,"omikron",100,100),
		point(0x0,0x0,0xff,"pi",14,209),
		point(0x0,0xff,0x0,"ro",100,100),
	};
	
	//part A
	l.remove_if([](point p){return (p.name.length()>5);});
	
	cout << "\n\nPART A\n";
	for_each(l.begin(),l.end(),[](point p){cout << p << "\n";});
	cout << "\n";

	//part B
	cout << "\n\nPART B\n";
	cout << "I cwiartka: " <<   count_if(l.begin(),l.end(),[](point p){return (p.X >= 0) && (p.Y >= 0);}) << "\n";
	cout << "II cwiartka: " <<  count_if(l.begin(),l.end(),[](point p){return (p.X <= 0) && (p.Y >= 0);}) << "\n";
	cout << "III cwiartka: " << count_if(l.begin(),l.end(),[](point p){return (p.X <= 0) && (p.Y <= 0);}) << "\n";
	cout << "IV cwiartka: " <<  count_if(l.begin(),l.end(),[](point p){return (p.X >= 0) && (p.Y <= 0);}) << "\n";
	
	//part C
    l.sort([](point A, point B){
        return A.luminance() < B.luminance();
    });
	
    cout << "\n\nPART C\n";
    for_each(l.begin(),l.end(),[](point p){cout << p << " luminance = " << p.luminance() << "\n";});
	cout << "\n";

	//part D
	auto it = find_if(l.begin(),l.end(),[](point A){return A.luminance()>=64.0;});
	
	int n = distance(l.begin(),it);
	list<point>dark;
	dark.resize(n,point(0,0,0," ",0,0));
	swap_ranges(l.begin(),it,dark.begin());
	while(n--)
    	l.pop_front();
	
    cout << "\n\nPART D\n";
    cout << "dark:\n";
    for_each(dark.begin(),dark.end(),[](point p){cout << p << " luminance = " << p.luminance() << "\n";});
	cout << "\n";

    cout << "bright:\n";
    for_each(l.begin(),l.end(),[](point p){cout << p << " luminance = " << p.luminance() << "\n";});
	cout << "\n";


}