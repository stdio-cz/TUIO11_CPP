/*
 TUIO C++ GUI Demo
 Copyright (c) 2005-2017 Martin Kaltenbrunner <martin@tuio.org>
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "TuioDemo.h"

static bool _verbose = false;
static bool _fullscreen = false;
static std::string _address("localhost");
static bool _udp = true;
static int _port = 3333;

void TuioDemo::addTuioObject(TuioObject *tobj) {
	if (verbose)
		std::cout << "add obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << "/"<<  tobj->getTuioSourceID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() << std::endl;
}

void TuioDemo::updateTuioObject(TuioObject *tobj) {
	
	if (verbose)
		std::cout << "set obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << "/"<<  tobj->getTuioSourceID() << ") "<< tobj->getX() << " " << tobj->getY() << " " << tobj->getAngle() 
		<< " " << tobj->getMotionSpeed() << " " << tobj->getRotationSpeed() << " " << tobj->getMotionAccel() << " " << tobj->getRotationAccel() << std::endl;
}

void TuioDemo::removeTuioObject(TuioObject *tobj) {
	
	if (verbose)
		std::cout << "del obj " << tobj->getSymbolID() << " (" << tobj->getSessionID() << "/"<<  tobj->getTuioSourceID() << ")" << std::endl;
}

void TuioDemo::addTuioCursor(TuioCursor *tcur) {
	
	if (verbose)
		std::cout << "add cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << "/"<<  tcur->getTuioSourceID() << ") " << tcur->getX() << " " << tcur->getY() << std::endl;
}

void TuioDemo::updateTuioCursor(TuioCursor *tcur) {
	
	if (verbose)
		std::cout << "set cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << "/"<<  tcur->getTuioSourceID() << ") " << tcur->getX() << " " << tcur->getY() 
		<< " " << tcur->getMotionSpeed() << " " << tcur->getMotionAccel() << " " << std::endl;
}

void TuioDemo::removeTuioCursor(TuioCursor *tcur) {
	
	if (verbose)
		std::cout << "del cur " << tcur->getCursorID() << " (" <<  tcur->getSessionID() << "/"<<  tcur->getTuioSourceID() << ")" << std::endl;
}

void TuioDemo::addTuioBlob(TuioBlob *tblb) {
	
	if (verbose)
		std::cout << "add blb " << tblb->getBlobID() << " (" << tblb->getSessionID()  << "/"<<  tblb->getTuioSourceID()<< ") "<< tblb->getX() << " " << tblb->getY() << " " << tblb->getAngle() << " " << tblb->getWidth() << " " << tblb->getHeight() << " " << tblb->getArea() << std::endl;
}

void TuioDemo::updateTuioBlob(TuioBlob *tblb) {
	
	if (verbose)
		std::cout << "set blb " << tblb->getBlobID() << " (" << tblb->getSessionID() << "/"<<  tblb->getTuioSourceID() << ") "<< tblb->getX() << " " << tblb->getY() << " " << tblb->getAngle() << " "<< tblb->getWidth() << " " << tblb->getHeight() << " " << tblb->getArea() 
		<< " " << tblb->getMotionSpeed() << " " << tblb->getRotationSpeed() << " " << tblb->getMotionAccel() << " " << tblb->getRotationAccel() << std::endl;
}

void TuioDemo::removeTuioBlob(TuioBlob *tblb) {
	
	if (verbose)
		std::cout << "del blb " << tblb->getBlobID() << " (" << tblb->getSessionID() << "/"<<  tblb->getTuioSourceID() << ")" << std::endl;
}


void TuioDemo::refresh(TuioTime /*frameTime*/) {
	//drawObjects();
}


