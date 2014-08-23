/******************************************************************************
   Filename: chess.cpp

     Author: David C. Drake (http://davidcdrake.com), with initial assistance
             from Dr. Barton Stander (http://cit.dixie.edu/faculty/stander.php)

Description: Displays a chess board, chess pieces, and animations for a few
             chess moves. For now, it does not actually simulate a chess game
             or allow the user to play chess. (Note: "Y" is the "up" axis.)
******************************************************************************/

#include "chess.h"

using namespace std;

// Camera-related constants:

#define CAMERA_SPEED        75
#define INTRO_ZOOM_DURATION 4 // seconds

#define INITIAL_EYE_X       -300000
#define DEFAULT_EYE_X       4500
#define DEFAULT_EYE_Y       8000
#define DEFAULT_EYE_Z       -12000
#define Y_MIN               0
#define Y_MAX               50000

#define DEFAULT_AT_X        4500
#define DEFAULT_AT_Y        0
#define DEFAULT_AT_Z        4000

// Board-related constants:

#define BOARD_TOP               -10
#define BOARD_TOP_FRONT_EDGE    -1000
#define BOARD_TOP_BACK_EDGE     10000
#define BOARD_TOP_RIGHT_EDGE    -1000
#define BOARD_TOP_LEFT_EDGE     10000

#define BOTTOM_MODIFIER         0
#define BOARD_BOTTOM            -100000 // BOARD_TOP - BOTTOM_MODIFIER
#define BOARD_BOTTOM_FRONT_EDGE BOARD_TOP_FRONT_EDGE - BOTTOM_MODIFIER
#define BOARD_BOTTOM_BACK_EDGE  BOARD_TOP_BACK_EDGE + BOTTOM_MODIFIER
#define BOARD_BOTTOM_RIGHT_EDGE BOARD_TOP_RIGHT_EDGE - BOTTOM_MODIFIER
#define BOARD_BOTTOM_LEFT_EDGE  BOARD_TOP_LEFT_EDGE + BOTTOM_MODIFIER

#define BOARD_SQUARE_SIZE       1000
#define BOARD_SQUARE_ORIGIN_X   500
#define BOARD_SQUARE_ORIGIN_Z   500
#define BOARD_SQUARE_BORDER     20
#define BOARD_BORDER            500
#define Y_MODIFIER              5

// Color-related constants:

#define BOARD_MAIN_COLOR   redMaterial
#define BOARD_TOP_COLOR    blackMaterial
#define LIGHT_SQUARE_COLOR whiteMaterial
#define DARK_SQUARE_COLOR  blackMaterial
#define LIGHT_PIECE_COLOR  whiteMaterial
#define DARK_PIECE_COLOR   greenMaterial

// Global material-related variables:

GLfloat redMaterial[]         = { 0.7, 0.1, 0.2, 1.0 };
GLfloat greenMaterial[]       = { 0.1, 0.7, 0.4, 1.0 };
GLfloat brightGreenMaterial[] = { 0.1, 0.9, 0.1, 1.0 };
GLfloat blueMaterial[]        = { 0.1, 0.2, 0.7, 1.0 };
GLfloat whiteMaterial[]       = { 1.0, 1.0, 1.0, 1.0 };
GLfloat blackMaterial[]       = { 0.0, 0.0, 0.0, 0.0 };

// Global screen-related variables:

double screen_x = 900;
double screen_y = 600;

// Global camera-related variables:

double eye[3] = { DEFAULT_EYE_X, DEFAULT_EYE_Y, DEFAULT_EYE_Z };
double at[3]  = { DEFAULT_AT_X, DEFAULT_AT_Y, DEFAULT_AT_Z    };

// Global mouse-related variables:

bool leftMouseDown   = false;
bool rightMouseDown  = false;
bool middleMouseDown = false;


//-----------------------------------------------------------------------------
// Outputs a string of text at the specified location.
//-----------------------------------------------------------------------------
void text_output(double x, double y, char *string)
{
  void *font = GLUT_BITMAP_9_BY_15;

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  int len, i;
  glRasterPos2d(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++)
  {
    glutBitmapCharacter(font, string[i]);
  }

    glDisable(GL_BLEND);
}

