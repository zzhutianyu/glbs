#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <algorithm>
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
        auto suspicious_var = get_suspicious_var(var_vec, average);
        auto g = abs(var_vec[suspicious_var] - average) / standard;
        cout << "平均值:" << average << " 标准差:" << standard  << "g:" << g << "可疑值:"<<  var_vec[suspicious_var] <<  endl;
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
    file.open("../glbs.txt");
    /**
     * glbs.txt
     *  n 90.00% 95.00% 97.50% 99.00% 99.50%
        3 1.148 1.453 1.155 1.155 1.155
        4 1.425 1.463 1.481 1.492 1.496
        5 1.602 1.672 1.715 1.749 1.764
        6 1.729 1.822 1.887 1.944 1.973
        7 1.828 1.938 2.020 2.097 2.139
        8 1.909 2.032 2.126 2.221 2.274
        9 1.977 2.110 2.215 2.323 2.387
        10 2.036 2.176 2.290 2.410 2.482
        11 2.088 2.234 2.355 2.485 2.564
        12 2.134 2.285 2.412 2.550 2.636
        13 2.175 2.331 2.462 2.607 2.699
        14 2.213 2.371 2.507 2.659 2.755
        15 2.247 2.409 2.549 2.705 2.806
        16 2.279 2.443 2.585 2.747 2.852
        17 2.309 2.475 2.620 2.785 2.894
        18 2.335 2.501 2.651 2.821 2.932
        19 2.361 2.532 2.681 2.954 2.968
        20 2.385 2.557 2.709 2.884 3.001
        21 2.408 2.580 2.733 2.912 3.031
        22 2.429 2.603 2.758 2.939 3.060
        23 2.448 2.624 2.781 2.963 3.087
        24 2.467 2.644 2.802 2.987 3.112
        25 2.486 2.663 2.822 3.009 3.135
        26 2.502 2.681 2.841 3.029 3.157
        27 2.519 2.698 2.859 3.049 3.178
        28 2.534 2.714 2.876 3.068 3.199
        29 2.549 2.730 2.893 3.085 3.218
        30 2.583 2.745 2.908 3.103 3.236
        31 2.577 2.759 2.924 3.119 3.253
        32 2.591 2.773 2.938 3.135 3.270
        33 2.604 2.786 2.952 3.150 3.286
        34 2.616 2.799 2.965 3.164 3.301
        35 2.628 2.811 2.979 3.178 3.316
        36 2.639 2.823 2.991 3.191 3.330
        37 2.650 2.835 3.003 3.201 3.313
        38 2.661 2.846 3.011 3.216 3.356
        39 2.671 2.857 3.025 3.228 3.369
        40 2.682 2.866 3.036 3.240 3.381
        41 2.692 2.877 3.046 3.251 3.393
        42 2.700 2.887 3.057 3.261 3.404
        43 2.710 2.896 3.067 3.271 3.415
        44 2.719 2.905 3.075 3.282 3.425
        45 2.727 2.914 3.085 3.292 3.435
        46 2.736 2.923 3.094 3.302 3.445
        47 2.744 2.931 3.103 3.310 3.455
        48 2.753 2.940 3.111 3.319 3.464
        49 2.760 2.948 3.120 3.329 3.474
        50 2.768 2.956 3.128 3.336 3.483
        51 2.775 2.943 3.136 3.345 3.491
        52 2.783 2.971 3.143 3.353 3.500
        53 2.790 2.978 3.151 3.361 3.507
        54 2.798 2.986 3.158 3.388 3.516
        55 2.804 2.992 3.166 3.376 3.574
        56 2.811 3.000 3.172 3.383 3.531
        57 2.818 3.006 3.180 3.391 3.539
        58 2.824 3.013 3.186 3.397 3.546
        59 2.831 3.019 3.193 3.405 3.553
        60 2.837 3.025 3.199 3.411 3.560
        61 2.842 3.032 3.205 3.418 3.566
        62 2.849 3.037 3.212 3.424 3.573
        63 2.854 3.044 3.218 3.430 3.579
        64 2.860 3.049 3.224 3.437 3.586
        65 2.866 3.055 3.230 3.442 3.592
        66 2.871 3.061 3.235 3.449 3.598
        67 2.877 3.066 3.241 3.454 3.605
        68 2.883 3.071 3.246 3.460 3.610
        69 2.888 3.076 3.252 3.466 3.617
        70 2.893 3.082 3.257 3.471 3.622
        71 2.897 3.087 3.262 3.476 3.627
        72 2.903 3.092 3.267 3.482 3.633
        73 2.908 3.098 3.272 3.487 3.638
        74 2.912 3.102 3.278 3.492 3.643
        75 2.917 3.107 3.282 3.496 3.648
        76 2.922 3.111 3.287 3.502 3.654
        77 2.927 3.117 3.291 3.507 3.658
        78 2.931 3.121 3.297 3.511 3.663
        79 2.935 3.125 3.301 3.516 3.669
        80 2.940 3.130 3.305 3.521 3.673
        81 2.945 3.134 3.309 3.525 3.677
        82 2.949 3.139 3.315 3.529 3.682
        83 2.953 3.143 3.319 3.534 3.687
        84 2.957 3.147 3.323 3.539 3.691
        85 2.961 3.151 3.327 3.543 3.695
        86 2.966 3.155 3.331 3.547 3.699
        87 2.970 3.160 3.335 3.551 3.704
        88 2.973 3.163 3.339 3.555 3.708
        89 2.977 3.167 3.343 3.559 3.712
        90 2.981 3.171 3.347 3.563 3.716
        91 2.984 3.174 3.350 3.567 3.720
        92 2.989 3.179 3.355 3.570 3.725
        93 2.993 3.182 3.358 3.575 3.728
        94 2.996 3.186 3.362 3.579 3.732
        95 3.000 3.189 3.365 3.582 3.736
        96 3.003 3.193 3.369 3.086 3.739
        97 3.006 3.196 3.372 3.589 3.744
        98 3.011 3.201 3.377 3.593 3.747
        99 3.014 3.204 3.380 3.597 3.750
        100 3.017 3.207 3.383 3.600 3.754
     *
     */
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