#pragma once
#include "../common.h"
#include "fwk.h"
#include "../aud/aud_interface.h"

template<class T>
struct ListElement {
	T val;

	//Used for identifying recently-accessed elements
	long long lastAccess = 0;
	int lastThread = 0;
	int lastInstanceID = -1;
	//

	ListElement();
	ListElement(const T&);

	inline ListElement<T>& operator=(const ListElement<T>&);
	inline operator T&();
};

//Called when an element is accessed
template<class T>
void onAccess(ListElement<T>&);

template<class T>
struct ListContext;

template<class T>
struct List {
	int sz;
	//Parent associated list
	List<T>* parent = nullptr;
	//Absolute offset from main list
	int offset = 0;
private:
	//Stores ListContext objects for child lists
	vector<ListContext<T>> children;
	//Mutex for modifying children vector
	sortMutex mtx_children;
public:
	//Associates a list as a child of this one with an absolute offset
	void associateAbs(List<T>& list, int offset);
	//Associates a list as a child of this one with a relative offset
	inline void associateRel(List<T>& list,int offset=0);
	//Associates a list as a child of this one
	inline void associate(List<T>& list);
	
	//De-associates a list from this one
	void deassociate(List<T>& list);
	//Iterate through each child List
	void iterateChildren(function<void(ListContext<T>&)>);

	virtual ~List();

	//Iterate through each element
	virtual void iterate(function<void(ListElement<T>&)>) = 0;
};

template<class T>
struct ListContext {
	//The List this context wraps
	List<int>* list;
	//The list that this list is relative to
	List<int>* parent;
	//The offset of this list within the relative list
	int offset;

	ListContext(List<int>* parent, List<int>* list, int offset = 0);
};

template<class T>
struct ArrayList : public List<T> {
	ListElement<T>* arr;

	ArrayList(int sz);
	~ArrayList();

	//Swaps the elements located at positions a and b
	void swap(int a, int b);

	//Accesses the a'th element
	inline ListElement<T>& operator[](int a);

	//Iterate through each element
	void iterate(function<void(ListElement<T>&)>);
};

template<class T>
struct LinkedList;

//Element of a LinkedList
template<class T>
struct LinkedListElement : public ListElement<T> {
	friend struct LinkedList<T>;
	LinkedList<T>* list;
private:
	LinkedListElement<T>* nxt;
	LinkedListElement<T>* prv;
public:

	LinkedListElement();
	LinkedListElement(const T&);
	~LinkedListElement();

	//Returns pointer to next element
	inline LinkedListElement<T>* next();
	//Returns pointer to previous element
	inline LinkedListElement<T>* prev();
	//Cleanly removes element from the linked list
	void extract();
	//Swaps values with element a
	void swapVal(LinkedListElement<T>* a);
	//Cleanly moves this element to be positioned after element a
	void moveAfter(LinkedListElement<T>* a);
	//Cleanly moves this element to be positioned before element a
	void moveBefore(LinkedListElement<T>* a);
	//Extracts a sublist from starting with this element and ending with subTail for moving to dest
	void cutSublistForMove(LinkedListElement<T>* subTail, LinkedList<T>* dest);
	//Places a sublist beginning with this node and ending with subTail after element a
	void cutSublistToAfter(LinkedListElement<T>* subTail, LinkedListElement<T>* a);
	//Places a sublist beginning with this node and ending with subTial before element a
	void cutSublistToBefore(LinkedListElement<T>* subTail, LinkedListElement<T>* a);
	//Cleanly moves this element, and all after it, to after another element
	void cutAfterToAfter(LinkedListElement<T>* a);
	//Cleanly moves this element, and all after it, to before another element
	void cutAfterToBefore(LinkedListElement<T>* a);
	//Cleanly moves this element, and all before it, to after another element
	void cutBeforeToAfter(LinkedListElement<T>* a);
	//Cleanly moves this element, and all before it, to before another element
	void cutBeforeToBefore(LinkedListElement<T>* a);
	//Cleanly deletes this element
	//void erase();

	//Returns the value of this element
	inline T& operator*();
};

//Multipurpose Array/Linked List
template<class T>
struct LinkedList : public List<T> {
	LinkedListElement<T>* head;
	LinkedListElement<T>* tail;

	LinkedList();
	LinkedList(ArrayList<T>&);
	~LinkedList();

	//Adds an element to the front of this list
	void addBack(LinkedListElement<T>* a);
	//Adds an element to the front of this list
	void addFront(LinkedListElement<T>* a);
	//Moves this entire list to the back of another element
	void moveListAfter(LinkedListElement<T>* a);
	//Moves this entire list to the back of another list
	void moveListAfter(LinkedList<T>* l);
	//Moves this entire list to the front of another element
	void moveListBefore(LinkedListElement<T>* a);
	//Moves this entire list to the front of another list
	void moveListBefore(LinkedList<T>* l);

	//Clears the entire list
	void clear();
	//Append element
	LinkedList<T>& operator+=(const T&);

	//Iterate through each element
	void iterate(function<void(ListElement<T>&)>);
};

//OnAccess

