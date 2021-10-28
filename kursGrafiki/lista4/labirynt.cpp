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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <unistd.h>	//usleep na linux, nie znalazlem sposobu na crossplatform

#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>

// ==========================================================================
// Drawable object: no-data only: vertex/fragment programs
// ==========================================================================


// ==========================================================================
// ==========================================================================
int n=5;

class Camera{
   public:
   bool mode;//false - first person, true - third person
   float rotateSpeed = 0.02f;
   float speed = 0.015f;
   glm::vec4 position;
   glm::vec4 forward;
   glm::vec4 up;
   glm::mat4 projMat = glm::perspective(45.0f, 1.0f, 0.1f, 5.0f);
   glm::mat4 modeMatrix;
   Camera(){
      mode = false;
      up = glm::vec4(0,1,0,0);
      position = glm::vec4(0,0,0,1);
      forward = glm::vec4(0,0,-1,0);
   }
   void changeMode(){
      float v = 0.3f;
      if(mode)
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      else
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      
      mode = !mode;
   }
   glm::mat4 view(){
      if(mode)
         return glm::lookAt(glm::vec3(glm::translate(glm::mat4(1),glm::vec3(-0.3f*forward))*position),glm::vec3(position+forward),glm::vec3(0.0f,1.0f,0.0f));
      else
         return glm::lookAt(glm::vec3(position),glm::vec3(position+forward),glm::vec3(0.0f,1.0f,0.0f));
   }
};

Camera cam;
// ==========================================================================
// ==========================================================================

