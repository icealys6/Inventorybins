

#include "resource.h"
#include <string>
#include <wchar.h>
class bin
{
public:
	int Mbinnum;
	wchar_t Mpartdesc[50];
	int Mnumofparts;
	
	/*void addParts(bin*binptr, int binnumb, int addparts)
	{
		binnumb--;
		binptr[binnumb].Mnumofparts += addparts;
	}
	void removeParts(bin*binptr, int binnumb, int remparts)
	{
		binnumb--;
		binptr[binnumb].Mnumofparts -= remparts;
		Mnumofparts -= remparts;
		// ^these two lines above are IDENTICAL.
		// Sorry i closed the steam window, but anyways
		// the line on line 21 is doing THE EXACT SAME THING as line 31 here.
	}*/
	void addParts(int addparts)
	{
		Mnumofparts += addparts;
	}
	void removeParts(int remparts)
	{
		Mnumofparts -= remparts;
	}
};
