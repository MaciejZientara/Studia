default: labirynt

%: %.cpp
	g++ -I. $< -o $@ AGL3Window.cpp  -lepoxy -lGL -lglfw 
clean:
	rm a.out *.o *~ labirynt
