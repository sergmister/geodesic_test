#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <math.h>
#include "board.h"

// when we use list of lists only check smaller nodes

const int winw = 800;
const int winh = 800;
const int scale = 720;

sf::Color black(0, 0, 0);
sf::Color grey(128, 128, 128);
sf::Color red(200, 0, 0);
sf::Color blue(0, 0, 200);
sf::Color c = grey;
float radius = 24.0f;

struct vertex {
    int player = 0; 
    float X;
    float Y;
};

int n = 93;
std::vector<uint8_t> graph = matrix_graph(n, neighbors);
std::vector<vertex> vertices(n);
std::vector<sf::Vertex> edges;

void print(const char* msg) {
    std::cout << msg << std::endl;
}

void print(int msg) {
    std::cout << msg << std::endl;
}

void print(float msg) {
    std::cout << msg << std::endl;
}

float convertX(float x) {
    x -= 0.5f;
    x *= scale;
    x += winw / 2;
    return x;
}

float convertY(float y) {
    y = 0.5f - y;
    y *= scale;
    y += winh / 2;
    return y;
}

void load_cords() {
    for (int i = 0; i < n; i++) {
        vertices[i].X = convertX(cords[i][0]);
        vertices[i].Y = convertY(cords[i][1]);
    }
}

sf::CircleShape circle(float x, float y, sf::Color color) {
    sf::CircleShape circle(radius, 32);
    circle.setOrigin(radius, radius);
    circle.setPosition(x, y);
    circle.setFillColor(color);
    return circle;
}

void draw(sf::RenderWindow* window) {
    window->clear();
    window->draw(edges.data(), edges.size(), sf::Lines);
        for (int v = 0; v < n; v++) {
            switch (vertices[v].player) {
                case 1:
                    window->draw(circle(vertices[v].X, vertices[v].Y, blue));
                    break;
                case -1:
                    window->draw(circle(vertices[v].X, vertices[v].Y, red));
                    break;
            }
        }
        window->display();
}

void run() {

    int turn = 1;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(winw, winh), "Geodesic Y", sf::Style::Default, settings);

    load_cords();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < (i + 1); j++) {
            if (graph[i * n + j] == 1) {

                sf::Vertex vert1;
                sf::Vertex vert2;
                
                vert1.position = {vertices[i].X, vertices[i].Y};
                vert2.position = {vertices[j].X, vertices[j].Y};

                vert1.color = c;
                vert2.color = c;
                
                edges.push_back(vert1);
                edges.push_back(vert2);
            }
        }
    }

    bool refreshed = false;
 
    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf:: Mouse::Left) {
                    int mx = event.mouseButton.x;
                    int my = event.mouseButton.y;
                    float cmin = 1000;
                    int cv = -1;
                    for (int v = 0; v < n; v++) {
                        float distance = sqrt(pow(vertices[v].X - mx, 2) + pow(vertices[v].Y - my, 2));
                        if (distance < radius && distance < cmin) {
                            cmin = distance;
                            cv = v;
                        }
                    }
                    if (cv > -1) {
                        if (vertices[cv].player == 0) {
                            vertices[cv].player = turn;
                            turn *= -1;
                            //refreshed = false;
                        }
                    }
                }
            }
        }

        if (!refreshed) {
            draw(&window);
            //refreshed = true;
        }
    }
}

int main() {
    run();
}