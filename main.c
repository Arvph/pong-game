// Here we go
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#define PADDLE_LENGTH 5

typedef struct Paddle // Создаем новую структуру данных под названием Paddle
{
    int x,y,length;
} Paddle;

typedef struct Ball // Создаем структуру данных мяча
{
    int x,y,vx,vy;   //координаты мяча и его скорости(которые так же определяют направление движения)
} Ball;

void draw_paddle(Paddle *p) // Эта херовина будет рисовать нам ракетки. Она принимает указатель на размеры ракетки а потом грузит их в поле
{
    for(int i=0; i < p->length; i++)
    {
        mvaddch(p->y+i, p->x, '|');
    }
}

void draw_ball(Ball *b) //А эта херовина отрисует мяч
{
    mvaddch(b->y, b->x, 'O');
}

int player1_score = 0;
int player2_score = 0;

void initGame()
{
    initscr(); //запускаем поле
    noecho(); //никакого отражения набранных клавиш в строке
    cbreak(); //в жопу буфферизацию
    curs_set(0); //делает невидимым курсор. Об этой фишке я вообще не знал
    keypad(stdscr, TRUE); //Эта штука по большому счету нам не нужна, я ее включил чтобы ты ознакомился. Она позволяет использовать спец клавиши типа стрелок вверх вниз ил F1 F2 F3 и тд которые по умолчанию недоступны
   
    start_color(); //Инициализируем цветовую подсистему. Да, я уже выебываюсь
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);

    timeout(100); //установка таймаута в 100 миллисекунд. Чкм меньше значение - тем выше скорость мяча
}

int main () 
{     
    srand(time(NULL));

    int max_y = 25, max_x = 80;  //Задаем размеры будущего поля
    
    
    //Создаем ракетки в указанных координатах длинной в 3 символа. По иксу делаем отступ в 2 столбца с левой границы для левой ракетки и с правой для правой. По У же тупо берем максимальную высоту и делим на два чтобы центрировать
    Paddle left_paddle = {5, max_y / 2, PADDLE_LENGTH}; 
    Paddle right_paddle = {max_x - 6, max_y / 2, PADDLE_LENGTH};  

    Ball ball = {max_x / 2, max_y / 2, rand()%2 == 0 ? 1 : -1, rand()%2 == 0 ? 1: -1}; //начальные координаты мяча по центру поля + рандомное определение направления полета

    initGame();

    //Наконец то переходим к основному циклу игры, я уже заебался печатать

    while (1)    //запускаем бесконечный цикл
    {
        clear(); //очистка экрана
        
        //рисуем ракетки и мяч
        draw_paddle(&left_paddle);
        draw_paddle(&right_paddle); 
        draw_ball(&ball);  
 

        //рисуем поле нормальным способом так чтобы оно не улетало в ебеня

        attron(COLOR_PAIR(1));  //врубить красный цвет

        for (int i=0; i< max_y; i++)  //рисуем вертикальные границы поля
        {
            mvaddch(i, 0, '#');
            mvaddch(i, max_x - 1, '#');
        } 

        attroff(COLOR_PAIR(1)); //вырубить красный цвет

        for (int i=0; i< max_x; i++) //рисуем горизонтальные границы поля
        {
            mvaddch(0, i, '#');
            mvaddch(max_y - 1, i, '#');
        } 

        //отрисовка счета
        attron(COLOR_PAIR(2));
        mvprintw(0, 34, "P1: %d P2 : %d", player1_score, player2_score);
        attroff(COLOR_PAIR(2));

        mvprintw (26, 5,"GAME RULES!");
        mvprintw (27, 5,"1) The game runs to 21 points");
        mvprintw (28, 5,"2) Press \"q\" to exit");
        mvprintw (29, 5,"3) The left paddle controllers are A and Z");
        mvprintw (30, 5,"4) The right paddle controllers are K and M");

        switch (getch()) //пора ебошить контроллеры
        {
            case 'k':
                if (right_paddle.y > 1) right_paddle.y--;  //да, контроллеры инвертированы. Да, это раздражает
                break;
            case 'm':
                if (right_paddle.y < max_y - PADDLE_LENGTH - 1) right_paddle.y++;  //а знаешь почему минус четыре? Потому что это длина ракетки + ширина границы поля
                break; 
            case 'a':
                if(left_paddle.y > 1) left_paddle.y--;
                break;
            case 'z':
                if(left_paddle.y < max_y - PADDLE_LENGTH - 1) left_paddle.y++;
                break;
            case 'q':  //тут я думаю очевидно
                endwin();
                return 0;
        }

        ball.x += ball.vx;    //обновление положения мяча
        ball.y += ball.vy;

        //проверка на забитый гол
        if(ball.x <= 0)
        {
            player2_score++;
            ball.x = max_x/2;
            ball.y = max_y/2;
            ball.vx = (rand()%2 == 0) ? 1 : -1;
            ball.vy = (rand()%2 == 0) ? 1 : -1;
        }
        if(ball.x >= max_x - 1)
        {
            player1_score++;
            ball.x = max_x/2;
            ball.y = max_y/2;
            ball.vx = (rand()%2 == 0) ? 1 : -1;
            ball.vy = (rand()%2 == 0) ? 1 : -1;
        }

        //столкновение мяча с ракетками
        if(ball.x == left_paddle.x +1 && ball.y >= left_paddle.y && ball.y < left_paddle.y + left_paddle.length) 
        {
            ball.vx = -ball.vx;  //отскок мяча
        }

        if(ball.x == right_paddle.x -1 && ball.y >= right_paddle.y && ball.y < right_paddle.y + right_paddle.length) 
        {
            ball.vx = -ball.vx;  //отскок мяча
        }

        //столкновение мяча с границами поля
        if(ball.x<=0 || ball.x >= max_x - 1)
        {
            ball.vx *= -1;
        }
        if(ball.y<=1 || ball.y >= max_y - 2)
        {
            ball.vy *= -1;
        }

        if(player1_score == 21) //ограничение счета
        {
            endwin();
            printf("Player 1 wins!\n");
            return 0;
        }
        if(player2_score == 21)
        {
            endwin();
            printf("Player 2 wins!\n");
            return 0;
        }
        
        clear();
        draw_paddle(&left_paddle);
        draw_paddle(&right_paddle);
        mvaddch(ball.y, ball.x, 'O');
        refresh();   //обновляем экран
    }

    endwin(); //заканчиваем эту ебатеку


    return 0;
}