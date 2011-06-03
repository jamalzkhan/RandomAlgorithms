#include <iostream>
#include "RBST.hpp"
int main (){


	RBST * s = new RBST();


	/*
	BloomFilter * b = new BloomFilter(1000);
	b -> add("Jamal");
	b -> add("Qaiser");
	b -> dump();
	b -> testExist("Zaran", true);
	b -> testExist("Jamal", true);
	b -> del("Jamal");
	b -> del("Zaran");
	b -> testExist("Jamal", true);
	 */
	/*
	SkipList * s = new SkipList(10);
	s -> add("Jamal", true);
	 */
	s -> find("Jamal", true);
	s -> add("Alex", true);
	s -> add("Javed", true);
	s -> add("Aleph", true);
	s -> del("Alexr", true);
	s -> add("Rabbit", true);
	s -> add("Lala", true);
	s -> add("Goat", true);
	s -> add("Qa", true);
	s -> add("High", true);
	s -> add("Lama", true);
	s -> dump();
	s -> del("Qa", true);
	s -> del("Javed", true);
	s -> dump();
	/*, true

	s -> find("Goat", true);
	s -> del("Goat", true);
	s -> find("Goat", true);

	s -> dump('\n');
	s -> del("Aleph", true);
	s -> dump();
	*/


}