class MySqu : public AGLDrawable {
public:
   MySqu() : AGLDrawable(0) {
      setShaders();
   } 
   void setShaders() {
      compileShaders(R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) uniform vec3 camPos;
         layout(location = 1) uniform mat4 trans;

         out vec4 vcolor;
         void main(void) {
            const vec3 vertices[36] = vec3[36](
               //down
               vec3( 1.25, 1.25,-1.25),
               vec3( 1.25, 1.25, 0.25),
               vec3(-0.25, 1.25, 0.25),
               vec3(-0.25, 1.25, 0.25),
               vec3(-0.25, 1.25,-1.25),
               vec3( 1.25, 1.25,-1.25),
               //up
               vec3( 1.25,-0.25,-1.25),
               vec3(-0.25,-0.25, 0.25),
               vec3( 1.25,-0.25, 0.25),
               vec3( 1.25,-0.25,-1.25),
               vec3(-0.25,-0.25,-1.25),
               vec3(-0.25,-0.25, 0.25),
               //left
               vec3( 1.25,-0.25, 0.25),
               vec3( 1.25, 1.25, 0.25),
               vec3( 1.25, 1.25,-1.25),
               vec3( 1.25, 1.25,-1.25),
               vec3( 1.25,-0.25,-1.25),
               vec3( 1.25,-0.25, 0.25),
               //right
               vec3(-0.25,-0.25,-1.25),
               vec3(-0.25, 1.25,-1.25),
               vec3(-0.25, 1.25, 0.25),
               vec3(-0.25, 1.25, 0.25),
               vec3(-0.25,-0.25, 0.25),
               vec3(-0.25,-0.25,-1.25),
               //front
               vec3(-0.25,-0.25, 0.25),
               vec3(-0.25, 1.25, 0.25),
               vec3( 1.25, 1.25, 0.25),
               vec3( 1.25, 1.25, 0.25),
               vec3( 1.25,-0.25, 0.25),
               vec3(-0.25,-0.25, 0.25),
               //back
               vec3( 1.25,-0.25,-1.25),
               vec3( 1.25, 1.25,-1.25),
               vec3(-0.25, 1.25,-1.25),
               vec3(-0.25, 1.25,-1.25), 
               vec3(-0.25,-0.25,-1.25),
               vec3( 1.25,-0.25,-1.25)
            );

            vec4 col = vec4(0.2,0.2,0.2,1.0) + vec4(0.7*camPos,0.0);
            vcolor = col;

            gl_Position = trans * vec4(vertices[gl_VertexID], 1.0); 
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
      glm::mat4 tmp = cam.projMat * cam.view();
      glm::vec3 t = glm::vec3(cam.position);
      glUniform3fv(0, 1, &t[0]);
		glUniformMatrix4fv(1,1,false,&tmp[0][0]);
      glDrawArrays(GL_TRIANGLES, 0, 36);
   }
};


class MyWall : public AGLDrawable {
GLuint offbuff;
public:
	glm::mat4 *offsets, *offsetsBuffer;
   MyWall() : AGLDrawable(0) {
      offsets = new glm::mat4[n*n*n];
      offsetsBuffer = new glm::mat4[n*n*n];
		setShaders();
      setBuffers();
   }
   void setShaders() {
		compileShaders(R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in vec3 pos;
         layout(location = 1) in mat4 model;
			layout(location = 5) uniform mat4 trans;
         layout(location = 9) uniform int n;
         out vec4 col;

         void main(void) {
            vec4 p = model * vec4(pos,1.0);
            if(gl_InstanceID == (n*n*n-1)){
               col = vec4(0.0,0.0,0.0,1.0);
            }
            else{
               col = vec4(0.1+0.9*p.x, 0.1+0.9*p.y, 0.1-0.9*p.z, 1.0);
            }
            gl_Position = trans * p;
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
		glGenBuffers(1, &offbuff);      
		glBindBuffer(GL_ARRAY_BUFFER, offbuff);

      glm::vec3 vert[6] = {
         glm::vec3( 1.0f,-1.0f, 0.001f),
         glm::vec3( 1.0f, 1.0f, 0.001f),
         glm::vec3(-1.0f, 1.0f, 0.001f),
         
         glm::vec3(-1.0f, 1.0f, 0.001f),
         glm::vec3(-1.0f,-1.0f, 0.001f),
         glm::vec3( 1.0f,-1.0f, 0.001f)
      };

		float dist = 1.0f/(n-1), s = 0.75f/n;
      glm::mat4 skala = glm::scale(glm::mat4(1),glm::vec3(s,s,s));
      for(int i=0; i<n; i++)
         for(int j=0; j<n; j++)
            for(int k=0; k<n; k++){
               glm::mat4 pos = glm::translate(glm::mat4(1),glm::vec3(i*dist,j*dist,-k*dist));
               glm::mat4 rot = glm::rotate(glm::mat4(1),(float)(rand()%180),glm::vec3(1.0f+rand()%5,(float)(rand()%5),(float)(rand()%5)));
               offsets[n*n*i+n*j+k] = pos * rot * skala;
               offsetsBuffer[n*n*i+n*j+k] = offsets[n*n*i+n*j+k];
            }
      offsets[0] = glm::translate(offsets[0],glm::vec3(0,100,0));
      offsetsBuffer[0] = glm::translate(offsetsBuffer[0],glm::vec3(0,100,0));


		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*n*n*n, &offsetsBuffer[0], GL_STATIC_DRAW);


		bindBuffers();
      glBufferData(GL_ARRAY_BUFFER, 6*sizeof(glm::vec3), vert, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(
         0,                 // attribute 0, must match the layout in the shader.
         3,                  // size
         GL_FLOAT,           // type
         GL_FALSE,           // normalized?
         0,//24,             // stride
         (void*)0            // array buffer offset
      );


		glBindBuffer(GL_ARRAY_BUFFER,offbuff);
		glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(glm::mat4), (void*)(0  * sizeof(float)));
      glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(glm::mat4), (void*)(4  * sizeof(float)));
      glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(glm::mat4), (void*)(8  * sizeof(float)));
      glVertexAttribPointer(4, 4, GL_FLOAT, false, sizeof(glm::mat4), (void*)(12 * sizeof(float)));
      glEnableVertexAttribArray(1);
      glEnableVertexAttribArray(2);
      glEnableVertexAttribArray(3);
      glEnableVertexAttribArray(4);
		glVertexAttribDivisor(1,1);
      glVertexAttribDivisor(2,1);
      glVertexAttribDivisor(3,1);
      glVertexAttribDivisor(4,1);

   }
   void draw() {
      bindProgram();
      bindBuffers();

      glm::mat4 proj = cam.projMat * cam.view();

      glUniformMatrix4fv(5,1,false,&proj[0][0]);
      glUniform1i(9,n);

      glDrawArraysInstanced(GL_TRIANGLES, 0, 6, n*n*n);
   }

};
// ==========================================================================
// ==========================================================================
// ==========================================================================


