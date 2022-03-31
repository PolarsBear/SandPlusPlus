# Sand++
Sand made in c++ in a week, I wanted to learn how to code in c++

## Building
You can simply build it with `g++ -std=gnu++17 main.cpp color.cpp particle.cpp particle_table.cpp -lglut -lGLU -lGL`

You'll need OpenGL and glut tho

## Running
It has two command line arguments: `-tps` and `-size`. `-tps` changes the speed at which the game will update the cells, in Ticks Per Second (TPS), you can use it like so: `-tps=50`. `-size` **SHOULD** change the size of the game, but for now, all it does is NOT WORK properly, you can use it like so: `-size=500`.
