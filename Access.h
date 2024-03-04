#include <stdint.h>

struct Position{
    int x,y,z,zx,zy;

};

Position GetPosition(int i){
    Position out;
    out.y = i%9;
    out.x = i/9;
    if(out.y==0){out.y=9;out.x--;}
    

}