#include <iostream>
#include <random>
#include <sstream>
#include <utility>
using namespace std;

namespace uuid {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::string generate_uuid_v4() {
        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++) {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++) {
            ss << dis(gen);
        };
        return ss.str();
    }
}

class Unit {
public:
    Unit() = default;

    Unit(string _name) {
      name = _name;
      UUID = uuid:: generate_uuid_v4();
    }

    Unit(const Unit &unit) : name(unit.getName()), UUID(unit.getUUID()) {}

    string getName() const {
        return name;
    }

    string getUUID() const {
        return UUID;
    }

    bool isEqualTo(Unit unit) const {
        if (unit.getUUID() == UUID) {
            return true;
        } else {
            return false;
        }
    }

private:
    string UUID;
    string name;
};

class Batch {
public:
    Batch(Unit _unit, double _price, int _quantity) {
        unit = _unit;
        price = _price;
        quantity = _quantity;
    }

    int availableQuantityFor(double money) {
        int q = money / price;
        if (q <= quantity) {
            return q;
        } else {
            return quantity;
        }
    }

    void setPrice(double newPrice) {
        price = newPrice;
    }

    void setQuantity(int newQuantity) {
        quantity = newQuantity;
    }

    Unit getUnit() {
        return unit;
    }

    double getPrice() {
        return price;
    }

    int getQuantity() {
        return quantity;
    }

private:
    Unit unit;
    double price;
    int quantity;
};

class Group {
public:
    Group() {
        group = vector< pair<Unit, int>>();
    }

    void addNewPair(Unit u, int q) {
      group.push_back({u, q});
    }

    Unit getUnitAt(int index) const {
      return group[index].first;
    }

    int getQuantityAt(int index) const {
      return group[index].second;
    }

    int getQuantityOfUnits() {
      return group.size();
    }

    vector< pair<Unit, int>> getGroup() const {
      vector< pair<Unit, int>> newGroup = group;
      return newGroup;
    }

private:
  vector< pair<Unit, int>> group;
};

class Store {
public:

    Store(string _name, string _address) {
        name = _name;
        address = _address;
    }

    string getName() {
      return name;
    }

    void printAll() {
        for (auto batch : goods) {
            cout << batch.getUnit().getName() << " -> " << batch.getPrice() << " -> " << batch.getQuantity() << endl;
        }
    }

    int findUnit(Unit unit) {
        for (int i = 0; i < goods.size(); i++) {
            if (goods[i].getUnit().isEqualTo(unit)) {
                return i;
            }
        }
        string str = "There is no this unit with name " + unit.getName();
        throw str;
    }

    void incomeBatch(Unit unit, double price, int quantity) {
        int index;
        bool unitExists = false;
        for (int i = 0; i < goods.size(); i++) {
          if (goods[i].getUnit().isEqualTo(unit)) {
            index = i;
            break;
          }
        }
        if (unitExists) {
          goods[index].setQuantity(goods[index].getQuantity() + quantity);
          if (goods[index].getPrice() != price) {
            goods[index].setPrice(price);
          }
        } else {
          goods.push_back(Batch(unit, price, quantity));
        }
    }

    void incomeBatch(Batch batch) {
      Unit unit = batch.getUnit();
      double price = batch.getPrice();
      int quantity = batch.getQuantity();

      int index;
        bool unitExists = false;
        for (int i = 0; i < goods.size(); i++) {
          if (goods[i].getUnit().isEqualTo(unit)) {
            index = i;
            break;
          }
        }
        if (unitExists) {
          goods[index].setQuantity(goods[index].getQuantity() + quantity);
          if (goods[index].getPrice() != price) {
            goods[index].setPrice(price);
          }
        } else {
          goods.push_back(Batch(unit, price, quantity));
        }
    }

    Group opportunitiesToBuyOn(double money) {
      Group result = Group();
      for (Batch batch : goods) {
        int quantityForUnit = batch.availableQuantityFor(money);
        Unit curUnit = batch.getUnit();
        result.addNewPair(curUnit, quantityForUnit);
      }

      return result;
    }

    double getPriceFor(Unit u) {
        try {
            int index = findUnit(u);
            return goods[index].getPrice();
        } catch(string exception) {
            cerr << exception << endl;
        }
        return -1;
    }

