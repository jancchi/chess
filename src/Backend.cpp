#include "Backend.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

constexpr char abc[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

Pos get_pos(sf::Vector2i cord, const sf::RenderWindow *window) {
  Pos pos;

  cord = static_cast<sf::Vector2i>(
      window->mapPixelToCoords(sf::Mouse::getPosition(*window)));

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

// Return true if char in a-h
bool check_abc(const char c) {

  for (const char t : abc) {
    if (c == t) {
      return true;
    }
  }

  return false;
}

int get_num(const char c) {
  if (!check_abc(c)) {
    std::cerr << "\nInvalid char";
    abort();
  }

  for (int i = 0; i < 8; i++) {
    if (c == abc[i]) {
      return i;
    }
  }
  return -1;
}

Pos::Pos(char c, int y, Piece *p, char pixel) {
  if (!check_abc(c)) {
    cerr << "\nWrong char";
    abort();
  }
  if (y < 0 || y > 8) {
    cerr << "\nWrong pos";
    abort();
  }
  this->x = get_num(c);
  this->c = c;
  this->y = y;

  if (p) {
    this->pixel = p->ch();
    this->p = p;
  } else {
    this->pixel = pixel;
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

  return {from, i_from, to, i_to};
}

Grid::Grid() {
  pixels.resize(8);
  for (int i = 0; i < 8; i++) {
    pixels.at(i).resize(8);
    for (int j = 0; j < 8; j++) {
      pixels.at(i).at(j) = new Pos(char(i + 97), j);
    }
  }
}

Pos *Grid::at(const int x,const int y) const {
  if (x < 0 || x > 7 || y < 0 || y > 7) {
    cerr << "Wrong index in Grid::at()";
    abort();
  }

  if (!pixels.at(y).at(x)) {
    cerr << "\nInvalid pos in Grid::at()";
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
      grid->pixels.at(i).at(j) = new Pos(char(j + 97), i, nullptr, ' ');

      grid->pixels.at(i).at(j)->rect.setSize({TILE_SIZE, TILE_SIZE});
      grid->pixels.at(i).at(j)->rect.setOutlineThickness(-0.5);
      grid->pixels.at(i).at(j)->rect.setOutlineColor(sf::Color::Black);
      grid->pixels.at(i).at(j)->rect.setFillColor(
          (pixel_white) ? sf::Color(138, 138, 138, 255)
                        : sf::Color(200, 50, 0, 255));
      pixel_white = !pixel_white;

      grid->pixels.at(i).at(j)->rect.setPosition(
          {TILE_SIZE * j, TILE_SIZE * i});
    }
    pixel_white = !pixel_white;
  }
/*
  this->white->pieces.resize(16);
  this->black->pieces.resize(16);

  cout << "Creating pieces\n";
  this->white->pieces.at(0) = new Rook(true, grid->at(get_num('a'), 7));
  this->white->pieces.at(1) = new Rook(true, grid->at(get_num('h'), 7));
  this->white->pieces.at(2) = new Bishop(true, at(get_num('c'), 7));
  this->white->pieces.at(3) = new Bishop(true, at(get_num('f'), 7));
  this->white->pieces.at(4) = new Knight(true, at(get_num('b'), 7));
  this->white->pieces.at(5) = new Knight(true, at(get_num('g'), 7));
  this->white->pieces.at(6) = new Queen(true, at(get_num('d'), 7));
  this->white->pieces.at(7) = new King(true, at(get_num('e'), 7));

  for (int i =0;i<8;i++) {
    this->white->pieces.at(8 + i) = new Pawn(true, at(i,6));
  }

  // black
  this->black->pieces.at(0) = new Rook(false, grid->at(get_num('a'), 0));
  this->black->pieces.at(1) = new Rook(false, grid->at(get_num('h'), 0));
  this->black->pieces.at(2) = new Bishop(false, at(get_num('c'), 0));
  this->black->pieces.at(3) = new Bishop(false, at(get_num('f'), 0));
  this->black->pieces.at(4) = new Knight(false, at(get_num('b'), 0));
  this->black->pieces.at(5) = new Knight(false, at(get_num('g'), 0));
  this->black->pieces.at(6) = new Queen(false, at(get_num('e'), 0));
  this->black->pieces.at(7) = new King(false, at(get_num('d'), 0));

  for (int i =0;i<8;i++) {
    this->black->pieces.at(8 + i) = new Pawn(false, at(i,1));
  }

  this->white->king = dynamic_cast<King*>(this->white->pieces.at(7));
  this->black->king = dynamic_cast<King*>(this->black->pieces.at(7));
*/
  King* wking = new King(true, at(get_num('d'), 4));
  this->white->pieces.push_back(wking);
  this->white->king = wking;

  King* bking = new King(false, at(get_num('e'), 6));
  this->white->pieces.push_back(bking);
  this->black->king = bking;

  Pawn* wpawn = new Pawn(true, at(get_num('d'), 2));
  this->white->pieces.push_back(wpawn);

  for (Piece* p : this->black->pieces) {
    p->recompute();
    p->sprite.setTexture(p->texture);
    p->sprite.setPosition(p->pos->rect.getPosition());
  }

  for (Piece* p : this->white->pieces) {
    p->recompute();
    p->sprite.setTexture(p->texture);
    p->sprite.setPosition(p->pos->rect.getPosition());
  }

}

void Chessboard::show() const {
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

void Chessboard::handle_move(Move m) {
  for (Piece *p : white->pieces) {
      p->update(m);
  }
  for (Piece *p : black->pieces) {
      p->update(m);
  }
}


bool Piece::is_valid(const Pos *pos) const {
  for (const Pos* p : valid_pos) {
    if (p == pos)return true;
  }
  return false;
}
// **************************************8 Update

Pawn::Pawn(bool white, Pos *pos) : Piece(white, pos) {
  cout << "\nCurrent path is: " << filesystem::current_path() << endl;
  texture.loadFromFile("chess_pieces_sprite.png", sf::IntRect(275, (white) ? 0 : 55, 55, 50));
  sprite.setTexture(texture);
  sprite.setScale(2,2);
  pos->set_p(this);
  this->pos = pos;
}

Rook::Rook(bool white, Pos *pos) : Piece(white, pos) {
  cout << "\nCurrent path is: " << filesystem::current_path() << endl;
  texture.loadFromFile("chess_pieces_sprite.png", sf::IntRect(220, (white) ? 0 : 55, 55, 50));
  sprite.setTexture(texture);
  sprite.setScale(2,2);
  pos->set_p(this);
  this->pos = pos;
}

Bishop::Bishop(bool white, Pos *pos) : Piece(white, pos) {
  cout << "\nCurrent path is: " << filesystem::current_path() << endl;
  texture.loadFromFile("chess_pieces_sprite.png", sf::IntRect(110, (white) ? 0 : 55, 55, 50));
  sprite.setTexture(texture);
  sprite.setScale(2,2);
  pos->set_p(this);
  this->pos = pos;
}

Knight::Knight(bool white, Pos *pos) : Piece(white, pos) {
  cout << "\nCurrent path is: " << filesystem::current_path() << endl;
  texture.loadFromFile("chess_pieces_sprite.png", sf::IntRect(165, (white) ? 0 : 55, 55, 50));
  sprite.setTexture(texture);
  sprite.setScale(2,2);
  pos->set_p(this);
  this->pos = pos;
}

King::King(bool white, Pos *pos) : Piece(white, pos) {
  cout << "\nCurrent path is: " << filesystem::current_path() << endl;
  texture.loadFromFile("chess_pieces_sprite.png", sf::IntRect(0, (white) ? 0 : 55, 55, 50));
  sprite.setTexture(texture);
  sprite.setScale(2,2);
  pos->set_p(this);
  this->pos = pos;
}

Queen::Queen(bool white, Pos *pos) : Piece(white, pos) {
  cout << "\nCurrent path is: " << filesystem::current_path() << endl;
  texture.loadFromFile("chess_pieces_sprite.png", sf::IntRect(55, (white) ? 0 : 55, 55, 50));
  sprite.setTexture(texture);
  sprite.setScale(2,2);
  pos->set_p(this);
  this->pos = pos;
}

void Piece::move(Pos* pos) {
  this->pos->p = nullptr;
  Pos prev = *this->pos;
  this->pos = pos;
  pos->set_p(this);
  sprite.setPosition(pos->rect.getPosition());
  Move m(prev, *pos);
  board->handle_move(m);
}
void Pawn::move(Pos* pos) {
  this->pos->p = nullptr;
  Pos prev = *this->pos;
  this->pos = pos;
  pos->set_p(this);
  in_start_pos = false;
  sprite.setPosition(pos->rect.getPosition());
  Move m(prev, *pos);
  board->handle_move(m);
}

// return true when break
bool Piece::check_pos_and_handle_it(int x, int y) {

  if (x > 7 || y > 7 || x < 0 || y < 0) {
    return true;
  }

  Pos* target = board->at(x, y);

  if (target->p) {
    if (target->p->white != this->white) {
            board->white->king->recompute();
      valid_pos.push_back(target->p->pos);
    }
    return true;
  }

  valid_pos.push_back(target);
  return false;
}
// ------------------------------------------------------------------------------------------------------------------

void Rook::update(Move &m) {
  /*
  bool check = false;
  for (Pos *p : valid_pos) {
    if (*p == m.from || *p == m.to) {
      check = true;
    }
  }
  if (!check)
    return;
*/
  this->recompute();
}

void Rook::recompute() {

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
  this->recompute();
}

void Bishop::recompute() {

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

  this->recompute();
}

void Knight::recompute() {

  valid_pos.clear();

  check_pos_and_handle_it(pos->x + 2, pos->y + 1);
  check_pos_and_handle_it(pos->x + 2, pos->y - 1);

  check_pos_and_handle_it(pos->x - 2, pos->y + 1);
  check_pos_and_handle_it(pos->x - 2, pos->y - 1);

  check_pos_and_handle_it(pos->x + 1, pos->y + 2);
  check_pos_and_handle_it(pos->x + 1, pos->y - 2);

  check_pos_and_handle_it(pos->x - 1, pos->y + 2);
  check_pos_and_handle_it(pos->x - 1, pos->y - 2);
}

void Queen::update(Move &m) {

  this->recompute();
}

void Queen::recompute() {

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
  this->recompute();
}

bool King::check_expanded(int x, int y) {

  if (x < 0 || x >= 8 || y < 0 || y >= 8) {
    return true;
  }

  Pos* temp = board->at(x,y);

  if (temp->p) {
    if (temp->p->white == white) {
      return true;
    }
  }

  bool is_defended = false;

  for (Piece* piece : (white) ? board->black->pieces : board->white->pieces) {
    if (piece->ch() == 'P') {
      if (piece->pos->x + 1 < 8 && piece->pos->y - 1 >= 0) {
        Pos* target = board->at(pos->x + 1, pos->y - 1);
        if (temp == target) {
          is_defended = true;
          return true;
        }
      }

      if (pos->x - 1 >= 0 && pos->y - 1 >= 0) {
        Pos* target = board->at(pos->x - 1, pos->y - 1);
        if (temp == target) {
          is_defended = true;
          return true;
        }
      }
    }else if (piece->ch() == 'p'){
      if (pos->x + 1 < 8 && pos->y + 1 < 8) {
        Pos* target = board->at(pos->x + 1, pos->y + 1);
        if (temp == target) {
          is_defended = true;
          return true;
        }
        if (target == this->pos) {
          in_check = true;
        }
      }

      if (pos->x - 1 >= 0 && pos->y + 1 < 8) {
        Pos* target = board->at(pos->x - 1, pos->y + 1);
        if (temp == target) {
          is_defended = true;
          return true;
        }
      }
    }else {
      for (Pos* pos : piece->valid_pos) {
        if (pos == temp) {
          is_defended = true;
          return true;
        }

        if (pos == this->pos) {
          in_check = true;
        }
      }
    }
  }

  valid_pos.push_back(board->at(x,y));
  return false;
}

void King::recompute() {

  valid_pos.clear();

  for (Piece* piece : (white) ? board->black->pieces : board->white->pieces) {
    for (Pos* pos : piece->valid_pos) {
      if (pos->p && pos == this->pos) {
        in_check = true;
      }
    }
  }

  check_expanded(pos->x + 1, pos->y);
  check_expanded(pos->x - 1, pos->y);

  check_expanded(pos->x, pos->y + 1);
  check_expanded(pos->x, pos->y - 1);

  check_expanded(pos->x - 1, pos->y - 1);
  check_expanded(pos->x - 1, pos->y + 1);

  check_expanded(pos->x + 1, pos->y - 1);
  check_expanded(pos->x + 1, pos->y + 1);

  if (in_check && valid_pos.empty()) {
    board->mate = true;
  }

}

void Pawn::update(Move &m) {
  this->recompute();
}

void Pawn::recompute() {

  valid_pos.clear();

  Pos* target = nullptr;

  // moving
  if (white) {

    if (pos->y - 1 >= 0) {
      target = board->at(pos->x, pos->y - 1);
      if (!target->p)valid_pos.push_back(target);
    }

    if (in_start_pos) {
      if (pos->y - 2 >= 0) {
        target = board->at(pos->x, pos->y - 2);
        if (!target->p) {
          valid_pos.push_back(target);
        }
      }
    }


    // capture
    if (pos->x + 1 < 8 && pos->y - 1 >= 0) {
      target = board->at(pos->x + 1, pos->y - 1);
      if (target->p && target->p->white != this->white) {
        valid_pos.push_back(target->p->pos);
      }

    }

    if (pos->x - 1 >= 0 && pos->y - 1 >= 0) {
      target = board->at(pos->x - 1, pos->y - 1);
      if (target->p && target->p->white != this->white) {
        valid_pos.push_back(target->p->pos);
      }
    }
  }else {

    if (pos->y + 1 < 8) {
      target = board->at(pos->x, pos->y + 1);
      if (!target->p)valid_pos.push_back(target);
    }

    if (in_start_pos) {
      if (pos->y + 2 < 8) {
        target = board->at(pos->x, pos->y + 2);
        if (!target->p) {
          valid_pos.push_back(target);
        }
      }
    }


    // capture
    if (pos->x + 1 < 8 && pos->y + 1 < 8) {
      target = board->at(pos->x + 1, pos->y + 1);
      if (target->p && target->p->white != this->white) {
        valid_pos.push_back(target->p->pos);
      }
    }

    if (pos->x - 1 >= 0 && pos->y + 1 < 8) {
      target = board->at(pos->x - 1, pos->y + 1);
      if (target->p && target->p->white != this->white) {
        valid_pos.push_back(target->p->pos);
      }
    }
  }

}