//-----------------------------------------------------------------------------
// Given the three triangle points x[0],y[0],z[0],
//    x[1],y[1],z[1], and x[2],y[2],z[2],
//    Finds the normal vector n[0], n[1], n[2].
//-----------------------------------------------------------------------------
void FindTriangleNormal(double x[], double y[], double z[], double n[])
{
  // Convert the 3 input points to 2 vectors, v1 and v2:
  double v1[3], v2[3];
  v1[0] = x[1] - x[0];
  v1[1] = y[1] - y[0];
  v1[2] = z[1] - z[0];
  v2[0] = x[2] - x[0];
  v2[1] = y[2] - y[0];
  v2[2] = z[2] - z[0];

  // Get cross product of v1 and v2 to find the vector perpendicular to both:
  n[0] = v1[1]*v2[2] - v1[2]*v2[1];
  n[1] = -(v1[0]*v2[2] - v1[2]*v2[0]);
  n[2] = v1[0]*v2[1] - v1[1]*v2[0];

  double size = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
  n[0] /= -size;
  n[1] /= -size;
  n[2] /= -size;
}

//-----------------------------------------------------------------------------
// Loads a given data file and draws it at its default position.
// Call glTranslate before calling this to get it in the right place.
//-----------------------------------------------------------------------------
void DrawPiece(char filename[])
{
  // Try to open the file:
  char buffer[200];
  ifstream in(filename);
  if(!in)
  {
    cerr << "Error: could not open " << filename << endl;
    exit(1);
  }

  double x[100], y[100], z[100]; // stores a single polygon up to 100 vertices
  int done = false;
  int verts = 0;    // vertices in the current polygon
  int polygons = 0; // total polygons in this file
  do
  {
    in.getline(buffer, 200); // get one line (point) from the file
    int count = sscanf(buffer, "%lf, %lf, %lf", &(x[verts]), &(y[verts]), &(z[verts]));
    done = in.eof();
    if(!done)
    {
      if(count == 3) // if this line had an x,y,z point
      {
        verts++;
      }
      else // the line was empty: finish current polygon and start a new one
      {
        if(verts>=3)
        {
          glBegin(GL_POLYGON);
          double n[3];
          FindTriangleNormal(x, y, z, n);
          glNormal3dv(n);
          for(int i=0; i<verts; i++)
          {
            glVertex3d(x[i], y[i], z[i]);
          }
          glEnd(); // end previous polygon
          polygons++;
          verts = 0;
        }
      }
    }
  }while(!done);

  if(verts > 0)
  {
    cerr << "Error: extra vertices in file " << filename << endl;
    exit(1);
  }
}


//-----------------------------------------------------------------------------
// GLUT callback functions
//-----------------------------------------------------------------------------

void Interpolate(double t1,
                 double t,
                 double t2,
                 double x1,
                 double &x,
                 double x2)
{
  if (t < t1)
  {
    x = x1;
  }
  else if (t > t2)
  {
    x = x2;
  }
  else
  {
    double ratio = (t - t1) / (t2 - t1);
    x = x1 + (x2 - x1) * ratio;
  }
}

void TurnCameraClockwise()
{
  if (eye[2] > DEFAULT_AT_Z)
    eye[0] -= CAMERA_SPEED;
  else
    eye[0] += CAMERA_SPEED;
  if (eye[0] > DEFAULT_AT_X)
    eye[2] += CAMERA_SPEED;
  else
    eye[2] -= CAMERA_SPEED;
}

void TurnCameraCounterclockwise()
{
  if (eye[2] > DEFAULT_AT_Z)
    eye[0] += CAMERA_SPEED;
  else
    eye[0] -= CAMERA_SPEED;
  if (eye[0] > DEFAULT_AT_X)
    eye[2] -= CAMERA_SPEED;
  else
    eye[2] += CAMERA_SPEED;
}

