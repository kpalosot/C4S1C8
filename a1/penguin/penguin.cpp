/***********************************************************
             CSC418/2504, Fall 2009
  
                 penguin.cpp
                 
       Simple demo program using OpenGL and the glut/glui 
       libraries

  
    Instructions:
        Please read the assignment page to determine 
        exactly what needs to be implemented.  Then read 
        over this file and become acquainted with its 
        design.

        Add source code where it appears appropriate. In
        particular, see lines marked 'TODO'.

        You should not need to change the overall structure
        of the program. However it should be clear what
        your changes do, and you should use sufficient comments
        to explain your code.  While the point of the assignment
        is to draw and animate the character, you will
        also be marked based on your design.

***********************************************************/

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glui.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef _WIN32
#include <unistd.h>
#else
void usleep(unsigned int nanosec)
{
    Sleep(nanosec / 1000);
}
#endif


// *************** GLOBAL VARIABLES *************************


const float PI = 3.14159;

// --------------- USER INTERFACE VARIABLES -----------------

// Window settings
int windowID;               // Glut window ID (for display)
GLUI *glui;                 // Glui window (for controls)
int Win[2];                 // window (x,y) size


// ---------------- ANIMATION VARIABLES ---------------------

// Animation settings
int animate_mode = 0;       // 0 = no anim, 1 = animate
int animation_frame = 0;      // Specify current frame of animation

// Joint parameters
const float JOINT_MIN = -45.0f;
const float JOINT_MAX =  45.0f;
//float joint_rot = 0.0f;

//////////////////////////////////////////////////////
// TODO: Add additional joint parameters here
//////////////////////////////////////////////////////
float arm_rot = 0.0f;
float left_hip_rot = 0.0f;
float left_knee_rot = 0.0f;
float right_hip_rot = 0.0f;
float right_knee_rot = 0.0f;
float neck_rot = 0.0f;
float beak_trans = 0.0f;
float x_trans = 0.0f;
float y_trans = 0.0f;

const float BEAK_MIN = -5.0f;
const float BEAK_MAX = 5.0f;

const float NECK_MIN = -25.0f;
const float NECK_MAX = 25.0f;

float X_MAX = 0;
float X_MIN = 0;
float Y_MAX = 0;
float Y_MIN = 0;



// ***********  FUNCTION HEADER DECLARATIONS ****************


// Initialization functions
void initGlut(char* winName);
void initGlui();
void initGl();


// Callbacks for handling events in glut
void myReshape(int w, int h);
void animate();
void display(void);

// Callback for handling events in glui
void GLUI_Control(int id);


// Functions to help draw the object
void drawSquare(float size);
void drawBody(float size);
void drawHead(float size);
void drawArm(float size);
void drawUpperLeg(float size);
void drawLowerLeg(float size);
void drawUpperBeak(float size);
void drawLowerBeak(float size);
void drawLeg(const float &rotation);
void drawCircle(float radius);
void drawGenHead();

//Constant variable for drawing object
const float BODY_WIDTH = 30.0f;
const float BODY_LENGTH = 50.0f;

const float ARM_LENGTH = 50.0f;
const float ARM_WIDTH = 10.0f;

const float HEAD_WIDTH = 25.0f;
const float HEAD_LENGTH = 25.0f;

const float UPPER_LEG_WIDTH = 10.0f;
const float UPPER_LEG_LENGTH = 40.0f;

const float LOWER_LEG_WIDTH = 20.0f;
const float LOWER_LEG_LENGTH = 10.0f;

const float TOP_BEAK_WIDTH = 15.0f;
const float TOP_BEAK_LENGTH = 7.5f;

const float BOTTOM_BEAK_WIDTH = 15.0f;
const float BOTTOM_BEAK_LENGTH = 5.0f;

// Return the current system clock (in seconds)
double getTime();


// ******************** FUNCTIONS ************************



// main() function
// Initializes the user interface (and any user variables)
// then hands over control to the event handler, which calls 
// display() whenever the GL window needs to be redrawn.
int main(int argc, char** argv)
{

    // Process program arguments
    if(argc != 3) {
        printf("Usage: demo [width] [height]\n");
        printf("Using 300x200 window by default...\n");
        Win[0] = 300;
        Win[1] = 200;
    } else {
        Win[0] = atoi(argv[1]);
        Win[1] = atoi(argv[2]);
    }


    // Initialize glut, glui, and opengl
    glutInit(&argc, argv);
    initGlut(argv[0]);
    initGlui();
    initGl();

    // Invoke the standard GLUT main event loop
    glutMainLoop();

    return 0;         // never reached
}


