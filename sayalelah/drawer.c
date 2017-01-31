#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <pthread.h>


typedef struct {
    int x; //absis
    int y; //ordinate
}Point;

int fbfd = 0;                       //Filebuffer Filedescriptor
struct fb_var_screeninfo vinfo;     //Struct Variable Screeninfo
struct fb_fix_screeninfo finfo;     //Struct Fixed Screeninfo
long int screensize = 0;            //Ukuran data framebuffer
char *fbp = 0;                      //Framebuffer di memori internal

int slope(Point p1, Point p2){
    int s;
    s = p2.y-p1.y / p2.x -p1.x;
    return s;
}

void plotPixelRGBA(int _x, int _y, int r, int g, int b, int a) {

		long int location = (_x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (_y+vinfo.yoffset) * finfo.line_length;
		*(fbp + location) = b;        //blue
		*(fbp + location + 1) = g;    //green
		*(fbp + location + 2) = r;    //red
		*(fbp + location + 3) = a;      //
}

void drawLineNegative(Point p1, Point p2, int clear, int dot){
	int xold = 50;
	int yold = 50;
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	int p = 2*dy - dx;
	int x = p1.x; int y = p1.y;
    int print = 1;
	
    if (abs(dx) > abs(dy)) {
    	while (x != p2.x) {
            if (p < 0) {
                p = p + dy + dy;
                x++;
            }
            else {
                p = p + dy + dy - dx - dx;
                x++;
                y++;
            }
            
            if (dot!=0 && x%dot == 0) print = !print;
            if (!print) continue;

            if (clear == 1) {
              plotPixelRGBA(x,y,255,255,255,0);
            }
            else if (clear == 2) {
              plotPixelRGBA(x,y,255,0,0,0);
            }
            else if (clear == 3) {
			  Point few = {xold, yold};
			  Point pew = {x,y};
			  drawCircle(few, 10, 0);
			  usleep(400);
			  drawCircle(pew, 10, 1);
			  xold = x;
			  yold = y;
            }
            else {
              plotPixelRGBA(x,y,0,0,0,0);
            }
        }
    } else {
        p = 2*dx -dy;
        while (y != p2.y) {
            if (p < 0) {
                p = p + dx + dx;
                y++;
            }
            else {
                p = p + dx + dx - dy - dy;
                y++;
                x++;
            }
            
            if (dot!=0 && y%dot == 0) print = !print;
            if (!print) continue;

            if (clear == 1) {
              plotPixelRGBA(x,y,255,255,255,0);
            }
            else if (clear == 2) {
              plotPixelRGBA(x,y,255,0,0,0);
            }
            else if (clear == 3) {
			  Point few = {xold, yold};
			  Point pew = {x,y};
			  drawCircle(few, 10, 0);
			  usleep(400);
			  drawCircle(pew, 10, 1);
			  xold = x;
			  yold = y;
            }
            else {
              plotPixelRGBA(x,y,0,0,0,0);
            }
        }
    }

}

void drawLinePositive(Point p1, Point p2, int clear, int dot){
	int xold = 50;
	int yold = 50;
	int dx = p2.x - p1.x;
	int dy = p1.y - p2.y;
	int p = 2*dy - dx;
	int x = p1.x; int y = p1.y;
    int print = 1;
	
    if (abs(dx) > abs(dy)) {
    	while (x != p2.x) {
            if (p < 0) {
                p = p + dy + dy;
                x++;
            }
            else {
                p = p + dy + dy - dx - dx;
                x++;
                y--;
            }
            
            if (dot!=0 && x%dot == 0) print = !print;
            if (!print) continue;

            if (clear == 1) {
              plotPixelRGBA(x,y,255,255,255,0);
            }
            else if (clear == 2) {
              plotPixelRGBA(x,y,255,0,0,0);
            }
            else if (clear == 3) {
			  Point few = {xold, yold};
			  Point pew = {x,y};
			  drawCircle(few, 10, 0);
			  drawCircle(pew, 10, 1);
			  usleep(400);
			  xold = x;
			  yold = y;
            }
            else {
              plotPixelRGBA(x,y,0,0,0,0);
            }
        }
    } else {
        p = 2*dx - dy;
        y = p1.y;
        while (y != p2.y) {
            if (p < 0) {
                p = p + dx + dx;
                y--;
            }
            else {
                p = p + dx + dx - dy - dy;
                y--;
                x++;
            }
            
            if (dot!=0 && y%dot == 0) print = !print;
            if (!print) continue;

            if (clear == 1) {
              plotPixelRGBA(x,y,255,255,255,0);
            }
            else if (clear == 2) {
              plotPixelRGBA(x,y,255,0,0,0);
            }
            else if (clear == 3) {
			  Point few = {xold, yold};
			  Point pew = {x,y};
			  drawCircle(few, 10, 0);
			  drawCircle(pew, 10, 1);
			  usleep(400);
			  xold = x;
			  yold = y;
            }
            else {
              plotPixelRGBA(x,y,0,0,0,0);
            }
        }
    }

}

void drawHorizontalLine(Point p1, Point p2, int clear, int dot){
    //plotPixelRGBA(x1,y1,0,0,0,0);
    int print = 1;
    for(int i = p1.x ; i <= p2.x; i++){
        if (dot!=0 && i%dot == 0) print = !print;
        if (!print) continue;
        if (clear == 1) {
          plotPixelRGBA(i,p1.y,255,255,255,0);
        }
        else if (clear == 2) {
          plotPixelRGBA(i,p1.y,255,0,0,0);
        }
        else if (clear == 3) {
			printf("HAHAHAHAHAHAHA\n");
		}
        else {
          plotPixelRGBA(i,p1.y,0,0,0,0);
        }
    }
}

void drawVerticalLine(Point p1, Point p2, int clear, int dot){
	int xold = 150;
	int yold = 150;
    int print = 1;
    int tes = 0;
    if (clear == 3) {
		tes = 1;
		printf("NOOOOO\n");
	}
	for (int i=p1.y; i<=p2.y; i++){
        if (dot!=0 && i%dot == 0) print = !print;
        if (dot!=0 && !print) continue;
		if (clear ==1){
			plotPixelRGBA(p1.x,i,255,255,255,0);
        }
        else if (clear == 2) {
			plotPixelRGBA(p1.x,i,255,0,0,0);
        }
		else if (clear == 3) {
			printf("HAHAHAHAHAHAHA\n");
		  Point few = {xold, yold};
		  Point pew = {p1.x, vinfo.yres-i-150};
		  drawCircle(few, 10, 0);
		  drawCircle(pew, 10, 1);
		  usleep(1000);
		  xold = p1.x;
		  yold = vinfo.yres-i-150;
		}
        else {
			plotPixelRGBA(p1.x,i,0,0,0,0);
        }
	}	
}

void drawLine(Point p1, Point p2, int clear){
	if (p1.x > p2.x) {
        drawLine(p2, p1, clear);
    }
    else if (p1.x == p2.x) {
        drawVerticalLine(p1, p2, clear,0);
    }
    else if (p1.y == p2.y){
        drawHorizontalLine(p1, p2, clear,0);
    }
    else if ((p2.x > p1.x) && (p2.y > p1.y)) {
        drawLineNegative(p1, p2, clear, 0);
    }
    else if ((p2.x > p1.x) && (p2.y < p1.y)) {
        drawLinePositive(p1, p2, clear,0);
    }
}

void drawDottedLine(Point p1, Point p2, int clear, int dot){
    if (p1.x > p2.x) {
        drawDottedLine(p2, p1, clear, dot);
    }
    else if (p1.x == p2.x) {
        drawVerticalLine(p1, p2, clear, dot);
    }
    else if (p1.y == p2.y){
        drawHorizontalLine(p1, p2, clear, dot);
    }
    else if ((p2.x > p1.x) && (p2.y > p1.y)) {
        drawLineNegative(p1, p2, clear, dot);
    }
    else if ((p2.x > p1.x) && (p2.y < p1.y)) {
        drawLinePositive(p1, p2, clear, dot);
    }
}

// draw circle section

void drawCircle(Point center, int radius, int clear) {
    int x = 0;
    int y = radius;
    int p = 1 - radius;
    /* plot first set of point */
    plot8points(center, x, y, clear);

    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
        plot8points(center, x, y, clear);
    }
}

