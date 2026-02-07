#include <iostream>

#include <vector>
#include <unistd.h>
//#include "Chessboard.cpp"

using namespace std;

const char abc[8] = {'a','b','c','d','e','f','g','h'};

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

    virtual void update(Move&) const = 0;

    Piece(bool white, Pos* p) : pos(p),white(white) {}

   ~Piece(){
        for(Pos*p : valid_pos) { p = nullptr; }
    }

};

class Rook : public Piece{
public:
    
    using Piece::Piece;

    char ch() const override{ return (white) ? 'R' : 'r'; }

    
    void update(Move& m) const override;
    
};

class Bishop : public Piece{
public:
    
    using Piece::Piece;

    char ch() { return (white) ? 'B' : 'b'; }

    
    void update(Move& m) const override;
};

class Pos{
public:

    char c;
    int y;
    int x;
    Piece* p;

    Pos(char c, int p, Piece* pic = nullptr) {
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
        if(!pic) {
            c = ' ';
        }else{
            c = pic->ch();
        }

    }

    Pos() : c('0'), y(0), x(0) {}

    bool operator==(Pos& other){
        return (other.x == x && other.y == y);
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
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                pixels.at(j).at(i) = new Pos(i + 48, j);
            }  
        }
        Rook r()
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

    Grid grid;

    Chessboard() {}

    bool mate = false;

    Player* white;
    Player* black;

    void init(Player* white,Player* black){

        grid.pixels.clear();
        this->white = white;
        this->black = black;

        this->white->pieces.clear();
        this->black->pieces.clear();


        grid.pixels.resize(8);
        for(int i =0;i<8;i++){
            grid.pixels.at(i).resize(8);
            
        }

        this->white->pieces.resize(16);
        this->black->pieces.resize(16);
        
        

        this->white->pieces.at(0) = new Rook(true, grid.at(get_num('a'), 0));
        this->white->pieces.at(1) = new Rook(true, grid.at(get_num('a'), 7));

    }

    void show(){
        cout << "----------------\n";
        for(vector<Pos*> v : grid.pixels){
            cout << "|";
            for(Pos* p : v){
                cout << p->c << "|";
            }
            cout << "\n----------------\n";
        }
    }

    void handle_move(Move m, string col){

        
    }


};

//******************************************************************************************************************************************************************* */





void Rook::update(Move& m) const {
    for (Pos* p:valid_pos){
        if(*p == m.from);
    }

}
    

int main(){

    Chessboard board;
cout << "Hello";
    Player* p1 = new Player();

    Player* p2 = new Player();

    board.init(p1,p2);
    
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