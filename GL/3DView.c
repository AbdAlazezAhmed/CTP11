#include "3DView.h"
#include <stdio.h>
#include <math.h>
guint vertixArray, vertixBuffer;
guint indexBuffer;
GLuint shaderProgram;
float *vertexData;
unsigned int *indexData;
const char *vertexSource = "\n"
                           "#version 150 core\n"

                           "in vec3 position;\n"
                           "out vec4 out_color;\n"

                           "void main()\n"
                           " {\n"
                           " gl_Position = vec4(position[0],position[1],position[2], 0.5);\n"
                           "out_color=vec4(gl_Position[2],gl_Position[2],gl_Position[2],1.0);\n"
                           "}\n"
                           "\n";
const char *fragmentSource = "#version 150 core\n"
                             "in vec4 out_color;\n"
                             "out vec4 outColor;\n"

                             "void main()\n"
                             "{\n"
                             "    outColor = out_color;\n"
                             "}\n";
void readData(float *data, unsigned int *index, FILE *file)
{
  for (int i = 0; i < 108 * 3; i += 3)
  {
    fscanf(file, "%*c %f %f %f\n", &data[i], &data[i + 1], &data[i + 2]);
    // float temp;
    // temp = data[i+1];
    // data[i+1]=data[i+2];
    // data[i+2]=-temp;
    // printf("%f %f %f\n",data[i],data[i+1],data[i+2]);
  }
  for (int i = 0; i < 108 * 3; i += 3)
  {
    float x, y, z;
    x = data[i + 0] * cos(-G_PI / 6) - data[i + 2] * sin(-G_PI / 6);
    z = data[i + 2] * cos(-G_PI / 6) + data[i + 0] * sin(-G_PI / 6);
    y = data[i + 1] * cos(-G_PI / 6) - z * sin(-G_PI / 6);
    z = z * cos(-G_PI / 6) + data[i + 1] * sin(-G_PI / 6);
    data[i + 0] = x;
    data[i + 1] = y;
    data[i + 2] = z;
    // printf("%f %f %f\n",data[i],data[i+1],data[i+2]);

    /* code */
  }
  for (int i = 0; i < 108 * 3; i++)
  {
    data[i] /= 220;
    // printf("%f \n",data[i]);
  }
  char testChar[100];
  while (testChar[0] != 's')
  {
    fgets(testChar, 100, file);
    // printf("%s",testChar);
  };

  fscanf(file, "f %u//%*u %u//%*u %u//%*u\n", &index[0], &index[1], &index[2]);
  for (size_t i = 3; i < 212 * 3; i += 3)
  {
    fscanf(file, "f %u//%*u %u//%*u %u//%*u\n", &index[i], &index[i + 1], &index[i + 2]);
  }

  for (size_t i = 0; i < 212 * 3; i++)
  {

    index[i]--;
    //  printf("%i\n\n",index[i]);
  }
}
void init_buffer_objects(GError *error)
{
  vertexData = malloc(108 * 3 * sizeof(float));
  indexData = malloc(212 * 3 * sizeof(unsigned int));
  FILE *file = fopen("models/untitled.obj", "r");
  readData(vertexData, indexData, file);
  fclose(file);

  glGenVertexArrays(1, &vertixArray);
  glBindVertexArray(vertixArray);

  glGenBuffers(1, &vertixBuffer);
  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertixBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ARRAY_BUFFER, 108 * 3 * sizeof(float), vertexData, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 212 * 3 * sizeof(unsigned int), indexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexData);
}
void on_realize(GtkGLArea *area)
{
  // We need to make the context current if we want to
  // call GL API
  gtk_gl_area_make_current(area);
  // If there were errors during the initialization or
  // when trying to make the context current, this
  // function will return a #GError for you to catch
  if (gtk_gl_area_get_error(area) != NULL)
    return;

  // You can also use gtk_gl_area_set_error() in order
  // to show eventual initialization errors on the
  // GtkGLArea widget itself
  GError *internal_error = NULL;
  init_buffer_objects(internal_error);
  if (internal_error != NULL)
  {
    gtk_gl_area_set_error(area, internal_error);
    g_error_free(internal_error);
    return;
  }
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);

  glUseProgram(shaderProgram);
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(posAttrib);

  // glDisable(GL_CULL_FACE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // init_shaders (&error);
  // if (error != NULL)
  //   {
  //     gtk_gl_area_set_error (area, error);
  //     g_error_free (error);
  //     return;
  //   }
}
gboolean render(GtkGLArea *area, GdkGLContext *context)
{

  printf("OpenGL version supported by this platform (%s): \n",
         glGetString(GL_VERSION));
  glClearColor(0.2, 0.2, 0.2, 1);

  glClear(GL_COLOR_BUFFER_BIT);

  // //
  // glClearDepth(1.0f);                   // Set background depth to farthest
  //  glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
  //  glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
  //  glShadeModel(GL_SMOOTH);   // Enable smooth shading
  //  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  //  //

  /* draw the three vertices as a triangle */
  // glDrawArrays (GL_TRIANGLES, 0, 108);

  glDrawElements(
      GL_TRIANGLES,    // mode
      212 * 3,         // count
      GL_UNSIGNED_INT, // type
      (void *)0        // element array buffer offset
  );

  glFlush();

  /* we finished using the buffers and program */
  // printf(vertexSource);

  // draw your object
  // draw_an_object ();

  // we completed our drawing; the draw commands will be
  // flushed at the end of the signal emission chain, and
  // the buffers will be drawn on the window
  return TRUE;
}