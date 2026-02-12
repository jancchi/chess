#include "Backend.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const char abc[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

Pos get_pos(sf::Vector2i cord, sf::RenderWindow *window) {
  Pos pos;

  cord = static_cast<sf::Vector2i>(
      window->mapPixelToCoords(sf::Mouse::getPosition()));

  pos.x = (int)cord.x / (int)TILE_SIZE;

  pos.y = (int)cord.y / (int)TILE_SIZE;

  cout << "\nTemp.x: " << pos.x << " temp.y: " << pos.y;
  cout << "\nMouse.x: " << cord.x << " Mouse.y: " << cord.y;

  if (pos.x < 0 || pos.x > 7 || pos.y < 0 || pos.y > 7) {
    cerr << "\nSomething went wrong\n";
    abort();
  }

  return pos;
}

Pos get_pos(float x, float y) { return Pos(); }

// Return true if char in a-h
bool check_abc(char c) {

  for (char t : abc) {
    if (c == t) {
      return true;
    }
  }

  return false;
}

int get_num(char c) {
  if (!check_abc(c)) {
    std::cerr << "\nInvalid char";
    abort();
    return -1;
  }

  for (int i = 0; i < 8; i++) {
    if (c == abc[i]) {
      return i;
    }
  }
  return -1;
}

Pos::Pos(char c, int p, Piece *pic, char pix) {
  if (p < 0 || p > 8) {
    cerr << "\nWrong char";
    abort();
  }

  if (!check_abc(c)) {
    cerr << "\nWrong char";
    abort();
  }

  this->y = get_num(c);
  this->c = c;

  if (pic) {
    pixel = pic->ch();
  } else {
    pixel = pix;
  }
}

Move get_move() {

  cout << "\nEnter move from[a-h,0-7] to[]: ";

  char from;
  cin >> from;

  int i_from;
  cin >> i_from;

  char to;
  cin >> to;

  int i_to;
  cin >> i_to;

  return Move(from, i_from, to, i_to);
}

Grid::Grid() {
  pixels.resize(8);
  for (int i = 0; i < 8; i++) {
    pixels.at(i).resize(8);
    for (int j = 0; j < 8; j++) {
      // cout << "i + 48 is: " << char(i + 97) << "\n";
      pixels.at(i).at(j) = new Pos(char(i + 97), j);
    }
  }
  Rook r(true, at(4, 4));
}

Pos *Grid::at(int x, int y) {
  if (x < 0 || x > 7 || y < 0 || y > 7) {
    cerr << "Wrong index in Grid::at()";
    abort();
  }
  return pixels.at(y).at(x);
}

void Chessboard::init(Player *white, Player *black) {

  cout << "Init started\n";

  grid->pixels.clear();
  this->white = white;
  this->black = black;
  this->white->pieces.clear();
  this->black->pieces.clear();

  cout << "Creating pixels\n";
  grid->pixels.resize(8);

  bool pixel_white = true;
  for (int i = 0; i < 8; i++) {
    grid->pixels.at(i).resize(8);
    for (int j = 0; j < 8; j++) {
      grid->pixels.at(i).at(j) = new Pos(char(i + 97), j, nullptr, ' ');

      grid->pixels.at(i).at(j)->rect.setSize({TILE_SIZE, TILE_SIZE});
      grid->pixels.at(i).at(j)->rect.setOutlineThickness(-0.5);
      grid->pixels.at(i).at(j)->rect.setOutlineColor(sf::Color::Black);
      grid->pixels.at(i).at(j)->rect.setFillColor(
          (pixel_white) ? sf::Color(138, 138, 138, 255)
                        : sf::Color(200, 50, 0, 255));
      pixel_white = !pixel_white;

      grid->pixels.at(i).at(j)->rect.setPosition(
          {TILE_SIZE * j, TILE_SIZE * i});

      grid->pixels.at(i).at(j)->p->sprite.setPosition(
          grid->pixels.at(i).at(j)->rect.getPosition());
    }
    pixel_white = !pixel_white;
  }

  this->white->pieces.resize(16);
  this->black->pieces.resize(16);

  cout << "Creating pieces\n";
  this->white->pieces.at(0) = new Rook(true, grid->at(get_num('a'), 0));
  this->white->pieces.at(1) = new Rook(true, grid->at(get_num('a'), 7));
  this->white->pieces.at(2) = new Bishop(true, at(get_num('a'), 2));
  this->white->pieces.at(3) = new Bishop(true, at(get_num('a'), 5));

  at(4, 4)->set_p(this->white->pieces.at(0));
  at(4, 1)->set_p(this->white->pieces.at(1));

  at(0, 0)->pixel = '0';
}

