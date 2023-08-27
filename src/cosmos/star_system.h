#include <cstdint>
#include <vector>
#include <utility>

class RandomContext {
    uint32_t lehmer_value;
public:
    RandomContext(uint32_t seed) : lehmer_value(seed) {

    }
    // A fast, reasonably robust random numbers
    inline uint32_t lehmer32_random() {
        lehmer_value += 0x3120fc15;
        uint64_t tmp;
        tmp = (uint64_t)lehmer_value * 0x4a39b70d;
        uint32_t m1 = (tmp >> 32) ^ tmp;
        tmp = (uint64_t)m1 * 0x12fad5c9;
        uint32_t m2 = (tmp>>32) ^ tmp;
        return m2;
    }
    int an_int(int min, int max) {
        return (lehmer32_random() % (max-min)) + min;
    }
    float a_float(float min, float max) {
        return ((float)lehmer32_random() / (float)(0x7FFF)) * (max-min) + min;
    }
    double a_double(float min, float max) {
        return ((double)lehmer32_random() / (double)(0x7FFF'FFFF)) * (max-min) + min;
    }
};


class Moon {
public:
    double distance = 0.0;
    double diameter = 0.0;
    double foliage = 0.0;
    double minerals = 0.0;
    double water = 0.0;
    double gases = 0.0;
    double temperature = 0.0;
    double population = 0.0;
    bool ring = false;
};

class Station {
public:
    int type;
    double population = 0.0;
};

class Planet {
public:
    double distance = 0.0;
    double diameter = 0.0;
    double foliage = 0.0;
    double minerals = 0.0;
    double gases = 0.0;
    double water = 0.0;
    double temperature = 0.0;
    double population = 0.0;
    bool ring = false;
    std::vector<Moon> moons;
    std::vector<Station> stations;
};



class StarSystem {
    bool exists = false;
    float star_diameter = 0.0;
    int star_color = 0;
    std::vector<Planet> planets;
    RandomContext rnd;
public:    
    StarSystem(uint32_t x, uint32_t y, bool generate_children, uint32_t seed) : rnd(seed + (x&0xFFFF) <<16 | (y&0xFFFF)){
        // 16 bit resolution of the system by masking x,y
        // this should allow for a wrapping of the universe

        // Is this an existing start system
        // odds 1 in 20
        exists = (rnd.an_int(0,20) == 1);
        if (!exists) {
            return;
        }
        star_diameter = rnd.a_float(10.0, 40.0);
        star_color = rnd.an_int(0,8);

        // If the children are not needed skip
        if (!generate_children) return;
        double distance = rnd.a_double(60.0, 200.0);
        int planet_count = rnd.an_int(0,10);

        for (int i=0;i< planet_count; i++) {
            Planet p;
            p.distance = distance;
            distance += rnd.a_double(60.0, 200.0);
            p.diameter = rnd.a_double(4.0, 20.0);
            p.temperature = rnd.a_double(-200.0, 300);
            // Make up
            p.foliage = rnd.a_double(0.0, 1.0);
            p.minerals = rnd.a_double(0.0, 1.0);
            p.gases = rnd.a_double(0.0, 1.0);
            p.water = rnd.a_double(0.0, 1.0);
            // Normalize make up 
            double sum = 1.0 / (p.foliage + p.minerals + p.gases + p.water);
            p.foliage *= sum;
            p.minerals *= sum;
            p.gases *= sum;
            p.water *= sum;

            p.population = std::max(rnd.an_int(-5'000'000, 20'000'000), 0);
            p.ring = rnd.an_int(0,10) == 1;

            double station_orbit_distance = rnd.a_double(0.5, 2.0);
            int stations = std::max(rnd.an_int(-5, 5), 0);
            for (int s=0; s<stations; s++) {
                Station station;
                station.type = rnd.an_int(0, 5);
                station.population = std::max(rnd.an_int(-500*station.type, 2000*station.type), 0);
                p.stations.push_back(station);
            }

            double orbit_distance = rnd.a_double(6.0, 20.0);

            int moon_count = std::max(rnd.an_int(-5, 5), 0);
            for (int m=0; m<moon_count; m++) {
                Moon moon;
                moon.distance = distance;

                orbit_distance += rnd.a_double(0.4, 2.0);
                moon.diameter = rnd.a_double(0.4, 2.0);


                moon.temperature = rnd.a_double(-200.0, 300);
                // Make up
                moon.foliage = rnd.a_double(0.0, 1.0);
                moon.minerals = rnd.a_double(0.0, 1.0);
                moon.gases = rnd.a_double(0.0, 1.0);
                moon.water = rnd.a_double(0.0, 1.0);
                // Normalize make up 
                double sum = 1.0 / (moon.foliage + moon.minerals + moon.gases + moon.water);
                moon.foliage *= sum;
                moon.minerals *= sum;
                moon.gases *= sum;
                moon.water *= sum;

                moon.population = std::max(rnd.an_int(-500'000, 2'000'000), 0);
                moon.ring = rnd.an_int(0,10) == 1;
                p.moons.push_back(moon);
            }
            planets.push_back(p);
        }
        
    }
};

class Galaxy {
    

};