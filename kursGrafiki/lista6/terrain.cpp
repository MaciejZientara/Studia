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
#include <fstream>

#include <epoxy/gl.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

// #include <unistd.h> //usleep na linux, nie znalazlem sposobu na crossplatform

#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>

// ==========================================================================
// Drawable object: no-data only: vertex/fragment programs
// ==========================================================================

// ==========================================================================
// ==========================================================================
int n = 5;
int punkty = 0;
float original_distance = 6700000.0f;//10.0f;
glm::vec4 originPos = glm::vec4(0.0f, 0.0f, original_distance, 1.0f);
int precision = 0;

class Camera
{
   bool firstMouse = true;
   float yaw = -90.0f;
   float pitch = 0.0f;
   float lastX = 800.0f / 2.0;
   float lastY = 600.0 / 2.0;
   float fov = 45.0f;

public:
   bool mode = false;
   float aspect = 1.0f;//8.0f/6.0f;
   float speed = 0.05f;
   float rotateSpeed = 0.0002f;
   glm::vec4 position;
   glm::vec4 position2D;
   glm::vec4 forward;
   glm::vec4 up;
   glm::mat4 projMat;
   glm::mat4 modeMatrix;
   Camera()
   {
      setPerspective();
      forward = glm::vec4(0,0,-1,0);
      up = glm::vec4(0, 1, 0, 0);
      position = originPos;
      position2D = glm::vec4(0,1.5f,0,0);
   }
   void mouseUpdate(double xpos, double ypos)
   {
      if(!mode)
         return;
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

      float sensitivity = 0.002f; // change this value to your liking
      xoffset *= sensitivity;
      yoffset *= sensitivity;

      // yaw += xoffset;
      // pitch += yoffset;

      // // make sure that when pitch is out of bounds, screen doesn't get flipped
      // if (pitch > 89.0f)
      //    pitch = 89.0f;
      // if (pitch < -89.0f)
      //    pitch = -89.0f;

      // glm::vec3 front;
      // front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      // front.y = sin(glm::radians(pitch));
      // front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
      // forward = glm::normalize(glm::vec4(front, 0.0));
      // if(forward.x==0 && forward.y==0 && forward.z==0 && forward.w==0)printf("forw = 0\n");

      float mx = 0.95f;
      glm::mat4 rotX = glm::rotate(glm::mat4(1),xoffset,glm::vec3(0,1,0));
      forward = forward * rotX;
      glm::mat4 rotY = glm::rotate(glm::mat4(1),-yoffset,glm::cross(glm::vec3(forward),glm::vec3(0,1,0)));
      glm::vec4 tmpFor = forward * rotY;
      if(abs(tmpFor.y)<mx)
         forward = tmpFor;


   }
   void mouseScroll(double yp)
   {
      fov -= (float)yp * 0.03f;
      if (fov < 44.0f)
         fov = 44.0f;
      if (fov > 45.0f)
         fov = 45.0f;
      // printf("fov = %f\n",fov);
      setPerspective();

      //rotateSpeed = 0.0002f * (fov-44.0f + 0.001f);//rotateSpeed = 0.8f - (45.0f-fov)/10.05f;
      // printf("rot = %f\n",rotateSpeed);
   }
   void setPerspective(){
      if(mode)
         projMat = glm::perspective(fov, aspect, 1.0f, 1000000.f);//pozmieniac near i far 
      else
         projMat = glm::perspective(fov, aspect, 0.1f, 25.0f); //10000.0f, 350000.f);//pozmieniac near i far 
   }
   glm::mat4 view()
   {
      if(mode)
         return glm::lookAt(glm::vec3(position), glm::vec3(position+forward)/*glm::vec3(0,0,0)*/, glm::vec3(0.0f, 1.0f, 0.0f));
      else
         return glm::lookAt(glm::vec3(position2D), glm::vec3(position2D+glm::vec4(0,-1,0.00001,0)), glm::vec3(0.0f, 1.0f, 0.0f));
   }
};

Camera cam;

// ==========================================================================


class triangle
{
public:
   glm::vec3 a, b, c;
   triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C)
   {
      a = A;
      b = B;
      c = C;
   }
   bool operator<(const triangle &val) const
   {
      float mxA = std::max(a.z, std::max(b.z, c.z));
      float mxB = std::max(val.a.z, std::max(val.b.z, val.c.z));
      return mxA < mxB;
   }
};

class Sphere : public AGLDrawable
{
   //default scale
   float s = 0.75f / 5.0f;

