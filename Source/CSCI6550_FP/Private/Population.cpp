// Copyright 2021 Matthew Sentell


#include "Population.h"

UPopulation::UPopulation() : head(nullptr), ln(0) { // cxn
}

UPopulation::~UPopulation() { // dxn
	if (head != nullptr) { // list not empty
		for (FPopNode* i = head->nxt; head != nullptr; i = head->nxt) { // store & iterate path until empty
			delete head; // delete node
			head = i; // restore path
		}
	}
}
void UPopulation::in(UPeople& item) { // insert
	if (head == nullptr) { // list is empty
		head = new FPopNode; // insert new node at head
		head->ppl.set(item); // insert ppl
		head->nxt = nullptr; // mark end of path
	}
	else {
		FPopNode* a;
		a = head;
		head = new FPopNode; // insert node at sorted location, breaking path
		head->ppl.set(item); // insert ppl
		head->nxt = a->nxt; // prep forward location in path
	}
	ln++; // update length
}
void UPopulation::del(UPeople& item) { // delete
	if (head != nullptr) { // unempty list
		if (item == head->ppl) { // first
			FPopNode* a = head; // prep node for deletion
			head = head->nxt; // move head
			delete a; // delete node
			ln--; // update length
		}
		else { // general case
			FPopNode* a, * b;
			bool f;
			findItem(a, b, f, item);
			if (f) { // node in list
				a->nxt = b->nxt; // remove node from forward path
				delete b; // delete node
				ln--; // update length
			}
		}
	}

}
int UPopulation::length() const{ // public access to ln
	return ln;
}
void UPopulation::swap(FPopNode*& a, FPopNode*& b) { // swaps 2 nodes in the list
	FPopNode* x, * y, * z;
	bool f;
	findItem(x, y, f, a->ppl); // get node before a
	findItem(y, z, f, b->ppl); // get node before b
	x->nxt = b; // swap path to b
	y->nxt = a; // swap path to a
	z = a->nxt; // store path from a - could use x or y, but need z for second finditem anyway
	a->nxt = b->nxt; // swap path from a
	b->nxt = z; // swap path from b
}
void UPopulation::randomOrder() { // swaps nodes until all nodes have been swapped
	FPopNode* a = head;
	for (int i = 0; i < ln; i++) { // iterate through list 
		int x = rand() % ln - i; // how far b should iterate (a <= x < ln)
		FPopNode* b = a, * c = a->nxt; // set b to head & prep to iterate a
		for (int j = 0; j <= x; j++) { // set b to random node - is there a more efficient way?
			b = b->nxt;
		}
		swap(a, b); // swap operation
		a = c; // iterate a
	}
}
void UPopulation::findItem(FPopNode*& ploc, FPopNode*& loc, bool& found, UPeople& item) { // internal placefinding
	found = false;
	loc = nullptr;
	ploc = nullptr;
	if (head != nullptr) { // list not empty
		for (loc = head; loc != nullptr; loc = loc->nxt) { // iterate to sorted location
			if (item == loc->ppl) { // at sorted location, does item exist?
				found = true;
				break;
			}
			ploc = loc; // loc imediately iterates after this so this should stay one behind
		}
	}
}
