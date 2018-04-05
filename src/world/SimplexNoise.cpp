/*  Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com>

    Noise calculation based on code by
    Stefan Gustavson <stegu@itn.liu.se> &
    Peter Eastman <peastman@drizzle.stanford.edu>
    itn.liu.se/~stegu/simplexnoise/SimplexNoise.java

    Octave calculation based on code by
    matheus23 @ http://www.java-gaming.org/index.php?topic=31637.0
*/

#include "SimplexNoise.hpp"

namespace mc::world {

static const double F2 = 0.5 * (sqrt(3.0) - 1.0);
static const double G2 = (3.0 - sqrt(3.0)) / 6.0;
static constexpr int gradients[][2] = { { 1, 1 }, { -1, 1 }, { 1, -1 },
                                        { -1, -1 }, {1, 0 }, { -1, 0 },
                                        { 1, 0 }, { -1, 0 }, { 0, 1 },
                                        { 0, -1 }, { 0, 1 }, { 0, -1 } };

static double dot(int gradX, int gradY, double x, double y) {
  return gradX * x + gradY * y;
}

SimplexNoise::SimplexNoise():
    SimplexNoise(static_cast<uint32_t>(std::random_device{ }())) {
}

SimplexNoise::SimplexNoise(uint32_t seed_):
    seed { seed_ },
    rng { seed_ } {

    permutation.resize(256);

    iota(permutation.begin(), permutation.end(), 0);
    shuffle(permutation.begin(), permutation.end(), rng);
    permutation.insert(permutation.end(), permutation.begin(), permutation.end());
    for(auto iter = permutation.begin() + 256; iter < permutation.end(); iter++){
        *iter = (*iter) % 12;
    }
}

uint32_t SimplexNoise::GetSeed() const{
    return seed;
}

double SimplexNoise::GetNoise(double x, double y) const{

    double n0 = 0, n1 = 0, n2 = 0; // Noise contributions from the three corners
    // Skew the input space to determine which simplex cell we're in
    double s = (x + y) * F2; // Hairy factor for 2D

    int i = static_cast<int>(x + s);
    int j = static_cast<int>(y + s);

    double t = (i + j) * G2;

    double X0 = i - t; // Unskew the cell origin back to (x,y) space
    double Y0 = j - t;
    double x0 = x - X0; // The x,y distances from the cell origin
    double y0 = y - Y0;

    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    int i1 = 0, j1 = 0; // Offsets for second (middle) corner of simplex in (i,j) coords
    if(x0 > y0){
        i1 = 1;
        j1 = 0;
    } else {    // lower triangle, XY order: (0,0)->(1,0)->(1,1)
        i1 = 0;
        j1 = 1;
    }      // upper triangle, YX order: (0,0)->(0,1)->(1,1)

    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where

    // c = (3-sqrt(3))/6
    double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    double y1 = y0 - j1 + G2;
    double x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
    double y2 = y0 - 1.0 + 2.0 * G2;

    // Work out the hashed gradient indices of the three simplex corners
    int ii = i & 0xFF;
    int jj = j & 0xFF;

    int gi0 = permutation.at(256 + ((ii + permutation.at(jj % 256)) % 256));
    int gi1 = permutation.at(256 + ((ii + i1 + permutation.at((jj + j1) % 256)) % 256));
    int gi2 = permutation.at(256 + ((ii + 1 + permutation.at((jj + 1) % 256)) % 256));

    // Calculate the contribution from the three corners
    double t0 = 0.5 - x0 * x0 - y0 * y0;
    if(t0 < 0){
        n0 = 0.0;
    }
    else {
      t0 *= t0;
      n0 = t0 * t0 * dot(gradients[gi0][0], gradients[gi0][1], x0, y0);  // (x,y) of grad3 used for 2D gradient
    }
    double t1 = 0.5 - x1 * x1 - y1 * y1;
    if(t1 < 0){
        n1 = 0.0;
    }
    else {
      t1 *= t1;
      n1 = t1 * t1 * dot(gradients[gi1][0], gradients[gi1][1], x1, y1);
    }
    double t2 = 0.5 - x2 * x2 - y2 * y2;
    if(t2 < 0){
        n2 = 0.0;
    }
    else {
      t2 *= t2;
      n2 = t2 * t2 * dot(gradients[gi2][0], gradients[gi2][1], x2, y2);
    }

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [-1,1].
    return 70.0 * (n0 + n1 + n2);
}

double SimplexNoise::GetOctavedNoise(int x, int y,
                                     int octaves,
                                     double roughness,
                                     double scale) const {
    double sum = 0;
    double frequency = scale;
    double weight = 1;
    double weightSum = 0;

    for(int octave = 0; octave < octaves; octave++){
        sum +=  GetNoise(x * frequency, y * frequency) * weight;
        frequency *= 2;
        weightSum += weight;
        weight *= roughness;
    }
    return sum / weightSum;
}

}   // namespace mc::world::chunk