void display(void)
{
  double x, y, z, angle;  // For chess piece animations.

  // If the user presses the 'esc' key, quit:
  if (IsKeyPressed(KEY_ESCAPE))
  {
    exit(0);
  }

  // Track the current time for animations:
  double currentTime = (double) clock() / CLOCKS_PER_SEC;

  // At launch, zoom toward the board from a distant vantage point:
  if (currentTime <= INTRO_ZOOM_DURATION)
  {
    Interpolate(0,
                currentTime,
                INTRO_ZOOM_DURATION,
                INITIAL_EYE_X, eye[0],
                DEFAULT_EYE_X);
  }
  else
  {
    // After the initial zoom, adjust perspective according to user input:
    if (leftMouseDown || IsKeyPressed(KEY_LEFT) || IsKeyPressed('a'))
    {
      TurnCameraClockwise();
    }
    if (rightMouseDown || IsKeyPressed(KEY_RIGHT) || IsKeyPressed('d'))
    {
      TurnCameraCounterclockwise();
    }
    if (middleMouseDown || IsKeyPressed(KEY_UP) || IsKeyPressed('w'))
    {
      if (eye[1] < Y_MAX)
        eye[1] += CAMERA_SPEED;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed('s'))
    {
      if (eye[1] > Y_MIN)
        eye[1] -= CAMERA_SPEED;
    }
  }

  // Prepare to draw to the screen:
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(eye[0], eye[1], eye[2],
        at[0], at[1], at[2],
        0, 1, 0); // Y is up!

  //
  // Draw the board:
  //

  // Top
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, BOARD_TOP_COLOR);
  glBegin(GL_QUADS);
  glVertex3d(BOARD_TOP_RIGHT_EDGE + BOARD_BORDER,
         BOARD_TOP + Y_MODIFIER,
         BOARD_TOP_FRONT_EDGE + BOARD_BORDER);
  glVertex3d(BOARD_TOP_LEFT_EDGE - BOARD_BORDER,
         BOARD_TOP + Y_MODIFIER,
         BOARD_TOP_FRONT_EDGE + BOARD_BORDER);
  glVertex3d(BOARD_TOP_LEFT_EDGE - BOARD_BORDER,
         BOARD_TOP + Y_MODIFIER,
         BOARD_TOP_BACK_EDGE - BOARD_BORDER);
  glVertex3d(BOARD_TOP_RIGHT_EDGE + BOARD_BORDER,
         BOARD_TOP + Y_MODIFIER,
         BOARD_TOP_BACK_EDGE - BOARD_BORDER);
  glEnd();
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, LIGHT_SQUARE_COLOR);
  for (int i = 0; i < 8; ++i)
  {
    for (int j = 0; j < 8; ++j)
    {
      glBegin(GL_QUADS);
      glVertex3d(BOARD_SQUARE_ORIGIN_X + i * BOARD_SQUARE_SIZE +
                   BOARD_SQUARE_BORDER,
                 BOARD_TOP + Y_MODIFIER * 2,
                 BOARD_SQUARE_ORIGIN_Z + j * BOARD_SQUARE_SIZE +
                   BOARD_SQUARE_BORDER);
      glVertex3d(BOARD_SQUARE_ORIGIN_X + i * BOARD_SQUARE_SIZE +
                   BOARD_SQUARE_BORDER,
                   BOARD_TOP + Y_MODIFIER * 2,
                   BOARD_SQUARE_ORIGIN_Z + j * BOARD_SQUARE_SIZE +
                     BOARD_SQUARE_SIZE - BOARD_SQUARE_BORDER);
      glVertex3d(BOARD_SQUARE_ORIGIN_X + i * BOARD_SQUARE_SIZE +
                   BOARD_SQUARE_SIZE - BOARD_SQUARE_BORDER,
                 BOARD_TOP + Y_MODIFIER * 2,
                 BOARD_SQUARE_ORIGIN_Z + j * BOARD_SQUARE_SIZE +
                   BOARD_SQUARE_SIZE - BOARD_SQUARE_BORDER);
      glVertex3d(BOARD_SQUARE_ORIGIN_X + i * BOARD_SQUARE_SIZE +
                   BOARD_SQUARE_SIZE - BOARD_SQUARE_BORDER,
                 BOARD_TOP + Y_MODIFIER * 2,
                 BOARD_SQUARE_ORIGIN_Z + j * BOARD_SQUARE_SIZE +
                   BOARD_SQUARE_BORDER);
      glEnd();
    }
  }
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, BOARD_MAIN_COLOR);
  glBegin(GL_QUADS);
  glVertex3d(BOARD_TOP_RIGHT_EDGE, BOARD_TOP, BOARD_TOP_FRONT_EDGE);
  glVertex3d(BOARD_TOP_LEFT_EDGE, BOARD_TOP, BOARD_TOP_FRONT_EDGE);
  glVertex3d(BOARD_TOP_LEFT_EDGE, BOARD_TOP, BOARD_TOP_BACK_EDGE);
  glVertex3d(BOARD_TOP_RIGHT_EDGE, BOARD_TOP, BOARD_TOP_BACK_EDGE);
  glEnd();

  // Front
  glBegin(GL_QUADS);
  glVertex3d(BOARD_TOP_RIGHT_EDGE, BOARD_TOP, BOARD_TOP_FRONT_EDGE);
  glVertex3d(BOARD_TOP_LEFT_EDGE, BOARD_TOP, BOARD_TOP_FRONT_EDGE);
  glVertex3d(BOARD_BOTTOM_LEFT_EDGE, BOARD_BOTTOM, BOARD_BOTTOM_FRONT_EDGE);
  glVertex3d(BOARD_BOTTOM_RIGHT_EDGE, BOARD_BOTTOM, BOARD_BOTTOM_FRONT_EDGE);
  glEnd();

  // Left
  glBegin(GL_QUADS);
  glVertex3d(BOARD_TOP_LEFT_EDGE, BOARD_TOP, BOARD_TOP_FRONT_EDGE);
  glVertex3d(BOARD_TOP_LEFT_EDGE, BOARD_TOP, BOARD_TOP_BACK_EDGE);
  glVertex3d(BOARD_BOTTOM_LEFT_EDGE, BOARD_BOTTOM, BOARD_BOTTOM_BACK_EDGE);
  glVertex3d(BOARD_BOTTOM_LEFT_EDGE, BOARD_BOTTOM, BOARD_BOTTOM_FRONT_EDGE);
  glEnd();

  // Right
  glBegin(GL_QUADS);
  glVertex3d(BOARD_TOP_RIGHT_EDGE, BOARD_TOP, BOARD_TOP_FRONT_EDGE);
  glVertex3d(BOARD_TOP_RIGHT_EDGE, BOARD_TOP, BOARD_TOP_BACK_EDGE);
  glVertex3d(BOARD_BOTTOM_RIGHT_EDGE, BOARD_BOTTOM, BOARD_BOTTOM_BACK_EDGE);
  glVertex3d(BOARD_BOTTOM_RIGHT_EDGE, BOARD_BOTTOM, BOARD_BOTTOM_FRONT_EDGE);
  glEnd();

  // Back
  glBegin(GL_QUADS);
  glVertex3d(BOARD_TOP_LEFT_EDGE, BOARD_TOP, BOARD_TOP_BACK_EDGE);
  glVertex3d(BOARD_TOP_RIGHT_EDGE, BOARD_TOP, BOARD_TOP_BACK_EDGE);
  glVertex3d(BOARD_BOTTOM_RIGHT_EDGE, BOARD_BOTTOM, BOARD_BOTTOM_BACK_EDGE);
  glVertex3d(BOARD_BOTTOM_LEFT_EDGE, BOARD_BOTTOM, BOARD_BOTTOM_BACK_EDGE);
  glEnd();

  // Bottom
  glBegin(GL_QUADS);
  glVertex3d(BOARD_BOTTOM_RIGHT_EDGE, BOARD_BOTTOM, BOARD_BOTTOM_FRONT_EDGE);
  glVertex3d(BOARD_BOTTOM_LEFT_EDGE, BOARD_BOTTOM, BOARD_BOTTOM_FRONT_EDGE);
  glVertex3d(BOARD_BOTTOM_LEFT_EDGE, BOARD_BOTTOM, BOARD_BOTTOM_BACK_EDGE);
  glVertex3d(BOARD_BOTTOM_RIGHT_EDGE, BOARD_BOTTOM, BOARD_BOTTOM_BACK_EDGE);
  glEnd();

  //
  // Draw the 16 white pieces:
  //

  //GLfloat mat_amb_diff1[] = { 0.8, 0.9, 0.5, 1.0 };
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, LIGHT_PIECE_COLOR);

  // King
  glPushMatrix();
  glTranslatef(4000, 0, 1000);
  glCallList(KING);
  glPopMatrix();

  // Queen
  glPushMatrix();
  glTranslatef(5000, 0, 1000);
  glCallList(QUEEN);
  glPopMatrix();

  // Bishop 1
  //double yScale = 1.0;
  //Interpolate(INTRO_ZOOM_DURATION + 5, currentTime, INTRO_ZOOM_DURATION + 7, 1, yScale, 0);
  glPushMatrix();
  glTranslatef(3000, 0, 1000);
  //glScaled(1, yScale, 1);
  glCallList(BISHOP);
  glPopMatrix();

  // Bishop 2
  glPushMatrix();
  glTranslatef(6000, 0, 1000);
  glCallList(BISHOP);
  glPopMatrix();

  // Knight 1
  glPushMatrix();
  glTranslatef(2000, 0, 1000);
  glCallList(KNIGHT);
  glPopMatrix();

  // Knight 2
  glPushMatrix();
  glTranslatef(7000, 0, 1000);
  glCallList(KNIGHT);
  glPopMatrix();

  // Rook 1
  y = (currentTime > INTRO_ZOOM_DURATION + 7.5) ? -100 : 0;
  Interpolate(INTRO_ZOOM_DURATION + 6.5, currentTime, INTRO_ZOOM_DURATION + 7.5, 0, angle, -180);
  Interpolate(INTRO_ZOOM_DURATION + 5, currentTime, INTRO_ZOOM_DURATION + 6, 1000, z, 7000);
  glPushMatrix();
  glTranslatef(1000, y, z);
  glRotatef(angle, 1, 0, 0);
  glCallList(ROOK);
  glPopMatrix();

  // Rook 2
  glPushMatrix();
  glTranslatef(8000, 0, 1000);
  glCallList(ROOK);
  glPopMatrix();

  // Pawns
  for(int i = 1; i <= 8; ++i)
  {
    x = i * 1000;
    z = 2000;
    if (i == 1)
    {
      if (currentTime <= INTRO_ZOOM_DURATION + 2)
      {
        Interpolate(INTRO_ZOOM_DURATION + 1,
                    currentTime,
                    INTRO_ZOOM_DURATION + 2,
                    2000,
                    z,
                    4000);
      }
      else
      {
        Interpolate(INTRO_ZOOM_DURATION + 3,
                    currentTime,
                    INTRO_ZOOM_DURATION + 4,
                    4000,
                    z,
                    5000);
      }
      Interpolate(INTRO_ZOOM_DURATION + 3,
                  currentTime,
                  INTRO_ZOOM_DURATION + 4,
                  i * 1000,
                  x,
                  i * 1000 + 1000);
    }
    glPushMatrix();
    glTranslatef(x, 0, z);
    glCallList(PAWN);
    glPopMatrix();
  }

  //
  // Draw the 16 black pieces:
  //

  //GLfloat mat_amb_diff2[] = { 0.1, 0.5, 0.8, 1.0 };
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, DARK_PIECE_COLOR);

  // King
  glPushMatrix();
  glTranslatef(4000, 0, 8000);
  glCallList(KING);
  glPopMatrix();

  // Queen
  glPushMatrix();
  glTranslatef(5000, 0, 8000);
  glCallList(QUEEN);
  glPopMatrix();

  // Bishop 1
  glPushMatrix();
  glTranslatef(3000, 0, 8000);
  glCallList(BISHOP);
  glPopMatrix();

  // Bishop 2
  glPushMatrix();
  glTranslatef(6000, 0, 8000);
  glCallList(BISHOP);
  glPopMatrix();

  // Knight 1
  glPushMatrix();
  glTranslatef(2000, 0, 8000);
  glRotatef(180, 0, 1, 0);
  glCallList(KNIGHT);
  glPopMatrix();

  // Knight 2
  Interpolate(INTRO_ZOOM_DURATION + 4.5,
              currentTime,
              INTRO_ZOOM_DURATION + 5,
              7000,
              x,
              6000);
  Interpolate(INTRO_ZOOM_DURATION + 4,
              currentTime,
              INTRO_ZOOM_DURATION + 5,
              8000,
              z,
              6000);
  glPushMatrix();
  glTranslatef(x, 0, z);
  glRotatef(180, 0, 1, 0);
  glCallList(KNIGHT);
  glPopMatrix();

  // Rook 1
  Interpolate(INTRO_ZOOM_DURATION + 6,
              currentTime,
              INTRO_ZOOM_DURATION + 7,
              8000,
              z,
              7000);
  glPushMatrix();
  glTranslatef(1000, 0, z);
  glCallList(ROOK);
  glPopMatrix();

  // Rook 2
  glPushMatrix();
  glTranslatef(8000, 0, 8000);
  glCallList(ROOK);
  glPopMatrix();

  // Pawns
  for(int i = 1; i <= 8; ++i)
  {
    z = 7000;
    y = 0;
    angle = 0;
    if (i == 1)
    {
      Interpolate(INTRO_ZOOM_DURATION + 5.9,
                  currentTime,
                  INTRO_ZOOM_DURATION + 6.4,
                  0,
                  angle,
                  180);
      if (currentTime > INTRO_ZOOM_DURATION + 6.4)
        y = -100;
    }
    else if (i == 2)
    {
      Interpolate(INTRO_ZOOM_DURATION + 2,
                  currentTime,
                  INTRO_ZOOM_DURATION + 3,
                  7000,
                  z,
                  5000);
      Interpolate(INTRO_ZOOM_DURATION + 3.75,
                  currentTime,
                  INTRO_ZOOM_DURATION + 4.25,
                  0,
                  angle,
                  180);
      if (currentTime > INTRO_ZOOM_DURATION + 4.25)
        y = -100;
    }
    glPushMatrix();
    glTranslatef(i * 1000, y, z);
    glRotatef(angle, 1, 0, 0);
    glCallList(PAWN);
    glPopMatrix();
  }

  glutSwapBuffers();
  glutPostRedisplay();
}

