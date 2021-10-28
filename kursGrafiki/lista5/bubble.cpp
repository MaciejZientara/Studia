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
#include <vector>
#include <algorithm>

#include <epoxy/gl.h>
#include <GLFW/glfw3.h>
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
int punkty = 0;
glm::vec4 originPos = glm::vec4(0.0f,0.0f,0.9f,1.0f);

class Camera{
   glm::vec2 mousePos;
   bool firstMouse = true;
   float yaw   = -90.0f;
   float pitch =  0.0f;
   float lastX =  800.0f / 2.0;
   float lastY =  600.0 / 2.0;
   float fov = 45.0f;
   public:
   bool mode;//false - first person, true - third person
   float speed = 0.007f;
   glm::vec4 position;
   glm::vec4 forward;
   glm::vec4 up;
   glm::mat4 projMat = glm::perspective(fov, 1.0f, 0.1f, 5.0f);
   glm::mat4 modeMatrix;
   Camera(){
      mousePos = glm::vec2();
      mode = false;
      up = glm::vec4(0,1,0,0);
      position = originPos;
      forward = glm::vec4(0,0,-1,0);
   }
   void changeMode(){
      // if(mode)
      //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      // else
      //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      
      mode = !mode;
   }
   void mouseUpdate(double xpos, double ypos){
   if (firstMouse)
   {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
   }

   float xoffset = xpos - lastX;
   float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
   lastX = xpos;
   lastY = ypos;

   float sensitivity = 0.1f; // change this value to your liking
   xoffset *= sensitivity;
   yoffset *= sensitivity;

   yaw += xoffset;
   pitch += yoffset;

   // make sure that when pitch is out of bounds, screen doesn't get flipped
   if (pitch > 89.0f)
      pitch = 89.0f;
   if (pitch < -89.0f)
      pitch = -89.0f;

   glm::vec3 front;
   front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
   front.y = sin(glm::radians(pitch));
   front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
   forward = glm::normalize(glm::vec4(front,0.0));

   }
   void mouseScroll(double yp){
      fov -= (float)yp * 0.03f;
      if (fov < 44.0f)
         fov = 44.0f;
      if (fov > 45.0f)
         fov = 45.0f;
      printf("fov = %f\n",fov);
      projMat = glm::perspective(fov, 1.0f, 0.1f, 5.0f);
   }
   glm::mat4 view(){
      if(mode)
         return glm::lookAt(glm::vec3(3.5f,0.0,0.0),glm::vec3(2.0,0.0,0.0),glm::vec3(0.0f,1.0f,0.0f));
      else
         return glm::lookAt(glm::vec3(position),glm::vec3(position+forward),glm::vec3(0.0f,1.0f,0.0f));
   }
};

