#include "include/Angel.h"
#include <cmath>
#include <vector>
#include <sstream>
#include <ctime>
#include <string.h>
#include <fstream>


#include "drawable.h"
#include "circle.h"
#include "square.h"
#include "Kart.h"

#ifndef M_PI
#  define M_PI  3.14159265358979323846
#endif

using namespace std;

const GLfloat WindowWidth = 900;
const GLfloat WindowHeight = 600;
int frameCount;
clock_t lastUpdateTime, startTime, pauseTime, pauseStartTime;
float nextScale,nextSpeed, simSpeed;
char nextLane;
int nextTime,filePos = 0, accidents = 0,totalKartNum = 0;
string fileContent;
bool paused;
vec2 points[4];
//some basic colors
color3 blue  = color3(0,0,1);
color3 red   = color3(1,0,0);
color3 circlegreen = color3(0,1,0);
color3 black = color3(0,0,0);
color3 maginta = color3(1,0,1);
color3 grey = color3(.4,.4,.4);
color3 white = color3(1,1,1);
color3 green = color3(0,.7,0);


vector<Drawable*> shapes;
vector<Drawable*> kartShapes;
vector<Kart*> karts;
vector<string> kartData;

//----------------------------------------------------------------------------
color3 getRandomColor(){
	color3 color = color3(rand() / double(RAND_MAX), rand() / double(RAND_MAX), rand() / double(RAND_MAX));
	return color;
}
//----------------------------------------------------------------------------
void readNextKartInfo(){
	std::ifstream infile("traffic_flow.txt",ifstream::binary);
	std::string line;
	infile.seekg(filePos);
	getline(infile, line);
	std::istringstream iss(line);
	if (iss >> nextScale >> nextTime >> nextSpeed >> nextLane)
	{
		filePos = infile.tellg();
	}
}
//----------------------------------------------------------------------------
void updateWorld(){
	if(!paused){
		kartShapes.clear();
		float dt = (float)(clock()-lastUpdateTime)/CLOCKS_PER_SEC;
		if((float)(clock() - startTime - pauseTime) * simSpeed >= nextTime){
			// Time to insert a new kart
			totalKartNum++;
			Kart* kart = new Kart(nextScale,nextSpeed,nextLane,getRandomColor(), totalKartNum++);
			karts.push_back(kart);
			readNextKartInfo();
			
		}
		for(int i=0; i<karts.size(); i++){
			if(karts[i]->getDelete())
				karts.erase(karts.begin() + i);
			else{
				for(int j=0; j<i; j++){
					if(!(karts[i]->getCollision() || karts[j]->getCollision())){
						if(karts[i]->checkCollision(karts[j])){
							karts[i]->setCollision();
							karts[j]->setCollision();
							accidents++;
							printf("Accident between car %d and car %d at %d ms\n", karts[i]->getId(), karts[j]->getId(), lastUpdateTime);
							break;
						}
					}
				}
				kartShapes.push_back(karts[i]->update(dt*simSpeed));
			}
		}
	}
	else
		pauseTime = clock() - pauseStartTime;

	lastUpdateTime = clock();
}
//----------------------------------------------------------------------------
void applyTransform(mat3 m){
	vec3 temp;
	for(int i=0; i<4; i++){
		temp = m * vec3(points[i],1.0);
		points[i].x = temp.x;
		points[i].y = temp.y;
	}
}
//----------------------------------------------------------------------------
void
build_world(){

    //Simulation circle
	vec2 center = vec2(-400,250);
    Drawable* circle = new FilledCircle(center,30,30,circlegreen);
    shapes.push_back(circle);

	//Draw track(ellipse)
	//1st. Draw outside circle:
	center = vec2(-140,0);
	circle = new FilledCircle(center,275,50,grey);
	shapes.push_back(circle);
	
	//2nd. Draw inside circle:
	center = vec2(-140,0);
	circle = new FilledCircle(center,200,50,green);
	shapes.push_back(circle);

	//3rd. Remove right side of circle by drawing a square over it:
	vec2 a = vec2(-140,275); vec2 b = vec2(450,275);
    vec2 c = vec2(-140,-275); vec2 d = vec2(450,-275);
    Drawable* square = new FilledSquare(a,b,c,d,green);
    shapes.push_back(square);

	//4th. Draw the rest of the track:
	a = vec2(-140,275); b = vec2(450,275);
	c = vec2(-140,200); d = vec2(450,200);
	square = new FilledSquare(a,b,c,d,grey);
	shapes.push_back(square);
	a = vec2(-140,-275); b = vec2(450,-275);
	c = vec2(-140,-200); d = vec2(450,-200);
	square = new FilledSquare(a,b,c,d,grey);
	shapes.push_back(square);

	//Draw lane separators:
	a = vec2(440,-240); b = vec2(425,-240);
	c = vec2(440,-235); d = vec2(425,-235);
	vec2 delta = 2*(a - b);
	
	while(a.x>-140){
		square = new FilledSquare(a,b,c,d,white);
		shapes.push_back(square);
		a.y = -a.y; b.y = -b.y; c.y = -c.y; d.y = -d.y;
		square = new FilledSquare(a,b,c,d,white);
		shapes.push_back(square);
		a -= delta; b -= delta; c -= delta; d -= delta;
	}

	points[0] = a + delta;
	points[1] = b + delta;
	points[2] = c + delta;
	points[3] = d + delta;
	double angle = (delta.x/(237.5));
	float deltaX = 140.0;
	mat3 m,tm;
	m[0][2] = 0.0f; m[1][2] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;

	while(points[0].x <= a.x + delta.x)
	{
		
		tm[0][0] = 1.0f; tm[0][1] = 0.0f; tm[0][2] = deltaX;
		tm[1][0] = 0.0f; tm[1][1] = 1.0f; tm[1][2] = 0.0f;
		tm[2][0] = 0.0f; tm[2][1] = 0.0f; tm[2][2] = 1.0f;
		applyTransform(tm);

		m[0][0] = cos(angle); m[0][1] = sin(angle);
		m[1][0] = -sin(angle); m[1][1] = cos(angle);
		applyTransform(m);
		
		tm[0][2] = -deltaX;
		applyTransform(tm);

		Drawable* laneBreak = new FilledSquare(points[0], points[1], points[2], points[3],white);
		shapes.push_back(laneBreak);
	}
	startTime = clock();
	lastUpdateTime = startTime;
	readNextKartInfo();
	paused = false;
	simSpeed = 1;
}

