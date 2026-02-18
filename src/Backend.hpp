#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#ifndef BACKEND
#define BACKEND

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

class Chessboard;
using namespace std;

const float TILE_SIZE = 100.f;

/**
 * @brief Checks if char is a-h
 * @param c Character to check
 * @return True if char is a-h
 */
bool check_abc(char c);

/**
 * @brief Get number from character a = 1
 * @param c A character to convert
 * @return A number representing the character (a = 1)
 */
int get_num(char c);

class Pos;
class Move;

Pos get_pos(sf::Vector2i cord,const sf::RenderWindow *);

Pos get_pos(float, float);

class Piece {

  char c = '0';

public:
  static Chessboard *board;

  sf::Sprite sprite;

  sf::Texture texture;

  Pos *pos;

  bool white;

  vector<Pos *> valid_pos;

  virtual char ch() const = 0;

  virtual void update(Move &) = 0;

  bool check_pos_and_handle_it(int x, int y);

  virtual void recompute() = 0;

  void virtual move(Pos *pos);

  Piece(bool white, Pos* pos) : white(white) ,pos(pos) {}

  virtual ~Piece() {
    for (Pos *p : valid_pos) {
      p = nullptr;
    }
  }

  bool is_valid(const Pos *pos) const;

};
class Pawn : public Piece {
public:

  bool in_start_pos = true;

  Pawn(bool white, Pos* pos);

  static Chessboard *board;

  char ch() const override { return (white) ? 'P' : 'p'; }

  void update(Move &m) override;

  void recompute() override;

  void move(Pos *pos) override;
};

class Rook : public Piece {
public:

  Rook(bool white, Pos* pos);

  static Chessboard *board;

  char ch() const override { return (white) ? 'R' : 'r'; }

  void update(Move &m) override;

  void recompute() override;
};

class Bishop : public Piece {
public:
  Bishop(bool white, Pos *pos);

  static Chessboard *board;

  char ch() const override { return (white) ? 'B' : 'b'; }

  void update(Move &m) override;

  void recompute() override;
};

class Knight : public Piece {
public:
  Knight(bool white, Pos *pos);

  static Chessboard *board;

  char ch() const override { return (white) ? 'N' : 'n'; }

  void update(Move &m) override;

  void recompute() override;
};

class Queen : public Piece {
public:

  Queen(bool white, Pos *pos);

  static Chessboard *board;

  char ch() const override { return (white) ? 'Q' : 'q'; }

  void update(Move &m) override;

  void recompute() override;
};

class King : public Piece {
public:

  bool in_check = false;

  bool check_and_handle_expanded(int x, int y);

  King(bool white, Pos *pos);

  static Chessboard *board;

  char ch() const override { return (white) ? 'K' : 'k'; }

  void update(Move &m) override;

  void recompute() override;
};

class Pos {
public:
  char pixel;
  char c;
  int y;
  int x;
  Piece *p;
  sf::RectangleShape rect;

  Pos(char c, int p, Piece *pic = nullptr, char pix = ' ');

  Pos() : c('0'), y(0), x(0) {}

  bool operator==(const Pos &other) const { return (other.x == x && other.y == y); }

  void set_p(Piece *p) {
    this->p = p;
    pixel = p->ch();
    x = p->pos->x;
    y = p->pos->y;
    c = p->pos->c;
  }
};

class Move {
public:
  Pos from;
  Pos to;

  Move(char from, int i_from, char to, int i_to) {
    this->from = Pos(from, i_from);
    this->to = Pos(to, i_to);
  }

  Move(Pos from, Pos to) {
    this->from = from;
    this->to = to;
  }

  Move() : from(Pos()), to(Pos()) {}
};

Move get_move();

class Player {
public:
  vector<Piece *> pieces;

  King* king;
};

class Grid {
public:
  vector<vector<Pos *>> pixels;

  Grid();

  [[nodiscard("Dont forget to use the object")]]
  Pos *at(const int x,const  int y) const;
};

class Chessboard {

public:
  Grid *grid;

  Chessboard(Grid *g) : grid(g) {}

  bool mate = false;

  Player *white;
  Player *black;

  [[nodiscard("Dont forget to use the object")]]
  Pos *at(const int x,const  int y) const { return grid->at(x, y); }

  [[nodiscard("Dont forget to use the object")]]
  Pos *at(const sf::Vector2i vec) const { return grid->at(vec.x, vec.y); }
  [[nodiscard("Dont forget to use the object")]]
  Pos *at(const Pos p) const { return grid->at(p.x, p.y); }

  void init(Player *white, Player *black);

  void show() const;
  void handle_move(Move m);
};
#endif