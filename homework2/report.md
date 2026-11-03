# 41343105

# 問題一

Polynomial Class

## 解題說明

這題是要使用 C++ 的 class 來完成多項式的運算，包含多項式的加法、乘法和代入數值。

因為多項式裡面每一項都有係數和指數，所以我先使用Term來儲存每一項的資料，再使用Polynomial來處理整個多項式。

### 解題策略

1. 先建立Term類別，用來存放每一項的係數和指數。
2. 建立Polynomial 類別，使用陣列來儲存多項式的資料。
3. 加法的部分，先把兩個多項式的資料放在一起，如果遇到相同的指數，就把係數加起來。
4. 乘法的部分，使用兩層迴圈，讓兩個多項式的每一項互相相乘。
5. 如果相乘後有相同的指數，就把係數合併。
6. 最後使用Eval()代入數字，計算多項式的結果。

## 程式實作

以下是主要程式碼：

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

public:
    Polynomial() {
        capacity = 100;
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
            Term* temp = new Term[p.capacity];

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

        if (terms >= capacity)
            return;

        termArray[terms].coef = coef;
        termArray[terms].exp = exp;
        terms++;
    }

    Polynomial Add(Polynomial poly) {
        Polynomial result;

        for (int i = 0; i < terms; i++) {
            result.AddTerm(termArray[i].coef,
                           termArray[i].exp);
        }

        for (int i = 0; i < poly.terms; i++) {
            result.AddTerm(poly.termArray[i].coef,
                           poly.termArray[i].exp);
        }

        return result;
    }

    Polynomial Mult(Polynomial poly) {
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

    float Eval(float f) {
        float result = 0;

        for (int i = 0; i < terms; i++) {
            result += termArray[i].coef * pow(f, termArray[i].exp);
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
```

## 效能分析

1. 時間複雜度：加法需要使用迴圈檢查兩個多項式有沒有相同的指數。乘法則需要使用兩層迴圈，讓每一項互相相乘，所以乘法的計算量會比較大。Eval()則是把每一項依序計算一次。
2. 空間複雜度：程式使用動態陣列來儲存多項式的資料，每個 Polynomial 會配置 100 個位置，所以需要使用額外的記憶體來存放資料。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入資料                   | 預期輸出         | 實際輸出                      |
| ---- | ---------------------- | ------------ | ------------------------- |
| 測試一  | P(x)=2x²+3x+1，Q(x)=x+2 | 2x²+4x+3     | 2x^2 + 4x^1 + 3x^0        |
| 測試二  | P(x) × Q(x)            | 2x³+7x²+7x+2 | 2x^3 + 7x^2 + 7x^1 + 2x^0 |
| 測試三  | P(2)                   | 15           | 15                        |
| 測試四  | P(0)                   | 1            | 1                         |

### 編譯與執行指令

```bash
g++ -std=c++17 -o polynomial polynomial.cpp
./polynomial
```

## 結論

1. 這次使用 class 來完成多項式的運算，讓我比較了解如何把資料和函數放在同一個類別裡面。
2. 一開始我覺得乘法的部分比較麻煩，因為每一項相乘後還要把相同指數的係數合併。
3. 使用 `Term` 來儲存係數和指數，讓程式比較容易管理資料。
4. 寫完之後，我對 class 和動態陣列的使用比較有概念。

## 申論及開發報告

這次作業主要是使用 C++ 的 class 來完成多項式的運算。

一開始我先想要怎麼把多項式的資料存起來，因為每一項都有係數和指數，所以我使用 Term 來儲存這兩個資料，再使用Polynomial 來處理整個多項式。

在寫加法的時候，我先把兩個多項式的資料放進去，如果遇到相同的指數，就把係數加起來。

乘法的部分我覺得比較難，因為每一項都要互相相乘，而且指數也要相加。一開始比較不清楚要怎麼合併相同的項目，後來使用兩層迴圈來處理，才比較了解整個計算過程。

另外，Eval()的部分就是把指定的數字代入多項式，再把每一項的結果加起來。

寫完這次作業後，我比較了解 class 可以怎麼使用，也知道可以把不同的功能分開寫成函數，讓程式看起來比較清楚。

---

# 問題二

Polynomial Input / Output

## 解題說明

這題是要使用 C++ 的運算子多載，讓自己建立的 Polynomial 類別也可以使用 cin 和 cout 來輸入和輸出資料。

因為 Polynomial 是自己建立的類別，所以不能直接使用原本的輸入和輸出方式，需要自己定義 >> 和 << 的功能。

### 解題策略

1. 先建立 Term 類別，用來存放每一項的係數和指數。
2. 建立 Polynomial 類別，使用陣列來儲存多項式的資料。
3. 使用 friend 讓輸入和輸出的函數可以讀取類別裡面的 private 資料。
4. 在 >> 的部分，先輸入多項式的項數，再使用迴圈輸入每一項的係數和指數。
5. 在 << 的部分，使用迴圈把陣列裡面的資料依序印出來。
6. 最後在main() 中使用 cin >> p 和 cout << p，確認輸入和輸出是否正常。

## 程式實作

以下是主要程式碼：

```cpp
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

public:
    Polynomial() {
        capacity = 100;
        terms = 0;
        termArray = new Term[capacity];
    }

    ~Polynomial() {
        delete[] termArray;
    }

    friend istream& operator>>(istream& in, Polynomial& p);
    friend ostream& operator<<(ostream& out, const Polynomial& p);
};

istream& operator>>(istream& in, Polynomial& p) {
    int n;

    cout << "Enter number of terms: ";
    in >> n;

    if (!in || n < 0 || n > p.capacity) {
        in.setstate(ios::failbit);
        return in;
    }

    p.terms = 0;

    for (int i = 0; i < n; i++) {
        float coef;
        int exp;

        cout << "Enter coefficient and exponent: ";
        in >> coef >> exp;

        if (!in || coef == 0 || exp < 0) {
            in.setstate(ios::failbit);
            p.terms = 0;
            return in;
        }

        p.termArray[i].coef = coef;
        p.termArray[i].exp = exp;
        p.terms++;
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
```

## 效能分析

1. 時間複雜度：輸入和輸出都是使用迴圈來處理資料，如果多項式有 n 項，就需要處理 n 次，所以時間複雜度為 O(n)。
2. 空間複雜度：程式使用動態陣列來儲存多項式，目前設定最多可以存放 100 項，所以需要使用一些額外的記憶體空間。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入資料               | 預期輸出     | 實際輸出               |
| ---- | ------------------ | -------- | ------------------ |
| 測試一  | 3，接著輸入 3 2、2 1、1 0 | 3x²+2x+1 | 3x^2 + 2x^1 + 1x^0 |
| 測試二  | 2，接著輸入 5 3、4 1     | 5x³+4x   | 5x^3 + 4x^1        |
| 測試三  | 1，接著輸入 7 0         | 7        | 7x^0               |
| 測試四  | 0                  | 0        | 0                  |

### 編譯與執行指令

```bash
g++ -std=c++17 -o polynomial_io polynomial_io.cpp
./polynomial_io
```

## 結論

1. 這次使用運算子多載來完成多項式的輸入和輸出。
2. 一開始我不太了解 >> 和 << 也可以自己定義功能，寫完之後比較了解它們的用法。
3. 使用 `friend` 可以讓運算子函數讀取類別裡面的 private 資料。
4. 這次也讓我知道，自己建立的類別也可以使用類似一般資料型態的輸入和輸出方式。

## 申論及開發報告

這次作業主要是練習運算子多載，讓 Polynomial 類別可以直接使用 cin 和cout。

一開始我先建立 Term 和 Polynomial 兩個類別，再想辦法讓外面的輸入和輸出函數可以讀取類別裡面的資料。

後來使用 friend 來處理這個問題，讓 operator>> 和 operator<< 可以讀取 private 裡面的資料。

輸入的部分，我先讓使用者輸入多項式有幾項，再使用迴圈輸入每一項的係數和指數，並把資料存進陣列。

輸出的部分就比較簡單，只要使用迴圈把陣列裡面的資料依序印出來就可以了。

一開始我覺得運算子多載比較難理解，因為平常都是直接使用 cin 和 cout，沒有想過自己建立的類別也可以定義這些功能。

實際寫完之後，我比較了解 friend 和運算子多載的用法，也知道可以利用這種方式讓程式使用起來更方便。

透過這次練習，我對 C++ 的 class 和運算子多載有更多了解，也比較知道怎麼把課堂上學到的東西用在程式裡面。
