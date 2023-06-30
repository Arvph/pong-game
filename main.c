// Here we go
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

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

int main () 
{     
    initscr(); //запускаем поле
    noecho(); //никакого отражения набранных клавиш в строке
    cbreak(); //в жопу буфферизацию
    curs_set(0); //делает невидимым курсор. Об этой фишке я вообще не знал
    keypad(stdscr, TRUE); //Эта штука по большому счету нам не нужна, я ее включил чтобы ты ознакомился. Она позволяет использовать спец клавиши типа стрелок вверх вниз ил F1 F2 F3 и тд которые по умолчанию недоступны

    int max_y = 25, max_x = 80;  //Задаем размеры будущего поля

    //Создаем ракетки в указанных координатах длинной в 3 символа. По иксу делаем отступ в 2 столбца с левой границы для левой ракетки и с правой для правой. По У же тупо берем максимальную высоту и делим на два чтобы центрировать
    Paddle left_paddle = {5, max_y / 2, PADDLE_LENGTH}; 
    Paddle right_paddle = {max_x - 6, max_y / 2, PADDLE_LENGTH};  

    Ball ball = {max_x / 2, max_y / 2, rand()%2 == 0 ? 1 : -1, rand()%2 == 0 ? 1: -1}; //начальные координаты мяча по центру поля + рандомное определение направления полета

    timeout(100); //установка таймаута в 100 миллисекунд. Чкм меньше значение - тем выше скорость мяча

    //Наконец то переходим к основному циклу игры, я уже заебался печатать

    while (1)    //запускаем бесконечный цикл
    {
        clear(); //очистка экрана

        //рисуем ракетки и мяч
        draw_paddle(&left_paddle);
        draw_paddle(&right_paddle); 
        draw_ball(&ball);  
 

        //рисуем поле нормальным способом так чтобы оно не улетало в ебеня

        for (int i=0; i< max_y; i++)  //рисуем горизонтальные границы поля
        {
            mvaddch(i, 0, '#');
            mvaddch(i, max_x - 1, '#');
        } 

        for (int i=0; i< max_x; i++) //рисуем вертикальные границы поля
        {
            mvaddch(0, i, '#');
            mvaddch(max_y - 1, i, '#');
        } 

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
        if(ball.x<=1 || ball.x >= max_x - 2)
        {
            ball.vx *= -1;
        }
        if(ball.y<=1 || ball.y >= max_y - 2)
        {
            ball.vy *= -1;
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