void TuioDemo::drawObjects() {
	glClear(GL_COLOR_BUFFER_BIT);
	char id[16];

	// draw the cursors
	std::list<TuioCursor*> cursorList = tuioClient->getTuioCursors();
	tuioClient->lockCursorList();
	for (std::list<TuioCursor*>::iterator iter = cursorList.begin(); iter!=cursorList.end(); iter++) {
		TuioCursor *tuioCursor = (*iter);
		std::list<TuioPoint> path = tuioCursor->getPath();
		if (path.size()>0) {
			
			TuioPoint last_point = path.front();
			glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 1.0);
			
			for (std::list<TuioPoint>::iterator point = path.begin(); point!=path.end(); point++) {
				glVertex3f(last_point.getScreenX(width), last_point.getScreenY(height), 0.0f);
				glVertex3f(point->getScreenX(width), point->getScreenY(height), 0.0f);
				last_point.update(point->getX(),point->getY());
			} glEnd();
			
			// draw the finger tip
			glColor3f(0.75, 0.0, 0.75);
			glPushMatrix();
			glTranslatef(last_point.getScreenX(width), last_point.getScreenY(height), 0.0);
			glBegin(GL_TRIANGLE_FAN);
			for(double a = 0.0f; a <= 2*M_PI; a += 0.2f) {
				glVertex2d(cos(a) * height/100.0f, sin(a) * height/100.0f);
			} glEnd();
			glPopMatrix();
			
			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(tuioCursor->getScreenX(width),tuioCursor->getScreenY(height));
			sprintf(id,"%d",tuioCursor->getCursorID());
			drawString(id);
		}
	}
	tuioClient->unlockCursorList();
	
	// draw the objects
	std::list<TuioObject*> objectList = tuioClient->getTuioObjects();
	tuioClient->lockObjectList();
	for (std::list<TuioObject*>::iterator iter = objectList.begin(); iter!=objectList.end(); iter++) {
		TuioObject *tuioObject = (*iter);
		int pos_size = height/20.0f;
		int neg_size = -1*pos_size;
		float xpos  = tuioObject->getScreenX(width);
		float ypos  = tuioObject->getScreenY(height);
		float angle = tuioObject->getAngleDegrees();
		
		glColor3f(0.25, 0.0, 0.0);
		glPushMatrix();
		glTranslatef(xpos, ypos, 0.0);
		glRotatef(angle, 0.0, 0.0, 1.0);
		glBegin(GL_QUADS);
		glVertex2f(neg_size, neg_size);
		glVertex2f(neg_size, pos_size);
		glVertex2f(pos_size, pos_size);
		glVertex2f(pos_size, neg_size);
		glEnd();
		glPopMatrix();
		
		glColor3f(1.0, 1.0, 1.0);
		glRasterPos2f(xpos,ypos+5);
		sprintf(id,"%d",tuioObject->getSymbolID());
		drawString(id);
	}
	tuioClient->unlockObjectList();
	
	// draw the blobs
	std::list<TuioBlob*> blobList = tuioClient->getTuioBlobs();
	tuioClient->lockBlobList();
	for (std::list<TuioBlob*>::iterator iter = blobList.begin(); iter!=blobList.end(); iter++) {
		TuioBlob *tuioBlob = (*iter);
		float blob_width = tuioBlob->getScreenWidth(width)/2;
		float blob_height = tuioBlob->getScreenHeight(height)/2;
		float xpos  = tuioBlob->getScreenX(width);
		float ypos  = tuioBlob->getScreenY(height);
		float angle = tuioBlob->getAngleDegrees();
		
		glColor3f(0.25, 0.25, 0.25);
		glPushMatrix();
		glTranslatef(xpos, ypos, 0.0);
		glRotatef(angle, 0.0, 0.0, 1.0);
		
		/*glBegin(GL_QUADS);
		 glVertex2f(blob_width/-2, blob_height/-2);
		 glVertex2f(blob_width/-2, blob_height/2);
		 glVertex2f(blob_width/2, blob_height/2);
		 glVertex2f(blob_width/2, blob_height/-2);
		 glEnd();*/
		
		glBegin(GL_TRIANGLE_FAN);
		for(double a = 0.0f; a <= 2*M_PI; a += 0.2f) {
			glVertex2d(cos(a) * blob_width, sin(a) * blob_height);
		} glEnd();
		
		glPopMatrix();
		
		glColor3f(1.0, 1.0, 1.0);
		glRasterPos2f(xpos,ypos+5);
		sprintf(id,"%d",tuioBlob->getBlobID());
		drawString(id);
	}
	tuioClient->unlockBlobList();
	
	SDL_GL_SwapWindow(window);
}

void TuioDemo::drawString(char *str) {
	if (str && strlen(str)) {
		while (*str) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *str);
			str++;
		}
	}
}

