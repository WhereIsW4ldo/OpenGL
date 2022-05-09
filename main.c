#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "all.h"

GLint winWidth = 800, winHeight = 800;
int x_cam = 100, y_cam = 100, z_cam = 100;
int x_cam_f = 150, y_cam_f = 150, z_cam_f = 150;
GLdouble xwmin = -100, xwmax = 100, ywmin = -100, ywmax = 100;
GLdouble near = 0.1, far = 1000;
char mode = 's';
const float coo[4][3] = {{0, HEIGHT/2.0, 0}, {WIDTH, HEIGHT/2.0, 0},{0, HEIGHT/2.0, WIDTH}, {WIDTH, HEIGHT/2.0, WIDTH}};
int angle = 0;
float afstand_bal = 50;
float length_kabel = 10;
int bol_dia = 3;
int delta = 0;

bool anim_ver = false, anim_hor = false, anim_rot = false;
bool up = true, right = true;
bool smooth = false;

bool am_wit = true, dig_glbl = true, spec_ro = true, spot = true;
int spotHoek = 30;
int spotHoogte = 0;
int exp_ = 20;
bool doorzichtig = false;

GLfloat grijs[3][3] = {{0.22, 0.22, 0.22}, {0.33, 0.33, 0.33}, {0.11, 0.11, 0.11}};
GLfloat wit[3][4] = {{0.66, 0.66, 0.66, 0.2}, {0.77, 0.77, 0.77, 0.1}, {0.55, 0.55, 0.55, 0.3}};
GLfloat chroom[3][3] = {{0.46, 0.58, 0.35}, {0.23, 0.29, 0.17}, {0.69, 0.87, 0.52}};
GLfloat brons[3][3] = {{0.21, 0.13, 0.10}, {0.39, 0.27, 0.17}, {0.71, 0.43, 0.18}};
GLfloat geel[3][3] = {{0.65, 0.55, 0.15}, {0.75, 0.45, 0.15}, {0.85, 0.35, 0.15}};
GLfloat lila[3][3] = {{0.45, 0.15, 0.75}, {0.55, 0.15, 0.65}, {0.35, 0.15, 0.85}};
GLfloat shininess = 10;

