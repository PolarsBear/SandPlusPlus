#include "particle_table.h"
#include <iostream>

ParticleTable::ParticleTable() = default;

ParticleTable::ParticleTable(int size) {
    
    for (uint x = 0; x != uint(size); x++) {
        for (uint y = 0; y != uint(size); y++) {
            table[std::pair<uint,uint>(x,y)] = Particle::Air(x,y);
        }
    }
}

Particle ParticleTable::get(uint x, uint y){
    return table[std::pair<uint,uint>(x,y)];
}

void ParticleTable::set(uint x, uint y, Particle value) {
    value.x = x;
    value.y = y;
    table[std::pair<uint,uint>(x,y)] = value;
    value.draw();
}

void ParticleTable::update() {
    for (auto& a : table) {
        auto v = a.second;
        if (v.id != 0) {
            v.update();
        }
    }
}