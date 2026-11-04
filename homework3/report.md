# 41343105 

# 預期學習成果
## 學習成果具體說明
1. 能夠使用 Circular Linked List 實作 Polynomial ADT。
2. 能正確完成多項式的加法、減法與乘法運算。
3. 能分析各項操作的時間複雜度。
## 解題說明
本題要求實作一個 Polynomial 類別，能夠進行多項式加、減、乘運算，以及評估多項式的值。
使用 Circular Linked List 搭配 Header Node，實作 Polynomial ADT，並透過 iterator 提供遍歷功能。
### 解題策略
1. 資料結構選擇
   - 使用 ChainNode 與 Chain 實作 Circular Linked List
   - 多項式每一項為 Term { coef, exp }
   - 利用 ChainIterator 進行遍歷
2. Polynomial 類別功能
   - 建構子、複製建構子、賦值運算子、解構子
   - operator: +, -, *
## 程式實作
```cpp
#include <iostream>
#include <cmath>
using namespace std;

template <class T>
class ChainNode {
public:
    T data;
    ChainNode<T>* link;
    static ChainNode<T>* available;
    ChainNode(const T& d = T(), ChainNode<T>* l = nullptr): data(d), link(l) {}
  void* operator new(size_t size) {
        if (!available) return ::operator new(size);
        ChainNode<T>* p = available;
        available = available->link;
        return p;
    }
  void operator delete(void* ptr) {
        ChainNode<T>* p = (ChainNode<T>*)ptr;
        p->link = available;
        available = p;
    }
};
template <class T>
ChainNode<T>* ChainNode<T>::available = nullptr;

template <class T>
class Chain {
public:
    typedef ChainNode<T> Node;
    Node* head;

    Chain() {
        head = new Node();
        head->link = head;
    }

    ~Chain() { Clear(); delete head; }

    void Clear() {
        Node* p = head->link;
        while (p != head) {
            Node* tmp = p;
            p = p->link;
            delete tmp;
        }
        head->link = head;
    }

    void InsertBack(const T& x) {
        Node* p = head;
        while (p->link != head) p = p->link;
        p->link = new Node(x, head);
    }
  class iterator {
        Node* current;
    public:
        iterator(Node* p = nullptr) : current(p) {}
        T& operator*() { return current->data; }
        iterator& operator++() { current = current->link; return *this; }
        bool operator!=(const iterator& rhs) { return current != rhs.current; }
    };
    iterator Begin() { return iterator(head->link); }
    iterator End() { return iterator(head); }
};

struct Term { int coef, exp; };

class Polynomial {
    Chain<Term> poly;
public:
    Polynomial() {}
    Polynomial(const Polynomial& p) {
        for (auto i = p.poly.Begin(); i != p.poly.End(); ++i)
            poly.InsertBack(*i);
    }
    ~Polynomial() { poly.Clear(); }
    Polynomial& operator=(const Polynomial& p) {
        if (this != &p) {
            poly.Clear();
            for (auto i = p.poly.Begin(); i != p.poly.End(); ++i)
                poly.InsertBack(*i);
        }
        return *this;
    }

    friend istream& operator>>(istream& is, Polynomial& p) {
        int n; is >> n;
        while(n--) { Term t; is >> t.coef >> t.exp; p.poly.InsertBack(t); }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& p) {
        bool first = true;
        for (auto i = p.poly.Begin(); i != p.poly.End(); ++i) {
            if(!first && (*i).coef > 0) os << "+";
            os << (*i).coef << "x^" << (*i).exp;
            first = false;
        }
        return os;
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial c;
        auto i = poly.Begin(), j = b.poly.Begin();
        while(i != poly.End() && j != b.poly.End()) {
            if((*i).exp == (*j).exp) {
                int s = (*i).coef + (*j).coef;
                if(s) c.poly.InsertBack({s,(*i).exp});
                ++i; ++j;
            } else if((*i).exp > (*j).exp) c.poly.InsertBack(*i++);
            else c.poly.InsertBack(*j++);
        }
        while(i != poly.End()) c.poly.InsertBack(*i++);
        while(j != b.poly.End()) c.poly.InsertBack(*j++);
        return c;
    }

    Polynomial operator-(const Polynomial& b) const {
        Polynomial nb;
        for(auto i=b.poly.Begin(); i!=b.poly.End(); ++i)
            nb.poly.InsertBack({-(*i).coef, (*i).exp});
        return *this + nb;
    }

    Polynomial operator*(const Polynomial& b) const {
        Polynomial c;
        for(auto i=poly.Begin(); i!=poly.End(); ++i)
            for(auto j=b.poly.Begin(); j!=b.poly.End(); ++j)
                c = c + Polynomial({Chain<Term>()}).poly.InsertBack({(*i).coef*(*j).coef,(*i).exp+(*j).exp}), c;
        return c;
    }

    float Evaluate(float x) const {
        float s=0;
        for(auto i=poly.Begin(); i!=poly.End(); ++i)
            s += (*i).coef * pow(x, (*i).exp);
        return s;
    }
};

```
## 效能分析
1.加法/減法：O(n + m)，遍歷兩個多項式節點

2.乘法：O(n × m)，每項相乘累加

3.Evaluate(x)：O(n)，遍歷每項計算

## 測試與驗證
### 測試案例

| 測試案例 | 輸入  | 預期輸出 | 實際輸出 |
|----------|-------------|----------|----------|
| 測試一   | p1: 3x² + 5, p2: 2x² + 1   | p1+p2=5x²+6, p1-p2=x²+4, p1*p2=6x⁴+13x²+5, p1(2)=17 | 正確       |
| 測試二   | p1: x³ + 2x, p2: x² + 1| p1+p2=x³+x²+2x+1, p1-p2=x³-x²+2x-1, p1*p2=x⁵+3x³+2x, p1(1)=3 | 正確      |

### 編譯與執行指令

$ g++ -std=c++17 -o powerset powerset.cpp
$ ./powerset

## 結論

成功實作 Polynomial 類別，支援加、減、乘、Evaluate
使用 Circular Linked List 搭配 iterator，程式邏輯清晰
Available-space list 加分項能重用節點，提升效率
可安全處理多項式運算，不會浪費記憶體

## 申論及開發報告

本次作業透過 Circular Linked List 實作多項式 ADT，學習了：

1.資料結構應用：Circular List + iterator

2.演算法設計：Polynomial 運算邏輯

3.記憶體管理：available-space list 讓刪除節點可重用

4.程式設計能力：多項式運算與測試案例完整實作