GLfloat ctrlpoints[6][4][3] = {
    {{0, 0, 0}, {5, 0, 5}, {5, 0, 10}, {0, 0, 15}},
    {{0, 5, 0}, {5, 5, 5}, {5, 5, 10}, {0, 5, 15}},
    {{0, 10, 0}, {5, 10, 5}, {5, 10, 10}, {0, 10, 15}},
    {{0, 15, 0}, {5, 15, 5}, {5, 15, 10}, {0, 15, 15}},
    {{0, 12.5, 0}, {5, 12.5, 5}, {5, 12.5, 10}, {0, 12.5, 15}},
    {{0, 15, 0}, {5, 15, 5}, {5, 15, 10}, {0, 15, 15}}
};
GLfloat controlPoints[2][4][3] = {
    { {0, 0, 0},  {5, 0,  5}, {5, 0,  10}, {0, 0,  15} },
    { {0, 0, 0},  {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};


bool controle = false;

int amount = 1;
bool on[PARTS];
bool as = false;
bool draad = false;
int draw = GL_FILL;
int tijd = 16;

bool mist = false;
bool exp_fog = false;

/*
 * void init(): 
 * clear screen, fix that depth is tested, look with camera on x_cam, y_cam, z_cam to 0, 5, 0;
 * give lights each color in different modes, enable view of every component by default
 */
void init(void)
{
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MAP2_VERTEX_3);

    gluLookAt(x_cam, y_cam, z_cam, 0, 5, 0, 0, 0, 0);
    
    GLfloat amb[] = {1, 1, 1, 1};
    GLfloat dif[] = {0, 1, 1, 1};
    GLfloat spec[] = {1, 0, 0, 1};
    GLfloat spot_[] = {1, 1, 0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
    glLightfv(GL_LIGHT2, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT3, GL_AMBIENT, spot_);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, spot_);
    glLightfv(GL_LIGHT3, GL_SPECULAR, spot_);

    for (int i = 0; i < PARTS; i++)
        on[i] = true;
}

/*
 * void winReshapeFcn(GLint w, GLint h): 
 * map viewport coordinates to new screen coordinates, fix all perspectives
 */
void winReshapeFcn(GLint w, GLint h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    winWidth = w;
    winHeight = h;

    switch (mode)
    {
    case 'o':
        if (w <= h)
            glOrtho(xwmin, xwmax, ywmin*(GLfloat)h/(GLfloat)w,
                    ywmax*(GLfloat)h/(GLfloat)w, near, far);
        else
            glOrtho(xwmin*(GLfloat)w/(GLfloat)h,
                    xwmax*(GLfloat)w/(GLfloat)h, ywmin, ywmax, near, far);
        break;
    case 's': // gluPerspective(fovy, aspect, near, far)
        gluPerspective(100, (float)w / (float)h, 0.1, 1000);
        break;
    case 'a': // glFrustum(left, right, bottom, top, near, far)
        glFrustum(-100, 100, -100, 100, 100, 1000);
        break;
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
/*
 * void displayFcn(): 
 * show everything on screen, enable lights if possible, print amount of cranes
 */
void displayFcn(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (mode == 'a')
        gluLookAt(x_cam_f, y_cam_f, z_cam_f, 0, 0, 0, 0, 5, 0);
    else
        gluLookAt(x_cam, y_cam, z_cam, 0, 0, 0, 0, 5, 0);
    if (as)
        drawAssen();

    glPushMatrix();

    if (mist)
    {
        glEnable(GL_FOG);
        GLfloat kleur[4] = {0.0, 0.0, 0.0, 1};
        glFogfv(GL_FOG_COLOR, kleur);
        if (!exp_fog)
        {
            glFogf(GL_FOG_MODE, GL_LINEAR);
            glFogf(GL_FOG_START, 100);
            glFogf(GL_FOG_END, 200);
        }
        else
        {
            glFogf(GL_FOG_MODE, GL_EXP);
        }
    }

    GLfloat pos1[] = {100, 0, 0, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, pos1);
    glEnable(GL_LIGHTING);
    if (am_wit)
        glEnable(GL_LIGHT0);

    GLfloat pos2[] = {-100, 0, 0, 1};
    glLightfv(GL_LIGHT1, GL_POSITION, pos2);
    if (dig_glbl)
        glEnable(GL_LIGHT1);

    GLfloat pos3[] = {0, 0, 100, 1};
    glLightfv(GL_LIGHT2, GL_POSITION, pos3);
    if (spec_ro)
        glEnable(GL_LIGHT2);
    
    if (spot)
        glEnable(GL_LIGHT3);

    for (int i = 0; i < amount; i++)
    {
        drawKraan();
        glTranslatef(0, 0, 40);
    }

    if (draad)
        draw = GL_LINE;
    else
        draw = GL_FILL;
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_FOG);

    glutSwapBuffers();
}
/*
 * void drawKraan(): 
 * draw each part including some materials
 */
void drawKraan()
{
    if (on[0]){
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grijs[0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, grijs[1]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grijs[2]);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
        drawBalk();
    }
    if (on[1]){
        glMaterialfv ( GL_FRONT_AND_BACK , GL_AMBIENT , chroom[0]);
        glMaterialfv ( GL_FRONT_AND_BACK , GL_DIFFUSE , chroom[1]);
        glMaterialfv ( GL_FRONT_AND_BACK , GL_SPECULAR , chroom[2]);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
        drawVakwerk('a');
    }
    if (on[2])
        drawVakwerk('l');
    if (on[3])
        drawVakwerk('v');
    if (on[4])
        drawVakwerk('r');
    if (on[6])
        drawCylinderVast();
    if (on[7])
        drawCylinderLos();
    if (on[8])
        drawTopPart();   

}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Crane or something");
    init();

    glutDisplayFunc(displayFcn);
    glutKeyboardFunc(toetsPress);
    glutReshapeFunc(winReshapeFcn);
    glutTimerFunc(tijd, animFunc, delta);
    glutMainLoop();

    return 0;
}
/*
 * void toestPress(unsigned char toets, int x_muis, int y_muis): 
 * deal with all input
 */
void toetsPress(unsigned char toets, int x_muis, int y_muis)
{
    int* x;
    int* y;
    int* z;
    if (mode == 'a')
    {
        x = &x_cam_f;
        y = &y_cam_f;
        z = &z_cam_f;
    }
    else
    {
        x = &x_cam;
        y = &y_cam;
        z = &z_cam;
    }
    switch (toets)
    {
        case 'g':
            anim_hor = !anim_hor;
            break;
        case 'G':
            anim_ver = !anim_ver;
            break;
        case 'r':
            anim_rot = !anim_rot;
            break;
        case 'p':
            if (mode != 'a')
                mode = 'a';
                winReshapeFcn(winWidth, winHeight);
            break;
        case 'i':
            if (mode != 's')
                mode = 's';
                winReshapeFcn(winWidth, winHeight);
            break;
        case 'o':
            if (mode != 'o')
                mode = 'o';
                winReshapeFcn(winWidth, winHeight);
            break;
        case 'n':
            amount += 1;
            break;
        case 'N':
            if (amount > 1)
                amount -= 1;
            break;
        case 'x':
            *x += 1;
            break;
        case 'X':
            *x -= 1;
            break;
        case 'y':
            *y += 1;
            break;
        case 'Y':
            *y -= 1;
            break;
        case 'z':
            *z += 1;
            break;
        case 'Z':
            *z -= 1;
            break;
        case 'j':
            as = !as;
            break;
        case 'l':
            draad = !draad;
            break;
        case 'a':
            am_wit = !am_wit;
            break;
        case 'b':
            dig_glbl = !dig_glbl;
            break;
        case 'c':
            spec_ro = !spec_ro;
            break;
        case 'd':
            spot = !spot;
            break;
        case 'v':
            spotHoek += 1;
            break;
        case 'V':
            if (spotHoek > 0)
                spotHoek -= 1;
            break;
        case 'w':
            exp_ += 5;
            break;
        case 'W':
            if (exp_ > 0)
                exp_ -= 5;
            break;
        case 'e':
            shininess += 5;
            break;
        case 'E':
            if (shininess > 0)
                shininess -= 5;
            break;
        case 'h':
            spotHoogte += 1;
            break;
        case 'H':
            spotHoogte -= 1;
            break;
        case 'k':
            controle = !controle;
            break;
        case 's':
            smooth = !smooth;
            break;
        case 'f':
            doorzichtig = !doorzichtig;
            break;
        case 'm':
            mist = !mist;
            break;
        case 'M':
            exp_fog = !exp_fog;
            break;
        default:
            int x;
            x = ((int) toets) - 48;
            if (x < PARTS)
                on[x] = !on[x];
    }
    glutPostRedisplay();
}
/*
 * void drawBalk(): 
 * draw every balk
 */
void drawBalk()
{
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();

            glTranslatef(coo[i][0], coo[i][1], coo[i][2]);
            glScalef(1, HEIGHT, 1);

            glutSolidCube(1);

            glutWireCube(1);

        glPopMatrix();
    }
}
/*
 * void drawAssen(): 
 * draw every as
 */
