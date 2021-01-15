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

int main()
{   
    DataPoint dpt(42, 3.14);
    cout << dpt.id << " : " << dpt.value << endl;
    
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
    
	return 0;
}
