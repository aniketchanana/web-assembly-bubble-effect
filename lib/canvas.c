#include<stdio.h>
#include<time.h>
#include<emscripten.h>
#include<stdlib.h>

#define NUM_CIRCLES 500

//defining a circle
struct circle{
    //dimension
    int x ;
    int y ;
    int r;

    //color
    int cr;
    int cg;
    int cb;
};

//circle animation data structure
struct circleAnimation{
    //dimension
    int x ;
    int y ;
    int r;

    //velocity
    int xv;
    int yv;

    //direction
    int xd;
    int yd;
};

struct circle circles[NUM_CIRCLES];
struct circleAnimation circleAnimation[NUM_CIRCLES];

int getRand(max){
    return (rand()%max);
}

int main(){
    // seed random number generator
    srand(time(NULL));

    for(int i=0;i<NUM_CIRCLES;i++){
        int radius = getRand(50);

        int x = getRand(2000) + radius;
        int y = getRand(2000) + radius;

        //animation data
        circleAnimation[i].x = x;
        circleAnimation[i].y = y;
        circleAnimation[i].r = radius;
        circleAnimation[i].xv = getRand(10);
        circleAnimation[i].yv = getRand(10);
        circleAnimation[i].xd = 1;
        circleAnimation[i].yd = 1;
        
        //drawing data
        circles[i].r = radius;
        circles[i].x = x;
        circles[i].y = y;
        circles[i].cr = getRand(255);
        circles[i].cg = getRand(255);
        circles[i].cb = getRand(255);
    }

    // emscripten_run_script("render()");
    EM_ASM( { render($0,$1); } ,NUM_CIRCLES*6,6);
}

// returns circles to js
struct circle * getCircles(int canvasWidth,int canvasHeight){

    for(int i=0;i<NUM_CIRCLES;i++){
        //collison detection
        if(circleAnimation[i].x+circleAnimation[i].r >= canvasWidth)
        circleAnimation[i].xd = 0;

        if(circleAnimation[i].x-circleAnimation[i].r <= 0)
        circleAnimation[i].xd = 1;

        if(circleAnimation[i].y-circleAnimation[i].r <= 0)
        circleAnimation[i].yd = 1;

        if(circleAnimation[i].y+circleAnimation[i].r >= canvasHeight)
        circleAnimation[i].yd = 0;

        if(circleAnimation[i].xd == 1){
            circleAnimation[i].x += circleAnimation[i].xv;
        }else{
            circleAnimation[i].x -= circleAnimation[i].xv;
        }

        if(circleAnimation[i].yd == 1){
            circleAnimation[i].y += circleAnimation[i].yv;
        }else{
            circleAnimation[i].y -= circleAnimation[i].yv;
        }

        circles[i].x = circleAnimation[i].x;
        circles[i].y = circleAnimation[i].y;
    }
    return circles;
}