void drawAssen()
{
    glBegin(GL_LINES);

        glColor3f(1, 0, 0);
        glVertex3d(0, 0, 0);
        glVertex3d(100, 0, 0);

        glColor3f(0, 1, 0);
        glVertex3d(0, 0, 0);
        glVertex3d(0, 100, 0);

        glColor3f(0, 0, 1);
        glVertex3d(0, 0, 0);
        glVertex3d(0, 0, 100);
    glEnd();
}
/*
 * void drawVakwerk(char vlak): 
 * depending on the 'vlak', it draws in the front-plane etc. using a simple rotation and-or translation
 */
void drawVakwerk(char vlak)
{
    glPushMatrix();
    switch (vlak)
    {
    case 'a':
        break;
    case 'v':
        glTranslatef(0, 0, WIDTH);
        break;
    case 'r':
        glRotatef(-90, 0, 1, 0);
        break;
    case 'l':
        glRotatef(-90, 0, 1, 0);
        glTranslatef(0, 0, -WIDTH);
        break;
    }

    glPushMatrix();
    glTranslatef(0, WIDTH/2, 0);
    glPushMatrix();
    for (int i = 0; i < HEIGHT; i += WIDTH*2)
    {
        glPushMatrix();

            glTranslatef(0, WIDTH/2, 0);
            glRotatef(45, 0, 0, 1);
            glTranslatef(0, -(WIDTH/2 + 2), 0);
            glScalef(1, WIDTH * 1.4, 1);

            glutSolidCube(1);

            glutWireCube(1);

        glPopMatrix();
        glPushMatrix();

            glTranslatef(0, WIDTH/2, 0);
            glRotatef(-45, 0, 0, 1);
            glTranslatef(0, WIDTH/2 + 2, 0);
            glScalef(1, WIDTH * 1.4, 1);

            glutSolidCube(1);

            glutWireCube(1);

        glPopMatrix();
        glTranslatef(0, WIDTH*2, 0);
    }
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}
/*
 * void drawCylinderVast(): 
 * draws cylinder that does not rotate
 */