Camera cam;
glm::vec3 Lamp = glm::vec3(1.5f,1.5f,1.5f);//0.5f,0.2f,0.2f
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
         
         uniform mat4 trans;


         out vec3 norm;
         out vec3 FragPos;
         out vec3 vcolor;

         void main(void) {
            const vec3 vertices[72] = vec3[72](
               vec3(-1.25f, -1.25f, -1.25f), vec3( 0.0f,  0.0f, -1.0f),
               vec3( 1.25f, -1.25f, -1.25f), vec3( 0.0f,  0.0f, -1.0f), 
               vec3( 1.25f,  1.25f, -1.25f), vec3( 0.0f,  0.0f, -1.0f), 
               vec3( 1.25f,  1.25f, -1.25f), vec3( 0.0f,  0.0f, -1.0f), 
               vec3(-1.25f,  1.25f, -1.25f), vec3( 0.0f,  0.0f, -1.0f), 
               vec3(-1.25f, -1.25f, -1.25f), vec3( 0.0f,  0.0f, -1.0f), 

               vec3(-1.25f, -1.25f,  1.25f), vec3( 0.0f,  0.0f,  1.0f),
               vec3( 1.25f, -1.25f,  1.25f), vec3( 0.0f,  0.0f,  1.0f),
               vec3( 1.25f,  1.25f,  1.25f), vec3( 0.0f,  0.0f,  1.0f),
               vec3( 1.25f,  1.25f,  1.25f), vec3( 0.0f,  0.0f,  1.0f),
               vec3(-1.25f,  1.25f,  1.25f), vec3( 0.0f,  0.0f,  1.0f),
               vec3(-1.25f, -1.25f,  1.25f), vec3( 0.0f,  0.0f,  1.0f),

               vec3(-1.25f,  1.25f,  1.25f), vec3(-1.0f,  0.0f,  0.0f),
               vec3(-1.25f,  1.25f, -1.25f), vec3(-1.0f,  0.0f,  0.0f),
               vec3(-1.25f, -1.25f, -1.25f), vec3(-1.0f,  0.0f,  0.0f),
               vec3(-1.25f, -1.25f, -1.25f), vec3(-1.0f,  0.0f,  0.0f),
               vec3(-1.25f, -1.25f,  1.25f), vec3(-1.0f,  0.0f,  0.0f),
               vec3(-1.25f,  1.25f,  1.25f), vec3(-1.0f,  0.0f,  0.0f),

               vec3( 1.25f,  1.25f,  1.25f), vec3( 1.0f,  0.0f,  0.0f),
               vec3( 1.25f,  1.25f, -1.25f), vec3( 1.0f,  0.0f,  0.0f),
               vec3( 1.25f, -1.25f, -1.25f), vec3( 1.0f,  0.0f,  0.0f),
               vec3( 1.25f, -1.25f, -1.25f), vec3( 1.0f,  0.0f,  0.0f),
               vec3( 1.25f, -1.25f,  1.25f), vec3( 1.0f,  0.0f,  0.0f),
               vec3( 1.25f,  1.25f,  1.25f), vec3( 1.0f,  0.0f,  0.0f),

               vec3(-1.25f, -1.25f, -1.25f), vec3( 0.0f, -1.0f,  0.0f),
               vec3( 1.25f, -1.25f, -1.25f), vec3( 0.0f, -1.0f,  0.0f),
               vec3( 1.25f, -1.25f,  1.25f), vec3( 0.0f, -1.0f,  0.0f),
               vec3( 1.25f, -1.25f,  1.25f), vec3( 0.0f, -1.0f,  0.0f),
               vec3(-1.25f, -1.25f,  1.25f), vec3( 0.0f, -1.0f,  0.0f),
               vec3(-1.25f, -1.25f, -1.25f), vec3( 0.0f, -1.0f,  0.0f),

               vec3(-1.25f,  1.25f, -1.25f), vec3( 0.0f,  1.0f,  0.0f),
               vec3( 1.25f,  1.25f, -1.25f), vec3( 0.0f,  1.0f,  0.0f),
               vec3( 1.25f,  1.25f,  1.25f), vec3( 0.0f,  1.0f,  0.0f),
               vec3( 1.25f,  1.25f,  1.25f), vec3( 0.0f,  1.0f,  0.0f),
               vec3(-1.25f,  1.25f,  1.25f), vec3( 0.0f,  1.0f,  0.0f),
               vec3(-1.25f,  1.25f, -1.25f), vec3( 0.0f,  1.0f,  0.0f)
            );

            vec3 pos = vertices[2*gl_VertexID];

            norm = vertices[2*gl_VertexID+1];
            FragPos = pos;

            vcolor = vec3(0.0,0.0,1.0);

            gl_Position = trans * vec4(pos, 1.0); 
         }

      )END", R"END(

         #version 330 

         in vec3 norm;
         in vec3 FragPos;
         in vec3 vcolor;
         uniform vec3 viewPos;
         uniform vec3 LightPos;

         out vec4 color;

         void main(void) {
            vec3 lightColor = vec3(1.0,1.0,1.0);

            float ambientStrength = 0.3;
            vec3 ambient = ambientStrength * lightColor;

            vec3 lightDir = normalize(LightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;

            float specularStrength = 0.5;
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);  
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * lightColor;  


				color = vec4((ambient + diffuse + specular) * vcolor,1.0);
         } 

      )END");
   }
   void reset(){}
   void draw() {
      bindProgram();
      glm::mat4 tmp = cam.projMat * cam.view();

      glUniformMatrix4fv(glGetUniformLocation(p(), "trans"),1,false,&tmp[0][0]);
      glUniform3f(glGetUniformLocation(p(), "viewPos"),cam.position.x,cam.position.y,cam.position.z);
      glUniform3f(glGetUniformLocation(p(), "LightPos"),Lamp.x,Lamp.y,Lamp.z);
      glDrawArrays(GL_TRIANGLES, 0, 36);
   }
};

