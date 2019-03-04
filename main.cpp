#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <algorithm>
#define MAX_INPUT 1000
#include <unordered_map>


using namespace std;

void initGlbs();
unordered_map<int, unordered_map<string, double>>GLBS_Table;
//unordered_map<int, unordered_map<string, double>>& getTable() {
//    static ;
//    return GLBS_table;
//}

map<string, string> aToConfienceInterval{
        {"0.1", "90.00%"},
        {"0.05", "95.00%"},
        {"0.025", "97.50%"},
        {"0.01", "99.00%"},
        {"0.005", "99.50%"},
};


//计算平均值
double cal_average(vector<double> num) {
    double sum = 0;
    for (int i = 0; i < num.size(); ++i) {
        sum += num[i];
    }
    return sum / num.size();
}


//计算标准差
double cal_standard(vector<double> num, double average) {
    double result;
    double sum = 0;
    for (int i = 0; i < num.size(); ++i) {
        sum += pow((num[i] - average), 2);
    }
    return sqrt(sum / (num.size() - 1));
}

//获取可疑值
unsigned int get_suspicious_var(vector<double> num, double average) {
    double max_num = num[num.size() - 1];
    double min_num = num[0];
    auto max_average = max_num - average;
    auto min_average = average - min_num;
    return max_average > min_average ? num.size() - 1 :  0;
}







int main() {
    initGlbs();
    vector<double> gross_error;
    vector<double> var_vec;
//    auto table = getTable();
    int n;
    string a;
    cout << "请输入测量参数数量:";
    cin >> n;
    cout << "请输入"<< n <<"个测量数据:";
    auto *var = new double(n);
    for(int i = 0; i < n; i++){
        cin >> var[i];
        var_vec.push_back(var[i]);
    }
    sort(var, var+n);
    cout << "选择检出水平a(0.1 0.05 0.025 0.01 0.005):";
    cin >> a;
    auto map_temp = GLBS_Table[n];
    auto Gp = map_temp[aToConfienceInterval[a]];
    cout << "Gp:" << Gp << endl;
    for (int j = 0; j < n; ++j) {
        sort(var_vec.begin(), var_vec.end());
        auto average = cal_average(var_vec);
        auto standard = cal_standard(var_vec,average);
        cout << "平均值:" << average << " 标准差:" << standard << endl;
        auto suspicious_var = get_suspicious_var(var_vec, average);
        auto g = abs(var_vec[suspicious_var] - average) / standard;
        if (g > Gp) {
            gross_error.push_back(var_vec[suspicious_var]);
            var_vec.erase(var_vec.begin() + suspicious_var);
        }
        map_temp = GLBS_Table[var_vec.size()];
        Gp = map_temp[aToConfienceInterval[a]];
    }
    cout << "正常数据:";
    for (const auto &item : var_vec) {
        cout << item << " ";
    }
    cout << endl;
    cout << "粗大误差:";
    for (const auto &item : gross_error) {
        cout << item << " ";
    }
    cout << endl;
    return 0;
}


void initGlbs() {
    ifstream file;
    file.open("/Users/zhutianyu/Code/Cpp/glbs/glbs.txt");
    cout << "--------init glbs 表--------\n";
    bool first = true;
    vector<string> temp;
    while(1) {
        if (first) {
            for(int j = 0; j <= 5; j++) {
                string str;
                file >> str;
                temp.push_back(str);
            }
//            file >> temp[0] >> temp[1] >> temp[2] >> temp[3] >> temp[4] >> temp[5];
            first = false;
        } else {
            int index;
            double tempNum[5];
            while(!file.eof()) {
                file >> index >> tempNum[0] >> tempNum[1] >> tempNum[2] >> tempNum[3] >> tempNum[4];
                unordered_map<string, double> tempMap;
                for(int i = 0; i <= 4; i++) {
                    tempMap.insert(pair<string, double>(temp[i+1], tempNum[i]));
                }
                GLBS_Table.insert(pair<int, unordered_map<string, double>>(index, tempMap));
            }
            break;
        }
    }
    cout << "-------init success glbs table------\n";
}