void drawCylinderVast()
{
    int am = 20;
    glPushMatrix();

        glTranslatef(0, HEIGHT, 0);

        glPushMatrix();

            glTranslatef(WIDTH/2, 0, WIDTH/2);
            glBegin(GL_TRIANGLES);

                float hoek = 2 * M_PI / am;
                for (int i = 0; i < am; i++)
                {
                    glVertex3f(0, 0, 0);
                    glVertex3f(WIDTH * cos(hoek * i), 0, WIDTH * sin(hoek * i));
                    glVertex3f(WIDTH * cos(hoek * (i + 1)), 0, WIDTH * sin(hoek * (i + 1)));
                }

            glEnd();

        glPopMatrix();

        glRotatef(90, 1, 0, 0);
        glTranslatef(WIDTH/2, WIDTH/2, -HEIGHT_CYL);

        GLUquadric* cyl = gluNewQuadric();
        gluQuadricDrawStyle(cyl, GLU_FILL);

        gluCylinder(cyl, WIDTH, WIDTH, HEIGHT_CYL, am, 1);
        if (smooth)
        {
            gluQuadricNormals(cyl, GLU_SMOOTH);
        }
        else 
            gluQuadricNormals(cyl, GLU_FLAT);


    glPopMatrix();
}
/*
 * void drawCylinderLos(): 
 * draws cylinder that rotates with top part of crane.
 */
void drawCylinderLos()
{
    int am = 20;
    glPushMatrix();

        glTranslatef(0, HEIGHT+ HEIGHT_CYL, 0);
        glPushMatrix();

            glTranslatef(WIDTH/2, HEIGHT_CYL, WIDTH/2);
            glBegin(GL_TRIANGLES);

                float hoek = 2 * M_PI / am;
                for (int i = 0; i < am; i++)
                {
                    glVertex3f(0, 0, 0);
                    glVertex3f(WIDTH * cos(hoek * i), 0, WIDTH * sin(hoek * i));
                    glVertex3f(WIDTH * cos(hoek * (i + 1)), 0, WIDTH * sin(hoek * (i + 1)));
                }

            glEnd();

        glPopMatrix();
        glRotatef(90, 1, 0, 0);
        glTranslatef(WIDTH/2, WIDTH/2, -HEIGHT_CYL);

        glRotatef(angle, 0, 0, 1);

        GLUquadric* cyl = gluNewQuadric();
        gluQuadricDrawStyle(cyl, GLU_FILL);
        gluCylinder(cyl, WIDTH, WIDTH, HEIGHT_CYL, am, 1);
        if (smooth)
        {
            gluQuadricNormals(cyl, GLU_SMOOTH);
            printf("smooth as fuck boy");
        }
        else 
            gluQuadricNormals(cyl, GLU_FLAT);

    glPopMatrix();
}
/*
 * void drawTopPart(): 
 * draw everything of the top part, it is already being rotated with a certain angle 
 * and translated to be on top
 */
void drawTopPart()
{
    glPushMatrix();

        glTranslatef(WIDTH/2, HEIGHT+ HEIGHT_CYL*2, WIDTH/2);
        glRotatef(angle, 0, 1, 0);
        drawArm();
        drawBalDing();
        drawKabine();
        drawGewichten();

    glPopMatrix();
}
/*
 * void drawArm(): 
 * draws the vertical part of the crane, including a vakwerk
 */