    int getAvailableQuantityFor(Unit u) {
      try {
            int index = findUnit(u);
            return goods[index].getQuantity();
        } catch(string exception) {
            cerr << exception << endl;
      }
      return -1;
    }

    void buy(Unit u) {
        try {
            int index = findUnit(u);
            goods[index].setQuantity(goods[index].getQuantity() - 1);
            if (goods[index].getQuantity() == 0) {
                goods.erase(goods.begin() + index);
            }
        } catch(string exception) {
            cerr << exception << endl;
        }
    }

    void buy(Unit u, int q) {
        try {
            int index = findUnit(u);
            if (goods[index].getQuantity() >= q) {
                goods[index].setQuantity(goods[index].getQuantity() - q);
            } else {
                throw "There is not that much units";
            }

            if (goods[index].getQuantity() == 0) {
                goods.erase(goods.begin() + index);
            }
        } catch(string exception) {
            cerr << exception << endl;
        }
    }
    
    string getUUID() {
      return uuid;
    }

private:
    string uuid = uuid:: generate_uuid_v4();
    string name;
    string address;
    vector<Batch> goods = vector<Batch>();
};

class Chain {
public:
  Chain() {
    stores = vector<Store>();
  }

  void addStore(Store _store) {
    for (Store store : stores) {
      if (store.getUUID() == _store.getUUID()) {
        throw "Already exists";
      }
    }

    stores.push_back(_store);
  }

  vector<Store> cheapestUnit(Unit u) {
    double minPrice = MAXFLOAT;
    vector<Store> result = vector<Store>();
    for (Store store : stores) {
      double price = store.getPriceFor(u);
      if (price < minPrice) {
        minPrice = price;
        result.clear();
        result.push_back(store);
      } else if (price == minPrice) {
        result.push_back(store);
      }
    }

    return result;
  }

  vector<Store> cheapestGroup(Group g) {
    double minPrice = 100000.0;
    vector<Store> result = vector<Store>();

    for (Store store : stores) {
      double groupPrice = 0;
      for (int i = 0; i < g.getQuantityOfUnits(); ++i) {
        double price = store.getPriceFor(g.getUnitAt(i));
        int availableQuantity = store.getAvailableQuantityFor(g.getUnitAt(i));

        if (g.getQuantityAt(i) > availableQuantity) {
          break;
          groupPrice = 0;
        }
        groupPrice += price * g.getQuantityAt(i);
      }

      if (groupPrice == 0) {
        continue;
      } else if (groupPrice < minPrice) {
        minPrice = groupPrice;
        result.clear();
        result.push_back(store);
      } else if (groupPrice == minPrice) {
        result.push_back(store);
      }
    }

    return result;
  }

private:
  vector<Store> stores;
};

int main() {
  Chain chain = Chain();

  Store mag1 = Store("magname1", "address1");
  Store mag2 = Store("magname2", "address2");
  Store mag3 = Store("magname3", "address3");

  Unit a = Unit("unita");
  Unit b = Unit("unitb");
  Unit c = Unit("unitc");

  mag1.incomeBatch(a, 15.5, 1);
  mag1.incomeBatch(b, 17, 4);
  mag1.incomeBatch(c, 24, 78);
  mag1.printAll();

  mag2.incomeBatch(a, 13.6, 3);
  mag2.incomeBatch(b, 13, 4);
  mag2.incomeBatch(c, 23, 78);
  // mag2.printAll();

  mag3.incomeBatch(a, 1.5, 19);
  mag3.incomeBatch(b, 13, 4);
  mag3.incomeBatch(c, 212, 78);
  // mag3.printAll();

  chain.addStore(mag1);
  chain.addStore(mag2);
  chain.addStore(mag3);
  
  Group g = Group();
  g.addNewPair(a, 3);
  g.addNewPair(b, 3); 

  for (Store store : chain.cheapestGroup(g)) {
    cout << store.getName() << endl;
  }

  for (Store store : chain.cheapestUnit(a)) {
    cout << store.getName() << endl;
  }

  Group group = mag1.opportunitiesToBuyOn(1000);
  cout << "Opportunities:" << endl;
  for (int i = 0; i < group.getQuantityOfUnits(); ++i) {
     cout << i+1 << ") " << group.getUnitAt(i).getName() << "->" << group.getQuantityAt(i) << endl;
  }
}