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
    app.setKeyRepeatEnabled(false);

    Texture t1, t2, t3;
    t1.loadFromFile("neptune-small.png");
    t2.loadFromFile("neptune-small.png");
    t3.loadFromFile("neptune-small.png");

    Sprite sPlanet1(t1), sPlanet2(t2), sPlanet3(t3);

    float x1 = 500, y1 = 500,
          x2 = 100, y2 = 500,
          x3 = 300, y3 = 846.4101;
    float dx1 = 0, dy1 = 0,
          dx2 = 0, dy2 = 1.58113, // sqrt(m1 * G / r)
          dx3 = 1.36930, dy3 = 0.79057;

    sPlanet1.setPosition(x1, y1);
    sPlanet2.setPosition(x2, y2);
    sPlanet3.setPosition(x3, y3);

    gf gf12, gf13, gf23;
    gf12.x = 0;
    gf12.y = 0;
    gf13.x = 0;
    gf13.y = 0;
    gf23.x = 0;
    gf23.y = 0;

    float G = 0.001; // gravitational constant G
    float m1 = 1000000,
          m2 = 1000,
          m3 = 0.0001;

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Space)
                    m3 *= 10;
        }

        gf12 = calc_gf(x1, y1, x2, y2, m1, m2, G);
        gf13 = calc_gf(x1, y1, x3, y3, m1, m3, G);
        gf23 = calc_gf(x2, y2, x3, y3, m2, m3, G);

        if (clk % 100 == 0)
        {
            std::cout << "----------" << std::endl;
            std::cout << x2 << " " << y2 << std::endl;
            std::cout << x3 << " " << y3 << std::endl;
            std::cout << "s1-s2: " << calc_distance(x1, y1, x2, y2) << std::endl;
            std::cout << "s2-s3: " << calc_distance(x2, y2, x3, y3) << std::endl;
            std::cout << "m3: " << m3 << std::endl;
        }
        clk++;

        dx1 += (gf12.x + gf13.x) / m1;
        dy1 += (gf12.y + gf13.y) / m1;
        dx2 += (-gf12.x + gf23.x) / m2;
        dy2 += (-gf12.y + gf23.y) / m2;
        dx3 += (-gf23.x - gf13.x) / m3;
        dy3 += (-gf23.y - gf13.y) / m3;

        x1 += dx1; y1 += dy1;
        x2 += dx2; y2 += dy2;
        x3 += dx3; y3 += dy3;
        sPlanet1.setPosition(x1, y1);
        sPlanet2.setPosition(x2, y2);
        sPlanet3.setPosition(x3, y3);

        app.clear();
        app.draw(sPlanet1);
        app.draw(sPlanet2);
        app.draw(sPlanet3);

        app.display();
    }

    return 0;
}
