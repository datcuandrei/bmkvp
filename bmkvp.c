/*
 * Copyright (c) 2022 Andrei Datcu
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/extensions/XTest.h>

/* CONFIGURE BMKVP */
#define COLOR 0x89cff0		// hex code for "baby blue".
#define mod 20			// the precision of the grid.

/* controls */
#define UP '8'
#define DOWN '2'
#define LEFT '4'
#define RIGHT '6'
#define SHRINK_TOP_LEFT '7'
#define SHRINK_TOP_RIGHT '9'
#define SHRINK_BOTTOM_LEFT '1'
#define SHRINK_BOTTOM_RIGHT '3'
#define CONFIRM_CLICK '5'
#define PRESS_QUIT 'q'

/* transparency
 *
 * uncomment the line that has the opacitiy you want
 *
 * NOTE: the transparency effect requires your 
 * desktop environment or window manager to have
 * a compositor installed (e.g. compton)
 * without a compositor, bmkvp will not be able
 * to render RGBA colors, and as such, you will
 * have a black background.
 */
//#define TRANSPARENCY 0x00000000	// 0%
//#define TRANSPARENCY 0x1a000000	// 10%
//#define TRANSPARENCY 0x33000000	// 20%
#define TRANSPARENCY 0x4d000000	// 30%
//#define TRANSPARENCY 0x66000000 // 40%
//#define TRANSPARENCY 0x80000000 // 50%
//#define TRANSPARENCY 0x99000000 // 60%
//#define TRANSPARENCY 0xb3000000 // 70%
//#define TRANSPARENCY 0xcc000000 // 80%
//#define TRANSPARENCY 0xe6000000 // 90%
//#define TRANSPARENCY 0xFF000000 // 100%

Display *dpy;
int scr;
Screen *screen;
XEvent ev;
XVisualInfo vinfo;

bool initx();
void destroyx();

/*
 * GRID
 * 
 * The trick in creating the 2x2 grid is to
 * create a main window which contains two
 * child windows. One sits in the top left
 * corner of the main window, and the other
 * one in the opposite corner.
 */
Window grid_win;
Window top_left;
Window bot_right;

/*
 * The following variables represent the
 * parameters for each window.
 * This includes their sizes and coordinates.
 */
int w_grid_win;
int h_grid_win;
int x_grid_win;
int y_grid_win;
	
int x_tl_cd;
int y_tl_cd;
int w_tl_cd;
int h_tl_cd;

int x_br_cd;
int y_br_cd;
int w_br_cd;
int h_br_cd;

/*
 * Controlling the grid
 */
void display_grid();
void move_up();
void move_down();
void move_left();
void move_right();
void shrink_tl_left();
void shrink_tl_right();
void shrink_br_left();
void shrink_br_right();
void grow_left();
void grow_right();
void move_ptr();
void print_debug();

int 
main()
{
	initx(); 		// connecting to the X server.	
	char kP[255];		// pressed keys will be inserted in this string.	
	
	/* initializing variables for the grid */
	w_grid_win = XWidthOfScreen(screen) - 10;
	h_grid_win = XHeightOfScreen(screen) - 10;
	x_grid_win = 0;
	y_grid_win = 0;
		
	x_tl_cd = -1;
	y_tl_cd = -1;
	w_tl_cd = w_grid_win/2;
	h_tl_cd = h_grid_win/2;
	
	x_br_cd = w_grid_win/2;
	y_br_cd = h_grid_win/2;
	w_br_cd = w_grid_win/2;
	h_br_cd = h_grid_win/2;

	/* getting visual info from the display with 32-bit depth for color */
	XMatchVisualInfo(dpy,
		       	DefaultScreen(dpy),
		       	32,
		       	TrueColor,
		       	&vinfo);

	/* basic window attributes to set for the grid and child windows to get transparency */
	XSetWindowAttributes attr;
    	attr.colormap = XCreateColormap(dpy,DefaultRootWindow(dpy), vinfo.visual, AllocNone);
	attr.border_pixel = COLOR;
	attr.background_pixel = TRANSPARENCY;

	grid_win = XCreateWindow(
		dpy,
		DefaultRootWindow(dpy),
		x_grid_win,
		y_grid_win,
		w_grid_win,
		h_grid_win,
		0,
		vinfo.depth,
	       	InputOutput,
	       	vinfo.visual,
	       	CWColormap | CWBorderPixel | CWBackPixel,
	       	&attr);
	top_left = XCreateWindow(
		dpy,
		grid_win,
		x_tl_cd,
		y_tl_cd,
		w_tl_cd, 
		h_tl_cd,
		1,
		vinfo.depth,
	       	InputOutput,
	       	vinfo.visual,
	       	CWColormap | CWBorderPixel | CWBackPixel,
	       	&attr);

	bot_right = XCreateWindow(
		dpy,
		grid_win,
		x_br_cd,
		y_br_cd,
		w_br_cd,
		h_br_cd,
		1,
		vinfo.depth,
	       	InputOutput,
	       	vinfo.visual,
	       	CWColormap | CWBorderPixel | CWBackPixel,
	       	&attr);
			
	display_grid();
	
	while(1)
	{
		XNextEvent(dpy, &ev);
		KeySym key;
		
		XWarpPointer(dpy, NULL , NULL, -1,-1,-1,-1, w_grid_win, w_grid_win); // placing the pointer out of bounds
		
		// if it goes under 0x0, it becomes unstable and bugs out.
		if (w_grid_win < mod || h_grid_win < mod)
		{
				destroyx();
				exit(0);
		}
		
		if(ev.type==KeyPress && XLookupString(&ev.xkey, kP, 255, &key,0)==1) {
			switch(kP[0])
			{
				case UP:
				    move_up();
					break;
				case DOWN:
				    move_down();
					break;
				case LEFT:
					move_left();
					break;
				case RIGHT:
				    move_right();
					break;
				case SHRINK_TOP_RIGHT:
					shrink_tl_right();
					break;
				case SHRINK_TOP_LEFT:
					shrink_tl_left();
					break;
				case SHRINK_BOTTOM_RIGHT:
					shrink_br_right();	
					break;
				case SHRINK_BOTTOM_LEFT:
					shrink_br_left();
					break;
				case CONFIRM_CLICK:
					move_ptr();
					break;
				case PRESS_QUIT:
					destroyx();
					exit(0);
					break;
			}
			//print_debug();
		}
	}

	destroyx();
}