template<class T>
void onAccess(ListElement<T>& a) {
	if (!state.accessRestricted)
		return;
	state.token_sort();
	//Update current state
	auto& thr_st = threadState();
	thr_st.cycle_accessCount++;
	a.lastThread = threadId;
	a.lastAccess = thr_st.accessCount + thr_st.cycle_accessCount;
	a.lastInstanceID = instanceID;
	state.mtx_state.lock();
	playSound(a.val);
	if (state.token_sort) {
		state.cycle_accessValue += state.accessValuePer * thr_st.accessValueMul * state.accessValueMul;
		if (state.cycle_accessValue > state.cycle_valueLimit) {
			state.token_sort++;
		}
	}
	state.mtx_state.unlock();
	state.token_sort();
}

//List Context
template<class T>
ListContext<T>::ListContext(List<int>* parent, List<int>* list, int offset) {
	this->parent = parent;
	this->list = list;
	this->offset = offset;
}

//List Element
template<class T>
ListElement<T>::ListElement() {
	
}

template<class T>
ListElement<T>::ListElement(const T& a) : ListElement() {
	val = a;
}

template<class T>
ListElement<T>& ListElement<T>::operator=(const ListElement<T>& a) {
	val = a.val;
	onAccess(*this);
	return *this;
}

template<class T>
ListElement<T>::operator T&() {
	onAccess(*this);
	return val;
}

//List

template<class T>
void List<T>::associateAbs(List<T>& a, int offset) {
	if (a.parent == this)
		return;
	mtx_children.lock();
	if (a.parent != nullptr)
		a.parent->deassociate(a);
	children.push_back(ListContext<T>(this,&a,offset));
	a.parent = this;
	a.offset = offset;
	mtx_children.unlock();
}

template<class T>
void List<T>::associateRel(List<T>& a, int offset) {
	associateAbs(a, this->offset + offset);
}

template<class T>
void List<T>::associate(List<T>& a) {
	associateAbs(a, 0);
}

template<class T>
void List<T>::deassociate(List<T>& a) {
	mtx_children.lock();
	auto i = std::find_if(children.begin(), children.end(), [&](const ListContext<T>& b) {return b.list == &a; });
	if (i != children.end()) {
		children.erase(i);
		a.parent = nullptr;
	}
	mtx_children.unlock();
}

template<class T>
void List<T>::iterateChildren(function<void(ListContext<T>&)> f) {
	mtx_children.lock();
	for (auto& i : children)
		f(i);
	mtx_children.unlock();
}

template<class T>
List<T>::~List() {
	for (auto& i : children) {
		i.list->parent = nullptr;
	}
	if (parent != nullptr)
		parent->deassociate(*this);
}

//Array List

template<class T>
ArrayList<T>::ArrayList(int sz) {
	arr = new ListElement<T>[sz];
	this->sz = sz;
	//Clear the array
	for (int i = 0; i < sz; i++)
		arr[i].val = -1;
}

template<class T>
ArrayList<T>::~ArrayList() {
	delete[] arr;
}

template<class T>
void ArrayList<T>::swap(int a, int b) {
	T tmp = arr[a];
	onAccess(arr[a]);
	arr[a].val = arr[b].val;
	onAccess(arr[b]);
	arr[b].val = tmp;
}

template<class T>
ListElement<T>& ArrayList<T>::operator[](int a) {
	return arr[a];
}

template<class T>
void ArrayList<T>::iterate(function<void(ListElement<T>&)> a) {
	for (int i = 0; i < this->sz; i++) {
		a(arr[i]);
	}
}

//Linked List Element
template<class T>
LinkedListElement<T>::LinkedListElement() {
	nxt = nullptr;
	prv = nullptr;
	list = nullptr;
}

template<class T>
LinkedListElement<T>::LinkedListElement(const T& b) : LinkedListElement() {
	this->val = b;
}

template<class T>
LinkedListElement<T>::~LinkedListElement() {
	extract();
}

template<class T>
LinkedListElement<T>* LinkedListElement<T>::next() {
	if(nxt)
		onAccess(*nxt);
	return nxt;
}

template<class T>
LinkedListElement<T>* LinkedListElement<T>::prev() {
	if(prv)
		onAccess(*prv);
	return prv;
}

template <class T>
void LinkedListElement<T>::extract() {
	if (!list)
		return;
	if (nxt)
		nxt->prv = prv;
	else
		list->tail = prv;
	if (prv)
		prv->nxt = nxt;
	else
		list->head = nxt;
	nxt = nullptr;
	prv = nullptr;
	list->sz--;
	list = nullptr;
}

template<class T>
void LinkedListElement<T>::swapVal(LinkedListElement<T>* a) {
	T tmp = this->val;
	this->val = a->val;
	a->val = tmp;
}

template<class T>
void LinkedListElement<T>::moveAfter(LinkedListElement<T>* a) {
	onAccess(*this);
	extract();
	list = a->list;
	list->sz++;
	prv = a;
	nxt = a->nxt;
	if (nxt)
		nxt->prv = this;
	else
		list->tail = this;
	a->nxt = this;
}