   GLuint offbuff;
   int amount = 0;

public:
   constexpr static float radious = 6371000.0f;//7.5f;
   const static int       dlugosc = 360;   //pion
   const static int       szerokosc = 180; //poziom  

   Sphere() : AGLDrawable(0)
   {
      setShaders();
      setBuffers();
   }
   void setShaders()
   {
      compileShaders(R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in vec3 pos;
         
         uniform mat4 trans;
         
         void main(void) {
            vec4 p = vec4(pos,1.0);
            gl_Position = trans * p;
         }

      )END",
                     R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require

			out vec4 color;

         void main(void) {
            color = vec4( 1.0, 1.0, 1.0, 1.0);
         } 

      )END");
   }

   void setBuffers()
   {
      glGenBuffers(1, &offbuff);
      glBindBuffer(GL_ARRAY_BUFFER, offbuff);

      std::vector<float> vert;
      std::vector<triangle> tri;

      const float PI = acos(-1);

      float katPion = PI / szerokosc, katPoziom = 2.0f * PI / dlugosc;

      //dla pierwszego i ostatniego poziomu tylko trojkaty
      //dla pozostalych sa kwadraty

      //http://www.songho.ca/opengl/gl_sphere.html#sphere
      for (int j = 0; j < szerokosc; j++)
      {
         for (int i = 0; i < dlugosc; i++)
         {
            glm::vec3 tl = glm::vec3(radious*cosf((PI / 2.0f) - j * katPion) * sinf((i + 1) * katPoziom),
                                     radious*sinf((PI / 2.0f) - j * katPion),
                                     radious*cosf((PI / 2.0f) - j * katPion) * cosf((i + 1) * katPoziom)),

                      tr = glm::vec3(radious*cosf((PI / 2.0f) - j * katPion) * sinf(i * katPoziom),
                                     radious*sinf((PI / 2.0f) - j * katPion),
                                     radious*cosf((PI / 2.0f) - j * katPion) * cosf(i * katPoziom)),

                      bl = glm::vec3(radious*cosf((PI / 2.0f) - (j + 1) * katPion) * sinf((i + 1) * katPoziom),
                                     radious*sinf((PI / 2.0f) - (j + 1) * katPion),
                                     radious*cosf((PI / 2.0f) - (j + 1) * katPion) * cosf((i + 1) * katPoziom)),

                      br = glm::vec3(radious*cosf((PI / 2.0f) - (j + 1) * katPion) * sinf(i * katPoziom),
                                     radious*sinf((PI / 2.0f) - (j + 1) * katPion),
                                     radious*cosf((PI / 2.0f) - (j + 1) * katPion) * cosf(i * katPoziom));

            if (j != 0)
            {
               tri.push_back(triangle(tr, tl, bl));
               //top right
               //top left
               //bottom left
            }
            if (j != (szerokosc - 1))
            {
               tri.push_back(triangle(bl, br, tr));
               //bottom left
               //bottom right
               //top right
            }
         }
      }

      std::sort(tri.begin(), tri.end());

      for (int q = 0; q < tri.size(); q++)
      {
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

      amount = vert.size();

      //========================================================


      bindBuffers();
      glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), &vert[0], GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

   }
   void draw()
   {
      bindProgram();
      bindBuffers();

      glm::mat4 proj = cam.projMat * cam.view();

      glUniformMatrix4fv(glGetUniformLocation(p(), "trans"), 1, false, &proj[0][0]);

      glDrawArrays(GL_TRIANGLES, 0, amount / 3);
   }
};

// ==========================================================================
int latA, latB, lonA, lonB;
std::string path;
std::vector<float> v;

class indexBuffers{
   public:
   // GLuint skalaA, rozmiarA, skalaB, rozmiarB, skalaC, rozmiarC, skalaD, rozmiarD, skalaE, rozmiarE;

   GLuint *skala, *rozmiar;

   indexBuffers(){
      std::vector<unsigned int> ind;
      int ile = 1201;
      int nr = 5;

      int help[] = {1,4,12,60,120};

      skala = new GLuint[nr];
      rozmiar = new GLuint[nr];

      glGenBuffers(nr, skala);

      for(int q=0; q<nr; q++){
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skala[q]);
         int mult = help[q];
         for(int i=0; i<=(ile-1-mult); i+=mult)
            for(int j=0; j<=(ile-1-mult); j+=mult){
               ind.push_back( i      *ile+ j);
               ind.push_back( i      *ile+(j+mult));
               ind.push_back((i+mult)*ile+ j);

               ind.push_back((i+mult)*ile+(j+mult));
               ind.push_back((i+mult)*ile+ j);
               ind.push_back( i      *ile+(j+mult));
            }
         rozmiar[q] = ind.size();
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(unsigned int), &ind[0], GL_STATIC_DRAW);
         ind.clear();
      }
   }

};