void TuioDemo::initWindow() {
	
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0, &mode);
	screen_width = mode.w;
	screen_height= mode.h;
	
	int videoFlags = SDL_WINDOW_OPENGL;
	if( fullscreen ) {
		videoFlags |= SDL_WINDOW_FULLSCREEN;
		width = screen_width;
		height = screen_height;
	} else {
		width = window_width;
		height = window_height;
	}
	
	SDL_CreateWindowAndRenderer(width, height, videoFlags, &window, &renderer);

	if ( window == NULL ) {
		std::cerr << "Could not open window: " << SDL_GetError() << std::endl;
		SDL_Quit();
	}
	
	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!context) {
		fprintf(stderr, "Couldn't create context: %s\n", SDL_GetError());
		return;
	}

	SDL_ShowCursor(!fullscreen);
	glClearColor(0.0, 0.0, 0.25, 0.0);
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, (GLfloat)width, (GLfloat)height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void TuioDemo::processEvents()
{
    SDL_Event event;
	
    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {
			case SDL_KEYDOWN:
				if (event.key.repeat) continue;
				else if( event.key.keysym.sym == SDLK_ESCAPE ){
					running = false;
					SDL_ShowCursor(true);
					SDL_Quit();
				} else if( event.key.keysym.sym == SDLK_F1 ){
					if(fullscreen)
					{
						width = window_width;
						height = window_height;
						SDL_SetWindowFullscreen(window, SDL_FALSE);
						SDL_ShowCursor(1);
						fullscreen = false;
					}
					else
					{
						SDL_ShowCursor(0);
						width = screen_width;
						height = screen_height;
						SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
						fullscreen = true;
					}
					
					glClearColor(0.0, 0.0, 0.25, 0.0);
					glViewport(0, 0, (GLint)width, (GLint)height);
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					gluOrtho2D(0, (GLfloat)width, (GLfloat)height, 0);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					
				} else if( event.key.keysym.sym == SDLK_v ){
					verbose = !verbose;
				}
				
				break;
			case SDL_QUIT:
				running = false;
				SDL_ShowCursor(true);
				SDL_Quit();
        }
    }
}

TuioDemo::TuioDemo(const char *host, int port, bool udp, bool verb, bool full)
: screen_width (1024)
, screen_height (768)
, window_width (640)
, window_height (480)
{	
	verbose = verb;
	fullscreen = full;
	
	if (udp) osc_receiver = new UdpReceiver(port);
	else {
		if (strcmp(host,"incoming")==0) osc_receiver = new TcpReceiver(port);
		else osc_receiver = new TcpReceiver(host, port);
	}
	
	tuioClient = new TuioClient(osc_receiver);
	tuioClient->addTuioListener(this);
	tuioClient->connect();
	
	if (!tuioClient->isConnected()) {
		SDL_Quit();
	}
	
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		std::cerr << "SDL could not be initialized: " <<  SDL_GetError() << std::endl;
		SDL_Quit();
	}
	

	initWindow();
	SDL_SetWindowTitle(window,"TuioDemo");
}

void TuioDemo::run() {
	running=true;
	while (running) {
		drawObjects();
		processEvents();
		SDL_Delay(10);
	}
}


static void show_help() {
	std::cout << "Usage: TuioDemo -p [port] -t -a [address]" << std::endl;
	std::cout << "        -p [port] for alternative port number" << std::endl;
	std::cout << "        -t for TUIO/TCP (default is TUIO/UDP)" << std::endl;
	std::cout << "        -a [address] for remote TUIO/TCP server" << std::endl;
	std::cout << "           use 'incoming' for TUIO/TCP socket" << std::endl;
	std::cout << "        -v verbose output" << std::endl;
	std::cout << "        -h show this help" << std::endl;
}

static void init(int argc, char** argv) {
	char c;

#ifndef WIN32
	while ((c = getopt(argc, argv, "p:a:tfvh")) != -1) {
		switch (c) {
			case 't':
				_udp = false;
				break;
			case 'a':
				_address = std::string(optarg);
				break;
			case 'p':
				_port = atoi(optarg);
				break;
			case 'v':
				_verbose = true;
				break;
			case 'f':
				_fullscreen = true;
				break;
			case 'h':
				show_help();
				exit(0);
			default:
				show_help();
				exit(1);
		}
	}
#endif
}

#ifdef __APPLE__
static void removeArg(int index, int *argc, char **argv) {
	--*argc;
	for (; index < *argc; ++index)
		argv[index] = argv[index + 1];
}

static void removeXcodeArgs(int *argc, char **argv) {
	for (int i = 1; i < *argc; )
	{
		if (strcmp(argv[i], "-NSDocumentRevisionsDebugMode") == 0 ||
			strcmp(argv[i], "-ApplePersistenceIgnoreState" ) == 0)
		{
			removeArg(i, argc, argv);
			removeArg(i, argc, argv);
		} else
			++i;
	}
}
#endif

int main(int argc, char* argv[])
{

#ifdef __APPLE__
	// Xcode sometimes adds additional arguments.
	removeXcodeArgs(&argc, argv);
#else
	glutInit(&argc,argv);
#endif

	init(argc, argv);
	TuioDemo *app = new TuioDemo(_address.c_str(), _port, _udp, _verbose, _fullscreen);
	app->run();
	delete app;
	
	return 0;
}
