#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Backend.hpp"

using namespace std;

//*******************************************************************************************************************************************************************
//*/

Chessboard *Rook::board = nullptr;

Chessboard *Bishop::board = nullptr;

int main() {
  sf::Font font;
  if (!font.loadFromFile(
          "/usr/share/fonts/liberation/LiberationSans-Regular.ttf")) {
    cerr << "Error loading font!\n";
    return -1;
  }

  sf::RenderWindow entry(sf::VideoMode(TILE_SIZE * 8, TILE_SIZE * 8), "Chess");
  sf::View view(sf::FloatRect(0, 0, TILE_SIZE * 8, TILE_SIZE * 8));
  entry.setView(view);

  Grid g;

  Chessboard board(&g);

  Rook::board = &board;
  Bishop::board = &board;
  Queen::board = &board;
  King::board = &board;
  Knight::board = &board;

  cout << "Hello";

  Player *p1 = new Player();

  Player *p2 = new Player();

  board.init(p1, p2);

  cout << "Starting loop\n";
  sleep(2);

  vector<sf::Text> texts_num(8);
  int i = 1;
  for (auto &t : texts_num) {
    t.setFont(font);
    t.setString(to_string(i));
    t.setCharacterSize(24);
    t.setFillColor(sf::Color::Black);
    t.setPosition(board.at(0, i - 1)->rect.getPosition().x + 1,
                  board.at(0, i - 1)->rect.getPosition().y + 0.5f);
    i++;
  }

  vector<sf::Text> texts_ch(8);
  i = 1;
  for (auto &t : texts_ch) {
    t.setFont(font);
    string temp;
    temp.push_back(char(96 + i));
    t.setString(temp);
    t.setCharacterSize(24);
    t.setFillColor(sf::Color::Black);
    t.setPosition(board.at(i - 1, 7)->rect.getPosition().x + TILE_SIZE - 20,
                  board.at(i - 1, 7)->rect.getPosition().y + TILE_SIZE - 30);
    i++;
  }

  while (entry.isOpen()) {
    sf::Event event;
    while (entry.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        entry.close();
      if (event.type == sf::Event::Resized) {
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        entry.setView(sf::View(visibleArea));
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        board.at(get_pos(sf::Mouse::getPosition(), &entry))
            ->rect.setFillColor(sf::Color::Blue);
      }
    }

    entry.clear();

    for (vector<Pos *> &v : board.grid->pixels) {
      for (Pos *p : v) {
        entry.draw(p->rect);
        entry.draw(p->p->sprite);
      }
    }

    for (auto &t : texts_num) {
      entry.draw(t);
    }

    for (auto &t : texts_ch) {
      entry.draw(t);
    }
    entry.display();
  }

  return 0;
}