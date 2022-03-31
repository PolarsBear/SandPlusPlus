#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <sstream>
#include "particle_table.h"
using namespace std;

const int MOUSE_LEFT = 0;
const int MOUSE_RIGHT = 2;
const int MOUSE_MIDDLE = 1;
const int MOUSE_SCROLL_UP = 3;
const int MOUSE_SCROLL_DOWN = 4;
int window;

int SIZE = 720;
int PARTICLE_SIZE = 10;
int PARTICLE_AMOUNT = SIZE/PARTICLE_SIZE;

float TICK_SPEED = 48;
double MS_P_TICK = 1000.0*(1.0/TICK_SPEED);

GLubyte* buffer = new GLubyte[SIZE*SIZE*3];

int frame=0,timeNow,timeBase=0;
int tickCounter=0,tickTimeNow,tickTimeBase=0;

float fps;
float tps;

tuple<uint,uint,bool,Particle (*)(uint,uint)> click;

ParticleTable * particles = new ParticleTable(PARTICLE_AMOUNT);

void text(float x, float y, void* font, string text)
{
  glColor3f(1.0f,1.0f,1.0f);
  glRasterPos2f(x, y);
  int len, i;
  len = (int)text.length();
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, text[i]);
  }
}

void makePixel(int x, int y, Color3 color)
{
    int position = (x + y * SIZE) * 3;
    buffer[position] = color.R*255;
    buffer[position + 1] = color.G*255;
    buffer[position + 2] = color.B*255;
}

void drawParticle(uint x, uint y, Color3 color) {
    if (x < 0 || x > 71 || y < 0 || y > 71) {return;}
    uint x1 = x * PARTICLE_SIZE;
    uint y1 = y * PARTICLE_SIZE;
    for (uint x2 = x1; x2 < x1 + PARTICLE_SIZE; x2++) {
        for (uint y2 = y1; y2 < y1 + PARTICLE_SIZE; y2++) {
            makePixel(x2,y2,color);
        }
    }
}

void Particle::draw() {
    drawParticle(x, y, color);
}



bool Particle::fall() {
    if (y > 0) {
        Particle p;
        if ((p = particles->get(x,y-1)).density < density) {
            (*particles).set(x,y,p);
            (*particles).set(x,y-1,*this);
            return true;
        }
        else if (x < PARTICLE_AMOUNT-1 && (p = particles->get(x+1,y-1)).density < density)
        {
            (*particles).set(x,y,p);
            (*particles).set(x+1,y-1,*this);
            return true;
        }
        else if (x > 0 && (p = particles->get(x-1,y-1)).density < density)
        {
            (*particles).set(x,y,p);
            (*particles).set(x-1,y-1,*this);
            return true;
        }
        
    }
    return false;
}

bool Particle::liquidSpread() {
    int direction = (rand() % 5) - 2;
    if (x + direction > 0 && x + direction < PARTICLE_AMOUNT && particles->get(x+direction,y).id == 0)
    {
        this->x+=direction;
        (*particles).set(x,y,*this);
        (*particles).set(x-direction,y,Air(x-direction,y));
        return true;
    }
    return false;
}

void Particle::update() {
    switch (this->id)
    {
    case 0:
        break;
    
    case 2: {
        if (!fall()) {liquidSpread();}
        break;
    }

    default:
        fall();
        break;
    }
    
}

/*void ParticleTable::draw() { 
    for (auto& a : table) {
        auto v = a.second;
        if (v.name != "air") {
            v.draw();
        }
    }

    
}*/

void draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3d(1.0,1.0,1.0);
    
    //(*particles).draw();
    glDrawPixels(SIZE,SIZE,GL_RGB, GL_UNSIGNED_BYTE, buffer);
    
    text(-1.0f, 0.95f, GLUT_BITMAP_HELVETICA_12,"FPS: " + to_string(int(round(fps))));
    text(-1.0f, 0.9f,GLUT_BITMAP_HELVETICA_12, "TPS: " + to_string(int(round(tps))) + "/" + to_string(int(TICK_SPEED)));
    glRasterPos2f(-1, -1);

    glFlush();
}

void tick() {
    tickTimeNow=glutGet(GLUT_ELAPSED_TIME);
    tps = 1000.0/(tickTimeNow-tickTimeBase);
    tickTimeBase = tickTimeNow;

    (*particles).update();
    glutPostWindowRedisplay(window);
}

void idle() {
    frame++;
	timeNow=glutGet(GLUT_ELAPSED_TIME);

	if (timeNow - timeBase > MS_P_TICK) {
		fps = frame*1000.0/(timeNow-timeBase);
	 	timeBase = timeNow;
		frame = 0;

        tick();
	}
    if (get<2>(click)) {
        uint x=get<0>(click), y=get<1>(click);
        (*particles).set(x, y, get<3>(click)(x, y));
    }
}

void onMouse(int button, int state, int x, int y) {
    if (x > SIZE || x < 0 || y > SIZE || y < 0) {return;}
    uint x1 = round(x/PARTICLE_SIZE);
    float y1 = y/PARTICLE_SIZE;
    y1 = y1 / PARTICLE_AMOUNT - 0.5;
    y1 = round((-y1 + 0.5) * PARTICLE_AMOUNT) - 1;
    auto p = Particle::Air;
    switch (button) {
        case MOUSE_LEFT: {
            p = Particle::Sand;
            break;
        }
        case MOUSE_RIGHT: {
            p = Particle::Water;
            break;
        }
        default: {
            break;
        }
    }
    click = tuple<uint,uint,bool,Particle (*)(uint,uint)>(x1,y1,state==GLUT_DOWN,p);
    
    (*particles).set(x1, y1, p(x1,y1));
    glutPostWindowRedisplay(window);
}

void onMotion(int x, int y) {
    if (get<2>(click)) {
        if (x > SIZE || x < 0 || y > SIZE || y < 0) {return;}
        uint x1 = round(x/PARTICLE_SIZE);
        float y1 = y/PARTICLE_SIZE;
        y1 = y1 / PARTICLE_AMOUNT - 0.5;
        y1 = round((-y1 + 0.5) * PARTICLE_AMOUNT) - 1;
        click = tuple<uint,uint,bool,Particle (*)(uint,uint)>(x1,y1,true,get<3>(click));
    
        (*particles).set(x1, y1, get<3>(click)(x1,y1));
        glutPostWindowRedisplay(window);

    }
}

void resize(int, int) {
    glutReshapeWindow(SIZE,SIZE);
}

int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; ++i) {
        if (string(argv[i]).find("-tps=", 0) == 0) {
            TICK_SPEED = stof(string(argv[i]).erase(0,5));
            MS_P_TICK = 1000.0*(1.0/TICK_SPEED);
        }
        else if (string(argv[i]).find("-size=", 0) == 0)
        {
            SIZE = stof(string(argv[i]).erase(0,6));
            buffer = new GLubyte[SIZE*SIZE*3];
            particles = new ParticleTable(PARTICLE_AMOUNT);
        }
        
    } 

    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(SIZE, SIZE);
    glutInitWindowPosition(100, 100);
    window = glutCreateWindow("Sand++");

    glutMouseFunc(onMouse);
    glutMotionFunc(onMotion);
    glutDisplayFunc(draw);
    glutIdleFunc(idle);
    //glutReshapeFunc(resize);
    
    //glutTimerFunc(MS_P_TICK, tick, 0);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glutMainLoop();
    return 0;
}