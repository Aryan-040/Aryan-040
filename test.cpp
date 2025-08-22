#include <bits/stdc++.h>
#include "json.hpp"
#include <fstream>
// #include <boost/multiprecision/cpp_int.hpp>
// #include <boost/multiprecision/cpp_dec_float.hpp>

using namespace std;
using json = nlohmann::json;
// using boost::multiprecision::cpp_int;
// using boost::multiprecision::cpp_dec_float_100;


// Convert string in given base to decimal integer
long long convertToDecimal(const string& value, int base) {
    long long result = 0;
    for (char c : value) {
        int digit = (isdigit(c) ? c - '0' : toupper(c) - 'A' + 10);
        result = result * base + digit;
    }
    return result;
}

// Lagrange interpolation
vector<double> lagrangeInterpolation(const vector<double>& X, const vector<double>& Y) {
    int n = X.size();
    vector<double> poly(n, 0.0);

    for (int i = 0; i < n; i++) {
        vector<double> term = {1.0};
        double denom = 1.0;

        for (int j = 0; j < n; j++) {
            if (j == i) continue;
            vector<double> newTerm(term.size() + 1, 0.0);
            for (int k = 0; k < term.size(); k++) {
                newTerm[k] -= term[k] * X[j];
                newTerm[k+1] += term[k];
            }
            term = newTerm;
            denom *= (X[i] - X[j]);
        }

        for (int k = 0; k < term.size(); k++) {
            poly[k] += term[k] * Y[i] / denom;
        }
    }
    return poly;
}

// Print polynomial
void printPolynomial(const vector<double>& coeffs) {
    bool first = true;
    for (int i = coeffs.size()-1; i >= 0; i--) {
        if (fabs(coeffs[i]) < 1e-9) continue;
        if (!first) cout << " + ";
        cout << fixed << setprecision(3) << coeffs[i];
        if (i > 0) cout << "*x";
        if (i > 1) cout << "^" << i;
        first = false;
    }
    cout << endl;
}
int main() {
    ifstream file("test1.json");
    if (!file.is_open()) {
        cerr << "Error: JSON file not found!" << endl;
        return 1;
    }
    json j;
    file >> j;

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    vector<double> X, Y;
    for (auto& [key, value] : j.items()) {
        if (key == "keys") continue;
        int x = stoi(key);
        int base = stoi(value["base"].get<string>());
        string val = value["value"].get<string>();
        long long y = convertToDecimal(val, base);
        X.push_back(x);
        Y.push_back(y);
    }

    if (X.size() > k) {
        X.resize(k);
        Y.resize(k);
    }

    vector<double> poly = lagrangeInterpolation(X, Y);

    // ✅ Just print constant term
    cout << poly[0] << endl;

    return 0;
}