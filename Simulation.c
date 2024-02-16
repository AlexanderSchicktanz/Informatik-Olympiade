Can you change the code so that the user input is normalized and moving in two directions is not faster than moving in one!

#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

typedef struct{
    float x;
    float y;
} V2f;

typedef struct{
    V2f pos;
    char *name;
} player;

float dist(player h, player b){
    float x = h.pos.x-b.pos.x;
    float y = h.pos.y-b.pos.y;
    return sqrtf(x*x+y*y);
}

void rotate(player *b, float rotation, float radius){
    b->pos.x = cos(rotation)*radius;
    b->pos.y = sin(rotation)*radius;
}

void normalize(V2f *vector){
    float x = vector->x;
    float y = vector->y;
    float f = sqrt(x*x+y*y);
    vector->x/=f;
    vector->y/=f;
}

void gotoH(player b, player h, float *r, float radius){
    float bestR = *r;
    float dists[3];
    dists[1] = dist(h,b);
    rotate(&b, (*r)-(.045/PI), radius);
    dists[0] = dist(h,b);
    rotate(&b, (*r)+(.045/PI), radius);
    dists[2] = dist(h,b);
    if(dists[0]<dists[2]){
        bestR = *r-(.045/PI);
        if(dists[1]<dists[0]){
            bestR = *r;
        }
    }else{
        bestR = *r+(.045/PI);
        if(dists[1]<dists[2]){
            bestR = *r;
        }
    }
    *r = bestR;
}

char gotoInner(player C, player *H, float radius){
    V2f move;
    move.x = C.pos.x-H->pos.x;
    move.y = C.pos.y-H->pos.y;
    normalize(&move);
    H->pos.x += move.x;
    H->pos.y += move.y;
    return (dist(C, *H)<radius/4);
}

int main(int argc, char *argv[]){
    char pilot = 0;
    if(argc>1){
        if(argv[1][0]=='p'){
            pilot = 1;
        }
    }
    player Heldin = {
        .name = "Heldin",
        .pos =  (V2f){ 50, 50 }
    };
    player Böse = {
        .name = "Böse",
        .pos = (V2f){100,0}
    };
    player midlle = {
        .name = "Middle",
        .pos = (V2f){0,0}
    };
    float rotation = 180;
    int screenWidth = 1000;
    int screenHeight = 1000;
    int radius = 100;
    int smallRadius;
    char rotating = 0;
    SetTargetFPS(60);
    InitWindow(screenWidth,screenHeight,"Solution");
    SetWindowState(FLAG_WINDOW_UNDECORATED);
    while(!WindowShouldClose()){
        if(pilot){
            V2f move = {0, 0};
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                move.x += 1;
            } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                move.x -= 1;
            }
            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
                move.y -= 1;
            } else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
                move.y += 1;
            }
            normalize(&move);
            Heldin.pos.x += move.x;
            Heldin.pos.y += move.y;
        }else{
            if(!rotating){
                rotating = gotoInner(midlle, &Heldin, radius);
            }
        }
        gotoH(Böse, Heldin, &rotation, radius);
        //printf("Rotation: %f\n",rotation);
        printf("DistToCenter: %f\n",dist(Heldin,midlle));
        rotate( &Böse , rotation ,radius );
        ClearBackground(BLACK);
        BeginDrawing();
        DrawCircle(screenWidth/2,screenHeight/2,screenHeight/4,GREEN);
        DrawCircle(screenWidth/2+(Heldin.pos.x*screenHeight/4)/100,screenHeight/2+(Heldin.pos.y*screenHeight/4)/100,20,BLUE);
        DrawCircle(screenWidth/2+(Böse.pos.x*screenHeight/4)/100,screenHeight/2+(Böse.pos.y*screenHeight/4)/100,20,RED);
        EndDrawing();
    }
    CloseWindow();
}