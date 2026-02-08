#include <SFML/Graphics/RectangleShape.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include <SFML/Graphics.hpp>

using namespace std;

const char abc[8] = {'a','b','c','d','e','f','g','h'};

const float TILE_SIZE = 100.f;

// Return true if char in a-h
bool check_abc(char c){
    
    for(char t : abc){
        if(c == t){
            return true;
        }
    }

    return false;
}

int get_num(char c){
    if(!check_abc(c)){
        std::cerr << "\nInvalid char";
        abort();
        return -1;
    }

    for(int i =0;i<8;i++){
        if(c == abc[i]){
            return i;
        }
    }
    return -1;
}

class Pos;
class Move;
class Chessboard;

class Piece{

    char c = '0';

public:

    static Chessboard* board;

    Pos* pos;

    bool white;
    
    vector<Pos*> valid_pos;

    virtual char ch() const = 0;

    virtual void update(Move&) = 0;

    virtual bool check_pos_and_handle_it(int x, int y) = 0;

    virtual void recompute() = 0;

    Piece(bool white, Pos* p) : pos(p),white(white) {}

   ~Piece(){
        for(Pos*p : valid_pos) { p = nullptr; }
    }

};

class Rook : public Piece{
public:
    
    using Piece::Piece;

    static Chessboard* board;

    char ch() const override{ return (white) ? 'R' : 'r'; }

    void update(Move& m) override;
    
    bool check_pos_and_handle_it(int x, int y) override;

    void recompute() override;
};

class Bishop : public Piece{
public:
    
    using Piece::Piece;

    static Chessboard* board;

    char ch() { return (white) ? 'B' : 'b'; }
  
    void update(Move& m) override;
    
    bool check_pos_and_handle_it(int x, int y) override;
    
    void recompute() override;
};

class Pos{
public:

    char pixel;
    char c;
    int y;
    int x;
    Piece* p;
    sf::RectangleShape rect;

    Pos(char c, int p, Piece* pic = nullptr, char pix = ' ') {
        if(p < 0 || p > 8){
            cerr << "\nWrong char";
            abort();
        }

        if(!check_abc(c)){
            cerr << "\nWrong char";
            abort();
        }

        this->y = get_num(c);
        this->c = c;

        if(pic){
            pixel = pic->ch();
        }else{
            pixel = pix;
        }

    }

    Pos() : c('0'), y(0), x(0) {}

    bool operator==(Pos& other){
        return (other.x == x && other.y == y);
    }

    void set_p(Piece* p){
        this->p = p;
        pixel = p->ch();
        x = p->pos->x;
        y = p->pos->y;
        c = p->pos->c;
    }
};

class Move{
public:

    Pos from;
    Pos to;

    Move(char from, int i_from, char to, int i_to){
        this->from = Pos(from, i_from);
        this->to = Pos(to,i_to);
    }

    Move() : from(Pos()), to(Pos()) {}
};

Move get_move(){

    cout << "\nEnter move from[a-h,0-7] to[]: ";

    char from;
    cin >> from;

    int i_from;
    cin >> i_from;

    char  to;
    cin >> to;

    int i_to;
    cin >> i_to;

    return Move(from,i_from, to,i_to);
}

class Player{
public:


    vector<Piece*> pieces;

};

class Grid{
public:

    vector<vector<Pos*>> pixels;

    
    Grid(){
        pixels.resize(8);
        for(int i=0;i<8;i++){
            pixels.at(i).resize(8);
            for(int j=0;j<8;j++){
                //cout << "i + 48 is: " << char(i + 97) << "\n";
                pixels.at(i).at(j) = new Pos(char(i + 97), j);
            }  
        }
        Rook r(true, at(4, 4));
    }

    Pos* at(int x, int y){
        if(x < 0 || x > 7 || y < 0 || y > 7){
            cerr << "Wrong index in Grid::at()";
            abort();
        }
        return pixels.at(y). at(x);
    }

};

class Chessboard{

public:

    Grid* grid;

    Chessboard(Grid* g) : grid(g) {}

    bool mate = false;

    Player* white;
    Player* black;

    Pos* at(int x, int y){
        return grid->at(x, y);
    }

    void init(Player* white,Player* black){

        cout << "Init started\n";

        grid->pixels.clear();
        this->white = white;
        this->black = black;

        this->white->pieces.clear();
        this->black->pieces.clear();

        cout << "Creating pixels\n";
        grid->pixels.resize(8);
        for(int i =0;i<8;i++){
            grid->pixels.at(i).resize(8);
            for(int j=0;j<8;j++){
                grid->pixels.at(i).at(j) = new Pos(char(i + 97), j, nullptr, ' ');

                grid->pixels.at(i).at(j)->rect.setSize({TILE_SIZE,TILE_SIZE});
                grid->pixels.at(i).at(j)->rect.setOutlineThickness(0.5);
            }  
        }

        this->white->pieces.resize(16);
        this->black->pieces.resize(16);
        
        
        cout << "Creating pieces\n";
        this->white->pieces.at(0) = new Rook(true, grid->at(get_num('a'), 0));
        this->white->pieces.at(1) = new Rook(true, grid->at(get_num('a'), 7));

        at(4,4)->set_p( this->white->pieces.at(0));
        at(4,1)->set_p( this->white->pieces.at(1));


        at(0,0)->pixel = '0';

    }

