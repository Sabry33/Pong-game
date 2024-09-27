#include <iostream>
#include <raylib.h>

using namespace std;

Color Green =Color {38,185,154,255};
Color Dark_green =Color {20,150,133,255};
Color Light_green =Color {129,204,184,255};
Color Yellow =Color {243,213,91,255};

int player_score = 0 ;
int cpu_score = 0 ;

class Ball{
public:
float x,y;
int speed_x, speed_y ;  //how many space would the ball move and it is repeated 60 times per second so it will apear as it moves
int radius ;

void Draw (){
    DrawCircle(x,y,radius,Yellow);
}

void Update(){

    x += speed_x ;
    y += speed_y; 

    //check the collisons of the ball  GetScreenHeight because we cant use the vars defined into main
    if  (y+radius >= GetScreenHeight() || y-radius <= 0){
        speed_y *= -1 ;
        //  >= is very important here
    }
    if  (x+radius >= GetScreenWidth()) 
    {
        cpu_score++;
        resetBall();
    }
    
    if ( x - radius <= 0){
        player_score++;
        resetBall();
    }

}

 void resetBall()
 {
    x = GetScreenWidth()/2 ;
    y = GetScreenHeight()/2 ;
    int xy_resetSpeed[2] = {1,-1};
    speed_x *= xy_resetSpeed[GetRandomValue(0,1)];
    speed_y *= xy_resetSpeed[GetRandomValue(0,1)];
}


} ;

class paddle {

protected :
void limitedMovment()
{
    if (y <= 0)
{
    y =0 ;
}
if (y +height >= GetScreenHeight())
{
    y =GetScreenHeight() - height ;
}
}

public: 

float x,y;
float width,height ;
int speed ;
    
void Draw(){
DrawRectangleRounded(Rectangle{x,y,width,height},0.8,0,WHITE);
}

void Update(){
     if(IsKeyDown(KEY_UP))
 {
   y -=speed ;
 }
 if (IsKeyDown(KEY_DOWN))
 {
    y += speed ;
 }

limitedMovment() ;
}
};

class cpu_paddle :public paddle{
    public :
    void Update(int ball_y)
    {
        if (y+height/2 > ball_y)
        {
           y -= speed ;
        } 

        if (y+height/2 <= ball_y)
        {
            y+=speed ;
        }
        limitedMovment();
    }


};

Ball ball ;   
paddle player;
cpu_paddle cpu ;

int main () {
    const int screen_width = 1280 ;
    const int screen_height= 800 ;
    const int winning_score = 5 ;
 // InitWindow (width,height ,title);

  InitWindow(screen_width,screen_height ,"My Pong Game");

SetTargetFPS(60) ; //how fast should the game run frames if we dont set the frames number the computer will run it as fast as it can

ball.radius = 20 ;
ball.x =screen_width/2; 
ball.y =screen_height/2;
ball.speed_x =7 ;  //7 pixel
ball.speed_y=7;

player.width =25 ;
player.height =120;
player.x = screen_width-player.width-10;
player.y = screen_height/2 - player.height/2 ;
player.speed =6 ;

cpu.width =25 ;
cpu.height = 120 ;
cpu.x =10 ;
cpu.y =screen_height/2 - cpu.height/2 ;
cpu.speed =6 ;


//game loop
  while (WindowShouldClose() == false) {
    BeginDrawing();
    
    ball.Update();
    player.Update();
    cpu.Update(ball.y) ;

    //check for the paddle and the ball collison
    if (CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player.x,player.y,player.width,player.height}))
    {
        ball.speed_x *= -1 ;
    }

        if (CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{cpu.x,cpu.y,cpu.width,cpu.height}))
    {
        ball.speed_x *= -1 ;
    }
    

   //check the winner 
   if(player_score >= winning_score || cpu_score >= winning_score)
   {

        ClearBackground(Dark_green);
        const char* winnerText = player_score >= winning_score ? "Player Wins!" : "CPU Wins!";
        DrawText(winnerText, screen_width / 2 - MeasureText(winnerText, 80) / 2, screen_height / 2 - 40, 80, YELLOW);
        
        EndDrawing();
        
        // Wait for a few seconds and then break out of the loop to end the game
        WaitTime(3.0f);  // Wait for 3 seconds before closing
        break;

   }

    //drawing 
    ClearBackground(Dark_green);
    DrawRectangle(screen_width/2,0,screen_width/2 ,screen_height ,Green) ;
    DrawCircle(screen_width/2,screen_height/2,150,Light_green);
    DrawLine(screen_width/2,0,screen_width/2,screen_height,WHITE);

    ball.Draw();
    cpu.Draw();  //remember that rectangle starts its drawing from its top left corner
    player.Draw();
    
   DrawText(TextFormat("%i",cpu_score),screen_width/4-20,20,80,WHITE) ;
   DrawText(TextFormat("%i",player_score),3*screen_width/4-20,20,80,WHITE) ;
    
   EndDrawing();


  }
  //windowshouldclose function checks wether the esc key is pressed or the close icon on screen is clicked and return true
  CloseWindow();

  
    return 0;
}