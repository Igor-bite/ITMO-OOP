#include "Engine.cpp"
using namespace std;

int main(int argc, const char * argv[]) {
    BactrianCamel tr1;
    FastCamel tr2;
    Centaur tr3;
    AllTerrainBoots tr4;

    Stupa tr5;
    MagicCarpet tr6;
    Broom tr7;
    Stupa tr8;
    MagicCarpet tr9;
    Broom tr10;

    Race<Transport> race1 = Race<Transport>({9999, 0});
    Race<AirTransport> race2 = Race<AirTransport>({654, 0});
    Race<GroundTransport> race3 = Race<GroundTransport>({85, 0});
    Race<Transport> race4 = Race<Transport>({5322, 0});

    race1.registrate(&tr1);
    race1.registrate(&tr7);

    race3.registrate(&tr2);
    race3.registrate(&tr3);
    race3.registrate(&tr4);

    race2.registrate(&tr5);
    race2.registrate(&tr6);

    race4.registrate(&tr8);
    race4.registrate(&tr9);
    race4.registrate(&tr10);

    RaceEngine engine = RaceEngine();
    engine.addGeneralRace(race1);
    engine.addAirRace(race2);
    engine.addGroundRace(race3);
    engine.addGeneralRace(race4);
//    engine.getInfo();

//     cout << "Winner has id " << race1.whoWins()->getID() << endl;

    engine.allWinners();

    return 0;
}
