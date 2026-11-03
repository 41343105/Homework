# 41343105

# 問題一：Polynomial Class

## 解題說明

這題是要使用 C++ 的 class 來完成多項式的運算，包含多項式的加法、乘法和代入 X 計算結果。

因為多項式裡面每一項都有係數和指數，所以我先使用 Term 來儲存每一項的資料，再使用 Polynomial 來處理整個多項式。

根據老師上課的 PPT，Polynomial 使用動態陣列來儲存資料，並且將初始容量設定為 2。

當陣列空間不足時，就需要建立新的陣列，把原本的資料複製過去，再繼續存放新的資料。

這次也將輸入的部分改成可以在程式執行時自己輸入，不需要每次都修改程式裡面的資料。

### 解題策略

1. 建立 Term 類別，用來存放每一項的係數 coef 和指數 exp。
2. 建立 Polynomial 類別，使用 Term 陣列儲存多項式的資料。
3. 建構子將 capacity 設定為 2，並使用 new 動態配置陣列。
4. 當陣列容量不足時，使用 newTerm() 擴充陣列，將容量增加為原本的兩倍。
5. 多項式加法的部分，將兩個多項式的資料合併，如果遇到相同的指數，就把係數加起來。
6. 多項式乘法的部分，使用兩層迴圈，讓兩個多項式的每一項互相相乘，並將指數相加。
7. 最後使用 Eval() 代入指定的 X，計算多項式的結果。
8. 使用 cin 讓使用者輸入多項式的最高次數和各項係數，再顯示計算結果。

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
        if (coef == 0 || exp < 0) {
            return;
        }

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

        while (i > 0 &&
               termArray[i].exp > termArray[i - 1].exp) {

            Term temp = termArray[i];
            termArray[i] = termArray[i - 1];
            termArray[i - 1] = temp;

            i--;
        }
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

    float Eval(float x) const {
        float result = 0;

        for (int i = 0; i < terms; i++) {
            result += termArray[i].coef *
                      pow(x, termArray[i].exp);
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
                if (c >= 0) {
                    cout << " + ";
                }
                else {
                    cout << " - ";
                }
            }
            else if (c < 0) {
                cout << "-";
            }

            cout << abs(c) << "x^" << e;
        }

        cout << endl;
    }
};

int main() {
    Polynomial p, q;

    int degree;
    float coef;
    float x;

    cout << "Enter degree of P: ";
    cin >> degree;

    if (!cin || degree < 0 || degree > 99) {
        cout << "Invalid degree" << endl;
        return 1;
    }

    cout << "Enter coefficients from highest degree to 0: ";

    for (int i = degree; i >= 0; i--) {
        cin >> coef;

        if (!cin) {
            cout << "Invalid input" << endl;
            return 1;
        }

        p.AddTerm(coef, i);
    }

    cout << "Enter degree of Q: ";
    cin >> degree;

    if (!cin || degree < 0 || degree > 99) {
        cout << "Invalid degree" << endl;
        return 1;
    }

    cout << "Enter coefficients from highest degree to 0: ";

    for (int i = degree; i >= 0; i--) {
        cin >> coef;

        if (!cin) {
            cout << "Invalid input" << endl;
            return 1;
        }

        q.AddTerm(coef, i);
    }

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

    if (!cin) {
        cout << "Invalid input" << endl;
        return 1;
    }

    cout << "P(" << x << ") = "
         << p.Eval(x) << endl;

    cout << "Q(" << x << ") = "
         << q.Eval(x) << endl;

    return 0;
}


