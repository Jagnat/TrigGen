#include "SDL.h"
#include "SDL_opengl.h"

#include <stdio.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "trigfuncs.cpp"

#include "texture.c"

// This code is very primitive - 
// I wrote this code as fast as possible,
// it does the bare minimum to get an opengl window
// on the screen and render some textures and dots

SDL_Window *window;
SDL_GLContext context;

bool running = true;

bool sin_active = true;
bool cos_active = false;
bool tan_active = false;
bool csc_active = false;
bool sec_active = false;
bool cot_active = false;

uint32_t texture;

double offset = 0;

void do_events(void);
void render_funcs(void);
void render_gui(void);
void render_number_line(void);

double map_range(double oldMin, double oldMax, double newMin, double newMax, double val);

uint32_t load_texture(const char *path);
uint32_t textureit(void);

int main(int argc, char **argv)
{
	window = SDL_CreateWindow("Jagi's Trig Thing",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);

	glEnable(GL_TEXTURE_2D);

	//if ((texture = load_texture("res/texture.png")) == 0)
	//	printf("Failed to load textures!\n");
	texture = textureit();

	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 640, 0, 480, -1, 1);
	glMatrixMode(GL_MODELVIEW);

	while (running)
	{
		do_events();
		glClear(GL_COLOR_BUFFER_BIT);

		offset += 0.01f;
		if (offset > J_PI * 2)
			offset = 0;

		render_funcs();
		render_number_line();

		render_gui();

		SDL_GL_SwapWindow(window);
		SDL_Delay(10);
	}

	return 0;
}

bool get_enabled(int index)
{
	switch (index)
	{
		case 0: return sin_active;
		case 1: return cos_active;
		case 2: return tan_active;
		case 3: return csc_active;
		case 4: return sec_active;
		case 5: return cot_active;
	}
	return false;
}

void set_color(int index)
{
	switch (index)
	{
		case 0: glColor3f(1, 0, 0); return;
		case 1: glColor3f(0, 1, 0); return;
		case 2: glColor3f(0, 0, 1); return;
		case 3: glColor3f(1, 0, 1); return;
		case 4: glColor3f(1, 1, 0); return;
		case 5: glColor3f(0, 1, 1); return;
	}
}

double get_func(int index, double val)
{
	switch (index)
	{
		case 0: return j_sin(val);
		case 1: return j_cos(val);
		case 2: return j_tan(val);
		case 3: return j_csc(val);
		case 4: return j_sec(val);
		case 5: return j_cot(val);
	}
	return 0;
}

void render_number_line(void)
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glLineWidth(1.f);
	glBegin(GL_LINES);

	glVertex2f(0, 240);
	glVertex2f(640, 240);

	float x = map_range(0, 10.667, 0, 640, offset);
	glVertex2f(640 - x + 114, 220);
	glVertex2f(640 - x + 114, 260);

	x = map_range(0, 10.667, 0, 640, offset + 2 * J_PI);
	glVertex2f(640 - x + 114, 220);
	glVertex2f(640 - x + 114, 260);
	glEnd();
}

void render_funcs(void)
{
	glDisable(GL_TEXTURE_2D);
	glPointSize(3.f);
	glBegin(GL_POINTS);
	for (int i = 0; i < 6; ++i)
	{
		if (get_enabled(i))
		{
			set_color(i);
			for (int x = 0; x < 640; ++x)
			{
				double val = get_func(i, offset + map_range(0, 640, 0, 10.667, x));
				glVertex2f(x, (float)map_range(-2, 2, 0, 480, val));
			}
		}
	}
	glEnd();
}

void render_gui()
{
	glEnable(GL_TEXTURE_2D);

	for (int i = 0; i < 6; ++i)
	{
		float col = 0.3;
		float u = 0, v = 0;
		switch (i)
		{
			case 0: v = 0.5f; if (sin_active) col = 1; break;
			case 1: v = 0.5f; u = 0.25f; if (cos_active) col = 1; break;
			case 2: v = 0.5f; u = 0.5f; if (tan_active) col = 1; break;
			case 3: if (csc_active) col = 1; break;
			case 4: u = 0.25f; if (sec_active) col = 1; break;
			case 5: u = 0.5f; if (cot_active) col = 1; break;
		}
		int offs = 64 * i;
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3f(col, col, col);
		glBegin(GL_QUADS);
		{
			// bottom left
			const float a = 0.25f;
			const float b = 0.5f;
			glTexCoord2f(u, v + b);
			glVertex2f(offs, 0);
			// top left
			glTexCoord2f(u, v);
			glVertex2f(offs, 32);
			// top right
			glTexCoord2f(u + a, v);
			glVertex2f(offs + 64, 32);
			// bottom right
			glTexCoord2f(u + a, v + b);
			glVertex2f(offs + 64, 0);
		}
		glEnd();
	}
}

void do_events()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			running = false;

		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			if (y >= 448 && y <= 480)
			{
				if (x > 0 && x <= 64)
					sin_active = !sin_active;
				else if (x > 64 && x <= 128)
					cos_active = !cos_active;
				else if (x > 128 && x <= 192)
					tan_active = !tan_active;
				else if (x > 192 && x <= 256)
					csc_active = !csc_active;
				else if (x > 256 && x <= 320)
					sec_active = !sec_active;
				else if (x > 320 && x <= 384)
					cot_active = !cot_active;
			}
		}
	}
}

double map_range(double oldMin, double oldMax, double newMin, double newMax, double val)
{
	double oldRange = (oldMax - oldMin);
	if (oldRange == 0)
	{
		return newMin;
	}
	else
	{
		double newRange = (newMax - newMin);
		return (((val - oldMin) * newRange) / oldRange) + newMin;
	}
}

uint32_t textureit(void)
{
	uint32_t id = 0;
	glGenTextures(1, &id);
	GLenum format = GL_RGB;
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		format,
		256,
		64,
		0,
		format,
		GL_UNSIGNED_BYTE,
		image.pixel_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	return id;
}

uint32_t load_texture(const char *path)
{
	int w, h, n;
	void *data = stbi_load(path, &w, &h, &n, 3);
	if (n != 3)
		return 0;
	if (data == 0)
		return 0;
	uint32_t id = 0;
	glGenTextures(1, &id);
	GLenum format = GL_RGB;

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		format,
		w,
		h,
		0,
		format,
		GL_UNSIGNED_BYTE,
		data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	return id;
}