void drawArm()
{
    glPushMatrix();
        glPushMatrix(); // bottom 1


            glTranslatef(40, 0, WIDTH/2);
            glScalef(100, 1, 1);
            glutSolidCube(1);

            glutWireCube(1);
        
        glPopMatrix();
        glPushMatrix(); // bottom 2


            glTranslatef(40, 0, -WIDTH/2);
            glScalef(100, 1, 1);
            glutSolidCube(1);

            glutWireCube(1);

        glPopMatrix();
        glPushMatrix(); // top 


            glTranslatef(42.5, WIDTH, 0);
            glScalef(85, 1, 1);
            glutSolidCube(1);

            glutWireCube(1);

        glPopMatrix();
        glPushMatrix(); // bottom connection front


            glTranslatef(90, 0, 0);
            glScalef(1, 1, WIDTH);
            glutSolidCube(1);

            glutWireCube(1);

        glPopMatrix();
        glPushMatrix(); // bottom 1 to top connection


            glTranslatef(87.5, WIDTH/2, 2);
            glRotatef(30, -1, 0, 1);
            glScalef(1, WIDTH*1.25, 1);
            glutSolidCube(1);

            glutWireCube(1);

        glPopMatrix();
        glPushMatrix(); // bottom 2 to top connection


            glTranslatef(87.5, WIDTH/2, -2);
            glRotatef(30, 1, 0, 1);
            glScalef(1, WIDTH*1.25, 1);
            glutSolidCube(1);

            glutWireCube(1);

        glPopMatrix();
    glPopMatrix();
    glPushMatrix();

        glTranslatef(15, 0, WIDTH/2);
        glRotatef(-30, 1, 0, 0);
        drawVakwerkArm();

    glPopMatrix();
    glPushMatrix();

        glTranslatef(15, 0, -WIDTH/2);
        glRotatef(30, 1, 0, 0);
        drawVakwerkArm();

    glPopMatrix();
}
/*
 * void drawVakwerkArm(): 
 * draws vakwerf of top part of crane
 */
void drawVakwerkArm()
{
    glPushMatrix();

        float x, y;
        int angle_;

        for (int i = 0; i < 7; i += 1)
        {
            y = WIDTH/2 + 0.5;
            x = WIDTH/2 + 1;
            if (i%2 == 0)
            {
                angle_ = -45;
            }           
            else
            {
                angle_ = 45;
            }
            glPushMatrix();

                glTranslatef(x, y, 0);
                glRotatef(angle_, 0, 0, 1);
                glScalef(1, WIDTH*1.6, 1);

                glutSolidCube(1);

                glutWireCube(1);

            glPopMatrix();
            glTranslatef(WIDTH, 0, 0);

        }

    glPopMatrix();
}
/*
 * void drawBalDing(): 
 * draws the last + rope + little part that holds it
 */
void drawBalDing()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, geel[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, geel[1]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, geel[2]);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glPushMatrix();
        glTranslatef(afstand_bal, 0, 0);
        glPushMatrix();

            glScalef(WIDTH, 1, WIDTH);

            glutSolidCube(1); // ophangingsbalkje

        glPopMatrix();
        glPushMatrix();

            GLUquadric* cyl;
            cyl = gluNewQuadric();
            gluQuadricDrawStyle(cyl, GLU_LINE);
            glRotatef(90, 1, 0, 0);

            gluCylinder(cyl, 0.2, 0.2, length_kabel, 20, 1); // kabel naar bol

        glPopMatrix();
        glPushMatrix();
            
            glTranslatef(0, -length_kabel, 0);
            // glutSolidSphere(bol_dia , 20, 20);
            // draw complex B-spline surface

            GLfloat knot[8] = {0, 0, 0, 0, 1, 1, 1, 1};

            GLfloat ctlpoints[4][4][3] = {
                {{-3, -3, -3}, {-3, -1, -3}, {-3, 1, -3}, {-3, 3, -3}},
                {{-2, -3, -3}, {-2, -1, -1}, {-2, 1, -1}, {-2, 3, -3}},
                {{-1, -3, -3}, {-1, -1, 1}, {-1, 1, 1}, {-1, 3, -3}},
                {{0, -3, -3}, {0, -1, 3}, {0, 1, 3}, {0, 3, -3}}
            };

            GLUnurbsObj *oppNaam = gluNewNurbsRenderer();
            gluBeginSurface(oppNaam);
            gluNurbsSurface(oppNaam, 8, knot, 8, knot, 4*3, 3, &ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
            gluEndSurface(oppNaam);
            gluDeleteNurbsRenderer(oppNaam);

        glPopMatrix();
    glPopMatrix();
}
/*
 * void drawKabine(): 
 * draws everything from the kabine, including a glass front panel
 */
