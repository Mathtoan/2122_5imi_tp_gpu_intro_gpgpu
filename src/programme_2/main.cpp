#include <iostream>
#include <fstream>

#define GLEW_STATIC 1
#include <GL/glew.h>

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#define MACOSX_COMPATIBILITY GLUT_3_2_CORE_PROFILE
#else
#include <GL/glut.h>
#define MACOSX_COMPATIBILITY 0
#endif
#include "glhelper.h"
#include "mesh.h"
#include "camera.h"

GLuint VAO;
GLuint n_elements;

GLuint program_id;
GLuint texture_id;
Camera cam;


// Méthode d'initialisation pour afficher un carre qui recouvre la fenêtre
void init()
{
  glEnable(GL_DEPTH_TEST);

  program_id = glhelper::create_program_from_file("shaders/basic.vert", "shaders/texture.frag");

  Mesh m = Mesh::load_from_file("data/Frankie/Frankie.obj");
  auto rmat = glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  m.apply_matrix(rmat);
  m.compute_normales();
  m.normalize();
  VAO = m.load_to_gpu();
  n_elements= m.size_element();

  texture_id = glhelper::load_texture("data/Frankie/flyingsquirrel_skin_col.png");

  // TODO 
  // Create a new program and a quad for the post processing (you can inspire from the blur of program_1)
  // -> program_postprocess_id = glhelper::create_program_from_file("shaders/textured_quad.vert", "shaders/texture_blur.frag");
  // -> Mesh m = Mesh::create_grid(2);
  // -> auto rmat = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  // -> m.apply_matrix(rmat);
  // -> VAO_quad = m.load_to_gpu();
  // Create a new FBO and RBO and associated texture as described in the subject.
  // 
}

void set_uniform_mvp(GLuint program)
{
  GLint mvp_id = glGetUniformLocation(program, "MVP");
  GLint current_prog_id;
  glGetIntegerv(GL_CURRENT_PROGRAM, &current_prog_id);
  glUseProgram(program);
  if (mvp_id != -1)
  {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = cam.projection()*cam.view()*model;
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
  }
  glUseProgram(current_prog_id);
}

// Boucle d'affichage
static void display_callback()
{
  //TODO 
  // Render the scene in the texture -> you just have to be sure you use the correct program, object, viewport, fbo and texture

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program_id);
  glBindVertexArray(VAO);
  set_uniform_mvp(program_id);
  glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0); CHECK_GL_ERROR();

  //TODO 
  // Render in the window the rendered scene as a textured quad to postprocess
  // -> be sure you use the correct program, object, viewport, fbo and texture

  glBindVertexArray(0);
  glutSwapBuffers ();
}

static void keyboard_callback(unsigned char key, int, int)
{
  int viewport[4];

  switch (key)
  {
    case 'r':
      glDeleteProgram(program_id);
      program_id = glhelper::create_program_from_file("shaders/basic.vert", "shaders/texture.frag");
      break;
    case 'p':
      glGetIntegerv(GL_VIEWPORT, viewport);
      glhelper::print_screen(viewport[2], viewport[3]);
      break;
    case 'q':
    case 'Q':
    case 27:
              exit(0);
  }
  glutPostRedisplay();
}

static void reshape_callback(int width, int height)
{
  cam.common_reshape(width,height);

  glViewport(0,0, width, height);
  glutPostRedisplay();
}
static void mouse_callback (int button, int action, int x, int y)
{
  cam.common_mouse(button, action, x, y);
  glutPostRedisplay();
}

static void motion_callback(int x, int y)
{
  cam.common_motion(x, y);
  glutPostRedisplay();
}

static void timer_callback(int)
{
  glutTimerFunc(25, timer_callback, 0);
  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | MACOSX_COMPATIBILITY);
  glutInitWindowSize(cam.width(), cam.height());
  glutCreateWindow("opengl");
  glutDisplayFunc(display_callback);
  glutMotionFunc(motion_callback);
  glutMouseFunc(mouse_callback);
  glutKeyboardFunc(keyboard_callback);
  glutReshapeFunc(reshape_callback);
  glutTimerFunc(25, timer_callback, 0);
  glewExperimental=true;
  glewInit();

  int major, minor;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  std::cout << "OpenGL Version: " << major <<"."<< minor << std::endl;

  init();
  glutMainLoop();

  return 0;
}