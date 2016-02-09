#pragma once

#include <cairo.h>
#include <cairo-pdf.h>
#include <cairo-ps.h>
#include <cairo-xlib.h>

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// Window Creation related
void WCreate(int, int);
void WHandleEvents();
bool WIsOpen();
void WQuit();

// Window update / draw
void WClearFrame();
void WFillRectangle(int, int, int, int);

// Implemented in main 
void WUpdateFrame();