void drawKabine()
{

    // spot rotatie geven etc.
    glPushMatrix();
    
        GLfloat pos4[] = {0, spotHoogte, 0, 1};
        
        GLfloat richting[] = {afstand_bal - 10, -(length_kabel + 16), 0}; // xy-richting
        glTranslatef(10, 16, -10);
        glLightfv(GL_LIGHT3, GL_POSITION, pos4);
        glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, spotHoek);
        glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, richting);
        glLightf(GL_LIGHT3, GL_SPOT_EXPONENT,exp_);

    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, wit[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, wit[1]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, wit[2]);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glPushMatrix(); // front panel of kabine

        if (doorzichtig)
        {
            glEnable(GL_BLEND);
            glDepthMask(GL_FALSE);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        glTranslatef(10, 0, -15);

        if (controle)
        {
            glBegin(GL_LINE_STRIP);

            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 4; j++)
                {
                    glVertex3fv(ctrlpoints[i][j]);
                }

            glEnd();
        }
        
        glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 6, &ctrlpoints[0][0][0]);
        glEnable(GL_MAP2_VERTEX_3);

        glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
        glEvalMesh2(draw, 0, 20, 0, 20);
        if (draad)
        {
            glEvalMesh2(GL_LINE, 0, 20, 0, 20);
        }
        if (doorzichtig)
        {
            glDisable(GL_MAP2_VERTEX_3);
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
        }
    
    glPopMatrix();
    glPushMatrix(); // top plane that connects with front


        glTranslatef(10, 15, -15);
        glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4, &controlPoints[0][0][0]);
        glEnable(GL_MAP2_VERTEX_3);

        glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
        glEvalMesh2(draw, 0, 20, 0, 20);
        glDisable(GL_MAP2_VERTEX_3);

    glPopMatrix();
    glPushMatrix(); // bottom plane that connects with front


        glTranslatef(10, 0, -15);
        glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4, &controlPoints[0][0][0]);
        glEnable(GL_MAP2_VERTEX_3);

        glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
        glEvalMesh2(draw, 0, 20, 0, 20);
        if (draad)
        {

            glEvalMesh2(GL_LINE, 0, 20, 0, 20);
        }
        glDisable(GL_MAP2_VERTEX_3);

    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grijs[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, grijs[1]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grijs[2]);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glPushMatrix(); // right panel of kabine

        glTranslatef(5, 7.5, -15);
        glScalef(10, 15, 1);

        glutSolidCube(1);

    glPopMatrix();
    glPushMatrix(); // left panel of kabine

        glTranslatef(5, 7.5, 0);
        glScalef(10, 15, 1);

        glutSolidCube(1);

    glPopMatrix();
    
    glPushMatrix(); // rest of top plane


        glTranslatef(5, 15, -7.5);
        glRotatef(90, 1, 0, 0);
        glRectf(-5, -7.5, 5, 7.5);

    glPopMatrix();
    
    glPushMatrix(); // rest of bottom plane

        glTranslatef(5, 0, -7.5);
        glRotatef(90, 1, 0, 0);
        glRectf(-5, -7.5, 5, 7.5);

    glPopMatrix();
    glPushMatrix(); // back plane


        glTranslatef(0, 0, -7.5);
        glRotatef(90, 0, 1, 0);
        glRectf(-7.5, 0, 7.5, 15);

    glPopMatrix();
    // spot rotatie geven etc.
    glPushMatrix();
    
        GLfloat pos4[] = {0, spotHoogte, 0, 1};
        
        GLfloat richting[] = {1, -1, 0}; // xy-richting
        glTranslatef(10, 16, -10);
        glLightfv(GL_LIGHT3, GL_POSITION, pos4);
        glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, spotHoek);
        glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, richting);
        glLightf(GL_LIGHT3, GL_SPOT_EXPONENT,exp_);

    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, wit[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, wit[1]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, wit[2]);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glPushMatrix(); // front panel of kabine

        if (doorzichtig)
        {
            glEnable(GL_BLEND);
            glDepthMask(GL_FALSE);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        glTranslatef(10, 0, -15);

        if (controle)
        {
            glBegin(GL_LINE_STRIP);

            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 4; j++)
                {
                    glVertex3fv(ctrlpoints[i][j]);
                }

            glEnd();
        }
        
        glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 6, &ctrlpoints[0][0][0]);
        glEnable(GL_MAP2_VERTEX_3);

        glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
        glEvalMesh2(draw, 0, 20, 0, 20);
        if (draad)
        {
            glEvalMesh2(GL_LINE, 0, 20, 0, 20);
        }
        if (doorzichtig)
        {
            glDisable(GL_MAP2_VERTEX_3);
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
        }
    
    glPopMatrix();
}
/*
 * void drawGewichten(): 
 * draws 3 weights including bottom support
 */