class MyPlayer : public AGLDrawable {      
int iter=0;
glm::mat4 skala = glm::scale(glm::mat4(1),glm::vec3(0.1f/n,0.1f/n,0.1f/n));
public:
   glm::vec4 position = glm::vec4(0.0f,0.0f,0.0f,1.0f);
   MyPlayer() : AGLDrawable(0) {
      setShaders();
      setBuffers();
   }
   void setShaders() {
      compileShaders(R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in vec3 pos;
         layout(location = 1) uniform mat4 view;
         layout(location = 5) uniform mat4 model;

			out vec4 vtex;

         void main(void) {
            gl_Position = view * model * vec4(pos, 1.0);
         }

      )END", R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
			out vec4 color;

         void main(void) {
            color = vec4(1.0,0.0,0.0,1.0);
         } 

      )END");
   }
   void setBuffers() {
      bindBuffers();


      glm::vec3 vert[8*16*3];
      float dia = sqrt(2.0f)/4.0f, len = 1.0f/4.0f;
      glm::vec3   downFront = glm::vec3(0.0f,-len,len),
                  downBack = glm::vec3(0.0f,-len,-len), 
                  leftBack = glm::vec3(-dia,0.0f,-len),
                  rightBack = glm::vec3(dia,0.0f,-len);


      //https://stackoverflow.com/questions/7687148/drawing-sphere-in-opengl-without-using-glusphere
      //deklaracja sfery
      for(int i=1; i<=4; i++){
         glm::vec3 up = glm::vec3(0,1,0), down = glm::vec3(0,-1,0);
         for(int j=1; j<=i; j++){
            //left up front
            vert[iter++] = glm::normalize(up     + (float)i      * downFront + (float)j     * leftBack);
            vert[iter++] = glm::normalize(up     + (float)i      * downFront + (float)(j-1) * leftBack);
            vert[iter++] = glm::normalize(up     + (float)(i-1)  * downFront + (float)(j-1) * leftBack);
            if(i!=4){
               vert[iter++] = glm::normalize(up  + (float)i      * downFront + (float)j     * leftBack);
               vert[iter++] = glm::normalize(up  + (float)i      * downFront + (float)(j-1) * leftBack);
               vert[iter++] = glm::normalize(up  + (float)(i+1)  * downFront + (float)j     * leftBack);
            }
            //right up front
            vert[iter++] = glm::normalize(up     + (float)i      * downFront + (float)j     * rightBack);
            vert[iter++] = glm::normalize(up     + (float)i      * downFront + (float)(j-1) * rightBack);
            vert[iter++] = glm::normalize(up     + (float)(i-1)  * downFront + (float)(j-1) * rightBack);
            if(i!=4){
               vert[iter++] = glm::normalize(up  + (float)i      * downFront + (float)j     * rightBack);
               vert[iter++] = glm::normalize(up  + (float)i      * downFront + (float)(j-1) * rightBack);
               vert[iter++] = glm::normalize(up  + (float)(i+1)  * downFront + (float)j     * rightBack);
            }
            //left up back
            vert[iter++] = glm::normalize(up     + (float)i      * downBack - (float)j     * rightBack);
            vert[iter++] = glm::normalize(up     + (float)i      * downBack - (float)(j-1) * rightBack);
            vert[iter++] = glm::normalize(up     + (float)(i-1)  * downBack - (float)(j-1) * rightBack);
            if(i!=4){
               vert[iter++] = glm::normalize(up  + (float)i      * downBack - (float)j     * rightBack);
               vert[iter++] = glm::normalize(up  + (float)i      * downBack - (float)(j-1) * rightBack);
               vert[iter++] = glm::normalize(up  + (float)(i+1)  * downBack - (float)j     * rightBack);
            }
            //right up back
            vert[iter++] = glm::normalize(up     + (float)i      * downBack - (float)j     * leftBack);
            vert[iter++] = glm::normalize(up     + (float)i      * downBack - (float)(j-1) * leftBack);
            vert[iter++] = glm::normalize(up     + (float)(i-1)  * downBack - (float)(j-1) * leftBack);
            if(i!=4){
               vert[iter++] = glm::normalize(up  + (float)i      * downBack - (float)j     * leftBack);
               vert[iter++] = glm::normalize(up  + (float)i      * downBack - (float)(j-1) * leftBack);
               vert[iter++] = glm::normalize(up  + (float)(i+1)  * downBack - (float)j     * leftBack);
            }

            //left down back
            vert[iter++] = glm::normalize(down     - (float)i      * downFront - (float)j     * rightBack);
            vert[iter++] = glm::normalize(down     - (float)i      * downFront - (float)(j-1) * rightBack);
            vert[iter++] = glm::normalize(down     - (float)(i-1)  * downFront - (float)(j-1) * rightBack);
            if(i!=4){
               vert[iter++] = glm::normalize(down  - (float)i      * downFront - (float)j     * rightBack);
               vert[iter++] = glm::normalize(down  - (float)i      * downFront - (float)(j-1) * rightBack);
               vert[iter++] = glm::normalize(down  - (float)(i+1)  * downFront - (float)j     * rightBack);
            }
            //right down back
            vert[iter++] = glm::normalize(down     - (float)i      * downFront - (float)j     * leftBack);
            vert[iter++] = glm::normalize(down     - (float)i      * downFront - (float)(j-1) * leftBack);
            vert[iter++] = glm::normalize(down     - (float)(i-1)  * downFront - (float)(j-1) * leftBack);
            if(i!=4){
               vert[iter++] = glm::normalize(down  - (float)i      * downFront - (float)j     * leftBack);
               vert[iter++] = glm::normalize(down  - (float)i      * downFront - (float)(j-1) * leftBack);
               vert[iter++] = glm::normalize(down  - (float)(i+1)  * downFront - (float)j     * leftBack);
            }
            //left down front
            vert[iter++] = glm::normalize(down     - (float)i      * downBack + (float)j     * leftBack);
            vert[iter++] = glm::normalize(down     - (float)i      * downBack + (float)(j-1) * leftBack);
            vert[iter++] = glm::normalize(down     - (float)(i-1)  * downBack + (float)(j-1) * leftBack);
            if(i!=4){
               vert[iter++] = glm::normalize(down  - (float)i      * downBack + (float)j     * leftBack);
               vert[iter++] = glm::normalize(down  - (float)i      * downBack + (float)(j-1) * leftBack);
               vert[iter++] = glm::normalize(down  - (float)(i+1)  * downBack + (float)j     * leftBack);
            }
            //right down front
            vert[iter++] = glm::normalize(down     - (float)i      * downBack + (float)j     * rightBack);
            vert[iter++] = glm::normalize(down     - (float)i      * downBack + (float)(j-1) * rightBack);
            vert[iter++] = glm::normalize(down     - (float)(i-1)  * downBack + (float)(j-1) * rightBack);
            if(i!=4){
               vert[iter++] = glm::normalize(down  - (float)i      * downBack + (float)j     * rightBack);
               vert[iter++] = glm::normalize(down  - (float)i      * downBack + (float)(j-1) * rightBack);
               vert[iter++] = glm::normalize(down  - (float)(i+1)  * downBack + (float)j     * rightBack);
            }
            
         }
      }


      glBufferData(GL_ARRAY_BUFFER, iter*sizeof(glm::vec3), vert, GL_STATIC_DRAW );
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(
         0,                 // attribute 0, must match the layout in the shader.
         3,                  // size
         GL_FLOAT,           // type
         GL_FALSE,           // normalized?
         0,//24,             // stride
         (void*)0            // array buffer offset
      );
   }
	
   void draw(float dir, float angX, float angY) {
      bindProgram();
      bindBuffers();

      glm::mat4 rotY;
      glm::mat4 rotX;
      if(cam.mode){
         rotX = glm::rotate(-angX,glm::vec3(cam.up));
         rotY = glm::rotate(-angY,glm::vec3(glm::cross(glm::vec3(cam.forward),glm::vec3(cam.up))));
      }
      else{
         rotX = glm::rotate(angX,glm::vec3(cam.up));
         rotY = glm::rotate(angY,glm::vec3(glm::cross(glm::vec3(cam.forward),glm::vec3(cam.up))));
      }

      cam.forward = rotX * rotY * cam.forward;

      glm::mat4 move;
      move = glm::translate(glm::mat4(1),(float)dir*glm::vec3(cam.forward)+glm::vec3(position));
      
      cam.position += dir*cam.forward;
      position += dir*cam.forward;

      glm::mat4 tmp = move * rotX * rotY * skala;
      glm::mat4 m = cam.projMat * cam.view();

		glUniformMatrix4fv(1,1,false,&m[0][0]);
      glUniformMatrix4fv(5,1,false,&tmp[0][0]);

      glDrawArrays(GL_TRIANGLES, 0, iter);
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

   MyWall walls;
	MyPlayer player;
	MySqu square;

	float dir = 0.0f;
   float angX=0.0,angY=0.0;
	
	int targetFPS = 60;
	double timePerFrame = 1000/targetFPS;
	int frameCount = 0;

	int STARTtime = time(0);


	do {
		double startTime = time(0);
      glClear( GL_COLOR_BUFFER_BIT );
   
      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
		
		square.draw();                //tlo
		walls.draw();                 //przeszkody i cel
		
      AGLErrors("main-afterdraw");


      glm::vec3 plPos = glm::vec3(player.position+(dir*cam.forward));
      bool touch = false;
      for(int i=0; i<(n*n*n); i++){
         glm::vec3 middle = glm::vec3(walls.offsets[i]*glm::vec4(0,0,0,1));
         float dist = glm::distance(plPos,middle);
         if(abs(dist)<(2.0f/n)){
            glm::vec3 normal = glm::vec3(walls.offsets[i] * glm::vec4(0,0,1,0));
            // printf("%f,%f,%f\n",normal.x,normal.y,normal.z);

            float cosA = glm::dot(normal,plPos-middle)/(glm::length(plPos-middle)*glm::length(normal));
            float sinA = sqrt(1-cosA*cosA);

            float radius = 0.2f/n, side = 0.75f/(2.0f*n);//dane wziete z walls i player

            if((cosA!=0.0f) && (abs(cosA * dist) <= radius) && (abs(sinA * dist) <= (radius+side))){
               // printf("%d\n",i);
               //printf("%f <= %f && %f <= %f\n",abs(dist*cosA), radius, abs(dist*sinA), radius+side);
               touch = true;
               if(i==(n*n*n-1))
                  return;
            }
         }
      }

      if(abs(plPos.x-0.5f)>=0.75f || 
         abs(plPos.y-0.5f)>=0.75f || 
         abs(plPos.z+0.5f)>=0.75f)
         touch = true;


      if(touch)
         player.draw(0.0f,angX,angY);
      else
         player.draw(dir,angX,angY);



      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();
      //glfwWaitEvents();   


		dir = 0.0f;
      if (glfwGetKey(win(), GLFW_KEY_A ) == GLFW_PRESS) {
         dir = cam.speed;
      } else if (glfwGetKey(win(), GLFW_KEY_Z ) == GLFW_PRESS) {
         dir = -cam.speed;
      }
      angX = 0.0;
      angY = 0.0;
      if (glfwGetKey(win(), GLFW_KEY_DOWN ) == GLFW_PRESS) {
         angY -= cam.rotateSpeed;
      } else if (glfwGetKey(win(), GLFW_KEY_UP ) == GLFW_PRESS) {
         angY += cam.rotateSpeed;
      } else if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
         angX -= cam.rotateSpeed;
      } else if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
			angX += cam.rotateSpeed;
      }
      if (glfwGetKey(win(), GLFW_KEY_SPACE) == GLFW_PRESS)
         cam.changeMode();


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
			n=5;		//wartoscia n
	}
	//poprawnosc calej gry(10)
	//wysrodkowanie i skalowanie okna(1)
   //grywalnosc (1)
   //zmienianie koloru tla, czarny cel

   //glEnable(GL_CULL_FACE);
   glDisable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   //glDepthFunc(GL_LESS);

   MyWin win;
   win.Init(800,600,"AGL3 example",0,33);
   win.MainLoop();
   return 0;
}


