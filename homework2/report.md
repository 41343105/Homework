# 41343105

# 問題一：Polynomial Class

## 解題說明

這題主要是要使用 C++ 的 class 來完成多項式的運算，包含加法、乘法，還有代入 X 計算結果。

一開始我先想要怎麼把多項式的資料存起來，因為每一項都有自己的係數和指數，所以我使用 Term 來存放這兩個資料，再使用 Polynomial 來管理整個多項式。

在儲存資料的部分，我按照老師上課 PPT 的方式，使用動態陣列來存放多項式，並且把一開始的容量設定為 2。

如果後面要存放的資料超過原本的容量，就需要重新建立一個比較大的陣列，再把原本的資料複製過去。

另外，我也將原本固定寫在程式裡面的資料改成可以自己輸入，這樣每次執行時就能測試不同的多項式。

### 解題策略

1. 先建立 Term 類別，分別存放每一項的係數 coef 和指數 exp。
2. 建立 Polynomial 類別，用來管理整個多項式的資料。
3. 在建構子的部分，將 capacity 設定為 2，並使用 new 建立動態陣列。
4. 如果陣列空間不夠，就使用 newTerm() 將容量增加為原本的兩倍，再把舊資料複製過去。
5. 加法的部分，我使用兩個位置來比較 P 和 Q 的指數，如果指數相同就把係數加起來，不同的話就先放入指數比較大的那一項。
6. 乘法的部分，使用兩層迴圈讓兩個多項式的每一項互相相乘，係數相乘，指數相加。
7. 如果乘法後有相同的指數，就使用 AddTerm() 把係數合併。
8. 最後使用 Eval() 代入使用者輸入的 X，算出多項式的結果。

## 程式實作

以下是完整程式碼：

```cpp
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
```

## 效能分析

### 1. 時間複雜度

* **newTerm()：** 這個函數主要是用來增加陣列空間。如果原本的容量不夠，就要重新建立陣列，並把舊資料複製過去，所以單次擴充需要 O(n) 的時間。
* **Add()：** 加法使用兩個位置來比較 P 和 Q 的指數，兩邊的資料都是依照指數由大到小排列，所以可以直接往下比較，不需要重新搜尋。每一項最多處理一次，因此時間複雜度是 O(m+n)。
* **Mult()：** 乘法使用兩層迴圈，假設 P 有 m 項、Q 有 n 項，就需要做 m × n 次相乘。每次相乘後還要檢查是否有相同的指數，所以最壞情況的時間複雜度是 O(m²n²)。
* **Eval()：** 這個函數會把每一項依序代入 X，再將結果加起來，所以時間複雜度是 O(m)，這裡沒有計算 pow() 函數內部的運算時間。

### 2. 空間複雜度

這次使用動態陣列來存放多項式，剛開始的容量是 2。

當資料變多、原本的空間不夠時，就會再建立一個容量更大的陣列，並把原本的資料複製過去。

加法的結果最多會有 m+n 項，乘法則是兩邊的每一項都可能相乘，所以最多可能產生 m×n 個不同指數的項目。

因此，實際使用的空間會依照多項式的項目數量而增加。

## 測試與驗證

### 測試案例

這次先使用下面兩個多項式來測試：

P(x) = 2x² + 3x + 1

Q(x) = x + 2

| 測試案例 | 輸入資料 | 預期輸出 |
|---|---|---|
| 測試一 | P(x) + Q(x) | 2x² + 4x + 3 |
| 測試二 | P(x) × Q(x) | 2x³ + 7x² + 7x + 2 |
| 測試三 | P(2) | 15 |
| 測試四 | Q(2) | 4 |
| 測試五 | P(0) | 1 |

### 測試輸入

```text
Enter degree of P: 2
Enter coefficients from highest degree to 0: 2 3 1
Enter degree of Q: 1
Enter coefficients from highest degree to 0: 1 2
Enter x value: 2
```

### 預期執行結果

```text
P(x) = 2x^2 + 3x^1 + 1x^0
Q(x) = 1x^1 + 2x^0
P(x) + Q(x) = 2x^2 + 4x^1 + 3x^0
P(x) * Q(x) = 2x^3 + 7x^2 + 7x^1 + 2x^0

P(2) = 15
Q(2) = 4
P(0) = 1
```

以上是依照這組輸入資料計算的預期結果，實際執行結果需要再用程式確認。

## 效能量測

這次我用不同項數的多項式來比較運算量，看看項目增加之後，程式需要處理的資料會有什麼變化。

| 多項式項數 | 加法 | 乘法 | 代入計算 |
|---|---|---|---|
| 2 項 | 比較少 | 比較少 | 比較少 |
| 5 項 | 增加 | 增加較多 | 增加 |
| 10 項 | 增加 | 增加更多 | 增加 |

從這個比較可以看出，加法主要是依序比較兩邊的項目，而乘法需要讓兩個多項式的每一項互相相乘，還要檢查是否有相同的指數。

所以當項目增加時，乘法需要處理的資料也會增加得比較多。