class terrain : public AGLDrawable
{
public:
   int lat, lon;
   terrain(int a, int o) : AGLDrawable(0)
   {
      lat = a;
      lon = o;
      setShaders();
      setBuffers();
   }
   void setShaders()
   {
      compileShaders(R"END(
         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in float pos;
         layout(location = 1) uniform int lat;
         layout(location = 2) uniform int lon;
         layout(location = 3) uniform float radious;
         layout(location = 4) uniform float dlugosc;
         layout(location = 5) uniform float szerokosc;
         layout(location = 6) uniform int mode;
         layout(location = 7) uniform int lonA;
         
         uniform mat4 trans;

         out vec3 col;

         void main(){
            if      (pos== -32768)col = vec3(0.,       0.,         0.);          //black 
            else if (pos < 0  )   col = vec3(0.,       0.,         1.);          //blue
            else if (pos < 500)   col = vec3(0.,       pos/500,    0.);          //->green
            else if (pos < 1000)  col = vec3(pos/500-1, 1.,        0.);          //->yellow
            else if (pos < 2000)  col = vec3(1.,       2.-pos/1000,0.);          //->red
            else                  col = vec3(1.,       pos/2000-1 ,pos/2000-1);  //->white
            
            int i = lat, j = lon;

            const float PI = 3.14159265359;

            float katPion = PI / szerokosc, katPoziom = 2.0f * PI / dlugosc;

            int indLeft = (180+j), indRight = (180+j+1+360)%360, indTop = (90-i-1+180)%180, indBottom = (90-i);

            int x = gl_VertexID/1201, y = gl_VertexID%1201;
            

            vec3 position;
            if(mode == 0){
               float prop = sin((PI / 2.0f) - (indTop + x/1200.0) * katPion);//sin(PI/2.0 - indTop * katPion);
               position = vec3(prop*(lonA-lon-y/1200.0), 0, lat+1-x/1200.0);
            }
            else{
               float ht;
               if(pos == -32768)
                  ht = 2000;
               else
                  ht = pos;
                  
               position =vec3((radious+ht)*cos((PI / 2.0f) - (indTop + x/1200.0) * katPion) * sin((indLeft + y/1200.0) * katPoziom),
                              (radious+ht)*sin((PI / 2.0f) - (indTop + x/1200.0) * katPion),
                              (radious+ht)*cos((PI / 2.0f) - (indTop + x/1200.0) * katPion) * cos((indLeft + y/1200.0) * katPoziom));
            }

            gl_Position = trans*vec4(position, 1.0);
         }

      )END",
                     R"END(
         #version 330

         in vec3 col;
         out vec3 color;

         void main(){
            color = col;
         }
      )END");
   }
   void setBuffers()
   {
      bindBuffers();

      glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), &v[0], GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void *)0);//4 -> 1 sizeof

   }
   void draw(GLuint nr, GLuint rozm)
   {
      bindProgram();
      bindBuffers();

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nr);

      glm::mat4 proj = cam.projMat * cam.view();

      glUniform1i(1,lat);
      glUniform1i(2,lon);

      glUniform1f(3,(float)Sphere::radious);
      glUniform1f(4,Sphere::dlugosc);
      glUniform1f(5,Sphere::szerokosc);
      if(cam.mode)
         glUniform1i(6,1);
      else
         glUniform1i(6,0);

      glUniform1i(7,lonA);

      glUniformMatrix4fv(glGetUniformLocation(p(), "trans"), 1, false, &proj[0][0]);

      glDrawElements(GL_TRIANGLES,rozm,GL_UNSIGNED_INT,NULL);
      // glDrawElements(GL_TRIANGLE_STRIP, IndSize, GL_UNSIGNED_INT, NULL);
   }
};

std::vector<terrain*> terr;

// ==========================================================================
class MyWin : public AGLWindow
{
   bool md = false;
   bool toggle, toggleTab;

public:
   MyWin(){};
   MyWin(int _wd, int _ht, const char *name, int vers, int fullscr = 0)
       : AGLWindow(_wd, _ht, name, vers, fullscr){};
   virtual void KeyCB(int key, int scancode, int action, int mods);
   virtual void ScrollCB(double xp, double yp);
   virtual void MousePosCB(double xp, double yp);
   void movement();
   void MainLoop();
};

