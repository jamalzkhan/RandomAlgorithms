#include "RBST.hpp"

/* PROVIDED FUNCTIONS */

int RBST::add(const Key& key, bool verbose) {
	unsigned int oldSize = m_size;
	m_head = randomAdd(m_head, key);
	if (m_size > oldSize) {
		if (verbose) {
			cout<<"Node "<<key<< " is added into the tree."<<endl;
		}
		return 1;
	} else {
		if (verbose) {
			cout<<"Node "<<key<< " is already in the tree."<<endl;
		}
		return 0;
	}
	if (verbose) cout<<endl;
};

int RBST::del(const Key& key, bool verbose) {
	unsigned oldSize= m_size;
	m_head = del(m_head, key);
	if (m_size < oldSize) {
		if (verbose) {
			cout<<"Node "<<key<< " is deleted from the tree."<<endl;
		}
		return 1;
	} else {
		if (verbose) {
			cout<< "Node "<<key<< " is not in the tree."<<endl;
		}
		return 0;
	}
};

int RBST::find(const Key& key, bool verbose) {
	RBSTNode* ret = find(m_head, key);
	if (ret == NULL) {
		if (verbose) {
			cout<< "Node "<<key<< " is not in the tree."<<endl;
		}
		return 0;
	} else {
		if (verbose) {
			cout<<"Node "<<key<< " is in the tree."<<endl;
		}
		return 1;
	}
};

int RBST::dump(char sep) {
	int ret = dump(m_head, sep);
	cout<<"SIZE: " <<ret<<endl;
	return ret;
};

int RBST::dump(RBSTNode* target, char sep) {
	if (target == NULL) {
		return 0;
	}
	int ret = dump(target->left(), sep);
	cout<< *target<<sep;
	ret ++;
	ret += dump(target->right(), sep);
	return ret;
};


/* ADD FUNCTIONS */

RBSTNode*  RBST::rightRotate(RBSTNode* target) {

	RBSTNode * x = target -> left();
	target -> setLeft(x -> right());
	x -> setRight(target);
	return x;

};

RBSTNode*  RBST::leftRotate(RBSTNode* target) {

	RBSTNode * x =  target -> right();
	target -> setRight(x -> left());
	x -> setLeft(target);
	return x;
};

/* Root insertion function */
RBSTNode* RBST::addRoot(RBSTNode* target, const Key& key) {
	countAdd++;

	if (target == NULL)
		return new RBSTNode(key);

	if (key < *target){
		target -> setLeft(addRoot(target -> left(), key));
		return rightRotate(target);
	}
	else {
		target -> setRight(addRoot(target -> right(), key));
		return leftRotate(target);
	}
};

RBSTNode* RBST::randomAdd(RBSTNode* target, const Key& key) {
	countAdd++;

	/* The NULL case */
	if (target == NULL) {
		m_size++;
		RBSTNode * t = new RBSTNode(key);
		t -> setSize(t -> getSize()+1);
		return t;
	}

	/* Getting a random number */
	int r = rand() % (target -> getSize() + 1);

	/* The case for inserting in the root */
	if (r == 1){
		m_size++;
		target -> setSize(target -> getSize()+1);
		return addRoot(target, key);
	}

	if (key < *target){
		target -> setLeft(randomAdd(target -> left(), key));
	}
	else if (key > *target) {
		target -> setRight(randomAdd(target -> right(), key));
	}
	else{
		return target;
	}

	m_size++;
	target -> setSize(target -> getSize() + 1);
	return target;

};


/* Delete Function */
RBSTNode* RBST::del(RBSTNode* target, const Key& key) {
	countDelete++;

	if (target == NULL){
		return NULL;
	}

	if (key < *target){
			RBSTNode * newLeft = del(target -> left(), key);
			target -> setLeft(newLeft);
	}
	else if (key > *target){
			RBSTNode * newRight = del(target -> right(), key);
			target -> setRight(newRight);
	}
	/* We have found the node to delete */
	else {
		target = deleteNode(target);
		/* Decreasing the size of the tree */
		m_size--;
	}

	if (target != NULL)
		target -> setSize(target -> getSize() - 1);

	return target;
};


/* Function for deleting leftmost and returning new subtree */
RBSTNode* RBST::deleteLeftMost(RBSTNode * target){
	if (target -> left() == NULL){
		return target -> right();
	}
	else {
		RBSTNode * newChild = deleteLeftMost(target -> left());
		target -> setLeft(newChild);
		return target;
	}
}

/* Function for finding the left most node */
RBSTNode* RBST::leftMost(RBSTNode * target){
	if (target -> left() ==NULL){
		return target;
	}
	return leftMost(target -> left());
}


/* Deletes a given node returning a new subtree */
RBSTNode* RBST::deleteNode(RBSTNode * node){

	//When we have 2 subtrees attached to node
	if (node -> right() != NULL && node -> left() != NULL){
		RBSTNode * replace = leftMost(node -> right());
		RBSTNode * newRight = deleteLeftMost(node -> right());
		replace ->  setRight(newRight);
		replace -> setLeft(node -> left());
		return replace;
	}
	// One or no subtrees
	else {
		if (node -> right() != NULL && node -> left() == NULL){
			return node -> right();
		}
		else if(node -> left() != NULL && node -> right() == NULL){
			return node -> left();
		}
		else {
			return NULL;
		}
	}
}


/* Find Function */
RBSTNode* RBST::find(RBSTNode* target, const Key& key) {
	countFind++;

	if (target != NULL){
		if (key == *target)
			return target;
		else if (key < *target)
			return find(target -> left(), key);
		else
			return find(target -> right(), key);
	}
	else {
		return NULL;
	}
}