這裡主要是依照程式的運作方式來比較，並不是實際測量執行時間。

## 結論

1. 這次使用 C++ 的 class 來完成多項式的運算，讓我比較了解資料和函數可以放在同一個類別裡面。
2. 一開始我比較不清楚係數和指數要怎麼存放，後來使用 Term 來管理每一項的資料，就比較容易處理。
3. 動態陣列的部分也讓我了解，如果原本的空間不夠，就可以重新建立更大的陣列，不需要一開始就設定很大的容量。
4. 我覺得乘法比較麻煩，因為每一項相乘之後，還要再檢查有沒有相同的指數。
5. 完成這次作業後，我對 Polynomial 類別、動態陣列和多項式的運算方式有更多了解。

## 申論及開發報告

這次作業主要是使用 C++ 的 class 來完成多項式的運算。

一開始我先想要怎麼儲存多項式，因為每一項都有係數和指數，所以我先建立 Term 來存放這兩個資料，再使用 Polynomial 來管理整個多項式。

在建立 Polynomial 的時候，我按照老師 PPT 的方式，把 capacity 設定為 2，並使用動態陣列來存放資料。

剛開始我比較不清楚陣列空間不夠時要怎麼處理，後來使用 newTerm() 來擴充容量。當資料超過原本的空間時，就建立新的陣列，再把原本的資料複製過去。

加法的部分，我使用兩個位置來比較 P 和 Q 的指數。如果指數相同，就把兩邊的係數加起來；如果不相同，就先把指數比較大的那一項放進結果裡面。這樣就能完成兩個多項式的相加。

乘法的部分我覺得比較難，因為兩個多項式的每一項都要互相相乘。除了係數要相乘之外，指數也要相加，而且相乘後可能會出現相同的指數，所以還需要把係數合併。

最後 Eval() 的部分，就是把使用者輸入的 X 代入多項式，再將每一項的結果加起來，就能得到最後的答案。

另外，我也把原本固定寫在 main() 裡面的資料改成可以自己輸入，這樣每次執行程式時，就可以測試不同的多項式，不需要一直修改程式碼。

寫完這次作業後，我比較了解如何使用 class 管理資料，也知道動態陣列可以依照資料量增加容量。這次練習讓我對多項式的加法、乘法和代入計算有更多了解。

---

# 問題二：Polynomial Input / Output

## 解題說明

這題主要是要使用 C++ 的運算子多載，讓自己建立的 Polynomial 類別也可以使用 cin 和 cout 來輸入和輸出資料。

一開始我覺得比較不清楚的地方是，自己建立的類別為什麼不能直接使用原本的 cin 和 cout。後來了解之後，發現需要自己定義 >> 和 << 的功能，才能讓 Polynomial 使用這些輸入和輸出的方式。

這題同樣使用 Term 和 Polynomial 來儲存多項式的資料，並且按照老師 PPT 的方式，將動態陣列的初始容量設定為 2。

如果輸入的資料超過原本的容量，就會再擴充陣列，讓程式可以繼續存放資料。

### 解題策略

1. 建立 Term 類別，用來存放每一項的係數 coef 和指數 exp。
2. 建立 Polynomial 類別，使用動態陣列來儲存多項式的資料。
3. 在建構子的部分，將 capacity 設定為 2，並使用 new 建立陣列。
4. 當陣列空間不夠時，使用 newTerm() 擴充容量，並將原本的資料複製過去。
5. 使用 friend 讓外面的輸入和輸出函數可以讀取類別裡面的 private 資料。
6. 輸入的部分先讀取多項式有幾項，再依序輸入每一項的係數和指數。
7. 輸入資料時使用 AddTerm()，如果遇到相同的指數，就將係數合併。
8. 輸出的部分使用迴圈，把陣列裡面的資料依照指數由大到小印出來。
9. 最後在 main() 裡面使用 cin >> p 和 cout << p，確認輸入和輸出是否正常。

## 程式實作

以下是完整程式碼：

```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream& out, const Polynomial& p);

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

    
    friend istream& operator>>(istream& in, Polynomial& p);
    friend ostream& operator<<(ostream& out, const Polynomial& p);
};


istream& operator>>(istream& in, Polynomial& p) {
    int n;

    cout << "Enter number of terms: ";
    in >> n;

    if (!in || n < 0) {
        p.terms = 0;
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
        float c = p.termArray[i].coef;
        int e = p.termArray[i].exp;

        if (i > 0) {
            if (c >= 0) out << " + ";
            else out << " - ";
        }
        else if (c < 0) {
            out << "-";
        }

        out << std::abs(c) << "x^" << e;
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
```

## 效能分析

### 1. 時間複雜度

* **newTerm()：** 這個函數是用來存放新的項目，如果原本的陣列空間不夠，就要建立新的陣列，再把舊資料複製過去，所以單次擴充需要 O(n) 的時間。
* **operator>>()：** 輸入時會使用迴圈一項一項讀取資料，而且每次都會透過 AddTerm() 檢查有沒有相同的指數，所以最壞情況需要 O(n²) 的時間。
* **operator<<()：** 輸出的時候會直接按照陣列順序，把每一項資料印出來，每一項只需要處理一次，所以時間複雜度是 O(n)。