// Initialize glut and create a window with the specified caption 
void initGlut(char* winName)
{
    // Set video mode: double-buffered, color, depth-buffered
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Create window
    glutInitWindowPosition (0, 0);
    glutInitWindowSize(Win[0],Win[1]);
    windowID = glutCreateWindow(winName);

    // Setup callback functions to handle events
    glutReshapeFunc(myReshape); // Call myReshape whenever window resized
    glutDisplayFunc(display);   // Call display whenever new frame needed 
}


// Quit button handler.  Called when the "quit" button is pressed.
void quitButton(int)
{
  exit(0);
}

// Animate button handler.  Called when the "animate" checkbox is pressed.
void animateButton(int)
{
  // synchronize variables that GLUT uses
  glui->sync_live();

  animation_frame = 0;
  if(animate_mode == 1) {
    // start animation
    GLUI_Master.set_glutIdleFunc(animate);
  } else {
    // stop animation
    GLUI_Master.set_glutIdleFunc(NULL);
  }
}

// Initialize GLUI and the user interface
void initGlui()
{
    GLUI_Master.set_glutIdleFunc(NULL);

    // Create GLUI window
    glui = GLUI_Master.create_glui("Glui Window", 0, Win[0]+10, 0);

    // Create a control to specify the rotation of the joint
    // GLUI_Spinner *joint_spinner
    //     = glui->add_spinner("Joint", GLUI_SPINNER_FLOAT, &joint_rot);
    // joint_spinner->set_speed(0.1);
    // joint_spinner->set_float_limits(JOINT_MIN, JOINT_MAX, GLUI_LIMIT_CLAMP);

    ///////////////////////////////////////////////////////////
    // TODO: 
    //   Add controls for additional joints here
    ///////////////////////////////////////////////////////////

    // Create a control to specify the rotation of the joint

    X_MAX = Win[0]/2;
    X_MIN = -Win[0]/2;
    Y_MAX = Win[1]/2;
    Y_MIN = -Win[1]/2;

    GLUI_Spinner *arm_spinner
        = glui->add_spinner("Arm", GLUI_SPINNER_FLOAT, &arm_rot);
    arm_spinner->set_speed(0.1);
    arm_spinner->set_float_limits(JOINT_MIN, JOINT_MAX, GLUI_LIMIT_CLAMP);

    GLUI_Spinner *neck_spinner
        = glui->add_spinner("Neck", GLUI_SPINNER_FLOAT, &neck_rot);
    neck_spinner->set_speed(0.1);
    neck_spinner->set_float_limits(NECK_MIN, NECK_MAX, GLUI_LIMIT_CLAMP); //change min & max

    GLUI_Spinner *left_hip_spinner
        = glui->add_spinner("Left Hip", GLUI_SPINNER_FLOAT, &left_hip_rot);
    left_hip_spinner->set_speed(0.1);
    left_hip_spinner->set_float_limits(JOINT_MIN, JOINT_MAX, GLUI_LIMIT_CLAMP);

    GLUI_Spinner *left_knee_spinner
        = glui->add_spinner("Left Knee", GLUI_SPINNER_FLOAT, &left_knee_rot);
    left_knee_spinner->set_speed(0.1);
    left_knee_spinner->set_float_limits(JOINT_MIN, JOINT_MAX, GLUI_LIMIT_CLAMP);//change min & max

    GLUI_Spinner *right_hip_spinner
        = glui->add_spinner("Right Hip", GLUI_SPINNER_FLOAT, &right_hip_rot);
    right_hip_spinner->set_speed(0.1);
    right_hip_spinner->set_float_limits(JOINT_MIN, JOINT_MAX, GLUI_LIMIT_CLAMP);//change min & max

    GLUI_Spinner *right_knee_spinner
        = glui->add_spinner("Right Knee", GLUI_SPINNER_FLOAT, &right_knee_rot);
    right_knee_spinner->set_speed(0.1);
    right_knee_spinner->set_float_limits(JOINT_MIN, JOINT_MAX, GLUI_LIMIT_CLAMP);

    GLUI_Spinner *beak_trans_spinner
        = glui->add_spinner("Beak", GLUI_SPINNER_FLOAT, &beak_trans);
    beak_trans_spinner->set_speed(0.1);
    beak_trans_spinner->set_float_limits(BEAK_MIN, BEAK_MAX, GLUI_LIMIT_CLAMP);

    GLUI_Spinner *x_trans_spinner
        = glui->add_spinner("X Move", GLUI_SPINNER_FLOAT, &x_trans);
    x_trans_spinner->set_speed(0.1);
    x_trans_spinner->set_float_limits(X_MIN, X_MAX, GLUI_LIMIT_CLAMP);

    GLUI_Spinner *y_trans_spinner
        = glui->add_spinner("Y Move", GLUI_SPINNER_FLOAT, &y_trans);
    y_trans_spinner->set_speed(0.1);
    y_trans_spinner->set_float_limits(Y_MIN, Y_MAX, GLUI_LIMIT_CLAMP);

    // Add button to specify animation mode 
    glui->add_separator();
    glui->add_checkbox("Animate", &animate_mode, 0, animateButton);

    // Add "Quit" button
    glui->add_separator();
    glui->add_button("Quit", 0, quitButton);

    // Set the main window to be the "active" window
    glui->set_main_gfx_window(windowID);
}