```

## 效能分析

### 1. 時間複雜度

* newTerm()：當陣列容量不足時，需要建立新的陣列並複製原本的資料，所以單次擴充需要 O(n) 的時間。
* Add()：需要將兩個多項式的項目合併，如果遇到相同的指數就要檢查並加總係數，因此最壞情況為 O((m+n)²)。
* Mult()：使用兩層迴圈，兩個多項式分別有 m 項和 n 項時，需要進行 m × n 次相乘，再加上合併相同指數的操作，因此最壞情況為 O(m²n²)。
* Eval()：依序計算每一項的結果，所以時間複雜度為 O(m)，不包含 pow() 的內部運算時間。

### 2. 空間複雜度

程式使用動態陣列來儲存多項式的資料，初始容量為 2。

當資料超過容量時，會重新配置更大的陣列，並將原本的資料複製過去。

加法結果最多需要儲存 m+n 項，乘法結果最多可能產生 m×n 個項目，因此空間使用量會隨著多項式的項目數量增加。

## 測試與驗證

### 測試案例

設定：

P(x) = 2x² + 3x + 1

Q(x) = x + 2

| 測試案例 | 輸入資料        | 預期輸出               | 實際輸出                      |
| ---- | ----------- | ------------------ | ------------------------- |
| 測試一  | P(x) + Q(x) | 2x² + 4x + 3       | 2x^2 + 4x^1 + 3x^0        |
| 測試二  | P(x) × Q(x) | 2x³ + 7x² + 7x + 2 | 2x^3 + 7x^2 + 7x^1 + 2x^0 |
| 測試三  | P(2)        | 15                 | 15                        |
| 測試四  | P(0)        | 1                  | 1                         |

### 測試輸入

```text
Enter degree of P: 2
Enter coefficients from highest degree to 0: 2 3 1
Enter degree of Q: 1
Enter coefficients from highest degree to 0: 1 2
Enter x value: 2
```

### 執行結果

```text
P(x) = 2x^2 + 3x^1 + 1x^0
Q(x) = 1x^1 + 2x^0
P(x) + Q(x) = 2x^2 + 4x^1 + 3x^0
P(x) * Q(x) = 2x^3 + 7x^2 + 7x^1 + 2x^0