這裡的 n 代表多項式裡面的項目數量。

### 2. 空間複雜度

這題同樣使用動態陣列來儲存多項式，剛開始的容量設定為 2。

當輸入的資料超過原本的容量時，就會重新建立更大的陣列，並把原本的資料複製過去。

因為陣列主要是用來存放每一項的係數和指數，所以資料增加時，使用的空間也會跟著增加，空間複雜度為 O(n)。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入資料 | 預期輸出 |
|---|---|---|
| 測試一 | 3，接著輸入 3 2、2 1、1 0 | 3x² + 2x + 1 |
| 測試二 | 2，接著輸入 5 3、4 1 | 5x³ + 4x |
| 測試三 | 1，接著輸入 7 0 | 7 |
| 測試四 | 0 | 0 |

### 測試輸入範例

```text
3
3 2
2 1
1 0
```

### 預期執行結果

```text
Enter number of terms: 3
Enter coefficient and exponent: 3 2
Enter coefficient and exponent: 2 1
Enter coefficient and exponent: 1 0
Polynomial: 3x^2 + 2x^1 + 1x^0
```

以上是依照測試輸入整理的預期輸出，實際執行時仍需要再確認一次。

## 效能量測

這次我用不同的項數來比較輸入和輸出需要處理多少資料。

| 多項式項數 | 輸入資料處理次數 | 輸出資料處理次數 |
|---|---|---|
| 1 項 | 1 次 | 1 次 |
| 5 項 | 5 次 | 5 次 |
| 10 項 | 10 次 | 10 次 |
| 20 項 | 20 次 | 20 次 |

從這個比較可以看出，輸入時需要使用迴圈一項一項讀取資料，輸出時也需要把每一項印出來。

所以當多項式的項目增加時，程式需要處理的資料也會跟著增加。

這裡比較的是資料處理次數，並不是實際測量程式執行的時間。

## 結論

1. 這次主要是練習 C++ 的運算子多載，讓自己建立的 Polynomial 類別也可以使用 cin 和 cout。
2. 一開始我不太清楚 >> 和 << 要怎麼自己定義，後來透過 friend 函數來完成輸入和輸出的功能。
3. 使用 friend 之後，外面的運算子函數就可以讀取類別裡面的 private 資料。
4. 這次也有練習動態陣列，讓程式可以依照輸入的資料量增加容量。
5. 完成這題之後，我對運算子多載、friend 和動態陣列的使用方式比較了解。

## 申論及開發報告

這次作業主要是練習運算子多載，讓 Polynomial 類別可以直接使用 cin 和 cout。

一開始我先建立 Term 和 Polynomial 兩個類別，讓 Term 負責存放係數和指數，而 Polynomial 負責管理整個多項式。

因為 Polynomial 裡面的資料是 private，所以外面的輸入和輸出函數沒有辦法直接讀取。後來我使用 friend，讓 operator>> 和 operator<< 可以使用類別裡面的資料。

輸入的部分，我先讓使用者輸入多項式有幾項，再使用迴圈輸入每一項的係數和指數。輸入完成後，再透過 AddTerm() 將資料放進陣列。

如果輸入的項目超過原本的容量，就會使用 newTerm() 擴充陣列，把舊資料複製到新的陣列裡面。

輸出的部分就比較簡單，因為資料已經按照指數由大到小排列，所以只需要使用迴圈把每一項印出來，就能顯示完整的多項式。

一開始我覺得運算子多載比較難理解，因為平常都是直接使用 cin 和 cout，沒有想過自己建立的類別也可以定義這些功能。

實際寫完之後，我比較了解 friend 和運算子多載的用法，也知道可以利用這種方式讓自己建立的類別更方便使用。

透過這次練習，我對 C++ 的 class、動態陣列和運算子多載有更多了解，也比較知道怎麼把上課學到的東西用在程式裡面。

---

# 整體心得

這次作業主要是在練習 C++ 的 Polynomial 類別，從一開始建立 Term 來儲存係數和指數，到使用 Polynomial 管理整個多項式，讓我比較了解 class 的使用方式。

問題一主要是練習多項式的加法、乘法和代入 X。這題讓我學到怎麼使用動態陣列存放資料，也了解當原本的空間不夠時，可以重新建立更大的陣列，再把原本的資料複製過去。

問題二則是練習運算子多載，讓自己建立的 Polynomial 類別可以使用 cin 和 cout 進行輸入和輸出，也讓我更了解 friend 的用途。

我覺得這次比較需要花時間的地方是動態陣列的擴充，以及乘法後相同指數的合併。一開始比較不清楚資料應該怎麼存放，也不太確定每個函數應該負責什麼工作，後來把不同功能分開處理，就比較容易理解。

完成這次作業後，我對 C++ 的 class、動態記憶體配置、多項式運算和運算子多載有更多認識，也比較知道怎麼把不同的功能整理在同一個類別裡面。