void Chessboard::show() {
  cout << "\ngrid pixels has size: " << grid->pixels.size();

  cout << "----------------\n";
  char c = 104;
  for (int j = 0; j < 8; j++) {
    vector<Pos *> &v = grid->pixels.at(j);
    cout << "|" << c-- << "|";
    for (int i = 0; i < v.size(); i++) {
      if (!v.at(i)) {
        cerr << "v at(" << i << ") is nullptr";
      }
      cout << v.at(i)->pixel << "|";
    }
    cout << "\n----------------\n";
  }
  cout << "| |1|2|3|4|5|6|7|8|\n";
}

void Chessboard::handle_move(Move m, string col) {

  if (col == "white") {
    for (Piece *p : white->pieces) {
      p->update(m);
    }
  } else if (col == "black") {
    for (Piece *p : white->pieces) {
      p->update(m);
    }
  } else {
    cerr << "Failed";
    abort();
  }
}

// **************************************8 Update

void Rook::update(Move &m) {
  bool check = false;
  for (Pos *p : valid_pos) {
    if (*p == m.from || *p == m.to) {
      check = true;
    }
  }
  if (!check)
    return;

  this->recompute();
}

// return true when break
bool Rook::check_pos_and_handle_it(int x, int y) {

  if (x > 7 || y > 7 || x < 0 || y < 0) {
    return true;
  }

  if (board->at(x, y)->p) {
    if (board->at(x, y)->p->white == white) {

    } else {
      valid_pos.push_back(board->at(x, y)->p->pos);
    }
    return true;
  } else {
    valid_pos.push_back(board->at(x, y)->p->pos);
  }
  return false;
}
// return true when break
bool Bishop::check_pos_and_handle_it(int x, int y) {

  if (x > 7 || y > 7 || x < 0 || y < 0) {
    return true;
  }

  if (board->at(x, y)->p) {
    if (board->at(x, y)->p->white == white) {

    } else {
      valid_pos.push_back(board->at(x, y)->p->pos);
    }
    return true;
  } else {
    valid_pos.push_back(board->at(x, y)->p->pos);
  }
  return false;
}

bool Knight::check_pos_and_handle_it(int x, int y) {

  if (x > 7 || y > 7 || x < 0 || y < 0) {
    return true;
  }

  if (board->at(x, y)->p) {
    if (board->at(x, y)->p->white == white) {

    } else {
      valid_pos.push_back(board->at(x, y)->p->pos);
    }
    return true;
  } else {
    valid_pos.push_back(board->at(x, y)->p->pos);
  }
  return false;
}

bool Queen::check_pos_and_handle_it(int x, int y) {

  if (x > 7 || y > 7 || x < 0 || y < 0) {
    return true;
  }

  if (board->at(x, y)->p) {
    if (board->at(x, y)->p->white == white) {

    } else {
      valid_pos.push_back(board->at(x, y)->p->pos);
    }
    return true;
  } else {
    valid_pos.push_back(board->at(x, y)->p->pos);
  }
  return false;
}

bool King::check_pos_and_handle_it(int x, int y) {

  if (x > 7 || y > 7 || x < 0 || y < 0) {
    return true;
  }

  if (board->at(x, y)->p) {
    if (board->at(x, y)->p->white == white) {

    } else {
      valid_pos.push_back(board->at(x, y)->p->pos);
    }
    return true;
  } else {
    valid_pos.push_back(board->at(x, y)->p->pos);
  }
  return false;
}

void Rook::recompute() {

  for (auto *p : valid_pos) {
    delete p;
  }

  valid_pos.clear();

  // right
  for (int j = pos->x + 1; j < 8; j++) {

    if (check_pos_and_handle_it(j, pos->y)) {
      break;
    }
  }

  // left
  for (int j = pos->x - 1; j >= 0; j--) {

    if (check_pos_and_handle_it(j, pos->y)) {
      break;
    }
  }

  // up
  for (int j = pos->y + 1; j < 8; j++) {

    if (check_pos_and_handle_it(pos->x, j)) {
      break;
    }
  }

  // down
  for (int j = pos->y - 1; j >= 0; j--) {

    if (check_pos_and_handle_it(pos->x, j)) {
      break;
    }
  }
}

void Bishop::update(Move &m) {
  bool check = false;
  for (Pos *p : valid_pos) {
    if (*p == m.from || *p == m.to) {
      check = true;
    }
  }
  if (!check)
    return;

  this->recompute();
}