P(2) = 15
Q(2) = 4
```

### 編譯與執行指令

```bash
g++ -std=c++17 -o polynomial polynomial.cpp
./polynomial
```

## 效能量測

這次我用不同項數的多項式來比較運算量，看看當多項式的項目增加時，程式需要處理的資料會有什麼變化。

| 多項式項數 | 加法  | 乘法   | 代入計算 |
| ----- | --- | ---- | ---- |
| 2 項   | 比較少 | 比較少  | 比較少  |
| 5 項   | 增加  | 增加較多 | 增加   |
| 10 項  | 增加  | 增加更多 | 增加   |

從程式可以看出，乘法需要使用兩層迴圈，還要合併相同指數的項目，所以項數增加時，計算量也會變大。

這裡主要是依照程式的運算方式來比較，並不是實際測量執行時間。

## 結論

1. 這次使用 C++ 的 class 來完成多項式的運算，讓我比較了解如何把資料和函數放在同一個類別裡面。
2. 透過 Term 來儲存係數和指數，可以讓多項式的資料比較容易管理。
3. 這次也學到動態陣列的使用方式，當原本的容量不足時，可以建立新的陣列來增加儲存空間。
4. 多項式乘法的部分比較麻煩，因為每一項相乘後還需要合併相同的指數。
5. 寫完這次作業後，我對 Polynomial 類別、動態記憶體配置和多項式運算有更多了解。

## 申論及開發報告

這次作業主要是使用 C++ 的 class 來完成多項式的建構和運算。

一開始我先想要怎麼儲存多項式，因為每一項都有係數和指數，所以我使用 Term 來儲存這兩個資料，再使用 Polynomial 來管理整個多項式。

在建立 Polynomial 的時候，我按照老師 PPT 的方式，將 capacity 初始值設定為 2，並使用動態陣列來儲存資料。

當陣列空間不夠的時候，就需要建立新的陣列，把原本的資料複製過去，再增加新的項目。這部分讓我比較了解動態陣列是怎麼運作的。

加法的部分，我先將兩個多項式的項目放在一起，如果遇到相同的指數，就把係數加起來，這樣就可以得到相加後的多項式。

乘法的部分我覺得比較難，因為兩個多項式的每一項都要互相相乘，而且係數要相乘、指數要相加。如果遇到相同的指數，也需要再把係數合併，所以我使用兩層迴圈來完成。

最後 Eval() 的部分，就是把指定的 X 代入多項式，再將每一項的結果加起來，就可以得到最後的答案。

另外，我也把原本寫在 main() 裡面的固定資料改成可以自己輸入，這樣每次執行程式時，就可以測試不同的多項式，不需要一直修改程式碼。

寫完這次作業後，我比較了解如何使用 class 管理資料，也學到動態陣列可以依照資料量增加容量。這次練習讓我對多項式的加法、乘法和代入計算有更多了解。

---

# 問題二：Polynomial Input / Output

## 解題說明

這題是要使用 C++ 的運算子多載，讓自己建立的 Polynomial 類別也可以使用 cin 和 cout 來輸入和輸出資料。

因為 Polynomial 是自己建立的類別，所以不能直接使用原本的輸入和輸出方式，需要自己定義 >> 和 << 的功能。

這題同樣使用 Term 和 Polynomial 來管理多項式，並且依照老師 PPT 的方式，將初始容量設定為 2，使用動態陣列儲存資料。

### 解題策略

1. 建立 Term 類別，用來存放每一項的係數 coef 和指數 exp。
2. 建立 Polynomial 類別，使用動態陣列來儲存多項式的資料。
3. 建構子將 capacity 設定為 2，並動態配置陣列。
4. 當陣列容量不足時，擴充陣列並將原本的資料複製過去。
5. 使用 friend 讓輸入和輸出的函數可以讀取類別裡面的 private 資料。
6. 在 >> 的部分，先輸入多項式的項數，再使用迴圈輸入每一項的係數和指數。
7. 在 << 的部分，使用迴圈把陣列裡面的資料依照指數大小印出來。
8. 最後在 main() 中使用 cin >> p 和 cout << p，確認輸入和輸出是否正常。

## 程式實作

以下是完整程式碼：

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

    int maxExp = 0;

    for (int i = 0; i < p.terms; i++) {
        if (p.termArray[i].exp > maxExp) {
            maxExp = p.termArray[i].exp;
        }
    }

    bool first = true;

    for (int e = maxExp; e >= 0; e--) {
        for (int i = 0; i < p.terms; i++) {
            if (p.termArray[i].exp == e) {
                float c = p.termArray[i].coef;

                if (!first) {
                    if (c >= 0)
                        out << " + ";
                    else
                        out << " - ";
                }
                else if (c < 0) {
                    out << "-";
                }

                out << abs(c) << "x^" << e;
                first = false;
            }
        }
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

* newTerm()：當陣列容量不足時，需要建立新陣列並複製原本資料，單次擴充需要 O(n) 的時間。
* operator>>()：需要使用迴圈輸入每一項資料，並透過 AddTerm() 檢查是否有相同指數，最壞情況為 O(n²)。
* operator<<()：使用迴圈依照指數大小輸出每一項資料，這個版本會從最高指數開始檢查，因此時間複雜度為 O(n+d)，其中 d 是最高指數。

### 2. 空間複雜度

程式使用動態陣列來儲存多項式，初始容量為 2。

當輸入的項目超過目前容量時，就會建立新的陣列，並將原本的資料複製過去，因此儲存資料的空間複雜度為 O(n)。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入資料               | 預期輸出         | 實際輸出               |
| ---- | ------------------ | ------------ | ------------------ |
| 測試一  | 3，接著輸入 3 2、2 1、1 0 | 3x² + 2x + 1 | 3x^2 + 2x^1 + 1x^0 |
| 測試二  | 2，接著輸入 5 3、4 1     | 5x³ + 4x     | 5x^3 + 4x^1        |
| 測試三  | 1，接著輸入 7 0         | 7            | 7x^0               |
| 測試四  | 0                  | 0            | 0                  |

### 測試輸入範例

```text
3
3 2
2 1
1 0
```

### 執行結果

```text
Enter number of terms: 3
Enter coefficient and exponent: 3 2
Enter coefficient and exponent: 2 1
Enter coefficient and exponent: 1 0
Polynomial: 3x^2 + 2x^1 + 1x^0
```

### 編譯與執行指令

```bash
g++ -std=c++17 -o polynomial_io polynomial_io.cpp
./polynomial_io
```

## 效能量測

這次我用不同項數來比較輸入和輸出需要處理幾次資料。

| 多項式項數 | 輸入資料處理次數 | 輸出資料處理次數 |
| ----- | -------- | -------- |
| 1 項   | 1 項      | 1 項      |
| 5 項   | 5 項      | 5 項      |
| 10 項  | 10 項     | 10 項     |
| 20 項  | 20 項     | 20 項     |

因為輸入的時候會使用迴圈逐項處理資料，輸出的時候也需要檢查每一項，所以項數增加時，程式需要處理的資料也會跟著增加。

這裡比較的是資料處理次數，並不是實際執行時間。

## 結論

1. 這次使用運算子多載來完成多項式的輸入和輸出。
2. 一開始我不太了解 >> 和 << 也可以自己定義功能，寫完之後比較了解它們的用法。
3. 使用 friend 可以讓運算子函數讀取類別裡面的 private 資料。
4. 這次也學到動態陣列可以隨著輸入的資料量增加容量，不需要一開始就設定很大的空間。
5. 透過這次練習，我對 C++ 的 class、動態陣列和運算子多載有更多了解。

## 申論及開發報告

這次作業主要是練習運算子多載，讓 Polynomial 類別可以直接使用 cin 和 cout。

一開始我先建立 Term 和 Polynomial 兩個類別，再想辦法讓外面的輸入和輸出函數可以讀取類別裡面的資料。

因為 Polynomial 裡面的資料是 private，所以我使用 friend 來處理這個問題，讓 operator>> 和 operator<< 可以讀取 termArray 裡面的資料。

輸入的部分，我先讓使用者輸入多項式有幾項，再使用迴圈輸入每一項的係數和指數，並透過 AddTerm() 將資料存進陣列。

如果輸入的項目超過原本的容量，就會使用 newTerm() 擴充陣列，把原本的資料複製到新的陣列，再繼續輸入。

輸出的部分則是使用迴圈把陣列裡面的資料依照指數大小印出來，這樣就可以比較容易看出多項式的內容。

一開始我覺得運算子多載比較難理解，因為平常都是直接使用 cin 和 cout，沒有想過自己建立的類別也可以定義這些功能。

實際寫完之後，我比較了解 friend 和運算子多載的用法，也知道可以利用這種方式讓程式使用起來更方便。

透過這次練習，我對 C++ 的 class、動態陣列和運算子多載有更多了解，也比較知道怎麼把課堂上學到的東西用在程式裡面。

---

# 整體心得

這次作業主要練習 C++ 的 Polynomial 類別，從一開始建立 Term 儲存係數和指數，到使用 Polynomial 管理整個多項式，讓我比較了解類別的使用方式。

問題一主要是練習多項式的加法、乘法和代入 X，也讓我學到動態陣列在空間不足時，可以重新配置更大的陣列，再把原本的資料複製過去。

問題二則是練習運算子多載，讓自己建立的 Polynomial 類別可以使用 cin 和 cout 進行輸入和輸出。

我覺得這次比較需要花時間的地方是動態陣列的擴充，以及乘法後相同指數的合併。一開始比較不清楚資料應該怎麼儲存，後來將功能分開寫成函數後，就比較容易理解。

完成這次作業後，我對 C++ 的 class、動態記憶體配置、多項式運算和運算子多載有更多認識，也更了解如何將不同的功能整理在同一個類別裡面。