class triangle{
   public:
   glm::vec3 a,b,c;
   triangle(glm::vec3 A,glm::vec3 B,glm::vec3 C){
      a=A;b=B;c=C;
   }
   bool operator<( const triangle& val ) const { 
         float mxA = std::max(a.z,std::max(b.z,c.z));
         float mxB = std::max(val.a.z,std::max(val.b.z,val.c.z));
         return mxA < mxB;
   }
};

class MyBubble : public AGLDrawable {
//default scale
float s = 0.75f/5.0f;

GLuint offbuff;
int amount=0;
bool ready = false;
public:
   glm::vec3 rise;
   float skala;
	glm::vec3 *offsets, *offsetsBuffer;
   MyBubble() : AGLDrawable(0) {
      offsets = nullptr;
      offsetsBuffer = nullptr;
		setShaders();
      setBuffers();
   }
   void setShaders() {
		compileShaders(R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in vec3 pos;
         layout(location = 1) in vec3 move;
         uniform float scale;
			uniform mat4 trans;
         uniform vec3 rise;
         
         out vec3 col;
         out vec3 FragPos;
         out vec3 Normal;
         
         void main(void) {
            vec4 p = vec4(rise + move + scale*pos,1.0);
            gl_Position = trans * p;

            col = clamp(vec3(p.x, p.y, p.z),0.3,1.0);

            Normal = pos;
            FragPos = vec3(p);
         }

      )END", R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require

         in vec3 col;
         in vec3 FragPos;
         in vec3 Normal;
         uniform vec3 LightPos;
         uniform vec3 viewPos;

			out vec4 color;

         void main(void) {
            vec3 lightColor = vec3(1.0,1.0,1.0);
            float ambientStrength = 0.2;
            vec3 ambient = ambientStrength * lightColor;

            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(LightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;
            
            float specularStrength = 0.5;
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);  
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * lightColor;  

            vec3 result = (ambient + diffuse + specular) * col;
            color = vec4( clamp(result, 0.0, 1.0), 1.0);
         } 

      )END");
   }


   void reset(){
      skala = s;
      rise = glm::vec3(0,0,0);
      if(offsets!=nullptr)
         delete[] offsets;
      if(offsetsBuffer!=nullptr)
         delete[] offsetsBuffer;

      offsets = new glm::vec3[n];
      offsetsBuffer = new glm::vec3[n];


      for(int i=0; i<n; i++){
         glm::vec3 pos = glm::vec3(((rand()%21)-10)/10.0f,-1.5f-(rand()%10 / 10.0f),((rand()%21)-10)/10.0f);
         offsets[i] = pos;
         offsetsBuffer[i] = offsets[i];
      }

      glBindBuffer(GL_ARRAY_BUFFER,offbuff);
		glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(float)*3*n, &offsetsBuffer[0]);

   }


   void setBuffers() {
		glGenBuffers(1, &offbuff);      
		glBindBuffer(GL_ARRAY_BUFFER, offbuff);

      std::vector <float> vert;
      std::vector <triangle> tri;

      const float PI = acos(-1);

      int dlugosc = 20;//pion
      int szerokosc = 20;//poziom

      float katPion = PI/szerokosc, katPoziom = 2.0f*PI/dlugosc;

      //dla pierwszego i ostatniego poziomu tylko trojkaty
      //dla pozostalych sa kwadraty

      //http://www.songho.ca/opengl/gl_sphere.html#sphere
      for(int j=0; j<szerokosc; j++){
         for(int i=0; i<dlugosc; i++){
            glm::vec3   tl = glm::vec3(cosf( (PI/2.0f) - j*katPion) * sinf((i+1) * katPoziom),
                                       sinf( (PI/2.0f) - j*katPion),
                                       cosf( (PI/2.0f) - j*katPion) * cosf((i+1) * katPoziom)), 

                        tr = glm::vec3(cosf( (PI/2.0f) - j*katPion) * sinf(i * katPoziom),
                                       sinf( (PI/2.0f) - j*katPion),
                                       cosf( (PI/2.0f) - j*katPion) * cosf(i * katPoziom)),

                        bl = glm::vec3(cosf( (PI/2.0f) - (j+1)*katPion) * sinf((i+1) * katPoziom),
                                       sinf( (PI/2.0f) - (j+1)*katPion),
                                       cosf( (PI/2.0f) - (j+1)*katPion) * cosf((i+1) * katPoziom)), 

                        br = glm::vec3(cosf( (PI/2.0f) - (j+1)*katPion) * sinf(i * katPoziom),
                                       sinf( (PI/2.0f) - (j+1)*katPion),
                                       cosf( (PI/2.0f) - (j+1)*katPion) * cosf(i * katPoziom));


            if(j!=0){
               tri.push_back(triangle(tr,tl,bl));
               //top right
               //top left
               //bottom left 
            }
            if(j!=(szerokosc-1)){
               tri.push_back(triangle(bl,br,tr));
               //bottom left
               //bottom right
               //top right
            }
         }
      }

      std::sort(tri.begin(),tri.end());

      for(int q=0; q<tri.size(); q++){
         vert.push_back(tri[q].a.x);
         vert.push_back(tri[q].a.y);
         vert.push_back(tri[q].a.z);
         vert.push_back(tri[q].b.x);
         vert.push_back(tri[q].b.y);
         vert.push_back(tri[q].b.z);
         vert.push_back(tri[q].c.x);
         vert.push_back(tri[q].c.y);
         vert.push_back(tri[q].c.z);
      }

      tri.clear();

      amount=vert.size();

      //========================================================

      skala = s;
      rise = glm::vec3(0,0,0);

      offsets = new glm::vec3[n];
      offsetsBuffer = new glm::vec3[n];

      for(int i=0; i<n; i++){
         glm::vec3 pos = glm::vec3(((rand()%21)-10)/10.0f,-1.5f-(rand()%10 / 10.0f),((rand()%21)-10)/10.0f);
         offsets[i] = pos;
         offsetsBuffer[i] = offsets[i];
      }

		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*25, &offsetsBuffer[0], GL_STATIC_DRAW);

		bindBuffers();
      glBufferData(GL_ARRAY_BUFFER, vert.size()*sizeof(float), &vert[0], GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER,offbuff);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, (void*)0);
      glEnableVertexAttribArray(1);
		glVertexAttribDivisor(1,1);

      ready = true;
   }
   void draw() {
      bindProgram();
      bindBuffers();

      glm::mat4 proj = cam.projMat * cam.view();

      if(skala<2.4f*s)
         skala+=0.0005f;
      if(rise.y>3.5f)
         rise = glm::vec3(0,0,0);
      else
         rise+=glm::vec3(0,0.01f,0);

      glUniformMatrix4fv(glGetUniformLocation(p(), "trans"),1,false,&proj[0][0]);
      glUniform3f(glGetUniformLocation(p(), "LightPos"),Lamp.x,Lamp.y,Lamp.z);
      glUniform3f(glGetUniformLocation(p(), "viewPos"),cam.position.x,cam.position.y,cam.position.z);
      glUniform1f(glGetUniformLocation(p(), "scale"),skala);
      glUniform3f(glGetUniformLocation(p(), "rise"),rise.x,rise.y,rise.z);
      

      glDrawArraysInstanced(GL_TRIANGLES, 0, amount/3, n);
   }

};
// ==========================================================================
// ==========================================================================
// ==========================================================================


