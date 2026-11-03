#include <iostream>
#include <cmath>
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

  
    Polynomial Add(const Polynomial& poly) {
        Polynomial result;

        for (int i = 0; i < terms; i++) {
            result.AddTerm(
                termArray[i].coef,
                termArray[i].exp
            );
        }

        for (int i = 0; i < poly.terms; i++) {
            result.AddTerm(
                poly.termArray[i].coef,
                poly.termArray[i].exp
            );
        }

        return result;
    }

  
    Polynomial Mult(const Polynomial& poly) {
        Polynomial result;

        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < poly.terms; j++) {
                float c =
                    termArray[i].coef * poly.termArray[j].coef;

                int e =
                    termArray[i].exp + poly.termArray[j].exp;

                result.AddTerm(c, e);
            }
        }

        return result;
    }

  
    float Eval(float x) {
        float result = 0;

        for (int i = 0; i < terms; i++) {
            result += termArray[i].coef *
                      pow(x, termArray[i].exp);
        }

        return result;
    }

    
    void Print() {
        if (terms == 0) {
            cout << "0" << endl;
            return;
        }

        for (int i = 0; i < terms; i++) {
            if (i > 0)
                cout << " + ";

            cout << termArray[i].coef
                 << "x^" << termArray[i].exp;
        }

        cout << endl;
    }
};

int main() {
    Polynomial p, q;

    p.AddTerm(2, 2);
    p.AddTerm(3, 1);
    p.AddTerm(1, 0);

    q.AddTerm(1, 1);
    q.AddTerm(2, 0);

    cout << "P(x) = ";
    p.Print();

    cout << "Q(x) = ";
    q.Print();

    Polynomial sum = p.Add(q);

    cout << "P(x) + Q(x) = ";
    sum.Print();

    Polynomial product = p.Mult(q);

    cout << "P(x) * Q(x) = ";
    product.Print();

    cout << "P(2) = " << p.Eval(2) << endl;

    return 0;
}