void MyWin::ScrollCB(double xp, double yp)
{
   cam.mouseScroll(yp);
}

void MyWin::MousePosCB(double xp, double yp)
{
   cam.mouseUpdate(xp, yp);
}
// ==========================================================================
void MyWin::KeyCB(int key, int scancode, int action, int mods)
{
   AGLWindow::KeyCB(key, scancode, action, mods); // f-key full screen switch

   //wpisanie tutaj movement dawalo skakanie lub blokady ruchu
}
// ==========================================================================
void MyWin::movement()
{
   float angX = 0.0f, angY = 0.0f, angZ = 0.0f, mx = original_distance*0.995f;//6595000.0f;                                                    //mx - max wysokosci (abs)
   // if (glfwGetKey(win(), GLFW_KEY_DOWN ) == GLFW_PRESS) {
   //    angY -= cam.rotateSpeed;
   // } else if (glfwGetKey(win(), GLFW_KEY_UP ) == GLFW_PRESS) {
   //    angY += cam.rotateSpeed;
   // } else if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
   //    angX -= cam.rotateSpeed;
   // } else if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
   //    angX += cam.rotateSpeed;
   // } 
   // // else if (glfwGetKey(win(), GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS) {
   // //    angZ += 0.1f;//cam.rotateSpeed;
   // // } else if (glfwGetKey(win(), GLFW_KEY_SPACE ) == GLFW_PRESS) {
   // //    angZ -= 0.1f;//cam.rotateSpeed;
   // // }

   // // if (glfwGetKey(win(), GLFW_KEY_L ) == GLFW_PRESS)
   // //    precision = (precision+1)%4;

   // glm::mat4 rotX = glm::rotate(glm::mat4(1),angX,glm::vec3(0,1,0));
   // cam.position = cam.position * rotX;
   // glm::mat4 rotY = glm::rotate(glm::mat4(1),angY,glm::cross(glm::vec3(-1.0f*cam.position),glm::vec3(0,1,0)));
   // glm::vec4 tmpPos = cam.position * rotY;
   // if(abs(tmpPos.y)<mx)
   //    cam.position = tmpPos;
   
   if(glfwGetKey(win(), GLFW_KEY_1) == GLFW_PRESS)
      precision = 0;
   if(glfwGetKey(win(), GLFW_KEY_2) == GLFW_PRESS)
      precision = 1;
   if(glfwGetKey(win(), GLFW_KEY_3) == GLFW_PRESS)
      precision = 2;
   if(glfwGetKey(win(), GLFW_KEY_4) == GLFW_PRESS)
      precision = 3;
   if(glfwGetKey(win(), GLFW_KEY_5) == GLFW_PRESS)
      precision = 4;


   if (glfwGetKey(win(), GLFW_KEY_K) == GLFW_PRESS){
      cam.mode = false;
      glfwSetInputMode(win(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
      cam.setPerspective();
   }
   if (glfwGetKey(win(), GLFW_KEY_L) == GLFW_PRESS){
      cam.mode = true;
      glfwSetInputMode(win(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      cam.setPerspective();
   }

   glm::vec4 dir = glm::vec4(), dir2D = glm::vec4();
   if (glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS) {
      angY -= cam.rotateSpeed;
      //dir -= glm::vec4(glm::vec3(cam.forward),0);
      dir2D -= glm::vec4(0,0,1,0);
   }
   if (glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS) {
      angY += cam.rotateSpeed;
      //dir += glm::vec4(glm::vec3(cam.forward),0);
      dir2D += glm::vec4(0,0,1,0);
   }
   if (glfwGetKey(win(), GLFW_KEY_LEFT) == GLFW_PRESS) {
      angX += cam.rotateSpeed;
      // dir -= glm::vec4(glm::cross(glm::vec3(cam.forward),glm::vec3(0,1,0)),0);
      dir2D += glm::vec4(1,0,0,0);
   }
   if (glfwGetKey(win(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
      angX -= cam.rotateSpeed;
      // dir += glm::vec4(glm::cross(glm::vec3(cam.forward),glm::vec3(0,1,0)),0);
      dir2D -= glm::vec4(1,0,0,0);
   }
   if (glfwGetKey(win(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
      cam.position += glm::vec4(-1000.0f * glm::normalize(glm::vec3(cam.position)),0);
      // dir -= glm::vec4(0,1,0,0);
      dir2D -= glm::vec4(0,1,0,0);
   }
   if (glfwGetKey(win(), GLFW_KEY_SPACE) == GLFW_PRESS) {
      cam.position += glm::vec4(1000.0f * glm::normalize(glm::vec3(cam.position)),0);
      // dir += glm::vec4(0,1,0,0);
      dir2D += glm::vec4(0,1,0,0);
   }
   
   glm::mat4 rotX = glm::rotate(glm::mat4(1),angX,glm::vec3(0,1,0));
   cam.position = cam.position * rotX;
   glm::mat4 rotY = glm::rotate(glm::mat4(1),angY,glm::cross(-1.0f * glm::vec3(cam.position),glm::vec3(0,1,0)));
   glm::vec4 tmpPos = cam.position * rotY;
   if(abs(tmpPos.y)<mx)
      cam.position = tmpPos;



   // dir = glm::normalize(dir);
   //dir*=cam.speed;
   // dir*=1000.0;
   // glm::vec4 tmpPos = cam.position + dir;
   // if(abs(tmpPos.y)<mx)
   //    cam.position = tmpPos;

   dir2D*=cam.speed;
   cam.position2D += dir2D;//ograniczyc to jakos??


}

void setTerrVector(){
   int wczytane = 0;
   for (int i = latA; i < latB; i++)
      for (int j = lonA; j < lonB; j++)
      {
         std::string s = "";
         if (i < 0){
            s += "S";
            s += std::to_string(-i);
         }
         else{
            s += "N";
            s += std::to_string(i);
         }
            
         if (j < 0){
            s += "W";
            j*=-1;
            if (j < 10)
               s += "00";
            else if (j < 100)
               s += "0";
            s += std::to_string(j);
            j*=-1;
         }
         else{
            s += "E";
            if (j < 10)
               s += "00";
            else if (j < 100)
               s += "0";
            s += std::to_string(j);
         }
         s += ".hgt";

         std::ifstream file(path + s, std::ios::in | std::ios::binary);
         if (file.is_open())
         {
            printf("open: %s\n", (path + s).c_str());
            wczytane++;
            
            // const float PI = acos(-1);//3.14159265359 

            // float katPion = PI / Sphere::szerokosc, katPoziom = 2.0f * PI / Sphere::dlugosc;

            // int indLeft = (180+j), indRight = (180+j+1+360)%360, indTop = (90-i-1+180)%180, indBottom = (90-i);

            // glm::vec3   tl = glm::vec3(Sphere::radious*cosf((PI / 2.0f) - indTop * katPion) * sinf(indLeft * katPoziom),
            //                            Sphere::radious*sinf((PI / 2.0f) - indTop * katPion),
            //                            Sphere::radious*cosf((PI / 2.0f) - indTop * katPion) * cosf(indLeft * katPoziom)),

            //             tr = glm::vec3(Sphere::radious*cosf((PI / 2.0f) - indTop * katPion) * sinf(indRight * katPoziom),
            //                            Sphere::radious*sinf((PI / 2.0f) - indTop * katPion),
            //                            Sphere::radious*cosf((PI / 2.0f) - indTop * katPion) * cosf(indRight * katPoziom)),

            //             bl = glm::vec3(Sphere::radious*cosf((PI / 2.0f) - indBottom * katPion) * sinf(indLeft * katPoziom),
            //                            Sphere::radious*sinf((PI / 2.0f) - indBottom * katPion),
            //                            Sphere::radious*cosf((PI / 2.0f) - indBottom * katPion) * cosf(indLeft * katPoziom)),

            //             br = glm::vec3(Sphere::radious*cosf((PI / 2.0f) - indBottom * katPion) * sinf(indRight * katPoziom),
            //                            Sphere::radious*sinf((PI / 2.0f) - indBottom * katPion),
            //                            Sphere::radious*cosf((PI / 2.0f) - indBottom * katPion) * cosf(indRight * katPoziom));

            // glm::vec3 downL = (bl-tl)/1200.0f, downR = (br-tr)/1200.0f;

            // glm::vec3 hori = (tr-tl)/1200.0f, vert = (downR-downL)/1200.0f;;

            // int x = 0, y = 0;
            while (!file.eof()){
               char a, b;
               file.get(a);
               file.get(b);

               short int tmp = 0;

               tmp = (unsigned char)b + ((unsigned char)a << 8);

               // DEBUG input
               if((tmp<-500 || tmp>9000) && tmp!=-32768)
                  printf("%d\n",tmp);

               // y++;
               // if (y == 1201){
               //    y = 0;
               //    x++;
               //    hori+=vert;
               // }

               // glm::vec3 position = tl + (float)x*downL + (float)y*hori;
               // v.push_back(position.x);
               // v.push_back(position.y);
               // v.push_back(position.z);
               v.push_back((float)tmp);
            }

            
            terr.push_back(new terrain(i, j)); //czy dobra kolejnosc?
            v.clear();
            file.close();

            /*
                  ida po wierszach, albo od gory albo od dolu 
                  normalna kolejnosc 2 for w tablicy[][]

               
                  wyswietlic cala kule, mozna wireframe, tylko tam gdzie dane robic kolory

                  jak wiecej plikow w folderze to wiecej kawalkow pokolorowanych

                  najpierw zrobic 2d i potem przejsc z shaderem do 3d (zmiana tylko pozycji)

                  nie trzeba kompresowac danych, mozna je miec na jednym buforze ale przy wyswietlaniu uzywac indexowania i trianglne strip albo fan 
                  zeby nie powtarzac wierzcholkow tylko ich indexy oraz aby moc generowac obraz co ktorys wierzcholek (jak daleko to np co 2 albo 4, zeby szybciej
                  i z mniejsza precyzja)

                  zrobic test dla kotliny bo mozna porownac z obrazkiem ze strony
               */
              
         }
         else
         {
            printf("file IO error for %s\n", (path + s).c_str());
         }
      }
   printf("loaded files: %d\n", wczytane);
}

// ==========================================================================
void MyWin::MainLoop()
{
   ViewportOne(0, 0, wd, ht);

   //classes init

   Sphere s;
   indexBuffers indBuf;

   //set camera position
   cam.position =glm::vec4(original_distance*cosf((acos(-1) / 2.0f) - ((90-latA-1+180)%180) * (acos(-1) / Sphere::szerokosc)) * sinf((180+lonA) * (2.0f * acos(-1) / Sphere::dlugosc)),
                           original_distance*sinf((acos(-1) / 2.0f) - ((90-latA-1+180)%180) * (acos(-1) / Sphere::szerokosc)),
                           original_distance*cosf((acos(-1) / 2.0f) - ((90-latA-1+180)%180) * (acos(-1) / Sphere::szerokosc)) * cosf((180+lonA) * (2.0f * acos(-1) / Sphere::dlugosc)),1);
   originPos = cam.position;
   cam.forward = glm::vec4(-1.0f * glm::normalize(glm::vec3(originPos)),0);
   cam.position2D = glm::vec4(0,5,latA,0);//-lonA

   setTerrVector();

   double lastTime = glfwGetTime();
   int nbFrames = 0;

   unsigned int trojkaty = 0;

   do
   {
      double currentTime = glfwGetTime();
      nbFrames++;
      if ( currentTime - lastTime >= 1.0 ){
         printf("trojkaty = %u, FPS = %d\n", trojkaty, nbFrames);
         trojkaty = 0;
         nbFrames = 0;
         lastTime += 1.0;
      }
         
      glClear(GL_COLOR_BUFFER_BIT);

      AGLErrors("main-loopbegin");

      //resize dla aspect!=1.0
      // if(aspect!=cam.aspect){
      //    cam.aspect = aspect;
      //    cam.setPerspective();
      // }

      if(cam.mode){
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         s.draw();
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
      
      for (int i = 0; i < terr.size(); i++){
         terr[i]->draw(indBuf.skala[precision],indBuf.rozmiar[precision]);
         trojkaty += indBuf.rozmiar[precision];
      }

      AGLErrors("main-afterdraw");

      movement();

      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();
      //glfwWaitEvents();

   } while (glfwGetKey(win(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(win()) == 0);
}

int main(int argc, char *argv[]){
   // glEnable(GL_DEPTH_TEST);
   // glEnable(GL_CULL_FACE);
   // glCullFace(GL_BACK);
   // glDisable(GL_CULL_FACE);
   // glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   MyWin win;
   win.Init(800, 600, "Terrain", 0, 33);

   if (argc < 8)
   {
      printf("Not enough arguments\n");
      return 0;
   }

   path = argv[1];
   latA = std::stoi(argv[3]);
   latB = std::stoi(argv[4]);
   lonA = std::stoi(argv[6]);
   lonB = std::stoi(argv[7]);

   win.MainLoop();
   return 0;
}

/*
//mont blanc
./terrain ./data/ -lat 44 47 -lon 5 8
//Polska (część)
./terrain ./data/ -lat 50 55 -lon 18 23

*/