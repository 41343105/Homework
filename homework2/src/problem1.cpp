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

    void newTerm(const float theCoef, const int theExp) {
        if (theCoef == 0) return;

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
        if (coef == 0 || exp < 0) return;

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

        
        int i = terms - 1;

        while (i > 0 && termArray[i].exp > termArray[i - 1].exp) {
            Term temp = termArray[i];
            termArray[i] = termArray[i - 1];
            termArray[i - 1] = temp;
            i--;
        }
    }

    
    Polynomial Add(Polynomial poly) const {
        Polynomial c;
        int aPos = 0;
        int bPos = 0;

        while (aPos < terms && bPos < poly.terms) {
            if (termArray[aPos].exp == poly.termArray[bPos].exp) {
                float t = termArray[aPos].coef + poly.termArray[bPos].coef;

                if (t != 0) {
                    c.newTerm(t, termArray[aPos].exp);
                }

                aPos++;
                bPos++;
            }
            else if (termArray[aPos].exp < poly.termArray[bPos].exp) {
                c.newTerm(poly.termArray[bPos].coef,
                          poly.termArray[bPos].exp);
                bPos++;
            }
            else {
                c.newTerm(termArray[aPos].coef,
                          termArray[aPos].exp);
                aPos++;
            }
        }

        for (; aPos < terms; aPos++) {
            c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
        }

        for (; bPos < poly.terms; bPos++) {
            c.newTerm(poly.termArray[bPos].coef,
                      poly.termArray[bPos].exp);
        }

        return c;
    }

    Polynomial Mult(Polynomial poly) const {
        Polynomial result;

        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < poly.terms; j++) {
                float c = termArray[i].coef * poly.termArray[j].coef;
                int e = termArray[i].exp + poly.termArray[j].exp;

                result.AddTerm(c, e);
            }
        }

        return result;
    }

    float Eval(float f) const {
        float result = 0;

        for (int i = 0; i < terms; i++) {
            result += termArray[i].coef *
                      static_cast<float>(pow(f, termArray[i].exp));
        }

        return result;
    }

    void Print() const {
        if (terms == 0) {
            cout << "0" << endl;
            return;
        }

        for (int i = 0; i < terms; i++) {
            float c = termArray[i].coef;
            int e = termArray[i].exp;

            if (i > 0) {
                if (c >= 0) cout << " + ";
                else cout << " - ";
            }
            else if (c < 0) {
                cout << "-";
            }

            cout << std::abs(c) << "x^" << e;
        }

        cout << endl;
    }
};

int main() {
    char choice = 'y';

    do {
        Polynomial p, q;
        int degree;
        float coef, x;
        bool valid = true;

        cout << "===========================\n";

        cout << "Enter degree of P: ";
        cin >> degree;

        if (!cin || degree < 0 || degree > 99) {
            cout << "Invalid degree" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            valid = false;
        }

        if (valid) {
            cout << "Enter coefficients from highest degree to 0: ";

            for (int i = degree; i >= 0; i--) {
                cin >> coef;

                if (!cin) {
                    cout << "Invalid input" << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    valid = false;
                    break;
                }

                p.AddTerm(coef, i);
            }
        }

        if (valid) {
            cout << "Enter degree of Q: ";
            cin >> degree;

            if (!cin || degree < 0 || degree > 99) {
                cout << "Invalid degree" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                valid = false;
            }
        }

        if (valid) {
            cout << "Enter coefficients from highest degree to 0: ";

            for (int i = degree; i >= 0; i--) {
                cin >> coef;

                if (!cin) {
                    cout << "Invalid input" << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    valid = false;
                    break;
                }

                q.AddTerm(coef, i);
            }
        }

        if (valid) {
            cout << "\nP(x) = ";
            p.Print();

            cout << "Q(x) = ";
            q.Print();

            Polynomial sum = p.Add(q);

            cout << "P(x) + Q(x) = ";
            sum.Print();

            Polynomial product = p.Mult(q);

            cout << "P(x) * Q(x) = ";
            product.Print();

            cout << "\nEnter x value: ";
            cin >> x;

            if (cin) {
                cout << "P(" << x << ") = " << p.Eval(x) << endl;
                cout << "Q(" << x << ") = " << q.Eval(x) << endl;
                cout << "P(0) = " << p.Eval(0) << endl;
            }
            else {
                cout << "Invalid input" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                valid = false;
            }
        }

        if (!valid) {
            cout << "Please enter the polynomial again." << endl;
            continue;
        }

        cout << "\nDo you want to test another polynomial? (y/n): ";
        cin >> choice;
        cout << endl;

    } while (choice == 'y' || choice == 'Y');

    cout << "Program ended." << endl;

    return 0;
}
