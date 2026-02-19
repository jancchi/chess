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


// Problems in King recompute diagonal

Chessboard *Piece::board = nullptr;

Chessboard *Pawn::board = nullptr;

Chessboard *Rook::board = nullptr;

Chessboard *Bishop::board = nullptr;

Chessboard *Knight::board = nullptr;

Chessboard *Queen::board = nullptr;

Chessboard *King::board = nullptr;

//Chessboard *Bishop::board = nullptr;

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

  Piece::board = &board;
  Pawn::board = &board;
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





  bool is_white = true;
  Pos* selected = nullptr;
  while (entry.isOpen()) {
    bool pixel_white = true;
    for (i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        Pos* curr = board.at(i, j);

        curr->rect.setFillColor(
            (pixel_white) ? sf::Color(138, 138, 138, 255)
                          : sf::Color(200, 50, 0, 255));
        pixel_white = !pixel_white;
      }
      pixel_white = !pixel_white;
    }
    if (selected) {
      selected->rect.setFillColor(sf::Color::Black);

      for (Pos* p : selected->p->valid_pos) {
        p->rect.setFillColor(sf::Color::Green);
      }
    }

    for (Pos *p : board.white->king->valid_pos) {
      p->rect.setFillColor(sf::Color::Blue);
    }

    for (Pos *p : board.black->king->valid_pos) {
      p->rect.setFillColor(sf::Color::Magenta);
    }

    sf::Event event;
    while (entry.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        entry.close();
      if (event.type == sf::Event::Resized) {
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        entry.setView(sf::View(visibleArea));
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        Pos* curr = board.at(get_pos(sf::Mouse::getPosition(), &entry));
        curr->rect.setFillColor(sf::Color::Blue);

        if (selected) {
          if (selected->p->is_valid(curr)){
            Pos* prev = selected;
            Piece* p_prev = selected->p;
            p_prev->move(curr);
            if ((is_white) ? board.white->king->in_check : board.black->king->in_check) {
              cerr << "\nYou are in check";
              p_prev->move(prev);
              is_white = !is_white;
            }
            selected = nullptr;
            is_white = !is_white;
          }else {
            cout << "\nYou cant go there";
            // disable for testing
            selected = nullptr;
          }
        }else {
          if (curr->p && curr->p->white == is_white) {
            selected = curr;
            selected->rect.setFillColor(sf::Color::Black);
            cout << "\nSelected piece: " << selected->p->ch();
          }else {
            cout << "\nNo piece to pick up";
          }
        }
      }
    }

    entry.clear();

    for (vector<Pos *> &v : board.grid->pixels) {
      for (Pos *pos : v) {
        entry.draw(pos->rect);
        if (pos->p) {
          entry.draw(pos->p->sprite);
        }
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