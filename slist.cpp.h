#include <cstdlib>

template<class Object>
SList<Object>::SList() {
	init();
}

template<class Object>
SList<Object>::SList(const SList &rhs) {
	init();
	*this = rhs;                                   // then assign rhs to this.
}

template<class Object>
void SList<Object>::init() {
	for (int i = 0; i < LEVEL; i++) {  // for each level
		// create the left most dummy nodes;
		header[i] = new SListNode<Object>;
		header[i]->prev = NULL;
		header[i]->down = (i > 0) ? header[i - 1] : NULL;
		header[i]->up = NULL;
		if (i > 0) header[i - 1]->up = header[i];

		// create the right most dummy nodes;
		header[i]->next = new SListNode<Object>;
		header[i]->next->next = NULL;
		header[i]->next->prev = header[i];
		header[i]->next->down = (i > 0) ? header[i - 1]->next : NULL;
		header[i]->next->up = NULL;
		if (i > 0) header[i - 1]->next->up = header[i]->next;
	}

	// reset cost.
	cost = 0;
}

template<class Object>
SList<Object>::~SList() {
	clear();                                      // delete items starting 1st
	for (int i = 0; i < LEVEL; i++) {
		delete header[i]->next;                      // delete the right most dummy
		delete header[i];                            // delete the left most dummy
	}
}

template<class Object>
bool SList<Object>::isEmpty() const {
	return (header[0]->next->next == NULL);
}

template<class Object>
int SList<Object>::size() const {
	SListNode<Object> *p = header[0]->next; // at least the right most dummy
	int size = 0;

	for (; p->next != NULL; p = p->next, ++size);
	return size;
}

template<class Object>
void SList<Object>::clear() {
	for (int i = 0; i < LEVEL; i++) {        // for each level
		SListNode<Object> *p = header[i]->next;  // get the 1st item 
		while (p->next != NULL) {              // if this is not the left most
			SListNode<Object> *del = p;
			p = p->next;                           // get the next item
			delete del;                            // delete the current item
		}

		header[i]->next = p;                     // p now points to the left most 
	}                                          // let the right most point to it
}

template<class Object>
void SList<Object>::insert(const Object &obj) {
	// right points to the level-0 item before which a new object is inserted.
	SListNode<Object> *right = searchPointer(obj);
	SListNode<Object> *up = NULL;

	if (right->next != NULL && right->item == obj)
		// there is an identical object
		return;

	//----------------------------------------------------------( Implement )----------------------------------------------------------------------
	SListNode<Object> *newLink = new SListNode < Object > ; //creating new node, at at level 0
	newLink->item = obj; //assigning value


	newLink->next = right;//new link points forward to right
	newLink->prev = right->prev;//new link points back to previous
	right->prev->next = newLink;//previous points forward to new link
	right->prev = newLink;//right points back to new link

	int ranNum = rand() % 2;//Random Number returns 0 or 1
	SListNode<Object> *curLink = newLink;//current link for loop

	for (int i = 0; i <= 5; i++){//each iteration corresponds to each level of the list

		if (ranNum != 1) //don't add upper level
			break;
		else { //create new upper level link

			SListNode<Object> *upLink = new SListNode < Object > ; //creates new Node to be inserted in upper level 
			upLink->item = newLink->item;//assigning value

			curLink->up = upLink;//new lower link points to new upper link
			upLink->down = curLink;//upper Link points down to new lower link


			while (curLink->prev != NULL){//while loop, for finding an upper level node to the left of the new node

				if (curLink->prev->up != NULL){//if a node exists on upper level somewhere behind the newly inserted link, connect them

					if (curLink->prev->up->next == NULL) {
						upLink->prev = curLink->prev->up;//point new upper link back towards previous upper link
						curLink->prev->up->next = upLink;//point previous upper link towards new upper link
						break;
					}
					else {
						upLink->next = curLink->prev->up->next;//point new upper link towards whatever the previous upper link pointed to
						curLink->prev->up->next->prev = upLink;//point what previous upper link pointed to now points back to new upper link
						upLink->prev = curLink->prev->up;//point new upper link back towards previous upper link
						curLink->prev->up->next = upLink;//point previous upper link towards new upper link
						break;
				   }
				}
				curLink = curLink->prev;//if no upper level link found, change current link to next left link
			}
			curLink = upLink;//the pointer changes and goes up a level
		}
		//create new number
		ranNum = rand() % 2;//new random number returning 0 or 1
	}

	return;
}

template<class Object>
bool SList<Object>::find(const Object &obj) {
	// p points to the level-0 item close to a given object
	SListNode<Object> *p = searchPointer(obj);

	return (p->next != NULL && p->item == obj);     // true if obj was found
}

template<class Object>
SListNode<Object> *SList<Object>::searchPointer(const Object &obj) {
	SListNode<Object> *p = header[LEVEL - 1];     // start from the top left
	while (p->down != NULL) {                   // toward level 0
		p = p->down;                                // shift down once
		cost++;

		if (p->prev == NULL) {                    // at the left most item
			if (p->next->next == NULL)              // no intermediate items
				continue;
			else {                                    // some intermadiate items
				if (p->next->item <= obj)             // if 1st item <= obj
					p = p->next;                          // shift right to item 1
				cost++;
			}
		}

		while (p->next->next != NULL && p->next->item <= obj) {
			// shift right through intermediate items as far as the item value <= obj
			p = p->next;
			cost++;
		}
	}

	// now reached the bottom. shift right once if the current item < obj
	if (p->prev == NULL || p->item < obj) {
		p = p->next;
		cost++;
	}
	return p; // return the pointer to an item >= a given object.
}

template<class Object>
void SList<Object>::remove(const Object &obj) {
	// p points to the level-0 item to delete
	SListNode<Object> *p = searchPointer(obj);


	// validate if p is not the left most or right most and exactly contains the item to delete
	if (p->prev == NULL || p->next == NULL || p->item != obj)
		return;

	//----------------------------------------------------------( Implement )----------------------------------------------------------------------
	p->prev->next = p->next;//pointer for node before delete node to now point to the node after delete  
	p->next->prev = p->prev;//pointer for node after delete node to now point to the node before delete

	

	while (p->up != NULL){//while current has an upper node to delete

		SListNode<Object> *cur = p;
	
		if (p->up->next != NULL && p->up->prev != NULL){//upper node points to both left and right node(s)
			
			p->up->next->prev = p->up->prev;//upper delete's node next now points back to the node before it
			p->up->prev->next = p->up->next;//upper delete's node previous now points forward to the node in front of it
		}
		
		p = p->up;//current node equals the node above it
		delete cur;//delete lower node

	}
	
}

template<class Object>
const SList<Object> &SList<Object>::operator=(const SList &rhs) {
	if (this != &rhs) { // avoid self-assignment
		clear();           // deallocate old items

		int index;
		SListNode<Object> *rnode;
		for (index = 0, rnode = rhs.header[0]->next; rnode->next != NULL;
			rnode = rnode->next, ++index)
			insert(rnode->item);

		cost = rhs.cost;
	}
	return *this;
}

template<class Object>
int SList<Object>::getCost() const {
	return cost;
}

template<class Object>
void SList<Object>::show() const {
	cout << "contents:" << endl;
	for (SListNode<Object> *col = header[0]; col != NULL; col = col->next) {
		SListNode<Object> *row = col;
		for (int level = 0; row != NULL && level < LEVEL; level++) {
			if (row->prev == NULL)
				cout << "-inf\t";
			else if (row->next == NULL)
				cout << "+inf\t";
			else
				cout << row->item << "\t";
			row = row->up;
		}
		cout << endl;
	}
}