void SetPerspectiveView(int w, int h)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  double aspectRatio = (GLdouble) w / (GLdouble) h;
  gluPerspective(38.0, aspectRatio, 100.0, 1000000.0);
  glMatrixMode(GL_MODELVIEW);
}

void reshape(int w, int h)
{
  screen_x = w;
  screen_y = h;

  // Set the pixel resolution of the final picture (screen coordinates):
  glViewport(0, 0, w, h);

  SetPerspectiveView(w, h);
}

void mouse(int mouse_button, int state, int x, int y)
{
  if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    leftMouseDown = true;
  }
  if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP)
  {
    leftMouseDown = false;
  }
  if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
  {
    rightMouseDown = true;
  }
  if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
  {
    rightMouseDown = false;
  }
  if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
  {
    middleMouseDown = true;
  }
  if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
  {
    middleMouseDown = false;
  }

  glutPostRedisplay();
}

void InitializeMyStuff()
{
  // Set material properties:
  GLfloat mat_specular[]  = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 50.0 };
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  // Set light properties for up to 8 light sources (only one used here):
  GLfloat main_light[]   = { 1, 1, 1, 1 };
  GLfloat low_light[]     = { 0.3, 0.3, 0.3, 1 };
  GLfloat light_position[] = { -eye[0], -eye[1], -eye[2], 0 };
    // Last arg: 0 = vector light (no origin point), 1 = point light.
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, main_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, low_light);

  glEnable(GL_DEPTH_TEST);  // Turn on depth buffering.
  glEnable(GL_LIGHTING);    // Enable lighting.
  glEnable(GL_LIGHT0);      // Enable a specific light source (GL_LIGHT0).

  //
  // Initialize chess piece drawing lists:
  //

  glNewList(PAWN, GL_COMPILE);
  DrawPiece("PAWN.POL");
  glEndList();

  glNewList(KING, GL_COMPILE);
  DrawPiece("KING.POL");
  glEndList();

  glNewList(QUEEN, GL_COMPILE);
  DrawPiece("QUEEN.POL");
  glEndList();

  glNewList(ROOK, GL_COMPILE);
  DrawPiece("ROOK.POL");
  glEndList();

  glNewList(BISHOP, GL_COMPILE);
  DrawPiece("BISHOP.POL");
  glEndList();

  glNewList(KNIGHT, GL_COMPILE);
  DrawPiece("KNIGHT.POL");
  glEndList();
}


int main(int argc, char **argv)
{
  cout << "Welcome to David Drake's chess demonstration!" << endl << endl;
  cout << "CONTROLS:" << endl;
  cout << " 1. Use mouse buttons, arrow keys, and/or 'WSAD' keys to adjust ";
  cout << "camera position." << endl;
  cout << " 2. Press the 'esc' key to quit." << endl;

  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(screen_x, screen_y);
  glutInitWindowPosition(100, 50);

  bool fullscreen = false;
  if (fullscreen)
  {
    glutGameModeString("800x600:32");
    glutEnterGameMode();
  }
  else
  {
    glutCreateWindow("Chess");
  }

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  InitKeyboard();

  glClearColor(1, 1, 1, 1);
  InitializeMyStuff();

  glutMainLoop();

  return 0;
}
