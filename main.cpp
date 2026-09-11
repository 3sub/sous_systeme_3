#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    vector<string> item = {"Épée \n", "Potion \n", "Bouclier \n"};

    cout << "Item in the inventory: \n";
    for (int i = 0; i < item.size(); ++i) {
        cout << i + 1 << "." << item[i] <<"\n";
    }

    return 0;
}

