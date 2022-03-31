#include <string>
#include "color.h"

class Particle {
    public:
        Particle(uint x, uint y, Color3 color, std::string name, uint id, int density);
        Particle();
        static Particle Air(uint x, uint y);
        static Particle Sand(uint x, uint y);
        static Particle Water(uint x, uint y);
        void update();
        void draw();
        bool fall();
        bool liquidSpread();
        bool floatUp();
        uint x;
        uint y;
        Color3 color;
        std::string name;
        uint id;
        int density;
};