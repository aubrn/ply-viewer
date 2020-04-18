#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

typedef struct Normal
{
   float x;
   float y;
   float z;
} Normal;

typedef struct Vector
{
   float x, y, z;
} Vector;

typedef struct Vertex
{
   float x;
   float y;
   float z;
} Vertex;

typedef struct Poly
{
   int size;
   int vertices[8];
   Normal normal;
} Poly;

Poly *polyArray;
Vertex *vertexArray;
Normal *normalArray;

int numVertices = 0;
int numFaces = 0;
float yStart, zStart;
float lowestY = 0;

float rot = 0.0;

void init(void)
{
   GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};
   GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1.0};
   GLfloat light_specular[] = {0.7, 0.7, 0.7, 1.0};
   GLfloat light_ambient1[] = {0.2, 0.2, 0.2, 1.0};
   GLfloat light_diffuse1[] = {0.4, 0.4, 0.4, 1.0};
   GLfloat light_specular1[] = {0.9, 0.9, 0.9, 1.0};
   GLfloat light_full_off[] = {0.0, 0.0, 0.0, 1.0};
   GLfloat light_full_on[] = {1.0, 1.0, 1.0, 1.0};

   GLfloat light_position[] = {10.0, -5.0, 0.0, 0.0};
   // GLfloat light_position2[] = {-5.0, -5.0, -5.0, 0.0};
   GLfloat light_position2[] = {0, -15.0, 0.0, 0.0};

   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_full_on);

   glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
   glLightfv(GL_LIGHT1, GL_SPECULAR, light_full_on);

   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
   glEnable(GL_DEPTH_TEST);
}

void display(void)
{
   GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat gray[] = {0.8, 0.8, 0.8, 1.0};
   GLfloat darkgray[] = {0.3, 0.3, 0.3, 1.0};

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor(1, 1, 1, 1);

   glShadeModel(GL_SMOOTH);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   glMaterialf(GL_FRONT, GL_SHININESS, 128);

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   glPushMatrix();
   glTranslatef(0, yStart, zStart);
   glTranslatef(0.0, 0.0, -7.0);

   glRotatef(rot, 0.0, 1.0, 0.0);

   glMaterialfv(GL_FRONT, GL_AMBIENT, darkgray);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
   glMaterialfv(GL_FRONT, GL_SPECULAR, white);

   if (polyArray[0].size == 4)
   {
      for (int i = 0; i < numFaces; i++)
      {
         glBegin(GL_QUADS);
         glNormal3f(normalArray[i].x, normalArray[i].y, normalArray[i].z);
         glVertex3f(vertexArray[polyArray[i].vertices[0]].x, vertexArray[polyArray[i].vertices[0]].y, vertexArray[polyArray[i].vertices[0]].z);
         glVertex3f(vertexArray[polyArray[i].vertices[1]].x, vertexArray[polyArray[i].vertices[1]].y, vertexArray[polyArray[i].vertices[1]].z);
         glVertex3f(vertexArray[polyArray[i].vertices[2]].x, vertexArray[polyArray[i].vertices[2]].y, vertexArray[polyArray[i].vertices[2]].z);
         glVertex3f(vertexArray[polyArray[i].vertices[3]].x, vertexArray[polyArray[i].vertices[3]].y, vertexArray[polyArray[i].vertices[3]].z);
         glEnd();
      }
   }
   if (polyArray[0].size == 3)
   {
      for (int i = 0; i < numFaces; i++)
      {
         glBegin(GL_TRIANGLES);
         glNormal3f(normalArray[i].x, normalArray[i].y, normalArray[i].z);
         glVertex3f(vertexArray[polyArray[i].vertices[0]].x, vertexArray[polyArray[i].vertices[0]].y, vertexArray[polyArray[i].vertices[0]].z);
         glVertex3f(vertexArray[polyArray[i].vertices[1]].x, vertexArray[polyArray[i].vertices[1]].y, vertexArray[polyArray[i].vertices[1]].z);
         glVertex3f(vertexArray[polyArray[i].vertices[2]].x, vertexArray[polyArray[i].vertices[2]].y, vertexArray[polyArray[i].vertices[2]].z);
         glEnd();
      }
   }

   GLfloat m[16] = {
       1.0f, 0.0f, 0.0f, 0.0f,
       0.0f, 0.0f, 0.0f, 0.0f,
       0.0f, 0.0f, 1.0f, 0.0f,
       0.0f, lowestY, 0.0f, 1.0f};
   glMultMatrixf(m);
   if (polyArray[0].size == 3)
   {
      for (int i = 0; i < numFaces; i++)
      {
         glMaterialfv(GL_FRONT, GL_AMBIENT, darkgray);
         glMaterialfv(GL_FRONT, GL_DIFFUSE, darkgray);
         glMaterialfv(GL_FRONT, GL_SPECULAR, darkgray);
         glBegin(GL_TRIANGLES);
         glVertex3f(vertexArray[polyArray[i].vertices[0]].x, vertexArray[polyArray[i].vertices[0]].y, vertexArray[polyArray[i].vertices[0]].z);
         glVertex3f(vertexArray[polyArray[i].vertices[1]].x, vertexArray[polyArray[i].vertices[1]].y, vertexArray[polyArray[i].vertices[1]].z);
         glVertex3f(vertexArray[polyArray[i].vertices[2]].x, vertexArray[polyArray[i].vertices[2]].z, vertexArray[polyArray[i].vertices[2]].z);
         glEnd();
      }
   }
   else if (polyArray[0].size == 4)
   {
      for (int i = 0; i < numFaces; i++)
      {
         glMaterialfv(GL_FRONT, GL_AMBIENT, darkgray);
         glMaterialfv(GL_FRONT, GL_DIFFUSE, darkgray);
         glMaterialfv(GL_FRONT, GL_SPECULAR, darkgray);
         glBegin(GL_TRIANGLES);
         glVertex3f(vertexArray[polyArray[i].vertices[0]].x, 0, vertexArray[polyArray[i].vertices[0]].z);
         glVertex3f(vertexArray[polyArray[i].vertices[1]].x, 0, vertexArray[polyArray[i].vertices[1]].z);
         glVertex3f(vertexArray[polyArray[i].vertices[2]].x, 0, vertexArray[polyArray[i].vertices[2]].z);
         glVertex3f(vertexArray[polyArray[i].vertices[3]].x, 0, vertexArray[polyArray[i].vertices[3]].z);
         glEnd();
      }
   }

   glPopMatrix();
   glFlush();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 99.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 27:
   case 'q':
      exit(0);
      break;
   case 'a':
      zStart = zStart - 1;
      break;
   case 'd':
      zStart = zStart + 1;
      break;
   case 's':
      yStart = yStart + 0.5;
      break;
   case 'w':
      yStart = yStart - 0.5;
      break;
   }
}

