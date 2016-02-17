#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include "SDL_opengles2.h"


#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}

double p1X = 0;
double p1Y = 0;
double p2X = 0;
double p2Y = 0;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	

	SDL_Event event;
	bool done = false;
#ifdef _WINDOWS
	glewInit();
#endif

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");



	GLuint dogTexture = LoadTexture("dog.png");
	GLuint ballTexture = LoadTexture("ball.png");
	GLuint boneTexture = LoadTexture("bone.png");

	projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	glUseProgram(program.programID);

	float lastFrameTicks = 0.0f;

	double movement = 0.0;

	double ballBounce = 0.0;

	bool bounceHigh = false;

	bool firstHit = false;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		

		glClear(GL_COLOR_BUFFER_BIT);
		program.setModelMatrix(modelMatrix);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);
		glBindTexture(GL_TEXTURE_2D, ballTexture);

		float ticks = (float)SDL_GetTicks() / 1000.0f;

		float elapsed = ticks - lastFrameTicks;
			
		lastFrameTicks = ticks;

		if (firstHit == false){
			movement = movement + elapsed;
		}

		//makes the ball bounce
		if ((ballBounce * 0.75) >= 1){
			bounceHigh = true;
		}

		if ((ballBounce * 0.75) <= -1){
			bounceHigh = false;
		}
		
		if (bounceHigh == false){
			ballBounce = ballBounce + elapsed;
		}
		else if (bounceHigh == true){
			ballBounce = ballBounce - elapsed;
		}

		
		float vertices[] = { -1.4 + (movement * 0.25), -0.4 + (ballBounce * 0.25), -0.4 + (movement * 0.25), -0.4 + (ballBounce * 0.25),
			-0.4 + (movement * 0.25), 0.4 + (ballBounce * 0.25), -1.4 + (movement * 0.25), -0.4 + (ballBounce * 0.25),
			-0.4 + (movement * 0.25), 0.4 + (ballBounce * 0.25), -1.4 + (movement * 0.25), 0.4 + (ballBounce * 0.25) };
		
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);
		float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);


		if (keys[SDL_SCANCODE_A]) {
			--p1X;
		}
		else if (keys[SDL_SCANCODE_D]) {
			++p1X;
		}

		if (keys[SDL_SCANCODE_W]) {
			++p1Y;
		}
		else if (keys[SDL_SCANCODE_S]) {
			--p1Y;
		}

		glBindTexture(GL_TEXTURE_2D, dogTexture);

		float vertices2[] = { -2.5 + (p1X * 0.001), -0.5 + (p1Y * 0.001), -1.5 + (p1X * 0.001), -0.5 + (p1Y * 0.001),
			-1.5 + (p1X * 0.001), 0.5 + (p1Y * 0.001), -2.5 + (p1X * 0.001), -0.5 + (p1Y * 0.001),
			-1.5 + (p1X * 0.001), 0.5 + (p1Y * 0.001), -2.5 + (p1X * 0.001), 0.5 + (p1Y * 0.001) };


		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
		glEnableVertexAttribArray(program.positionAttribute);
		float texCoords2[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);


		if (keys[SDL_SCANCODE_J]) {
			--p2X;
		}
		else if (keys[SDL_SCANCODE_L]) {
			++p2X;
		}

		if (keys[SDL_SCANCODE_I]) {
			++p2Y;
		}
		else if (keys[SDL_SCANCODE_K]) {
			--p2Y;
		}

		glBindTexture(GL_TEXTURE_2D, dogTexture);

		float vertices3[] = { 1.5 + (p2X * 0.001), -0.5 + (p2Y * 0.001), 2.5 + (p2X * 0.001), -0.5 + (p2Y * 0.001),
			2.5 + (p2X * 0.001), 0.5 + (p2Y * 0.001), 1.5 + (p2X * 0.001), -0.5 + (p2Y * 0.001),
			2.5 + (p2X * 0.001), 0.5 + (p2Y * 0.001), 1.5 + (p2X * 0.001), 0.5 + (p2Y * 0.001) };

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
		glEnableVertexAttribArray(program.positionAttribute);
		float texCoords3[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords3);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);
		SDL_GL_SwapWindow(displayWindow);

		//collision
		if ((vertices[11] <= vertices2[5] && vertices[11] >= vertices2[3])
			|| (vertices[1] <= vertices2[5] && vertices[1] >= vertices2[3])
			&& vertices[10] == vertices2[4])
		{
			firstHit = true;
			movement = movement + elapsed;
		}
		else if ((vertices[5] <= vertices3[11] && vertices[5] >= vertices3[1])
			|| (vertices[3] <= vertices3[11] && vertices[3] >= vertices3[1])
			&& vertices[4] == vertices3[10])
		{
			firstHit = true;
			movement = movement - elapsed;
		}

		if (vertices[10] <= vertices2[4])
		{
			std::cout << "Player 2 Wins" << std::endl;
		}

		else if (vertices[4] >= vertices3[10])
		{
			std::cout << "Player 1 Wins" << std::endl;
		}
	}

	SDL_Quit();
	return 0;
}
