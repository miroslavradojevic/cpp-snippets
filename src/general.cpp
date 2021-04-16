#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;

class DataPoint
{
    public:
        int id;
        double value;
        DataPoint(int id_, double value_) {
                id = id_;
                value = value_;
        }
};

void create_data_point() {
    DataPoint dpt(42, 3.14);
    cout << dpt.id << " : " << dpt.value << endl;
}

void test_break_command() {
    cout << "\ntest_break_command()" << endl;
    
    for (int k = 1; k <=5; k++) {
        cout << k ;
        if (k >= 3) break;
        cout << " Do it" << endl;
    }
}

void test_find_value() {
    cout << "\ntest_find_value()" << endl;
    
    vector<int> indexes;
    
    for(int i = 0; i < 10; i++) {
        int val {rand()%20};
        cout << val << ", ";
        indexes.push_back(val);
    }
    
    cout << "\nFind: ";
    int value_to_find {0};
    cin >> value_to_find;
    
    bool found = std::find(indexes.begin(), indexes.end(), value_to_find) != indexes.end();
    
    if (found) {
        cout << "Found" << endl;
    }
    else {
        cout << "Not found" << endl;
    }
}

int main()
{   
    create_data_point();
    
    std::vector<DataPoint> dpt_array;
    
    cout << setprecision(2) << fixed;
    
    cout << "=========================" << endl;
    for(int i = 0; i < 10; i++) {
        int id {rand()%100};
        double value = (double)(rand()) / ((double)(RAND_MAX/100.0));
        dpt_array.push_back(DataPoint(id, value));
        cout << i << " :" << dpt_array.back().id << " | " << dpt_array.back().value << endl;
    }
    
    cout << dpt_array.size() << " elements" << endl;
    
    // sorting vector
    std::sort(dpt_array.begin(), dpt_array.end(), [](const DataPoint &a, const DataPoint &b) {
        return a.value < b.value;
    });
    
    cout << "=========================" << endl;
    for(int i = 0; i < 10; i++) {
        cout << i << " :" << dpt_array[i].id << " | " << dpt_array[i].value << endl;
    }
    
    test_break_command();
    
    test_find_value();
    
	return 0;
}