void update()
{
   rot += 1.0;
   display();
}

int main(int argc, char **argv)
{
   int header = 1;
   int vertices = 0;
   int faces = 0;
   char line[128];
   char *token1;
   char *token2;
   char *token3;
   FILE *file;
   file = fopen(argv[1], "r");
   if (file)
   {
      while (1)
      {
         fgets(line, sizeof(line), file);
         if (strcmp(line, "end_header\n") == 0)
         {
            break;
         }
         token1 = strtok(line, " ");
         token2 = strtok(NULL, " ");
         token3 = strtok(NULL, " ");
         if (strcmp(token1, "element") == 0)
         {
            if (strcmp(token2, "vertex") == 0)
            {
               numVertices = atoi(token3);
            }
            if (strcmp(token2, "face") == 0)
            {
               numFaces = atoi(token3);
            }
         }
      }
      if (!(numVertices > 0) || !(numFaces > 0))
      {
         printf("Invalid format, number of faces and vertices must be greater than 0\n");
         return 0;
      }
      vertexArray = malloc(sizeof(Vertex) * numVertices);
      polyArray = malloc(sizeof(Poly) * numFaces);
      normalArray = malloc(sizeof(Normal) * numFaces);
      while (vertices < numVertices)
      {
         fgets(line, sizeof(line), file);
         token1 = strtok(line, " ");
         token2 = strtok(NULL, " ");
         token3 = strtok(NULL, " ");
         vertexArray[vertices].x = strtof(token1, NULL);
         vertexArray[vertices].y = strtof(token2, NULL);
         vertexArray[vertices].z = strtof(token3, NULL);
         vertices++;
      }
      while (faces < numFaces)
      {
         fgets(line, sizeof(line), file);
         token1 = strtok(line, " ");
         int localVertices = atoi(token1);
         polyArray[faces].size = localVertices;
         for (int i = 0; i < localVertices; i++)
         {
            token2 = strtok(NULL, " ");
            polyArray[faces].vertices[i] = strtof(token2, NULL);
         }

         // calculate normals
         Vector V1, V2;
         V2.x = vertexArray[polyArray[faces].vertices[1]].x - vertexArray[polyArray[faces].vertices[0]].x;
         V2.y = vertexArray[polyArray[faces].vertices[1]].y - vertexArray[polyArray[faces].vertices[0]].y;
         V2.z = vertexArray[polyArray[faces].vertices[1]].z - vertexArray[polyArray[faces].vertices[0]].z;

         V1.x = vertexArray[polyArray[faces].vertices[2]].x - vertexArray[polyArray[faces].vertices[0]].x;
         V1.y = vertexArray[polyArray[faces].vertices[2]].y - vertexArray[polyArray[faces].vertices[0]].y;
         V1.z = vertexArray[polyArray[faces].vertices[2]].z - vertexArray[polyArray[faces].vertices[0]].z;

         float normx = (V1.y * V2.z) - (V1.z * V2.y);
         float normy = (V1.z * V2.x) - (V1.x * V2.z);
         float normz = (V1.x * V2.y) - (V1.y * V2.x);
         float length = sqrt(normx * normx + normy * normy + normz * normz);
         normalArray[faces].x = normx / length;
         normalArray[faces].y = normy / length;
         normalArray[faces].z = normz / length;

         faces++;
      }
   }
   else
   {
      printf("File not found or not compatible. Must be a text based .ply file.\n");
      return 0;
   }

   lowestY = vertexArray[0].y;
   for (int i = 1; i < numVertices; i++)
   {
      if (lowestY > vertexArray[i].y)
      {
         lowestY = vertexArray[i].y;
      }
   }

   fclose(file);

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(1024, 768);
   glutCreateWindow("PLY Model Viewer (Controls with WASD)");
   init();
   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(update);
   glutMainLoop();
   free(vertexArray);
   free(polyArray);
   free(normalArray);
   return 0;
}
