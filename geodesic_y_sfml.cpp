#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <math.h>
#include "board.h"
#include "geodesic_y.h"

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
    float X;
    float Y;
};

std::vector<vertex> geodesic_cords(int n) {
    if (n < 2) {
        throw "bruh";
    }
    float l = 1.0;
    float x1, y1;
    std::vector<uint16_t> top;
    std::vector<uint16_t> right;
    for (int i = 1; i < n; i++) {
        x1 = l * cos(11 * M_PI / 12);
        y1 = l * sin(11 * M_PI / 12);
        for (int d = 0; d < 3; d++) {

        }
    }
}

namespace geodesic_y {
    extern int n;
    extern std::vector<std::vector<uint16_t>> graph;
    extern std::vector<uint16_t> graph_left;
    extern std::vector<uint16_t> graph_bottem;
    extern std::vector<uint16_t> graph_right;
}

geodesic_y::State state(geodesic_y::n, 1);

extern float cords[93][2];
std::vector<vertex> vertices(geodesic_y::n);
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
    for (int i = 0; i < vertices.size(); i++) {
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
        for (int v = 0; v < vertices.size(); v++) {
            switch (state.board_[v].player) {
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

    //geodesic_y::State state(geodesic_y::n, 1);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(winw, winh), "Geodesic Y", sf::Style::Default, settings);

    load_cords();

    for (int i = 0; i < geodesic_y::graph.size(); i++) {
        for (int j = 0; j < geodesic_y::graph[i].size(); j++) {
            if (i < geodesic_y::graph[i][j]) {

                sf::Vertex vert1;
                sf::Vertex vert2;
                
                vert1.position = {vertices[i].X, vertices[i].Y};
                vert2.position = {vertices[geodesic_y::graph[i][j]].X, vertices[geodesic_y::graph[i][j]].Y};

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
                    for (int v = 0; v < vertices.size(); v++) {
                        float distance = sqrt(pow(vertices[v].X - mx, 2) + pow(vertices[v].Y - my, 2));
                        if (distance < radius && distance < cmin) {
                            cmin = distance;
                            cv = v;
                        }
                    }
                    if (cv > -1) {
                        if (state.board_[cv].player == 0) {
                            state.Move(cv);
                            if (state.CheckWin() != 0) {
                                print(state.win);
                                print("won");
                                draw(&window);
                                std::cin.get();
                                return; // wtf
                            }
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