void Bishop::recompute() {

  for (auto *p : valid_pos) {
    delete p;
  }

  valid_pos.clear();

  // up-right
  for (int j = 1; pos->x + j < 8 && pos->y + j < 8; j++) {

    if (check_pos_and_handle_it(pos->x + j, pos->y + j)) {
      break;
    }
  }

  // up-left
  for (int j = 1; pos->x - j >= 0 && pos->y + j < 8; j++) {

    if (check_pos_and_handle_it(pos->x - j, pos->y + j)) {
      break;
    }
  }

  // down-right
  for (int j = 1; pos->x + j < 8 && pos->y - j >= 0; j++) {

    if (check_pos_and_handle_it(pos->x + j, pos->y - j)) {
      break;
    }
  }

  // down-left
  for (int j = 1; pos->x - j >= 0 && pos->y - j >= 0; j++) {

    if (check_pos_and_handle_it(pos->x - j, pos->y - j)) {
      break;
    }
  }
}

void Knight::update(Move &m) {
  bool check = false;
  for (Pos *p : valid_pos) {
    if (*p == m.from || *p == m.to) {
      check = true;
    }
  }
  if (!check)
    return;

  this->recompute();
}

void Knight::recompute() {

  for (auto *p : valid_pos) {
    delete p;
  }

  valid_pos.clear();

  check_pos_and_handle_it(pos->x + 2, pos->y + 1);
  check_pos_and_handle_it(pos->x + 2, pos->y - 1);

  check_pos_and_handle_it(pos->x - 2, pos->y + 1);
  check_pos_and_handle_it(pos->x - 2, pos->y - 1);

  check_pos_and_handle_it(pos->x + 1, pos->y + 1);
  check_pos_and_handle_it(pos->x + 1, pos->y - 1);

  check_pos_and_handle_it(pos->x - 1, pos->y + 1);
  check_pos_and_handle_it(pos->x - 1, pos->y - 1);
}

void Queen::update(Move &m) {
  bool check = false;
  for (Pos *p : valid_pos) {
    if (*p == m.from || *p == m.to) {
      check = true;
    }
  }
  if (!check)
    return;

  this->recompute();
}

void Queen::recompute() {

  for (auto *p : valid_pos) {
    delete p;
  }

  valid_pos.clear();

  // up-right
  for (int j = 1; pos->x + j < 8 && pos->y + j < 8; j++) {

    if (check_pos_and_handle_it(pos->x + j, pos->y + j)) {
      break;
    }
  }

  // up-left
  for (int j = 1; pos->x - j >= 0 && pos->y + j < 8; j++) {

    if (check_pos_and_handle_it(pos->x - j, pos->y + j)) {
      break;
    }
  }

  // down-right
  for (int j = 1; pos->x + j < 8 && pos->y - j >= 0; j++) {

    if (check_pos_and_handle_it(pos->x + j, pos->y - j)) {
      break;
    }
  }

  // down-left
  for (int j = 1; pos->x - j >= 0 && pos->y - j >= 0; j++) {

    if (check_pos_and_handle_it(pos->x - j, pos->y - j)) {
      break;
    }
  }

  // right
  for (int j = pos->x + 1; j < 8; j++) {

    if (check_pos_and_handle_it(j, pos->y)) {
      break;
    }
  }

  // left
  for (int j = pos->x - 1; j >= 0; j--) {

    if (check_pos_and_handle_it(j, pos->y)) {
      break;
    }
  }

  // up
  for (int j = pos->y + 1; j < 8; j++) {

    if (check_pos_and_handle_it(pos->x, j)) {
      break;
    }
  }

  // down
  for (int j = pos->y - 1; j >= 0; j--) {

    if (check_pos_and_handle_it(pos->x, j)) {
      break;
    }
  }
}

void King::update(Move &m) {
  bool check = false;
  for (Pos *p : valid_pos) {
    if (*p == m.from || *p == m.to) {
      check = true;
    }
  }
  if (!check)
    return;

  this->recompute();
}

void King::recompute() {

  for (auto *p : valid_pos) {
    delete p;
  }

  valid_pos.clear();

  check_pos_and_handle_it(pos->x + 1, pos->y);
  check_pos_and_handle_it(pos->x - 1, pos->y);

  check_pos_and_handle_it(pos->x, pos->y + 1);
  check_pos_and_handle_it(pos->x, pos->y - 1);

  check_pos_and_handle_it(pos->x - 1, pos->y - 1);
  check_pos_and_handle_it(pos->x - 1, pos->y + 1);

  check_pos_and_handle_it(pos->x + 1, pos->y - 1);
  check_pos_and_handle_it(pos->x + 1, pos->y + 1);
}
