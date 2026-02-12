#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#ifndef BACKEND
#define BACKEND

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

using namespace std;

const float TILE_SIZE = 100.f;

// Return true if char in a-h
bool check_abc(char c);

int get_num(char c);

class Pos;

Pos get_pos(sf::Vector2i cord, sf::RenderWindow *);

Pos get_pos(float, float);

class Move;
class Chessboard;

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

  virtual bool check_pos_and_handle_it(int x, int y) = 0;

  virtual void recompute() = 0;

  Piece(bool white, Pos *p) : pos(p), white(white) {
    sf::Texture s;
    s.loadFromFile("chess_pieces_sprite.png");
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 60, 60));
  }

  ~Piece() {
    for (Pos *p : valid_pos) {
      p = nullptr;
    }
  }
};

class Rook : public Piece {
public:
  using Piece::Piece;

  static Chessboard *board;

  char ch() const override { return (white) ? 'R' : 'r'; }

  void update(Move &m) override;

  bool check_pos_and_handle_it(int x, int y) override;

  void recompute() override;
};

class Bishop : public Piece {
public:
  using Piece::Piece;

  static Chessboard *board;

  char ch() const override { return (white) ? 'B' : 'b'; }

  void update(Move &m) override;

  bool check_pos_and_handle_it(int x, int y) override;

  void recompute() override;
};

class Knight : public Piece {
public:
  using Piece::Piece;

  static Chessboard *board;

  char ch() const override { return (white) ? 'N' : 'n'; }

  void update(Move &m) override;

  bool check_pos_and_handle_it(int x, int y) override;

  void recompute() override;
};

class Queen : public Piece {
public:
  using Piece::Piece;

  static Chessboard *board;

  char ch() const override { return (white) ? 'Q' : 'q'; }

  void update(Move &m) override;

  bool check_pos_and_handle_it(int x, int y) override;

  void recompute() override;
};

class King : public Piece {
public:
  using Piece::Piece;

  static Chessboard *board;

  char ch() const override { return (white) ? 'K' : 'k'; }

  void update(Move &m) override;

  bool check_pos_and_handle_it(int x, int y) override;

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

  bool operator==(Pos &other) { return (other.x == x && other.y == y); }

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

  Move() : from(Pos()), to(Pos()) {}
};

Move get_move();

class Player {
public:
  vector<Piece *> pieces;
};

class Grid {
public:
  vector<vector<Pos *>> pixels;

  Grid();

  Pos *at(int x, int y);
};

class Chessboard {

public:
  Grid *grid;

  Chessboard(Grid *g) : grid(g) {}

  bool mate = false;

  Player *white;
  Player *black;

  Pos *at(int x, int y) { return grid->at(x, y); }
  Pos *at(sf::Vector2i vec) { return grid->at(vec.x, vec.y); }
  Pos *at(Pos p) { return grid->at(p.x, p.y); }

  void init(Player *white, Player *black);

  void show();
  void handle_move(Move m, string col);
};
#endif