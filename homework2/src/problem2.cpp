#include <iostream>
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;

private:
    float coef;
    int exp;
};

class Polynomial {
private:
    Term *termArray;
    int capacity;
    int terms;

    
    void newTerm(float theCoef, int theExp) {
        if (terms >= capacity) {
            capacity *= 2;

            Term *temp = new Term[capacity];

            for (int i = 0; i < terms; i++) {
                temp[i] = termArray[i];
            }

            delete[] termArray;
            termArray = temp;
        }

        termArray[terms].coef = theCoef;
        termArray[terms].exp = theExp;
        terms++;
    }

public:
 
    Polynomial() {
        capacity = 2;
        terms = 0;
        termArray = new Term[capacity];
    }

  
    Polynomial(const Polynomial& p) {
        capacity = p.capacity;
        terms = p.terms;

        termArray = new Term[capacity];

        for (int i = 0; i < terms; i++) {
            termArray[i] = p.termArray[i];
        }
    }

   
    Polynomial& operator=(const Polynomial& p) {
        if (this != &p) {
            Term *temp = new Term[p.capacity];

            for (int i = 0; i < p.terms; i++) {
                temp[i] = p.termArray[i];
            }

            delete[] termArray;

            termArray = temp;
            capacity = p.capacity;
            terms = p.terms;
        }

        return *this;
    }

    
    ~Polynomial() {
        delete[] termArray;
    }

    
    void AddTerm(float coef, int exp) {
        if (coef == 0 || exp < 0)
            return;

        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == exp) {
                termArray[i].coef += coef;

                if (termArray[i].coef == 0) {
                    for (int j = i; j < terms - 1; j++) {
                        termArray[j] = termArray[j + 1];
                    }

                    terms--;
                }

                return;
            }
        }

        newTerm(coef, exp);
    }

   
    friend istream& operator>>(istream& in, Polynomial& p);

    friend ostream& operator<<(ostream& out, const Polynomial& p);
};


istream& operator>>(istream& in, Polynomial& p) {
    int n;

    cout << "Enter number of terms: ";
    in >> n;

    if (!in || n < 0) {
        in.setstate(ios::failbit);
        return in;
    }

    p.terms = 0;

    for (int i = 0; i < n; i++) {
        float coef;
        int exp;

        cout << "Enter coefficient and exponent: ";
        in >> coef >> exp;

        if (!in || exp < 0) {
            p.terms = 0;
            in.setstate(ios::failbit);
            return in;
        }

        p.AddTerm(coef, exp);
    }

    return in;
}


ostream& operator<<(ostream& out, const Polynomial& p) {
    if (p.terms == 0) {
        out << "0";
        return out;
    }

    for (int i = 0; i < p.terms; i++) {
        if (i > 0)
            out << " + ";

        out << p.termArray[i].coef
            << "x^" << p.termArray[i].exp;
    }

    return out;
}

int main() {
    Polynomial p;

    cin >> p;

    if (!cin) {
        cout << "Invalid input" << endl;
        return 1;
    }

    cout << "Polynomial: " << p << endl;

    return 0;
}