    void show(){
        cout << "\ngrid pixels has size: " << grid->pixels.size();

        cout << "----------------\n";
        char c = 104;
        for(int j=0;j<8;j++){
            vector<Pos*>& v = grid->pixels.at(j); 
            cout << "|" << c-- << "|";
            for(int i =0;i<v.size();i++){
                if(!v.at(i)){
                    cerr << "v at(" << i << ") is nullptr";
                }
                cout << v.at(i)->pixel << "|";
            }
            cout << "\n----------------\n";
        }
        cout << "| |1|2|3|4|5|6|7|8|\n";
    }

    void handle_move(Move m, string col){

        if(col == "white"){
            for(Piece* p : white->pieces){
                p->update(m);
            }
        }
        else if(col == "black"){
            for(Piece* p : white->pieces){
                p->update(m);
            }
        }else {
            cerr << "Failed";
            abort();
        }
    }


};

//******************************************************************************************************************************************************************* */





void Rook::update(Move& m) {
    bool check = false;
    for (Pos* p:valid_pos){
        if(*p == m.from || *p == m.to){
            check = true;
        }
    }
    if(!check)return;

    this->recompute();
}
    
// return true when break
bool Rook::check_pos_and_handle_it(int x, int y){
    if(board->at(x, y)->p){
        if(board->at(x, y)->p->white == white){
                    
        }else{
            valid_pos.push_back(board->at(x, y)->p->pos);
        }
        return true;
    }else{
        valid_pos.push_back(board->at(x, y)->p->pos);
    }
    return false;
}
// return true when break
bool Bishop::check_pos_and_handle_it(int x, int y){
    if(board->at(x, y)->p){
        if(board->at(x, y)->p->white == white){
                    
        }else{
            valid_pos.push_back(board->at(x, y)->p->pos);
        }
        return true;
    }else{
        valid_pos.push_back(board->at(x, y)->p->pos);
    }
    return false;
}
void Rook::recompute() {

    for(auto* p : valid_pos){
        delete p;
    }

    valid_pos.clear();


    // right
    for(int j=pos->x + 1;j<8;j++){

        if(check_pos_and_handle_it(j, pos->y)){
            break;
        }

    }
    
    // left
    for(int j=pos->x - 1;j>=0;j--){

        if(check_pos_and_handle_it(j, pos->y)){
            break;
        }

    }

    // up
    for(int j=pos->y + 1;j<8;j++){

        if(check_pos_and_handle_it(pos->x, j)){
            break;
        }

    }

    // down
    for(int j=pos->y - 1;j>=0;j--){

        if(check_pos_and_handle_it(pos->x, j)){
            break;
        }

    }
}


void Bishop::update(Move& m) {
    bool check = false;
    for (Pos* p:valid_pos){
        if(*p == m.from || *p == m.to){
            check = true;
        }
    }
    if(!check)return;

    this->recompute();
}
    
void Bishop::recompute() {

    for(auto* p : valid_pos){
        delete p;
    }

    valid_pos.clear();

    // up-right
    for(int j=1;pos->x + j < 8 && pos->y + j < 8;j++){

        if(check_pos_and_handle_it(pos->x + j,pos->y + j)){
            break;
        }

    }
    
    // up-left
    for(int j=1;pos->x - j >= 0 && pos->y + j < 8;j++){

        if(check_pos_and_handle_it(pos->x - j,pos->y + j)){
            break;
        }

    }

    // down-right
    for(int j=1;pos->x + j < 8 && pos->y - j >= 0;j++){

        if(check_pos_and_handle_it(pos->x + j,pos->y - j)){
            break;
        }

    }

    // down-left
    for(int j=1;pos->x - j >= 0 && pos->y - j >= 0;j++){

        if(check_pos_and_handle_it(pos->x - j,pos->y - j)){
            break;
        }

    }
}

Chessboard* Rook::board = nullptr;

Chessboard* Bishop::board = nullptr;

int main(){

    sf::RenderWindow entry(sf::VideoMode(TILE_SIZE * 8,TILE_SIZE * 8), "Chess");

    Grid g;

    Chessboard board(&g);
    
    Rook::board = &board;
    Bishop::board = &board;

    cout << "Hello";
    
    Player* p1 = new Player();

    Player* p2 = new Player();

    board.init(p1,p2);
    
    cout <<"Starting loop\n";
    sleep(2);

    while (entry.isOpen()) {
        sf::Event event;
        while (entry.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                entry.close();
        }

        entry.clear();
        entry.draw(shape);
        entry.display();
    }


    bool is_white = true;
    while(!board.mate){
        board.show();
        sleep(1);

        Move m;
        m = get_move();

        if(is_white){
            //board.handle_move(m, "white");
        }else{
            //board.handle_move(m, "black");
        }
        cin.get();
        is_white = !is_white;
    }
    return 0;
}