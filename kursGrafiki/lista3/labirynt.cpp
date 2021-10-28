// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <new>
#include <string>

#include <unistd.h>	//usleep na linux, nie znalazlem sposobu na crossplatform

#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>

// ==========================================================================
// Drawable object: no-data only: vertex/fragment programs
// ==========================================================================
class MySqu : public AGLDrawable {
public:
   MySqu() : AGLDrawable(0) {
      setShaders();
   } 
   void setShaders() {
      compileShaders(R"END(

         #version 330 
         out vec4 vcolor;
         void main(void) {
            const vec2 vertices[6] = vec2[6](vec2( 0.0,  0.0),
															vec2(-0.9, -0.9),
															vec2( 0.9, -0.9),
                                             vec2( 0.9,  0.9),
															vec2(-0.9,  0.9),
                                             vec2(-0.9, -0.9));
            const vec4 colors[]    = vec4[6](vec4(0.8, 0.0, 0.8, 1.0),
															vec4(0.0, 0.0, 0.0, 1.0),
															vec4(1.0, 0.0, 0.0, 1.0),
                                             vec4(0.0, 1.0, 0.0, 1.0),
															vec4(0.0, 0.0, 1.0, 1.0),
                                             vec4(0.0, 0.0, 0.0, 1.0));
				vcolor   	= colors[gl_VertexID];
            gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0); 
         }

      )END", R"END(

         #version 330 
         in  vec4 vcolor;
         out vec4 color;

         void main(void) {
				color = vcolor;
         } 

      )END");
   }
   void draw() {
      bindProgram();
      glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
   }
};
// ==========================================================================

int n = 10;

class MyLines : public AGLDrawable {
GLuint offbuff;
public:
	float* points;
   MyLines() : AGLDrawable(0) {
      points = new float[4*n*n];
		setShaders();
      setBuffers();
   }
   void setShaders() {
		std::string s = R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in vec2 pos;
			layout(location = 1) in vec2 offsetA;
			layout(location = 2) in vec2 offsetB;
			out vec4 col;

         void main(void) {
				if(gl_VertexID%2==0){
					col = vec4(1.0,0.0,0.0,1.0);
         	   gl_Position = vec4(pos+offsetB, 0.0, 1.0);
				}
				else{
					col = vec4(1.0,1.0,0.0,1.0);
            	gl_Position = vec4(pos+offsetA, 0.0, 1.0);
				}
         }

      )END";

      
		compileShaders(s.c_str(), R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         in vec4 col;
			out vec4 color;

         void main(void) {
            color = col;
         } 

      )END");
   }
	void bindoffset(){
		//bindBuffers();
		glBindBuffer(GL_ARRAY_BUFFER,offbuff);
		
	}

   void setBuffers() {
		glGenBuffers(1, &offbuff);      
		glBindBuffer(GL_ARRAY_BUFFER, offbuff);
				

      GLfloat vert[2][2] = {  // line
         {0, 0},
         {0, 0}
      };

		float dist = 1.8/(n-1);
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				float x = -0.9+j*dist, y = 0.9-i*dist;
				
				float alfa = rand();
				float dy = sin(alfa)/n, dx = cos(alfa)/n;
				
				int indx = 2*(n*i+j);
				points[indx]=x-dx;
				points[indx+1]=y-dy;
				points[indx+2*n*n]=x+dx;
				points[indx+2*n*n+1]=y+dy;
				
				if(indx == (2*n*(n-1))){
					points[indx]=-10;
					points[indx+1]=-10;
					points[indx+2*n*n]=-10;
					points[indx+2*n*n+1]=-10;
				}
			}
		}

		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*n*n, &points[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER,0);
	
		bindBuffers();
      glBufferData(GL_ARRAY_BUFFER, 2*2*sizeof(float), vert, GL_STATIC_DRAW );
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(
         0,                 // attribute 0, must match the layout in the shader.
         2,                  // size
         GL_FLOAT,           // type
         GL_FALSE,           // normalized?
         0,//24,             // stride
         (void*)0            // array buffer offset
      );


		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER,offbuff);
		glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
		//glBindBuffer(GL_ARRAY_BUFFER,0);
		glEnableVertexAttribArray(2);
		//glBindBuffer(GL_ARRAY_BUFFER,offbuff);
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)(2*n*n*sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glVertexAttribDivisor(1,1);
		glVertexAttribDivisor(2,1);

   }
   void draw() {
      bindProgram();
      bindBuffers();


      glDrawArraysInstanced(GL_LINES, 0, 2, n*n);//moze bez -1 a tam gdzie start dac 
		//offset daleko za plansze
   }

};
// ==========================================================================
// ==========================================================================
// ==========================================================================

class MyPlayer : public AGLDrawable {
public:
	float posx, posy, rotx, roty, length, speed;
	void reset(){
		posx = -0.9;
		posy = -0.9;
		rotx = length;
		roty = 0.0;
	}