//-----------------------------------------------------------
void
cleanup() {
    //destroy shaders
    glUseProgram(0);
    glDeleteProgram(Drawable::shaderProgram);
    //destroy shapes and its vertices
    for (int i=0;i<shapes.size();i++){
        delete shapes[i];
    }
}

//-----------------------------------------------------------
void
init( void )
{
    // Load shaders and use the resulting shader program
    Drawable::shaderProgram = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( Drawable::shaderProgram );

    build_world();

    glClearColor( 0, .7, 0, 1.0);
}

//-----------------------------------------------------------

void
display( void )
{
	frameCount++;	
	glClear( GL_COLOR_BUFFER_BIT );     // clear the window
	updateWorld();
	for (int i=0;i<shapes.size();i++){
		shapes[i]->render();
	}
	for(int i=0; i<kartShapes.size(); i++){
		kartShapes[i]->render();
	}
	glFlush();
}

//----------------------------------------------------------------------------
void timerFunc(int value) {

    if (value != 0){ //not the first call
        stringstream ss;
        ss << "FPS: " << frameCount  * 4;
        glutSetWindowTitle(ss.str().c_str());
    }

    //reset frame count to 0
    frameCount = 0;
    //call the function again after 250 milli-seconds
    glutTimerFunc ( 250, timerFunc, 1 );
}
//-----------------------------------------------------------------------------
void toggleSimCircle(){
	vec2 center = vec2(-400,250);
    Drawable* circle = new FilledCircle(center,30,30,paused? red : circlegreen);
    shapes.push_back(circle);
}
//-----------------------------------------------------------------------------
void printPauseInfo(){
	if(paused)
		printf("Simulation time: %d\nNumber of cars: %d\nNumber of accidents: %d\nRate of accidents: %f\n",
			lastUpdateTime, karts.size(), accidents, accidents/lastUpdateTime);
}
//-----------------------------------------------------------------------------
void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
	case 32:
		paused = !paused;
		pauseStartTime = clock();
		toggleSimCircle();
		printPauseInfo();
		break;
    }
}

//----------------------------------------------------------------------------
void keyboardSpecial( int key, int x, int y )
{
    //Example on how keyboard handler function can be implemented
    switch ( key ) {
        case GLUT_KEY_UP:
            simSpeed *= 2;
			break;
        case GLUT_KEY_DOWN:
            simSpeed *= .5;
			break;;
    }
}
//----------------------------------------------------------------------------
void idle() {
    glutPostRedisplay();
}
//----------------------------------------------------------------------------
int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( WindowWidth, WindowHeight);

    glutCreateWindow( "lab1: Shapes and primitives" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	glutSpecialFunc( keyboardSpecial );
    glutCloseFunc(cleanup);
	glutIdleFunc( idle );
	glutTimerFunc( 250, timerFunc, 0 );

    glutMainLoop();
    return 0;
}
