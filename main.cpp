#include "raylib.h"
#include <deque>
#include <raymath.h>

Color Green = Color{173,204,96,255};
Color darkGreen = Color{43,51,24,255};

constexpr int cellsize{30};
constexpr int cellcount{25};
constexpr int offset{75};
int score{0};

double lastUpdateTime{0};
bool eventtrigger(double interval){
    double currenttime = GetTime();
    if(currenttime - lastUpdateTime >= interval){
        lastUpdateTime = currenttime;
        return true;
    }
    return false;
} 

bool inDeque(const Vector2& vector, const std::deque<Vector2>& deque){
    for(int i{0}; i < deque.size(); ++i){
        if(Vector2Equals(vector, deque[i]))return true;
    }
    return false;

}

class Snake{
    public:
    Vector2 direction{1,0};
    std::deque<Vector2> body{Vector2{5,15}, Vector2{4,15},Vector2{3,15}};
    bool addSegment{false};

    void Draw(){
        for(int i{0}; i < body.size(); ++i){
            Rectangle rec{offset+ body[i].x * cellsize,offset+ body[i].y * cellsize, cellsize, cellsize};
            DrawRectangleRounded(rec, 0.5, 6, darkGreen);
        }
    }

    void Update(){
        if(addSegment==true){
            body.push_front(Vector2Add(body[0],  direction));
            addSegment = false;
        }

        else{
            body.pop_back();
            body.push_front(Vector2Add(body[0], direction));
        }


    }

    void Reset(){
        body = {Vector2{5,15},Vector2{4,15},Vector2{3,15}};
        direction = Vector2{1,0};
    }


};

class Food{
    public:
    Vector2 pos;

    Food(const std::deque<Vector2>& snakebody){
        pos = generaterandompos(snakebody);
    }
    

    Vector2 generaterandomcell(){
        float x{static_cast<float>(GetRandomValue(0, cellcount-1))};
        float y{static_cast<float>(GetRandomValue(0, cellcount-1))};
        return Vector2{x,y};
    }

    Vector2 generaterandompos(const std::deque<Vector2>& body){
        Vector2 pos = generaterandomcell();
        while(inDeque(pos,body)){pos = generaterandomcell();}
        return pos;

}
    


    void Draw(){
        DrawCircle(offset+ pos.x * cellsize, offset+ pos.y * cellsize, 12, darkGreen);
    }
    
};


class Game{
    public:
        Snake snake = Snake();
        Food food = Food(snake.body);
        bool running{true};
        
        void Draw(){
            snake.Draw();
            food.Draw();
        }

        void Update(){
            if(running){
            snake.Update();
            CheckcolWithFood();
            if(CheckcolSnake()){GameOver();}
        }
        }

        void CheckcolWithFood(){
            if(Vector2Equals(snake.body[0], food.pos)){
                food.pos = food.generaterandompos(snake.body);
                snake.addSegment = true;
                score++;
            }

        }

        bool CheckcolSnake(){
            for(int i{1}; i < snake.body.size(); ++i){
                if(Vector2Equals(snake.body[0], snake.body[i])){return true;}
            }
            if(snake.body[0].x >= cellcount|| snake.body[0].x < 0 || snake.body[0].y >= cellcount || snake.body[0].y < 0 )return true;
            return false;
        }

        void GameOver(){
            snake.Reset();
            food.pos = food.generaterandompos(snake.body);
            running = false;
            score=0;
        }


};


int main(){
    InitWindow(cellsize * cellcount + (2 * offset), cellsize * cellcount + (2 * offset), "game");
    SetTargetFPS(60);
    Game game{};

    while(!WindowShouldClose()){
        BeginDrawing();
        
        

        if(eventtrigger(0.2)){
            game.Update();
        }
        if(IsKeyPressed(KEY_UP) && game.snake.direction != Vector2{0,1}){game.snake.direction={0,-1}; game.running=true;}
        else if(IsKeyPressed(KEY_DOWN) && game.snake.direction != Vector2{0,-1}){game.snake.direction={0,1}; game.running=true;}
        else if(IsKeyPressed(KEY_RIGHT) && game.snake.direction != Vector2{-1,0}){game.snake.direction={1,0}; game.running=true;}
        else if(IsKeyPressed(KEY_LEFT) && game.snake.direction != Vector2{1,0}){game.snake.direction={-1,0}; game.running=true;}

        game.CheckcolWithFood();
        


        ClearBackground(Green);
        game.Draw();
        DrawRectangleLinesEx(Rectangle{offset ,offset ,cellcount*cellsize , cellcount* cellsize },3,darkGreen);
        DrawText("Retro snake", offset-5, 20, 40, darkGreen);
        DrawText(TextFormat("%i",score),offset-5,offset+cellsize*cellcount+10,40,darkGreen);
        


        EndDrawing();
    }
    CloseWindow();
    return 0;


}