void plot4points(Point center, int x, int y, int clear) {
    if (clear == 1) {
        plotPixelRGBA(center.x + x, center.y + y, 255, 255, 255, 0);
        plotPixelRGBA(center.x - x, center.y + y, 255, 255, 255, 0);
        plotPixelRGBA(center.x + x, center.y - y, 255, 255, 255, 0);
        plotPixelRGBA(center.x - x, center.y - y, 255, 255, 255, 0);
    } else 
    if (clear == 2) {
        plotPixelRGBA(center.x + x, center.y + y, 255, 0, 0, 0);
        plotPixelRGBA(center.x - x, center.y + y, 255, 0, 0, 0);
        plotPixelRGBA(center.x + x, center.y - y, 255, 0, 0, 0);
        plotPixelRGBA(center.x - x, center.y - y, 255, 0, 0, 0);
    } else {
        plotPixelRGBA(center.x + x, center.y + y, 0, 0, 0, 0);
        plotPixelRGBA(center.x - x, center.y + y, 0, 0, 0, 0);
        plotPixelRGBA(center.x + x, center.y - y, 0, 0, 0, 0);
        plotPixelRGBA(center.x - x, center.y - y, 0, 0, 0, 0);
    }
        
}

void plot8points(Point center, int x, int y, int clear) {
        plot4points(center, x, y, clear);
        plot4points(center, y, x, clear);
}