// Performs most of the OpenGL intialization
void initGl(void)
{
    // glClearColor (red, green, blue, alpha)
    // Ignore the meaning of the 'alpha' value for now
    glClearColor(0.7f,0.7f,0.9f,1.0f);
}


// Callback idle function for animating the scene
void animate()
{
    // Update geometry
    /*const double joint_rot_speed = 0.1;
    double joint_rot_t = (sin(animation_frame*joint_rot_speed) + 1.0) / 2.0;
    joint_rot = joint_rot_t * JOINT_MIN + (1 - joint_rot_t) * JOINT_MAX;*/
    
    ///////////////////////////////////////////////////////////
    // TODO:
    //   Modify this function animate the character's joints
    //   Note: Nothing should be drawn in this function!  OpenGL drawing
    //   should only happen in the display() callback.
    ///////////////////////////////////////////////////////////

    const double arm_rot_speed = 0.1;
    double arm_rot_t = (sin(animation_frame*arm_rot_speed) + 1.0) / 2.0;
    arm_rot = arm_rot_t * JOINT_MIN + (1 - arm_rot_t) * JOINT_MAX;

    const double left_hip_rot_speed = 0.1;
    double left_hip_rot_t = (sin(animation_frame*left_hip_rot_speed) + 1.0) / 2.0;
    left_hip_rot = left_hip_rot_t * JOINT_MIN + (1 - left_hip_rot_t) * JOINT_MAX;

    const double right_hip_rot_speed = 0.1;
    double right_hip_rot_t = (sin(animation_frame*right_hip_rot_speed) + 1.0) / 2.0;
    right_hip_rot = right_hip_rot_t * JOINT_MIN + (1 - right_hip_rot_t) * JOINT_MAX;

    const double left_knee_rot_speed = 0.1;
    double left_knee_rot_t = (sin(animation_frame*left_knee_rot_speed) + 1.0) / 2.0;
    left_knee_rot = left_knee_rot_t * JOINT_MIN + (1 - left_knee_rot_t) * JOINT_MAX;

    const double right_knee_rot_speed = 0.1;
    double right_knee_rot_t = (sin(animation_frame*right_knee_rot_speed) + 1.0) / 2.0;
    right_knee_rot = right_knee_rot_t * JOINT_MIN + (1 - right_knee_rot_t) * JOINT_MAX;

    const double neck_rot_speed = 0.1;
    double neck_rot_t = (sin(animation_frame*neck_rot_speed) + 1.0) / 2.0;
    neck_rot = neck_rot_t * NECK_MIN + (1 - neck_rot_t) * NECK_MAX;

    const double beak_trans_speed = 0.1;
    double beak_trans_t = (sin(animation_frame*beak_trans_speed) + 1.0) / 2.0;
    beak_trans = beak_trans_t * BEAK_MIN + (1 - beak_trans_t) * BEAK_MAX;

    const double x_trans_speed = 0.1;
    double x_trans_t = (sin(animation_frame*x_trans_speed) + 1.0) / 2.0;
    x_trans = x_trans_t * X_MIN + (1 - x_trans_t) * X_MAX;

    const double y_trans_speed = 0.1;
    double y_trans_t = (sin(animation_frame*y_trans_speed) + 1.0) / 2.0;
    y_trans = y_trans_t * Y_MIN + (1 - y_trans_t) * Y_MAX;

    // Update user interface
    glui->sync_live();

    // Tell glut window to update itself.  This will cause the display()
    // callback to be called, which renders the object (once you've written
    // the callback).
    glutSetWindow(windowID);
    glutPostRedisplay();

    // increment the frame number.
    animation_frame++;

    // Wait 50 ms between frames (20 frames per second)
    usleep(50000);
}