void drawGewichten()
{
    glPushMatrix();


        glTranslatef(-15, 0, 0);
        glScalef(30, 1, WIDTH);
        glutSolidCube(1);

    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, lila[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, lila[1]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, lila[2]);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    for (int i = 1; i <= 3; i++)
    {
        glPushMatrix(); // front panel of kabine


            glTranslatef(-10 * i, 0, -7.5);
            if (controle)
            {
                glBegin(GL_LINE_STRIP);

                for (int i = 0; i < 6; i++)
                    for (int j = 0; j < 4; j++)
                    {
                        glVertex3fv(ctrlpoints[i][j]);
                    }

                glEnd();
            }
            glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 6, &ctrlpoints[0][0][0]);
            glEnable(GL_MAP2_VERTEX_3);

            glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
            glEvalMesh2(draw, 0, 20, 0, 20);
            
            glDisable(GL_MAP2_VERTEX_3);
        
        glPopMatrix();
        glPushMatrix(); // top plane that connects with front

            glTranslatef(-10*i, 15, -7.5);
            glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4, &controlPoints[0][0][0]);
            glEnable(GL_MAP2_VERTEX_3);

            glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
            glEvalMesh2(draw, 0, 20, 0, 20);
            
            glDisable(GL_MAP2_VERTEX_3);

        glPopMatrix();
        glPushMatrix(); // bottom plane that connects with front

            glTranslatef(-10*i, 0, -7.5);
            glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 12, 4, &controlPoints[0][0][0]);
            glEnable(GL_MAP2_VERTEX_3);

            glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
            glEvalMesh2(draw, 0, 20, 0, 20);
            
            glDisable(GL_MAP2_VERTEX_3);

        glPopMatrix();
        glPushMatrix();

            glTranslatef(-10*i, 0, 0);
            glRotatef(90, 0, 1, 0);
            glRectf(-7.5, 0,7.5, 15);

        glPopMatrix();
    }
    
}
/*
 * void animFunc(): 
 * function for animating the rotation, translation in horizontal and vertical way
 */
void animFunc(int delta)
{
    if (anim_hor)
    {
        if (right)
        {
            if (afstand_bal < HEIGHT - (2*WIDTH))
                afstand_bal += 0.5;
            else
                right = !right;
        }
        else
        {
            if (afstand_bal > WIDTH*2)
                afstand_bal -= 0.5;
            else 
                right = !right;
        }
    }
    if (anim_ver)
    {
        if (up)
        {
            if (length_kabel < HEIGHT)
                length_kabel += 0.5;
            else 
                up = !up;
        }
        else 
        {
            if (length_kabel > bol_dia)
                length_kabel -= 0.5;
            else
                up = !up;
        }
    }
    if (anim_rot)
    {
        angle += 1;
    }
    glutTimerFunc(tijd, animFunc, delta);
    glutPostRedisplay();
}