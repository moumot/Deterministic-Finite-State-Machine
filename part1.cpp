
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> 


using namespace std;

int verify(string[], string, int);
int verifyinput(string[], string, int);
int main(int argc, char* argv[])
{

	int statesnum;
	int alphabetnum;
	int start;
	int i;
	string stateTemp, alphaTemp;
	char* filename = argv[1];
	string tinput = argv[2];


	vector< vector<string> > dfa;
	string *states, *alphabet;
	int *final;

	string line;
	ifstream myfile;

	states = new string[2]();
	alphabet = new string[2]();
	final = new int[2]();
	myfile.open(filename);
		while (getline(myfile, line))
		{
			i = 0;
			while (line[i] != '\0')
			{
				if (line[i] == ' ')
					line.erase(i,1);
				i++;
			}

			if (line.substr(0, line.find(':')) == "states")
			{
				statesnum = count(line.begin(), line.end(), ';') + 1;
				states = new string[statesnum];
				states[0] = line.substr(line.find('q'), line.find(';') - line.find('q'));
				for (i = 1; i < statesnum; i++)
				{
					line.erase(0, line.find(states[i-1]) + states[i-1].size() + 1);
					states[i] = line.substr(0, line.find(';'));
					
				}


			}

			else if (line.substr(0, line.find(':')) == "input_alphabet")
			{
				alphabetnum = count(line.begin(), line.end(), ';') + 1;
				alphabet = new string[alphabetnum];
				line.erase(0, line.find(':') +1);
				alphabet[0] = line.substr(0, 1);

				for (i = 1; i < alphabetnum;i++)
				{
					line.erase(0, line.find(';') + 1);
					alphabet[i] = line.substr(0, 1);

				}

			}

			else if (line.substr(0, line.find(':')) == "accept_states")
			{
				final = new int[statesnum];
	
				for (i = 0; i < statesnum; i++)
				{
					final[i] = 0;
 				}

				line.erase(0, line.find('q'));
				line.erase(line.find('#'),line.find('/0') - line.find('#'));
				i = 0;

				while (line != "")
				{
					for (i = 0; i < statesnum; i++)
					{
						if (line.compare(line.find('q'), line.find(';') - line.find('q'), states[i], 0, states[i].size()) == 0)
						{
							final[i] = 1;
							line.erase(0, line.find(states[i]) + states[i].size()+1);
						}
						if (line == "")
							break;

					}
				}

				
			}

			else if (line.substr(0, line.find(':')) == "start_state") {
				line.erase(0, line.find('q'));
				line.erase(line.find('#') , line.find('/0') - line.find('#'));
		
				for (i = 0; i < statesnum; i++)
				{

					if (line.compare(line.find('q'), line.find(';') - line.find('q'), states[i], 0, states[i].size()) == 0)
					{
					
						start = i;
						line.erase(0, line.find(states[i]) + states[i].size() + 2);
						break;
					}

				}

			}

			else if (line.substr(0, line.find(':')) == "delta") {
				dfa.resize(alphabetnum, vector<string>(statesnum, " "));

				line.erase(0, line.find('q'));

				int counter = count(line.begin(), line.end(), ';') + 1;
				int dfax,dfay;
									
				while(counter != 0)
				{

					stateTemp = line.substr(line.find('q'), line.find(',') - line.find('q'));
					alphaTemp = line.substr(line.find(',')+1, 1);
					dfax = verify(alphabet, alphaTemp, alphabetnum);
					dfay = verify(states, stateTemp, statesnum);
					dfa[dfax][dfay] = line.substr(line.find('>') + 1, line.find(';') - line.find('>') - 1);
					line.erase(0, line.find(';')+1);
					counter--;
				}
					

			}
		}

	myfile.close();

	i = 1;
	int x,y;
	if (tinput == "")
	{
		if (final[start] ==1)
		{
			cout << "Yes" << "\n";
		}
		else
			cout << "No" << "\n";
		return 0;
	}

	y = verifyinput(alphabet, tinput.substr(0, 1),alphabetnum);
	if (y < 0)
	{
		cout << "No" << "\n";
		return 0;
	}
	x = verify(states,dfa[y][start],statesnum);

	while (tinput[i] != '\0')
	{
		y = verifyinput(alphabet, tinput.substr(i, 1), alphabetnum);
		if (y < 0)
		{
			cout << "No"<< "\n";
			return 0;
		}
		x = verify(states, dfa[y][x], statesnum);
		i++;
	}


	if (final[x] == 1)
		cout << "Yes" << "\n";
	else
		cout << "No"<< "\n";

	
    return 0;
}


int verify(string set[], string input, int size)
{
	
	for (int i = 0; i < size; i++)
	{
		if (set[i] == input)
		{
			return i;
		}
	}
	return (size-1);
}
int verifyinput(string set[], string input, int size)
{
	
	for (int i = 0; i < size; i++)
	{
		if (set[i] == input)
		{
			return i;
		}
	}
	return -1;
}