class MyPlayer : public AGLDrawable {      
int iter=0;
glm::mat4 skala;
public:
   float radious = 0.1f/5.0f;
   glm::vec4 position = originPos;
   MyPlayer() : AGLDrawable(0) {
      skala = glm::scale(glm::mat4(1),glm::vec3(radious,radious,radious));
      setShaders();
      setBuffers();
   }
   void setShaders() {
      compileShaders(R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in vec3 pos;
         layout(location = 1) in vec3 norm;
         layout(location = 2) uniform mat4 view;
         layout(location = 6) uniform mat4 model;

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


      float vert[2*8*16*3*3];
      float dia = sqrt(2.0f)/4.0f, len = 1.0f/4.0f;
      glm::vec3   downFront = glm::vec3(0.0f,-len,len),
                  downBack = glm::vec3(0.0f,-len,-len), 
                  leftBack = glm::vec3(-dia,0.0f,-len),
                  rightBack = glm::vec3(dia,0.0f,-len);


      //https://stackoverflow.com/questions/7687148/drawing-sphere-in-opengl-without-using-glusphere
      //deklaracja sfery
      for(int i=1; i<=4; i++){
         glm::vec3 up = glm::vec3(0,1,0), down = glm::vec3(0,-1,0);
         glm::vec3 a,b,c,norm;
         for(int j=1; j<=i; j++){
            //left up front
            a = glm::normalize(up     + (float)i      * downFront + (float)j     * leftBack);
            b = glm::normalize(up     + (float)i      * downFront + (float)(j-1) * leftBack);
            c = glm::normalize(up     + (float)(i-1)  * downFront + (float)(j-1) * leftBack);
            norm = glm::cross(c-b,a-b);
            vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
            vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
            vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
            vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
            vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
            vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
            iter+=18;
            if(i!=4){
               a = glm::normalize(up  + (float)i      * downFront + (float)j     * leftBack);
               b = glm::normalize(up  + (float)i      * downFront + (float)(j-1) * leftBack);
               c = glm::normalize(up  + (float)(i+1)  * downFront + (float)j     * leftBack);
               norm = glm::cross(a-b,c-b);
               vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
               vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
               vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
               vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
               vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
               vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
               iter+=18;
            }
            //right up front
            a = glm::normalize(up     + (float)i      * downFront + (float)j     * rightBack);
            b = glm::normalize(up     + (float)i      * downFront + (float)(j-1) * rightBack);
            c = glm::normalize(up     + (float)(i-1)  * downFront + (float)(j-1) * rightBack);
            norm = glm::cross(a-b,c-b);
            vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
            vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
            vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
            vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
            vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
            vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
            iter+=18;
            if(i!=4){
               a = glm::normalize(up  + (float)i      * downFront + (float)j     * rightBack);
               b = glm::normalize(up  + (float)i      * downFront + (float)(j-1) * rightBack);
               c = glm::normalize(up  + (float)(i+1)  * downFront + (float)j     * rightBack);
               norm = glm::cross(c-b,a-b);
               vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
               vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
               vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
               vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
               vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
               vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
               iter+=18;
            }
            //left up back
            a = glm::normalize(up     + (float)i      * downBack - (float)j     * rightBack);
            b = glm::normalize(up     + (float)i      * downBack - (float)(j-1) * rightBack);
            c = glm::normalize(up     + (float)(i-1)  * downBack - (float)(j-1) * rightBack);
            norm = glm::cross(a-b,c-b);
            vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
            vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
            vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
            vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
            vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
            vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
            iter+=18;
            if(i!=4){
               a = glm::normalize(up  + (float)i      * downBack - (float)j     * rightBack);
               b = glm::normalize(up  + (float)i      * downBack - (float)(j-1) * rightBack);
               c = glm::normalize(up  + (float)(i+1)  * downBack - (float)j     * rightBack);
               norm = glm::cross(c-b,a-b);
               vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
               vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
               vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
               vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
               vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
               vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
               iter+=18;
            }
            //right up back
            a = glm::normalize(up     + (float)i      * downBack - (float)j     * leftBack);
            b = glm::normalize(up     + (float)i      * downBack - (float)(j-1) * leftBack);
            c = glm::normalize(up     + (float)(i-1)  * downBack - (float)(j-1) * leftBack);
            norm = glm::cross(c-b,a-b);
            vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
            vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
            vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
            vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
            vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
            vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
            iter+=18;
            if(i!=4){
               a = glm::normalize(up  + (float)i      * downBack - (float)j     * leftBack);
               b = glm::normalize(up  + (float)i      * downBack - (float)(j-1) * leftBack);
               c = glm::normalize(up  + (float)(i+1)  * downBack - (float)j     * leftBack);
               norm = glm::cross(a-b,c-b);
               vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
               vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
               vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
               vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
               vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
               vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
               iter+=18;
            }

            //left down back
            a = glm::normalize(down     - (float)i      * downFront - (float)j     * rightBack);
            b = glm::normalize(down     - (float)i      * downFront - (float)(j-1) * rightBack);
            c = glm::normalize(down     - (float)(i-1)  * downFront - (float)(j-1) * rightBack);
            norm = glm::cross(c-b,a-b);
            vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
            vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
            vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
            vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
            vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
            vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
            iter+=18;
            if(i!=4){
               a = glm::normalize(down  - (float)i      * downFront - (float)j     * rightBack);
               b = glm::normalize(down  - (float)i      * downFront - (float)(j-1) * rightBack);
               c = glm::normalize(down  - (float)(i+1)  * downFront - (float)j     * rightBack);
               norm = glm::cross(a-b,c-b);
               vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
               vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
               vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
               vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
               vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
               vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
               iter+=18;
            }
            //right down back
            a = glm::normalize(down     - (float)i      * downFront - (float)j     * leftBack);
            b = glm::normalize(down     - (float)i      * downFront - (float)(j-1) * leftBack);
            c = glm::normalize(down     - (float)(i-1)  * downFront - (float)(j-1) * leftBack);
            norm = glm::cross(a-b,c-b);
            vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
            vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
            vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
            vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
            vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
            vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
            iter+=18;
            if(i!=4){
               a = glm::normalize(down  - (float)i      * downFront - (float)j     * leftBack);
               b = glm::normalize(down  - (float)i      * downFront - (float)(j-1) * leftBack);
               c = glm::normalize(down  - (float)(i+1)  * downFront - (float)j     * leftBack);
               norm = glm::cross(c-b,a-b);
               vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
               vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
               vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
               vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
               vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
               vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
               iter+=18;
            }
            //left down front
            a = glm::normalize(down     - (float)i      * downBack + (float)j     * leftBack);
            b = glm::normalize(down     - (float)i      * downBack + (float)(j-1) * leftBack);
            c = glm::normalize(down     - (float)(i-1)  * downBack + (float)(j-1) * leftBack);
            norm = glm::cross(a-b,c-b);
            vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
            vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
            vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
            vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
            vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
            vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
            iter+=18;
            if(i!=4){
               a = glm::normalize(down  - (float)i      * downBack + (float)j     * leftBack);
               b = glm::normalize(down  - (float)i      * downBack + (float)(j-1) * leftBack);
               c = glm::normalize(down  - (float)(i+1)  * downBack + (float)j     * leftBack);
               norm = glm::cross(c-b,a-b);
               vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
               vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
               vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
               vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
               vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
               vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
               iter+=18;
            }
            //right down front
            a = glm::normalize(down     - (float)i      * downBack + (float)j     * rightBack);
            b = glm::normalize(down     - (float)i      * downBack + (float)(j-1) * rightBack);
            c = glm::normalize(down     - (float)(i-1)  * downBack + (float)(j-1) * rightBack);
            norm = glm::cross(c-b,a-b);
            vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
            vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
            vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
            vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
            vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
            vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
            iter+=18;
            if(i!=4){
               a = glm::normalize(down  - (float)i      * downBack + (float)j     * rightBack);
               b = glm::normalize(down  - (float)i      * downBack + (float)(j-1) * rightBack);
               c = glm::normalize(down  - (float)(i+1)  * downBack + (float)j     * rightBack);
               norm = glm::cross(a-b,c-b);
               vert[0 +iter]=a.x;   vert[1 +iter]=a.y;   vert[2 +iter]=a.z;
               vert[3 +iter]=norm.x;vert[4 +iter]=norm.y;vert[5 +iter]=norm.z;
               vert[6 +iter]=b.x;   vert[7 +iter]=b.y;   vert[8 +iter]=b.z;
               vert[9 +iter]=norm.x;vert[10+iter]=norm.y;vert[11+iter]=norm.z;
               vert[12+iter]=c.x;   vert[13+iter]=c.y;   vert[14+iter]=c.z;
               vert[15+iter]=norm.x;vert[16+iter]=norm.y;vert[17+iter]=norm.z;
               iter+=18;
            }
         }
      }


      glBufferData(GL_ARRAY_BUFFER, iter*sizeof(float), vert, GL_STATIC_DRAW );
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
      
   }
	
   void reset(){
      position = originPos;
      cam.position = originPos;
   }

   void draw(glm::vec4 dir) {
      bindProgram();
      bindBuffers();

      glm::mat4 move = glm::translate(glm::mat4(1),glm::vec3(dir+position));
      
      cam.position += dir;
      position += dir;

      glm::mat4 tmp = move /* rotX * rotY*/ * skala;
      glm::mat4 m = cam.projMat * cam.view();

		glUniformMatrix4fv(2,1,false,&m[0][0]);
      glUniformMatrix4fv(6,1,false,&tmp[0][0]);

      glDrawArrays(GL_TRIANGLES, 0, iter/6);
   }

};


// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================
class MyWin : public AGLWindow {
	glm::vec4 dir;
   bool md = false;
   bool toggle, toggleTab;
public:
   MyWin() {};
   MyWin(int _wd, int _ht, const char *name, int vers, int fullscr=0)
      : AGLWindow(_wd, _ht, name, vers, fullscr) {};
   virtual void KeyCB(int key, int scancode, int action, int mods);
   virtual void ScrollCB(double xp, double yp);
   virtual void MousePosCB(double xp, double yp);
   void movement();
   void MainLoop();
};

void MyWin::ScrollCB(double xp, double yp){
   cam.mouseScroll(yp);
}

void MyWin::MousePosCB(double xp, double yp){
   cam.mouseUpdate(xp,yp);
}
// ==========================================================================
void MyWin::KeyCB(int key, int scancode, int action, int mods) {
    AGLWindow::KeyCB(key,scancode, action, mods); // f-key full screen switch

    //wpisanie tutaj movement dawalo skakanie lub blokady ruchu
}
// ==========================================================================
void MyWin::movement(){
   dir = glm::vec4();
   
   if (glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS) {
      dir -= cam.forward;
   }
   if (glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS) {
      dir += cam.forward;
   }
   if (glfwGetKey(win(), GLFW_KEY_LEFT) == GLFW_PRESS) {
      dir -= glm::vec4(glm::cross(glm::vec3(cam.forward),glm::vec3(0,1,0)),0);
   }
   if (glfwGetKey(win(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
      dir += glm::vec4(glm::cross(glm::vec3(cam.forward),glm::vec3(0,1,0)),0);
   }
   if (glfwGetKey(win(), GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
      dir -= glm::vec4(0,1,0,0);
   }
   if (glfwGetKey(win(), GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
      dir += glm::vec4(0,1,0,0);
   }

   // dir = glm::normalize(dir);
   dir*=cam.speed;

   if (glfwGetKey(win(), GLFW_KEY_SPACE) == GLFW_PRESS){
      if(toggle){
         if(md)
            glfwSetInputMode(win(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
         else
            glfwSetInputMode(win(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
         md=!md;
      }
      toggle = false;
   }
   else{
      toggle = true;
   }

   if (glfwGetKey(win(), GLFW_KEY_TAB) == GLFW_PRESS){
      if(toggleTab){
         cam.changeMode();
      }
      toggleTab = false;
   }
   else{
      toggleTab = true;
   }

/*
      // angX = 0.0;
      // angY = 0.0;
      // if (glfwGetKey(win(), GLFW_KEY_DOWN ) == GLFW_PRESS) {
      //    angY -= cam.rotateSpeed;
      // } else if (glfwGetKey(win(), GLFW_KEY_UP ) == GLFW_PRESS) {
      //    angY += cam.rotateSpeed;
      // } else if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
      //    angX -= cam.rotateSpeed;
      // } else if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
		// 	angX += cam.rotateSpeed;
      // }
      // if (glfwGetKey(win(), GLFW_KEY_SPACE) == GLFW_PRESS)
      //    cam.changeMode();
*/

}


// ==========================================================================
void MyWin::MainLoop() {
   ViewportOne(0,0,wd,ht);
   
   MyBubble bubbles;
	MyPlayer player;
	MySqu square;
	
	int targetFPS = 60;
	double timePerFrame = 1000/targetFPS;
	int frameCount = 0;

	int STARTtime = time(0);

	
   do {
		double startTime = time(0);
      glClear( GL_COLOR_BUFFER_BIT );
   
      AGLErrors("main-loopbegin");
      

		square.draw();                //tlo
		bubbles.draw();               //babelki
     
      AGLErrors("main-afterdraw");
		
      movement();

      glm::vec3 plPos = glm::vec3(player.position+dir);
      float plRadious = player.radious;

      for(int i=0; i<n; i++){
         glm::vec3 middle = bubbles.offsets[i]+bubbles.rise;
         float dist = glm::distance(plPos,middle);
         float radious = bubbles.skala;
         if(abs(dist)<=(plRadious+radious)){
            printf("Punkty = %d\n", punkty);
            return;
         }
      }

      bool touch = false;
      if(abs(plPos.x)>=1.25f-plRadious || 
         abs(plPos.y)>=1.25f-plRadious || 
         abs(plPos.z)>=1.25f-plRadious )
         touch = true;

      if(plPos.z<=(-1.25f+plRadious)){
         if(n<25)
            n++;
         punkty++;
         player.reset();//cam i player position reset
         square.reset();//void
         bubbles.reset();//subdata offsets buffor
         player.draw(glm::vec4());
      }
      else{
         if(touch)
            player.draw(glm::vec4());
         else
            player.draw(dir);
      }

      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();
      //glfwWaitEvents();   

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
		if(n<1)		//zabezpieczenie przed za mala
			n=5;		//wartoscia n
	}

   glEnable(GL_DEPTH_TEST);
   // glEnable(GL_CULL_FACE);
   // glCullFace(GL_BACK);
   // glDisable(GL_CULL_FACE);
   // glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   MyWin win;
   win.Init(800,600,"AGL3 example",0,33);
   win.MainLoop();
   return 0;
}

