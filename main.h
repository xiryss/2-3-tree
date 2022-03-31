#include <cstdint>
#include <cmath>
#include <random>
#include <iomanip>
#include <numeric>
#include <climits>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <algorithm>
#define all(a) (a).begin(), (a).end()
#define pbc push_back
#define pob pop_back()
#define vin(a) for (auto& i : a) cin >> i
#define sp system("pause")
#define mp make_pair
template<class ValueType> class Set {
public:
    struct Node {
        std::vector<size_t> sons;
        std::vector<std::pair<ValueType, ValueType>> keys;
        std::pair<ValueType, ValueType> val;
        int parent;
        Node() {
            parent = -1;
            sons.clear();
            keys.clear();
        }
        Node(const ValueType& x) {
            val = { x,x };
            parent = -1;
            sons.clear();
            keys.clear();
        }
    };
    Set(const Set& a) : tree(a.tree), root(a.root), sz(a.sz) {}
    Set() {
        tree.clear();
        sz = 0;
    }
    ~Set() {
        tree.clear();
        root = -1;
        sz = 0;
    }
    Set& operator=(const Set& a) {
        if (&a == this) {
            return *this;
        }
        tree = a.tree;
        root = a.root;
        sz = a.sz;
        return *this;
    }
    Set(std::initializer_list<ValueType> list_init) {
        tree.clear();
        for (auto j : list_init) {
            insert(j);
        }
    }
    template<typename Iter>
    Set(Iter beg, Iter en) {
        while (beg != en) {
            insert(*beg);
            ++beg;
        }
    }
    int getson(int v, int son) const {
        for (size_t j = 0; j < tree[v].sons.size(); ++j) {
            if (tree[v].sons[j] == son) return j;
        }
        assert(0);
    }
    void trecalc(int v) {
        if (!tree[v].sons.size()) {
            tree[v].keys = { tree[v].val };
        }
        else {
            tree[v].keys.resize(tree[v].sons.size());
            for (int j = 0; j < tree[v].sons.size(); ++j) {
                tree[v].keys[j] = tree[tree[v].sons[j]].val;
            }
            tree[v].val = { tree[v].keys[0].first, tree[v].keys.back().second };
        }
    }
    void recalc(int v) {
        while (v != -1) {
            trecalc(v);
            v = tree[v].parent;
        }
    }
    friend class iterator;
    class iterator {
    public:
        size_t node_id = 0;
        const Set<ValueType>* iter_set;
        iterator& operator=(const iterator& other) {
            node_id = other.v, iter_set = other.iter_set;
            return *this;
        }
        iterator() : node_id(0), s(nullptr) {}
        iterator(size_t u, const Set<ValueType>* t) {
            node_id = u;
            iter_set = t;
        }
        iterator(const iterator& other) : node_id(other.node_id), iter_set(other.iter_set) {}
        const ValueType* operator->()const {
            return &(iter_set->tree[node_id].val.first);
        }
        iterator& operator++() {
            while (node_id != iter_set->root)
            {
                size_t nw = iter_set->tree[node_id].parent;
                size_t son_id = iter_set->getson(nw, node_id);
                if (son_id == iter_set->tree[nw].sons.size() - 1) {
                    node_id = iter_set->tree[node_id].parent;
                    continue;
                }
                break;
            }
            int nw = iter_set->tree[node_id].parent;
            if (nw == -1) {
                node_id = iter_set->tree.size();
                return *this;
            }
            size_t son_id = iter_set->getson(nw, node_id);
            ++son_id;
            node_id = iter_set->tree[nw].sons[son_id];
            while (iter_set->tree[node_id].sons.size()) {
                node_id = iter_set->tree[node_id].sons[0];
            }
            return *this;
        }
        iterator& operator--() {
            if (node_id == iter_set->tree.size()) {
                node_id = iter_set->root;
                while (iter_set->tree[node_id].sons.size()) {
                    node_id = iter_set->tree[node_id].sons.back();
                }
                return *this;
            }
            while (node_id != iter_set->root) {
                size_t nw = iter_set->tree[node_id].parent;
                size_t son_id = iter_set->getson(nw, node_id);
                if (son_id == 0) {
                    node_id = iter_set->tree[node_id].parent;
                    continue;
                }
                break;
            }
            size_t nw = iter_set->tree[node_id].parent;
            size_t son_id = iter_set->getson(nw, node_id);
            --son_id;
            node_id = iter_set->tree[nw].sons[son_id];
            while (iter_set->tree[node_id].sons.size()) {
                node_id = iter_set->tree[node_id].sons.back();
            }
            return *this;
        }
        iterator operator--(int) {
            auto to_ret = *this;
            if (node_id == iter_set->tree.size()) {
                node_id = iter_set->root;
                while (iter_set->tree[node_id].sons.size()) {
                    node_id = iter_set->tree[node_id].sons.back();
                }
                return to_ret;
            }
            while (node_id != iter_set->root) {
                size_t nw = iter_set->tree[node_id].parent;
                size_t son_id = iter_set->getson(nw, node_id);
                if (son_id == 0) {
                    node_id = iter_set->tree[node_id].parent;
                    continue;
                }
                break;
            }
            size_t nw = iter_set->tree[node_id].parent;
            size_t son_id = iter_set->getson(nw, node_id);
            --son_id;
            node_id = iter_set->tree[nw].sons[son_id];
            while (iter_set->tree[node_id].sons.size()) {
                node_id = iter_set->tree[node_id].sons.back();
            }
            return to_ret;
        }
        iterator operator++(int) {
            auto to_ret = *this;
            while (node_id != iter_set->root) {
                size_t nw = iter_set->tree[node_id].parent;
                size_t son_id = iter_set->getson(nw, node_id);
                if (son_id == iter_set->tree[nw].sons.size() - 1) {
                    node_id = iter_set->tree[node_id].parent;
                    continue;
                }
                break;
            }
            int nw = iter_set->tree[node_id].parent;
            if (nw == -1) {
                node_id = iter_set->tree.size();
                return to_ret;
            }
            size_t son_id = iter_set->getson(nw, node_id);
            if (son_id == iter_set->tree[nw].sons.size()) {
                node_id = iter_set->tree.size();
                return to_ret;
            }
            ++son_id;
            node_id = iter_set->tree[nw].sons[son_id];
            while (iter_set->tree[node_id].sons.size()) {
                node_id = iter_set->tree[node_id].sons[0];
            }
            return to_ret;
        }
        ValueType operator*() const {
            return iter_set->tree[node_id].val.first;
        }
        bool operator==(const Set::iterator& a) const {
            return node_id == a.node_id;
        }
        bool operator!=(const Set::iterator& a) const {
            return node_id != a.node_id;
        }
    };
    Set::iterator end() const {
        return Set::iterator(tree.size(), this);
    }
    Set::iterator begin() const {
        if (root == -1) {
            return end();
        }
        Set::iterator it(root, this);
        while (tree[it.node_id].sons.size()) {
            it.node_id = tree[it.node_id].sons[0];
        }
        return it;
    }
    size_t size() const {
        return sz;
    }
    bool equal(const ValueType& v1, const ValueType& v2) const {
        return !(v1 < v2) && !(v2 < v1);
    }
    bool notequal(const ValueType& v1, const ValueType& v2) const {
        return !equal(v1, v2);
    }
    Set::iterator search(ValueType val) const {
        int v = root;
        if (root == -1) {
            return end();
        }
        while (tree[v].sons.size()) {
            if (tree[v].sons.size() == 2) {
                if (tree[v].keys[0].second < val) {
                    v = tree[v].sons[1];
                }
                else {
                    v = tree[v].sons[0];
                }
            }
            else if (tree[v].keys[1].second < val) {
                v = tree[v].sons[2];
            }
            else if (tree[v].keys[0].second < val) {
                v = tree[v].sons[1];
            }
            else v = tree[v].sons[0];
        }
        return Set::iterator(v, this);
    }
    Set::iterator find(const ValueType& val) const {
        auto res = search(val);
        if (res == end() || notequal(*res, val)) {
            return end();
        }
        return res;
    }
    void split(int v) {
        if (tree[v].sons.size() <= 3) {
            return;
        }
        Node nw = Node();
        nw.sons = { tree[v].sons[2], tree[v].sons[3] };
        nw.keys = { tree[v].keys[2], tree[v].keys[3] };
        nw.parent = tree[v].parent;
        tree.pbc(nw);
        trecalc(tree.size() - 1);
        tree[tree[v].sons[2]].p = tree.size() - 1;
        tree[tree[v].sons[3]].p = tree.size() - 1;

        tree[v].sons.resize(2);
        tree[v].keys.resize(2);
        trecalc(v);
        if (tree[v].parent != -1) {
            tree.back().parent = tree[v].parent;
            trecalc(tree.size() - 1);
            tree[tree.back().parent].sons.pbc(tree.size() - 1);
            std::sort(all(tree[tree.back().parent].sons), [&](int x, int y) {
                return tree[x].val < tree[y].val;
                });
            trecalc(tree.back().parent);
            split(tree.back().parent);
        }
        else {
            trecalc(v);
            tree.pbc(tree[root]);
            root = tree.size() - 1;
            tree[root].sons[0] = v;
            tree[root].sons[1] = tree.size() - 2;
            tree[tree[root].sons[0]].parent = root;
            tree[tree[root].sons[1]].parent = root;
            trecalc(root);
            trecalc(tree[root].sons[0]);
            trecalc(tree[root].sons[1]);
            std::sort(all(tree[root].sons), [&](int x, int y) {
                return tree[x].val < tree[y].val;
                });
            trecalc(root);
        }
    }
    void insert_size_one(Node nw) {
        root = tree.size() - 1;
        tree.pbc(nw);
        tree.pbc(tree[root]);
        tree[root].sons.resize(2);
        tree[root].keys.resize(2);
        tree[root].sons[0] = root + 1, tree[root].sons[1] = root + 2;
        std::sort(all(tree[root].sons), [&](int x, int y) {return tree[x].val < tree[y].val; });
        recalc(root);
        tree[root + 2].sons.clear();
        tree[root + 2].keys.clear();
        tree[root + 1].sons.clear();
        tree[root + 1].keys.clear();
        tree[root + 1].parent = tree[root + 2].parent = root;
    }
    void insert_general(Node nw, int v) {
        int parent = tree[v].parent;
        tree[parent].sons.pbc(tree.size());
        tree.pbc(nw);
        tree.back().parent = parent;
        std::sort(all(tree[parent].sons), [&](int x, int y) {
            return tree[x].val < tree[y].val;
            });
        recalc(tree.size() - 1);
        split(parent);
    }
    void insert(const ValueType& val) {
        Node nw = Node(val);
        if (root == -1) {
            ++sz;
            root = tree.size();
            tree.pbc(nw);
            return;
        }
        auto it = search(val);
        if (it != end() && equal(*it, val)) {
            return;
        }
        ++sz;
        size_t v = it.node_id;
        if (tree[v].parent == -1) {
            insert_size_one(nw);
        }
        else {
            insert_general(nw);
        }
        recalc(tree.size() - 1);
    }
    Set::iterator lower_bound(const ValueType& val) const {
        if (root == -1) {
            return end();
        }
        size_t v = root;
        if (tree[v].val.second < val) {
            return end();
        }
        while (tree[v].sons.size()) {
            for (size_t j = 0; j < 3; ++j) {
                if (equal(tree[v].keys[j].second, val) || val < tree[v].keys[j].second) {
                    v = tree[v].sons[j];
                    break;
                }
            }
        }
        return Set::iterator(v, this);
    }
    int getbrother(size_t v, size_t son) const {
        if (tree[v].sons.size() == 2) {
            return tree[v].sons[0] ^ tree[v].sons[1] ^ son;
        }
        else {
            if (tree[v].sons[1] == son) {
                return tree[v].sons[2];
            }
            return tree[v].sons[1];
        }
    }
    void erase_size_three(int parent, Set::iterator x) {
        tree[parent].sons.erase(std::find(all(tree[parent].sons), x.v));
        recalc(parent);
        tree[v].parent = -1;
    }
    void erase_root_parent(int parent, int v) {
        tree[tree[v].parent].sons.erase(std::find(all(tree[tree[v].parent].sons), v));
        root = tree[tree[v].parent].sons[0];
        tree[v].parent = -1;
        tree[root].parent = -1;    
    }
    int erase_general(int parent, int v) {
        size_t bro = (tree[parent].sons[0] == v ? tree[parent].sons[1] : tree[parent].sons[0]);
        size_t pp = tree[parent].parent;
        size_t bruh = getbrother(pp, parent);
        tree[bruh].sons.pbc(bro);
        std::sort(all(tree[bruh].sons), [&](int x, int y) {
            return tree[x].val < tree[y].val;
            });
        tree[bro].parent = bruh;
        tree[parent].sons.clear();
        tree[v].parent = -1;
        trecalc(bruh);
        split(bruh);
        eraser(Set::iterator(pparent, this));
        return bro;
    }
    int eraser(Set::iterator x) {
        if (x == end()) {
            return 0;
        }
        if (tree[x.node_id].parent == -1) {
            root = -1;
            return 0;
        }
        int parent = tree[x.node_id].parent;
        size_t v = x.node_id;
        if (tree[parent].sons.size() == 3) {
            erase_size_three(parent, x);
            return 0;
        }
        if (tree[v].parent == root) {
            erase_root_parent(parent, v);
            return 0;
        }
        return erase_general(parent, v);
    }
    bool empty() const {
        return root == -1;
    }
    void erase(const ValueType& x) {
        auto y = search(x);
        if (y == end() || *y != x) {
            return;
        }
        --sz;
        size_t bro = eraser(y);
        if (bro < tree.size() && root != -1) {
            recalc(bro);
        }
    }
private:
    std::vector<Node> tree;
    int root = -1;
    size_t sz = 0;
};
