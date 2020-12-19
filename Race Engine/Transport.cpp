#include <cmath>
using namespace std;

class Coordinate {
public:
    Coordinate() = default;

    Coordinate(double x, double y) : x(x) , y(y) {}

    double distanceTo(Coordinate pos) {
        return sqrt((pos.x-x)*(pos.x-x) + (pos.y-y)*(pos.y-y));
    }

    double x;
    double y;
};

class Transport {
public:
    void setID(int uid) {
        id = uid;
    }

    int getID() {
        return id;
    }
    virtual double MoveTo(Coordinate pos) = 0;
protected:
    Coordinate position;
    double speed;
    int id;
};

class AirTransport : public Transport {
public:
    double MoveTo(Coordinate pos) override {
        double distance = changeDistance(position.distanceTo(pos));
        double time = distance / speed;
        position = pos;

        return time;
    }
protected:
    virtual double changeDistance(double distance) = 0;
};

class GroundTransport : public Transport {
public:
    double MoveTo(Coordinate pos) override {
        double distance = position.distanceTo(pos);
        double time = 0;
        while (distance != 0) {
            double distBeforeRest = speed * restInterval;
            if (distance >= distBeforeRest) {
                time += restInterval;
                time += restDuration;

                changeRestDuration();
                distance -= distBeforeRest;
            } else {
                time += distance / speed;
                distance = 0;
            }
        }
        position = pos;

        return time;
    }
protected:
    virtual void changeRestDuration() = 0;
    int recoveries;
    double restDuration;
    double restInterval;
};

// MARK: - Ground Transport

class BactrianCamel : public GroundTransport {
public:
    BactrianCamel() {
        speed = 10;
        position = Coordinate(0, 0);
        recoveries = 0;
        restDuration = 5;
        restInterval = 30;
    }
private:
    void changeRestDuration() override {
        recoveries++;
        restDuration = 8;
    }
};

class FastCamel : public GroundTransport {
public:
    FastCamel() {
        speed = 10;
        position = Coordinate(0, 0);
        recoveries = 0;
        restDuration = 5;
        restInterval = 10;
    }
private:
    void changeRestDuration() override {
        recoveries++;
        switch (recoveries) {
            case 2:
                restDuration = 6.5;
                break;

            default:
                restDuration = 8;
                break;
        }
    }
};


class Centaur : public GroundTransport {
public:
    Centaur() {
        speed = 15;
        position = Coordinate(0, 0);
        recoveries = 0;
        restDuration = 2;
        restInterval = 8;
    }
private:
    void changeRestDuration() override {
        recoveries++;
        restDuration = 2;
    }
};

class AllTerrainBoots : public GroundTransport {
public:
    AllTerrainBoots() {
        speed = 6;
        position = Coordinate(0, 0);
        recoveries = 0;
        restDuration = 10;
        restInterval = 60;
    }
private:
    void changeRestDuration() override {
        recoveries++;
        restDuration = 5;
    }
};

// MARK: - Air Transport

class MagicCarpet : public AirTransport {
public:
    MagicCarpet() {
        speed = 10;
    }
private:
    double changeDistance(double distance) override {
        int decrease;
        if (distance < 1000) {
            decrease = 0;
        } else if (distance < 5000) {
            decrease = 3;
        } else if (distance < 10000) {
            decrease = 10;
        } else {
            decrease = 5;
        }

        double koeff = 1 - decrease/100;
        return distance * koeff;
    }
};

class Stupa : public AirTransport {
public:
    Stupa() {
        speed = 8;
    }

    double changeDistance(double distance) override {
        int decrease = 6;

        double koeff = 1 - decrease/100;
        return distance * koeff;
    }
};

class Broom : public AirTransport {
public:
    Broom() {
        speed = 200;
    }
private:
    double changeDistance(double distance) override {
        int count = distance / 1000;
        double result = 0;
        int decrease = 0;

        for (int i = 0; i < count; ++i) {
            double koeff = 1 - decrease/100;
            result += 1000 * koeff;
            decrease++;
        }

        int remaining = distance - count*1000;
        double koeff = 1 - decrease/100;
        result += remaining * koeff;
        return distance;
    }
};