template<class T>
void LinkedListElement<T>::moveBefore(LinkedListElement<T>* a) {
	onAccess(*this);
	extract();
	list = a->list;
	list->sz++;
	nxt = a;
	prv = a->prv;
	if (prv)
		prv->nxt = this;
	else
		list->head = this;
	a->prv = this;
}

template <class T>
void LinkedListElement<T>::cutSublistForMove(LinkedListElement<T>* subTail, LinkedList<T>* dest) {
	List<T>* from = list;
	//Extract the sublist
	if (list->head == this)
		list->head = subTail->nxt;
	else
		prv->nxt = subTail->nxt;
	if (list->tail == subTail)
		list->tail = prv;
	else
		subTail->nxt->prv = prv;

	prv = nullptr;
	subTail->nxt = nullptr;
	//Update list and sublist elements for relocation to dest
	LinkedListElement<T>* tmp = this;
	int subSz = 0;
	while (tmp != nullptr) {
		subSz++;
		tmp->list = dest;
		tmp = tmp->nxt;
	}
	//Update lists
	from->sz -= subSz;
	dest->sz += subSz;
}

template <class T>
void LinkedListElement<T>::cutSublistToAfter(LinkedListElement<T>* subTail, LinkedListElement<T>* a) {
	cutSublistForMove(subTail, a->list);

	subTail->nxt = a->nxt;
	if (subTail->nxt)
		subTail->nxt->prv = subTail;
	else
		a->list->tail = subTail;

	a->nxt = this;
	prv = a;
}

template <class T>
void LinkedListElement<T>::cutSublistToBefore(LinkedListElement<T>* subTail, LinkedListElement<T>* a) {
	cutSublistForMove(subTail, a->list);

	prv = a->prv;
	if (prv)
		prv->nxt = this;
	else
		a->list->head = this;

	a->prv = subTail;
	subTail->nxt = a;
}

template <class T>
void LinkedListElement<T>::cutAfterToAfter(LinkedListElement<T>* a) {
	cutSublistToAfter(list->tail, a);
}

template <class T>
void LinkedListElement<T>::cutAfterToBefore(LinkedListElement<T>* a) {
	cutSublistToBefore(list->tail, a);
}

template <class T>
void LinkedListElement<T>::cutBeforeToAfter(LinkedListElement<T>* a) {
	list->head->cutSublistToAfter(this, a);
}

template <class T>
void LinkedListElement<T>::cutBeforeToBefore(LinkedListElement<T>* a) {
	list->head->cutSublistToBefore(this, a);
}

//template<class T>
//void LinkedListElement<T>::erase() {
//	extract();
//	delete this;
//}

template<class T>
T& LinkedListElement<T>::operator*() {
	return this->val;
}

//Linked List

template<class T>
LinkedList<T>::LinkedList() {
	head = nullptr;
	tail = nullptr;
	this->sz = 0;
}

template<class T>
LinkedList<T>::LinkedList(ArrayList<T>& a) {

}

template<class T>
LinkedList<T>::~LinkedList() {
	clear();
}

template<class T>
void LinkedList<T>::addBack(LinkedListElement<T>* a) {
	if (tail)
		a->moveAfter(tail);
	else {
		a->extract();
		a->list = this;
		this->sz++;
		head = a;
		tail = a;
	}
}

template<class T>
void LinkedList<T>::addFront(LinkedListElement<T>* a) {
	if(head)
		a->moveBefore(head);
	else {
		a->extract();
		a->list = this;
		this->sz++;
		head = a;
		tail = a;
	}
}

template<class T>
void LinkedList<T>::moveListAfter(LinkedListElement<T>* a) {
	if(head)
		head->cutAfterToAfter(a);
}

template<class T>
void LinkedList<T>::moveListAfter(LinkedList<T>* l) {
	if (!head)
		return;
	if (!l->head)
		l->addBack(head);
	moveListAfter(l->tail);
}

template<class T>
void LinkedList<T>::moveListBefore(LinkedListElement<T>* a) {
	if (head)
		head->cutAfterToBefore(a);
}

template<class T>
void LinkedList<T>::moveListBefore(LinkedList<T>* l) {
	if (!head)
		return;
	if (!l->head)
		l->addBack(head);
	moveListBefore(l->head);
}

template<class T>
void LinkedList<T>::clear() {
	LinkedListElement<T>* tmp = head;
	LinkedListElement<T>* tmp2;
	while (tmp != nullptr) {
		tmp2 = tmp;
		tmp = tmp->nxt;
		delete tmp2;
	}
	this->sz = 0;
	head = nullptr;
	tail = nullptr;
}

template<class T>
LinkedList<T>& LinkedList<T>::operator+=(const T& a) {
	LinkedListElement<T>* tmp = new LinkedListElement<T>(a);
	
	addBack(tmp);

	return *this;
}

template<class T>
void LinkedList<T>::iterate(function<void(ListElement<T>&)> a) {
	LinkedListElement<T>* tmp = head;
	while (tmp != nullptr) {
		a(*tmp);
		tmp = tmp->nxt;
	}
}