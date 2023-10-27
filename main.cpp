
#include "main.h"
#include "Restaurant.cpp"
#include <fstream>


void simulate(string filename, imp_res* r)
{
	ifstream ss(filename);
	string str, maxsize, name, energy, num;
	int i=1;
	while(ss >> str)
	{   cout<<"*******************BUOC THU"<<i<<'\n';i++;
		if(str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize); 
    	}
        else if(str == "RED") // RED <NAME> <ENERGY>
        {
            ss >> name;
            ss >> energy;
            r->RED(name, stoi(energy));
    	}
    	else if(str == "BLUE") // BLUE <NUM>
    	{
                ss >> num;
    			r->BLUE(stoi(num));
		}
    	else if(str == "PURPLE") // PURPLE
    	{
    			r->PURPLE();
		}
		else if(str == "REVERSAL") // REVERSAL
		{
    			r->REVERSAL();	
		}
    	else if(str == "UNLIMITED_VOID") // UNLIMITED_VOID
     	{   	
    			r->UNLIMITED_VOID();
    	}
    	else if(str == "DOMAIN_EXPANSION") // DOMAIN_EXPANSION
    	{
    			r->DOMAIN_EXPANSION();
    	}
    	else if(str==  "LIGHT" )
    	{
                ss >> num;
    			r->LIGHT(stoi(num));
    	}
    }
}

int main(int argc, char* argv[])
{
    
    imp_res* r = new imp_res();
    string fileName = "test.txt";
    simulate(fileName, r);
    delete r;
	// printMemoryUsage();
	return 0;
}