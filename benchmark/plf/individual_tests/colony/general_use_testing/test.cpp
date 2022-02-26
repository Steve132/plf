#include "plf_colony.h"

int main()
{
	plf::colony<int> temp;
	plf::colony<int>::iterator it = temp.insert(15);
	temp.erase(it);
		
	return temp.size();
}