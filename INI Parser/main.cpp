//
//  main.cpp
//  1 INI Parser ООП
//
//  Created by Игорь Клюжев on 17.09.2020.
//  Copyright © 2020 CodeForces. All rights reserved.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class NoSectionOrValueException : public exception {
	virtual const char *what() const throw() {
		return "Error: there is no section or variable that you're looking for.";
	}
} NoSecOrValEx;

struct Variable {
	string name;
	string value;
};

struct Section {
	string name;
	vector<Variable> variables;
};

class Data {
public:
	Data() {
		sections = vector<Section>();
	}

	void parse(string path) {
		ifstream file(path);

    if (!file.is_open()) {
      cout << "Error: the file can't be opened or found." << endl;
      return;
    }

		while (!file.eof()) {
			string line;
			getline(file, line);

			char firstChar = line[0];
			if (firstChar == ';' || line == "") {
				continue;
			}
			if (firstChar == '[') {
				size_t len = line.find_first_of("]", 1);
        if (len == string::npos) {
        cout << "Error: file has incorrect format." << endl;
        sections.resize(0);
        return;
        }
        --len;
				Section sec;
				sec.name = line.substr(1, len);
        if (sec.name.find_first_of(" ;=", 0) != string::npos) {
          cout << "Error: file has incorrect format." << endl;
          sections.resize(0);
          return;
        }
				sec.variables = vector<Variable>();
				sections.push_back(sec);
				continue;
			}

			size_t len = line.find_first_of("=", 0);
      if (len == string::npos) {
        cout << "Error: file has incorrect format." << endl;
        sections.resize(0);
        return;
      }
			size_t pos_of_comment = line.find_first_of(";", len);
			Variable var;
			var.name = line.substr(0, len);
			var.value = line.substr(len + 1, pos_of_comment - 1 - len);
			sections[sections.size() - 1].variables.push_back(var);
		}

		file.close();
	}

	string getString(string sec, string var) {
		auto pred = [sec](const Section &item) { return item.name == sec; };
		auto iter = find_if(sections.begin(), sections.end(), pred);

		try {
			if (iter != sections.end()) {
				Section section = *iter;

				auto pred = [var](const Variable &item) {
					return item.name == var;
				};
				auto iter = find_if(
					section.variables.begin(), section.variables.end(), pred);

					if (iter != section.variables.end()) {
						Variable variable = *iter;

						string result = variable.value;
						return result;
					} else {
						throw NoSecOrValEx;
					}
			} else {
				throw NoSecOrValEx;
			}
		} catch (exception &e) {
			cout << e.what() << endl;
			return "Undefined";
		}
	}

  int getInt(string sec, string var) {
    auto pred = [sec](const Section &item) { return item.name == sec; };
    auto iter = find_if(sections.begin(), sections.end(), pred);

    try {
      if (iter != sections.end()) {
        Section section = *iter;

        auto pred = [var](const Variable &item) {
          return item.name == var;
        };
        auto iter = find_if(
          section.variables.begin(), section.variables.end(), pred);

        if (iter != section.variables.end()) {
          Variable variable = *iter;
          try {
            int result = stoi(variable.value);
            return result;
          } catch(exception &e) {
            cout << "Error: entered type for the variable, that you're looking for, is incorrect. Try using string to see the value." << endl;
            return 0;
          }
        } else {
            throw NoSecOrValEx;
        }
      } else {
        throw NoSecOrValEx;
      }
    } catch (exception &e) {
      cout << e.what() << endl;
      return 0;
    }
  }

  double getDouble(string sec, string var) {
    auto pred = [sec](const Section &item) { return item.name == sec; };
    auto iter = find_if(sections.begin(), sections.end(), pred);

    try {
      if (iter != sections.end()) {
        Section section = *iter;

        auto pred = [var](const Variable &item) {
          return item.name == var;
        };
        auto iter = find_if(
          section.variables.begin(), section.variables.end(), pred);

        if (iter != section.variables.end()) {
          Variable variable = *iter;

          try {
            double result = stod(variable.value);
            return result;
          } catch(exception &e) {
            cout << "Error: entered type for the variable, that you're looking for, is incorrect. Try using string to see the value." << endl;
            return 0;
          }
        } else {
            throw NoSecOrValEx;
        }
      } else {
        throw NoSecOrValEx;
      }
    } catch (exception &e) {
      cout << e.what() << endl;
      return 0;
    }
  }

	void printAll() {
    cout << "-------------------------" << endl;
		for (Section sec : sections) {
      if (sec.variables.empty()) {
        cout << sec.name << endl;
      }
			for (Variable var : sec.variables) {
				cout << sec.name << " -> " << var.name << " = " << var.value
					 << endl;
			}
      cout << "-------------------------" << endl;
		}
	}

private:
	vector<Section> sections;
};

int main(int argc, const char *argv[]) {
	Data data = Data();
	data.parse("text.txt");

	cout << data.getString("owner", "name") << endl
		 << data.getInt("database", "port") << endl
		 << endl;
  int var = data.getInt("database", "port");
  cout << var+var << endl;
	data.printAll();

	return 0;
}
 