void drawPlane (Point start, int clear){
	int posx = start.x;
    int posy = start.y;
    
    int nPoint = 58;
    Point arrPoint[58] = {
		//tail 10
		{0,2}, {2,2},
		{2,2}, {5,6},
		{5,6}, {9,8},
		{9,8}, {1,8},
		{1,8}, {0,2},
		
		//body 28
		{0,7},{1,7},
		{7,7},{19,7},
		{23,7},{25,7},
		{25,7},{30,9},
		{30,9},{25,11},
		{25,11},{18,11},
		{18,11},{17,12},
		{17,12},{15,12},
		{15,12},{9,21},
		{9,21},{5,21},
		{5,21},{8,12},
		{8,12},{9,11},
		{9,11},{0,11},
		{0,7},{0,11},
		
		//kaca 12
		{19,7},{20,6},
		{19,7},{20,8},
		{22,6},{23,7},
		{22,8},{23,7},
		{20,6},{22,6},
		{20,8},{22,8},
		
		//sayap 6
		{10,7},{17,0},
		{17,0},{19.5,0},
		{19.5,0},{18,7},
		
		//roda 2
		{19,11},{19,12}
	};
	
	Point pCircle = {19,13};
	
	//scale
    int scaleFactor = 10;
    for(int i = 0; i<nPoint; i++){
		arrPoint[i].x *= scaleFactor;
		arrPoint[i].y *= scaleFactor;
	}
	pCircle.x *= scaleFactor;
	pCircle.y *= scaleFactor;
	
	//position
	for (int i = 0; i<nPoint; i++){
		arrPoint[i].x += posx;
		arrPoint[i].y += posy;
	}
	pCircle.x += posx;
	pCircle.y += posy;
	
    //draw
    for (int i = 0; i<nPoint; i = i+2){
		drawLine(arrPoint[i],arrPoint[i+1],clear);
	}	
	
	drawCircle(pCircle,8,clear);
	drawCircle(pCircle,4,clear);
}