bool 
initx()
{
	dpy = XOpenDisplay(NULL);

	if(dpy == NULL)
	{
		fprintf(stderr, "[ERROR] Cannot connect to X server display!\nExiting now...\n");
		return 0;
		exit(1);
	}

	scr = DefaultScreen(dpy);
	screen = XDefaultScreenOfDisplay(dpy);
	return 1;
}

void 
destroyx()
{
	XCloseDisplay(dpy);
}

void 
display_grid()
{
	// the type of input that the grid will accept
	XSelectInput(
			dpy, 
			grid_win, 
			ButtonPressMask|StructureNotifyMask|KeyPressMask|KeyReleaseMask|KeymapStateMask);
			
			// mapping the windows to the display
			XMapWindow(dpy, grid_win);
			XMapWindow(dpy, top_left);
			XMapWindow(dpy, bot_right);
}
		
void
move_up()
{
	XMoveWindow(dpy, grid_win, x_grid_win, y_grid_win -= mod);	
}
		
void
move_down()
{
	XMoveWindow(dpy, grid_win, x_grid_win, y_grid_win += mod);
}
		
void
move_left()
{
	XMoveWindow(dpy, grid_win, x_grid_win -= mod, y_grid_win);
}

void
move_right()
{
	XMoveWindow(dpy, grid_win, x_grid_win += mod, y_grid_win);
}
		
void
shrink_tl_left()
{
	XResizeWindow(dpy, grid_win, w_grid_win -= mod, h_grid_win -= mod);
	XResizeWindow(dpy, top_left, w_tl_cd -= mod, h_tl_cd -= mod);
	XResizeWindow(dpy, bot_right,  w_br_cd -= mod, h_br_cd -= mod);
	XMoveWindow(dpy, bot_right, x_br_cd -= mod, y_br_cd -= mod);
	XResizeWindow(dpy, grid_win,  w_grid_win -= mod, h_grid_win -= mod);
}
	
void 
shrink_tl_right()
{
	shrink_tl_left();
	move_right();
	move_right();
}

void 
shrink_br_left()
{
	shrink_tl_left();
	move_down();
	move_down();
	move_left();
	move_left();
}

void 
shrink_br_right()
{
	shrink_tl_left();
	move_down();
	move_down();
	move_right();
	move_right();
}

void 
grow_left()
{
	XResizeWindow(dpy, grid_win, w_grid_win += mod, h_grid_win += mod);
	XResizeWindow(dpy, top_left, w_tl_cd += mod, h_tl_cd += mod);				
	XResizeWindow(dpy, bot_right, w_br_cd += mod, h_br_cd += mod);
	XMoveWindow(dpy, bot_right, x_br_cd += mod, y_br_cd += mod);
	XResizeWindow(dpy, grid_win, w_grid_win += mod, h_grid_win += mod);
}

void
grow_right()
{
	grow_left();
	move_left();
	move_left();
}

void 
move_ptr()
{
	XWarpPointer(dpy, NULL , grid_win, 0,0,w_grid_win,h_grid_win, w_grid_win/2, h_grid_win/2);
	XDestroyWindow(dpy, grid_win);
	
	/* the secret to simulating a click */
	XTestFakeButtonEvent(dpy, 1, true, CurrentTime);
	XTestFakeButtonEvent(dpy, 1, false, CurrentTime);
	XFlush(dpy);
	
	exit(0);
}	

void 
print_debug()
{
	fprintf(stdout, "(grid_win) x:%d, y:%d, w:%d, h:%d\n", x_grid_win, y_grid_win, w_grid_win, h_grid_win);
	fprintf(stdout, "(top_left) x:%d, y:%d, w:%d, h:%d\n", x_tl_cd, y_tl_cd, w_tl_cd, h_tl_cd);
	fprintf(stdout, "(bot_right) x:%d, y:%d, w:%d, h:%d\n\n\n", x_br_cd, y_br_cd, w_br_cd, h_br_cd);
}