// Handles the window being resized by updating the viewport
// and projection matrices
void myReshape(int w, int h)
{
    // Setup projection matrix for new window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-w/2, w/2, -h/2, h/2);

    // Update OpenGL viewport and internal variables
    glViewport(0,0, w,h);
    Win[0] = w;
    Win[1] = h;
}



// display callback
//
// This gets called by the event handler to draw
// the scene, so this is where you need to build
// your scene -- make your changes and additions here.
// All rendering happens in this function.  For Assignment 1,
// updates to geometry should happen in the "animate" function.
void display(void)
{
    // glClearColor (red, green, blue, alpha)
    // Ignore the meaning of the 'alpha' value for now
    glClearColor(0.7f,0.7f,0.9f,1.0f);

    // OK, now clear the screen with the background colour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

    // Setup the model-view transformation matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    ///////////////////////////////////////////////////////////
    // TODO:
    //   Modify this function draw the scene
    //   This should include function calls to pieces that
    //   apply the appropriate transformation matrice and
    //   render the individual body parts.
    ///////////////////////////////////////////////////////////

    // Push the current transformation matrix on the stack
    glPushMatrix();

        glTranslatef(0 + x_trans, 0 + y_trans,0);
        
        // Draw the 'body'
        glPushMatrix();
            // Scale square to size of body
            glScalef(BODY_WIDTH, BODY_LENGTH, 1.0);

            // Set the colour to blue
            glColor3f(0.0, 0.0, 1.0);

            // Draw the graphics for the body
            drawBody(2.0);
        glPopMatrix();

        //Draw the head
        glPushMatrix();
            glTranslatef(0.0, BODY_LENGTH/2 + HEAD_LENGTH*0.70, 0.0);

            glRotatef(neck_rot,0.0,0.0,1.0);

            drawGenHead();
            //drawHead(2.0);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0, BODY_LENGTH/2 + HEAD_LENGTH*0.70, 0.0);
            drawCircle(2.0);
        glPopMatrix();

        // Draw the 'arm'
        glPushMatrix();
            // Move the arm to the joint hinge
            glTranslatef(BODY_WIDTH/4, BODY_LENGTH/2, 0.0);

            // Rotate along the hinge
            glRotatef(arm_rot, 0.0, 0.0, 1.0);

            // Scale the size of the arm
            glScalef(ARM_WIDTH, ARM_LENGTH, 1.0);

            // Move to center location of arm, under previous rotation
            glTranslatef(0.0, -0.5, 0.0);

            glColor3f(1.0, 0.0, 0.0);

            drawArm(1.0);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(BODY_WIDTH/4, BODY_LENGTH/2, 0.0);
            drawCircle(2.0);
        glPopMatrix();

        //draw the left leg
        glPushMatrix();

            glTranslatef(-BODY_WIDTH/2, -BODY_LENGTH/2 - UPPER_LEG_LENGTH/4, 0.0);

            glRotatef(left_hip_rot, 0.0, 0.0, 1.0);

            drawLeg(left_knee_rot);

        glPopMatrix();

        glPushMatrix();
            glTranslatef(-BODY_WIDTH/2, -BODY_LENGTH/2 - UPPER_LEG_LENGTH/4, 0.0);
            drawCircle(2.0);
        glPopMatrix();

        //draw the right leg
        glPushMatrix();

            glTranslatef(BODY_WIDTH/2, -BODY_LENGTH/2 - UPPER_LEG_LENGTH/4, 0.0);

            glRotatef(right_hip_rot, 0.0, 0.0, 1.0);

            drawLeg(right_knee_rot);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(BODY_WIDTH/2, -BODY_LENGTH/2 - UPPER_LEG_LENGTH/4, 0.0);
            drawCircle(2.0);
        glPopMatrix();


    // Retrieve the previous state of the transformation stack
    glPopMatrix();


    // Execute any GL functions that are in the queue just to be safe
    glFlush();

    // Now, show the frame buffer that we just drew into.
    // (this prevents flickering).
    glutSwapBuffers();
}


// Draw a square of the specified size, centered at the current location
void drawSquare(float width)
{
    // Draw the square
    glBegin(GL_POLYGON);
    glVertex2d(-width/2, -width/2);
    glVertex2d(width/2, -width/2);
    glVertex2d(width/2, width/2);
    glVertex2d(-width/2, width/2);
    glEnd();
}

