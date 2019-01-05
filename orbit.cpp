#include <SFML/Graphics.hpp>
#include <cmath>
#include <time.h>
#include <iostream>

const float eps = 0.001;
const int WIDTH = 1000, HEIGHT = 1000, RADIUS = 30;
const float FRICTION = 1.0;

using namespace sf;

struct gf
{ float x, y; };

float calc_distance(float xs, float ys, float xt, float yt)
{
    return std::sqrt(std::pow(xt - xs, 2) + std::pow(yt - ys, 2));
}

// start -> target
// reverse should be negative
gf calc_gf(float xs, float ys, float xt, float yt, float ms, float mt, float G)
{
    float r = calc_distance(xs, ys, xt, yt);
    float g = (G * ms * mt) / (r * r + eps);
    gf res;
    res.x = g * (xt - xs + eps) / (r + eps);
    res.y = g * (yt - ys + eps) / (r + eps);
    if (r < 2 * RADIUS)
    {
        res.x = 0; res.y = 0;
    }
    return res;
}

int main()
{
    srand(time(0));

    int clk = 0;

    RenderWindow app(VideoMode(WIDTH, HEIGHT), "Three Body!");
    app.setFramerateLimit(60);

    Texture t1, t2;
    t1.loadFromFile("neptune-small.png");
    t2.loadFromFile("neptune-small.png");

    Sprite sPlanet1(t1), sPlanet2(t2);

    float x1 = 500, y1 = 500,
          x2 = 100, y2 = 500;
    float dx1 = 0, dy1 = 0,
          dx2 = 0, dy2 = 1.58113; // sqrt(m1 * G / r)

    sPlanet1.setPosition(x1, y1);
    sPlanet2.setPosition(x2, y2);

    gf gf12;
    gf12.x = 0;
    gf12.y = 0;

    float G = 0.001; // gravitational constant G
    float m1 = 1000000,
          m2 = 1000;

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }

        gf12 = calc_gf(x1, y1, x2, y2, m1, m2, G);

        if (clk % 100 == 0)
        {
            std::cout << "----------" << std::endl;
            std::cout << gf12.x << " " << gf12.y << std::endl;
            std::cout << x2 << " " << y2 << std::endl;
            std::cout << calc_distance(x1, y1, x2, y2) << std::endl;
        }
        clk++;

        dx1 += (gf12.x) / m1;
        dy1 += (gf12.y) / m1;
        dx2 += (-gf12.x) / m2;
        dy2 += (-gf12.y) / m2;

        x1 += dx1 * FRICTION;
        y1 += dy1 * FRICTION;
        x2 += dx2 * FRICTION;
        y2 += dy2 * FRICTION;
        sPlanet1.setPosition(x1, y1);
        sPlanet2.setPosition(x2, y2);

        app.clear();
        app.draw(sPlanet1);
        app.draw(sPlanet2);

        app.display();
    }

    return 0;
}