   MyPlayer() : AGLDrawable(0) {
		length = 1.0f/n;
		reset();
		speed = 0.01;
      setShaders();
      setBuffers();
   }
   void setShaders() {
      compileShaders(R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in vec2 pos;
         layout(location = 0) uniform vec2 move;
			layout(location = 1) uniform vec2 rot;
			out vec4 vtex;
			out vec4 col;

         void main(void) {
				vec2 p;
				if(gl_VertexID%2==0){
					col = vec4(0.1,0.1,0.1,1.0);
					p = pos + move + rot;
				}
				else{
					col = vec4(1.0,1.0,1.0,1.0);
					p = pos + move - rot;
				}
            gl_Position = vec4(p, 0.0, 1.0);
         }

      )END", R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         in vec4 col;
			out vec4 color;

         void main(void) {
            color = col;
         } 

      )END");
   }
   void setBuffers() {
      bindBuffers();
      GLfloat vert[2][2] = {
			{0,0},
			{0,0}
		};
		

      glBufferData(GL_ARRAY_BUFFER, 2*2*sizeof(float), vert, GL_STATIC_DRAW );
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(
         0,                 // attribute 0, must match the layout in the shader.
         2,                  // size
         GL_FLOAT,           // type
         GL_FALSE,           // normalized?
         0,//24,             // stride
         (void*)0            // array buffer offset
      );
   }
	
   void draw(int dir, float ang) {
      bindProgram();
      bindBuffers();

		float sn = sin(ang), cs = cos(ang);

		posx += dir*(cs*speed);
		posy += dir*(sn*speed);
//dodac obrot vertex do draw
		glUniform2f(0,posx,posy);

		rotx = cs * length;
		roty = sn * length;

		glUniform2f(1,rotx,roty);

      glDrawArrays(GL_LINES, 0, 2);
   }

};





// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================
class MyWin : public AGLWindow {
public:
    MyWin() {};
    MyWin(int _wd, int _ht, const char *name, int vers, int fullscr=0)
        : AGLWindow(_wd, _ht, name, vers, fullscr) {};
    virtual void KeyCB(int key, int scancode, int action, int mods);
    void MainLoop();
};


// ==========================================================================
void MyWin::KeyCB(int key, int scancode, int action, int mods) {
    AGLWindow::KeyCB(key,scancode, action, mods); // f-key full screen switch
    if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS) {
       ; // do something
    }
    if (key == GLFW_KEY_HOME  && (action == GLFW_PRESS)) {
       ; // do something
    }
}
// ==========================================================================

// ==========================================================================
void MyWin::MainLoop() {
   ViewportOne(0,0,wd,ht);

   MyLines lines;
	MyPlayer player;
	MySqu square;

	int dir = 0;
   float ang=0.0;
	
	int targetFPS = 60;
	double timePerFrame = 1000/targetFPS;
	int frameCount = 0;

	int STARTtime = time(0);

	do {
		double startTime = time(0);
      glClear( GL_COLOR_BUFFER_BIT );
   
      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
		square.draw();
		
		lines.draw();
      player.draw(dir,ang);
		
      AGLErrors("main-afterdraw");
	


		float p1x = player.posx + player.rotx, p1y = player.posy + player.roty;
		float p2x = player.posx - player.rotx, p2y = player.posy - player.roty;
		
		for(int i=0; i<n; i++)
			for(int j=0; j<n; j++){
				float q1x = lines.points[2*(n*i+j)], q1y = lines.points[2*(n*i+j)+1];
				float q2x = lines.points[2*(n*i+j)+2*n*n], q2y = lines.points[2*(n*i+j)+1+2*n*n];
				
				if((((q1x-p1x)*(p2y-p1y) - (q1y-p1y)*(p2x-p1x))
            * ((q2x-p1x)*(p2y-p1y) - (q2y-p1y)*(p2x-p1x)) < 0)
            &&
           (((p1x-q1x)*(q2y-q1y) - (p1y-q1y)*(q2x-q1x))
            * ((p2x-q1x)*(q2y-q1y) - (p2y-q1y)*(q2x-q1x)) < 0)){
					if(i==0 && j==n-1){
						printf("finished in %d sec\n",(int)time(0)-STARTtime);
						return;
					}

					dir = 0;
					ang = 0.0;
					player.reset();
				}

			}	

      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();
      //glfwWaitEvents();   

		dir = 0;
      if (glfwGetKey(win(), GLFW_KEY_DOWN ) == GLFW_PRESS) {
         dir = -1;
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      } else if (glfwGetKey(win(), GLFW_KEY_UP ) == GLFW_PRESS) {
         dir = 1;
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      } else if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
         ang -= 0.05;
      } else if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
			ang += 0.05;
      }

		double renderTime = time(0) - startTime;
		if(renderTime < timePerFrame)
			usleep((timePerFrame-renderTime)*1000);
		


   } while( glfwGetKey(win(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(win()) == 0 );
}

int main(int argc, char *argv[]) {
   if(argc>1)
		srand(atoi(argv[1]));
	else
		srand(time(0));

	if(argc==3){
		n = atoi(argv[2]);
		if(n<2)		//zabezpieczenie przed za mala
			n=10;		//wartoscia n
	}
	//argumenty wywolania(1)
	//instancing(2extra)
	//poprawnosc calej gry(10)
	//klatki na sekunde(1)
	//wysrodkowanie i skalowanie okna(1)


	MyWin win;
   win.Init(800,600,"AGL3 example",0,33);
   win.MainLoop();
   return 0;
}