void drawBody(float width){
    glBegin(GL_POLYGON);
    glVertex2d(-width/3, width/2); //1
    glVertex2d(width/3, width/2); //2
    glVertex2d(width/2, -width/4); //3
    glVertex2d(width/3, -width/2); //4
    glVertex2d(-width/3,-width/2); //5
    glVertex2d(-width/2, -width/4); //6
    glEnd();

}

void drawHead(float width){
    glPushMatrix();

        // Scale square to size of body
        glScalef(HEAD_WIDTH, HEAD_LENGTH, 1.0);

        glTranslatef(0.0, -1.0, 0.0);

        // Set the colour to red
        glColor3f(1.0, 0.0, 0.0);

        glBegin(GL_POLYGON);
        glVertex2d(-width/2, width/2.05); //1
        glVertex2d(-width/2.5, width); //2
        glVertex2d(-width/5, width*1.2); //3
        glVertex2d(width/4, width); //4
        glVertex2d(width/2.05,width/2.05); //5
        glEnd();

    glPopMatrix();
}

void drawArm(float width){
    glBegin(GL_POLYGON);
    glVertex2d(-width/2, -width/2);
    glVertex2d(width/2, -width/2);
    glVertex2d(width/1.05, width/2);
    glVertex2d(-width/1.05, width/2);
    glEnd();
}

void drawUpperLeg(float width){

    glPushMatrix();

        glScalef(UPPER_LEG_WIDTH, UPPER_LEG_LENGTH, 1.0);
        glTranslatef(0.0, -0.5, 0.0);
        glColor3f(1.0, 0.0, 0.0);

        glBegin(GL_POLYGON);
        glVertex2d(-width/2, -width/2); //lower left
        glVertex2d(width/2, -width/2); // lower right
        glVertex2d(width/2, width/2); // upper right
        glVertex2d(-width/2, width/2); //upper left
        glEnd();

    glPopMatrix();
}

void drawLowerLeg(float width){
    glBegin(GL_POLYGON);
    glVertex2d(-width/1.5, -width/2);
    glVertex2d(width/1.5, -width/2);
    glVertex2d(width/1.5, width/2);
    glVertex2d(-width/1.5, width/2);
    glEnd();
}

void drawLeg(const float &rotation){
    drawUpperLeg(1.0);

    glPushMatrix();
        glTranslatef(-LOWER_LEG_WIDTH/15, -LOWER_LEG_LENGTH*3.5, 0.0);
        //glTranslatef(0,0,0);
        glScalef(LOWER_LEG_WIDTH, LOWER_LEG_LENGTH, 1.0);
        glRotatef(rotation, 0.0, 0.0, 1.0);
        glTranslatef(-0.30, 0.0, 0.0);
        glColor3f(0.0, 0.0, 0.0);
        drawLowerLeg(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-LOWER_LEG_WIDTH/15, -LOWER_LEG_LENGTH*3.5, 0.0);
        drawCircle(2.0);
    glPopMatrix();
}

void drawUpperBeak(float width){
    glBegin(GL_POLYGON);
    glVertex2d(-width, -width/2);
    glVertex2d(width, -width/2);
    glVertex2d(width, width/1.5);
    glVertex2d(-width, width/3);
    glEnd();
}

void drawLowerBeak(float width){
    glBegin(GL_POLYGON);
    glVertex2d(-width, -width/4);
    glVertex2d(width, -width/4);
    glVertex2d(width, width/4);
    glVertex2d(-width, width/4);
    glEnd();
}

void drawGenHead(){
    drawHead(2.0);

    //draw the upper beak
    glPushMatrix();
        glTranslatef(-HEAD_WIDTH,HEAD_LENGTH/2,0);

        glScalef(TOP_BEAK_WIDTH, TOP_BEAK_LENGTH, 1.0);

        glColor3f(0.0, 0.0, 0.0);

        drawUpperBeak(1.0);
    glPopMatrix();

    //draw the lower beak
    glPushMatrix();

        glTranslatef(-HEAD_WIDTH,HEAD_LENGTH/8 + beak_trans,0);

        glScalef(BOTTOM_BEAK_WIDTH, BOTTOM_BEAK_LENGTH, 1.0);

        glColor3f(0.0, 0.0, 1.0);

        drawLowerBeak(1.0);
    glPopMatrix();
}

const float DEG2RAD = 3.14159/180;
 
void drawCircle(float radius){
   glBegin(GL_POLYGON);
   glColor3f(1.0,1.0,1.0);
 
   for (int i = 0; i <= 360; i++){
      float degInRad = i * DEG2RAD;
      glVertex2f(cos(degInRad) * radius, sin(degInRad) * radius);
   } 
   glEnd();
}
















