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
		int p;
		Node() {
			p = -1;
			sons.clear();
			keys.clear();
		}
		Node(const ValueType& x) {
			val = { x,x };
			p = -1;
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
	Set(std::initializer_list<ValueType> l) {
		tree.clear();
		for (auto j : l) {
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
			v = tree[v].p;
		}
	}
	friend class iterator;
	class iterator {
	public:
		size_t v = 0;
		const Set<ValueType>* s;
		iterator& operator=(const iterator& other) {
			v = other.v, s = other.s;
			return *this;
		}
		iterator() {
			v = 0;
			s = nullptr;
		}
		iterator(size_t u, const Set<ValueType>* t) {
			v = u;
			s = t;
		}
		iterator(const iterator& other) : v(other.v), s(other.s) {}
		const ValueType* operator->()const {
			return &(s->tree[v].val.first);
		}
		iterator& operator++() {
			while (v != s->root)
			{
				size_t nw = s->tree[v].p;
				size_t sonid = s->getson(nw, v);
				if (sonid == s->tree[nw].sons.size() - 1) {
					v = s->tree[v].p;
					continue;
				}
				break;
			}
			int nw = s->tree[v].p;
			if (nw == -1) {
				v = s->tree.size();
				return *this;
			}
			size_t sonid = s->getson(nw, v);
			++sonid;
			v = s->tree[nw].sons[sonid];
			while (s->tree[v].sons.size()) {
				v = s->tree[v].sons[0];
			}
			return *this;
		}
		iterator& operator--() {
			if (v == s->tree.size()) {
				v = s->root;
				while (s->tree[v].sons.size()) {
					v = s->tree[v].sons.back();
				}
				return *this;
			}
			while (v != s->root) {
				size_t nw = s->tree[v].p;
				size_t sonid = s->getson(nw, v);
				if (sonid == 0) {
					v = s->tree[v].p;
					continue;
				}
				break;
			}
			size_t nw = s->tree[v].p;
			size_t sonid = s->getson(nw, v);
			--sonid;
			v = s->tree[nw].sons[sonid];
			while (s->tree[v].sons.size()) {
				v = s->tree[v].sons.back();
			}
			return *this;
		}
		iterator operator--(int) {
			auto to_ret = *this;
			if (v == s->tree.size()) {
				v = s->root;
				while (s->tree[v].sons.size()) {
					v = s->tree[v].sons.back();
				}
				return to_ret;
			}
			while (v != s->root) {
				size_t nw = s->tree[v].p;
				size_t sonid = s->getson(nw, v);
				if (sonid == 0) {
					v = s->tree[v].p;
					continue;
				}
				break;
			}
			size_t nw = s->tree[v].p;
			size_t sonid = s->getson(nw, v);
			--sonid;
			v = s->tree[nw].sons[sonid];
			while (s->tree[v].sons.size()) {
				v = s->tree[v].sons.back();
			}
			return to_ret;
		}
		iterator operator++(int) {
			auto to_ret = *this;
			while (v != s->root) {
				size_t nw = s->tree[v].p;
				size_t sonid = s->getson(nw, v);
				if (sonid == s->tree[nw].sons.size() - 1) {
					v = s->tree[v].p;
					continue;
				}
				break;
			}
			int nw = s->tree[v].p;
			if (nw == -1) {
				v = s->tree.size();
				return to_ret;
			}
			size_t sonid = s->getson(nw, v);
			if (sonid == s->tree[nw].sons.size()) {
				v = s->tree.size();
				return to_ret;
			}
			++sonid;
			v = s->tree[nw].sons[sonid];
			while (s->tree[v].sons.size()) {
				v = s->tree[v].sons[0];
			}
			return to_ret;
		}
		ValueType operator*() const {
			return s->tree[v].val.first;
		}
		bool operator==(const Set::iterator& a) const {
			return v == a.v;
		}
		bool operator!=(const Set::iterator& a) const {
			return v != a.v;
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
		while (tree[it.v].sons.size()) {
			it.v = tree[it.v].sons[0];
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
		nw.p = tree[v].p;
		tree.pbc(nw);
		trecalc(tree.size() - 1);
		tree[tree[v].sons[2]].p = tree.size() - 1;
		tree[tree[v].sons[3]].p = tree.size() - 1;

		tree[v].sons.resize(2);
		tree[v].keys.resize(2);
		trecalc(v);
		if (tree[v].p != -1) {
			tree.back().p = tree[v].p;
			trecalc(tree.size() - 1);
			tree[tree.back().p].sons.pbc(tree.size() - 1);
			std::sort(all(tree[tree.back().p].sons), [&](int x, int y) {
				return tree[x].val < tree[y].val;
				});
			trecalc(tree.back().p);
			split(tree.back().p);
		}
		else {
			trecalc(v);
			tree.pbc(tree[root]);
			root = tree.size() - 1;
			tree[root].sons[0] = v;
			tree[root].sons[1] = tree.size() - 2;
			tree[tree[root].sons[0]].p = root;
			tree[tree[root].sons[1]].p = root;
			trecalc(root);
			trecalc(tree[root].sons[0]);
			trecalc(tree[root].sons[1]);
			std::sort(all(tree[root].sons), [&](int x, int y) {
				return tree[x].val < tree[y].val;
				});
			trecalc(root);
		}
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
		size_t v = it.v;
		if (tree[v].p == -1) {
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
			tree[root + 1].p = tree[root + 2].p = root;
		}
		else {
			int p = tree[v].p;
			tree[p].sons.pbc(tree.size());
			tree.pbc(Node(val));
			tree.back().p = p;
			std::sort(all(tree[p].sons), [&](int x, int y) {
				return tree[x].val < tree[y].val;
				});
			recalc(tree.size() - 1);
			split(p);
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
	int eraser(Set::iterator x) {
		if (x == end()) {
			return 0;
		}
		if (tree[x.v].p == -1) {
			root = -1;
			return 0;
		}
		int p = tree[x.v].p;
		size_t v = x.v;
		if (tree[p].sons.size() == 3) {
			tree[p].sons.erase(std::find(all(tree[p].sons), x.v));
			recalc(p);
			tree[v].p = -1;
			return 0;
		}
		if (tree[v].p == root) {
			tree[tree[v].p].sons.erase(std::find(all(tree[tree[v].p].sons), v));
			root = tree[tree[v].p].sons[0];
			tree[v].p = -1;
			tree[root].p = -1;
			return 0;
		}
		size_t bro = (tree[p].sons[0] == x.v ? tree[p].sons[1] : tree[p].sons[0]);
		size_t pp = tree[p].p;
		size_t bruh = getbrother(pp, p);
		tree[bruh].sons.pbc(bro);
		std::sort(all(tree[bruh].sons), [&](int x, int y) {
			return tree[x].val < tree[y].val;
			});
		tree[bro].p = bruh;
		tree[p].sons.clear();
		tree[v].p = -1;
		trecalc(bruh);
		split(bruh);
		eraser(Set::iterator(p, this));
		return bro;
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
