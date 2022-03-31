#include <tuple>
#include <map>
#include "particle.h"

class ParticleTable {
    public:
        ParticleTable();
        ParticleTable(int size);

        Particle get(uint x, uint y);
        void set(uint x, uint y, Particle value);

        void update();
        void draw();

    private:
        std::map<std::pair<uint,uint>, Particle> table;
};