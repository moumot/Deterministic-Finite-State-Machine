// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> 


using namespace std;

int verify(vector<string>, string, int);
int startGen(char*);
vector< vector<string> > dfaGen(char*);
vector<string> statesGen(char*);
vector<int> finalGen(char*);
vector<string> alphaGen(char*);
string check(vector< vector<string> >, vector< vector<string> >, vector<int>, vector<int>,int,int, vector<string>, vector<string>, vector<string>, vector<string>);
vector<int> complement(vector<int>);
vector<int> finalUnion(vector<int>, vector<int>);
vector<string> statesUnion(vector<string>, vector<string>);
int startUnion(int,int,int);
vector< vector<string> >  setUnion(vector< vector<string> >, vector< vector<string> >);
string bfs(vector< vector<string> >, vector< vector<int> >,vector<int>,int,int, vector<string>, string,  vector<string>);

int main(int argc, char* argv[])
{
	vector< vector<string> > dfa1,dfa2;
	vector<string>  states1, states2;
	vector<string> alphabet1, alphabet2;
	string answer;
	vector<int> final1, final2;

	int start1, start2,i;
	char* filename1 = argv[1];
	char* filename2 = argv[2];
	start1 = startGen(filename1);
	start2 = startGen(filename2);
	final1 = finalGen(filename1);
	final2 = finalGen(filename2);
	dfa1 = dfaGen(filename1);
	dfa2 = dfaGen(filename2);
	states1 = statesGen(filename1);
	states2 = statesGen(filename2);
	alphabet1 = alphaGen(filename1);
	alphabet2 = alphaGen(filename2);
	

	answer = check(dfa1, dfa2, final1, final2, start1, start2, states1, states2,alphabet1,alphabet2);

	return 0;
}


int verify(vector<string> set, string input, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (set[i] == input)
			return i;
	}

	return (size-1);
}

vector< vector<string> > dfaGen(char* filename) {

	int statesnum;
	int alphabetnum;
	int start;
	int i;
	string stateTemp, alphaTemp;
	vector< vector<string> > dfa;

	vector< string > states;
	vector< string > alphabet;


	string line;
	ifstream myfile;

	myfile.open(filename);

	while (getline(myfile, line))
	{
		i = 0;
		while (line[i] != '\0')
		{
			if (line[i] == ' ')
				line.erase(i, 1);
			i++;
		}

		if (line.substr(0, line.find(':')) == "states")
		{
			statesnum = count(line.begin(), line.end(), ';') + 1;
			states.resize(statesnum);
			states[0] = line.substr(line.find('q'), line.find(';') - line.find('q'));
			for (i = 1; i < statesnum; i++)
			{
				line.erase(0, line.find(states[i - 1]) + states[i - 1].size() + 1);
				states[i] = line.substr(0, line.find(';'));

			}


		}

		else if (line.substr(0, line.find(':')) == "input_alphabet")
		{
			alphabetnum = count(line.begin(), line.end(), ';') + 1;
			alphabet.resize(alphabetnum);
			line.erase(0, line.find(':') + 1);
			alphabet[0] = line.substr(0, 1);

			for (i = 1; i < alphabetnum; i++)
			{
				line.erase(0, line.find(';') + 1);
				alphabet[i] = line.substr(0, 1);

			}

		}


		else if (line.substr(0, line.find(':')) == "delta") {
			dfa.resize(alphabetnum, vector<string>(statesnum, " "));

			line.erase(0, line.find('q'));

			int counter = count(line.begin(), line.end(), ';') + 1;
			int dfax,dfay;
			while (counter != 0)
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

	return dfa;
}

vector<int> finalGen(char* filename)
{
	string *states;
	int statesnum, i;
	vector< int > final;


	string line;
	ifstream myfile;

	states = new string[2]();
	myfile.open(filename);

	while (getline(myfile, line))
	{
		i = 0;
		while (line[i] != '\0')
		{
			if (line[i] == ' ')
				line.erase(i, 1);
			i++;
		}

		if (line.substr(0, line.find(':')) == "states")
		{
			statesnum = count(line.begin(), line.end(), ';') + 1;
			states = new string[statesnum];
			states[0] = line.substr(line.find('q'), line.find(';') - line.find('q'));
			for (i = 1; i < statesnum; i++)
			{
				line.erase(0, line.find(states[i - 1]) + states[i - 1].size() + 1);
				states[i] = line.substr(0, line.find(';'));

			}

		}


		else if (line.substr(0, line.find(':')) == "accept_states")
		{
			final.resize(statesnum);
			for (i = 0; i < statesnum; i++)
			{
				final[i] = 0;
 			}

			line.erase(0, line.find('q'));
			line.erase(line.find('#'),line.find('/0') - line.find('#'));
				

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
	}
		myfile.close();

	return final;
}

int startGen(char* filename) {
	int start;
	int statesnum, i;
	string *states;


	string line;
	ifstream myfile;
	states = new string[2]();
	myfile.open(filename);

	while (getline(myfile, line))
	{
		i = 0;
		while (line[i] != '\0')
		{
			if (line[i] == ' ')
				line.erase(i, 1);
			i++;
		}

		if (line.substr(0, line.find(':')) == "states")
		{
			statesnum = count(line.begin(), line.end(), ';') + 1;
			states = new string[statesnum];
			states[0] = line.substr(line.find('q'), line.find(';') - line.find('q'));
			for (i = 1; i < statesnum; i++)
			{
				line.erase(0, line.find(states[i - 1]) + states[i - 1].size() + 1);
				states[i] = line.substr(0, line.find(';'));

			}


		}
		if (line.substr(0, line.find(':')) == "start_state") {
			line.erase(0, line.find('q'));
			line.erase(line.find('#'), line.find('/0') - line.find('#'));

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
	}
		myfile.close();
	return start;
}
vector<string> statesGen(char* filename) {
	int statesnum, i;
	vector<string> states;


	string line;
	ifstream myfile;
	myfile.open(filename);

	while (getline(myfile, line))
	{
		i = 0;
		while (line[i] != '\0')
		{
			if (line[i] == ' ')
				line.erase(i, 1);
			i++;
		}

		if (line.substr(0, line.find(':')) == "states")
		{
			statesnum = count(line.begin(), line.end(), ';') + 1;
			states.resize(statesnum);
			states[0] = line.substr(line.find('q'), line.find(';') - line.find('q'));
			for (i = 1; i < statesnum; i++)
			{
				line.erase(0, line.find(states[i - 1]) + states[i - 1].size() + 1);
				states[i] = line.substr(0, line.find(';'));

			}


		}
	}
		myfile.close();
	return states;
}
vector<string> alphaGen(char* filename) {
	int alphabetnum, i;
	vector<string> alphabet;


	string line;
	ifstream myfile;
	myfile.open(filename);

	while (getline(myfile, line))
	{
		i = 0;
		while (line[i] != '\0')
		{
			if (line[i] == ' ')
				line.erase(i, 1);
			i++;
		}

	    if (line.substr(0, line.find(':')) == "input_alphabet")
		{
			alphabetnum = count(line.begin(), line.end(), ';') + 1;
			alphabet.resize(alphabetnum);
			line.erase(0, line.find(':') + 1);
			alphabet[0] = line.substr(0, 1);

			for (i = 1; i < alphabetnum; i++)
			{
				line.erase(0, line.find(';') + 1);
				alphabet[i] = line.substr(0, 1);

			}

		}
	}
	myfile.close();
	return alphabet;
}


string check(vector< vector<string> > dfa1, vector< vector<string> > dfa2, vector<int> final1, vector<int> final2, int start1, int start2, vector<string> states1, vector<string> states2, vector<string> alphabet1, vector<string> alphabet2)
{
	vector< vector<string> > ndfa;
	vector<string> states;
	vector<int> final;
	string output;
	vector< vector<int> > visited;

	int start;

	states.resize((states1.size()*states2.size()));
	final.resize((final1.size()*final2.size()));
	ndfa.resize(dfa1.size(), vector<string>((dfa1[0].size()*dfa2[0].size()), " "));


	final = finalUnion(complement(finalUnion(complement(final1), final2)), complement(finalUnion(final1,complement(final2))));
	ndfa = setUnion(setUnion(dfa1,dfa2),setUnion(dfa1,dfa2));
	states = statesUnion(statesUnion(states1, states2), statesUnion(states1, states2));
	start = startUnion(start1, start2,dfa2.size());

	visited.resize(ndfa.size(), vector<int>(ndfa[0].size(),0));
	output = "";
	output = bfs(ndfa,visited,final,0,start,states,output,alphabet1);


	cout << output << "\n";
	return output;

}
string bfs(vector< vector<string> > dfa, vector< vector<int> > visited, vector<int> final, int x, int y, vector<string> states, string counter, vector<string> alphabet)
{
	bool end = true;
	int i,j;
	if (final[y] == 1)
	{

		counter = ("No  " + counter);
		return counter;
	}



	for (int i = 0; i < visited.size(); i++)
	{
		cout << visited[i][y] << "\n";
		if (visited[i][y] == 0)
			end = false;

	}

	if (end)
		return "Yes";

	counter = counter + alphabet[x];

	if (visited[x][y] == 1 && x < (visited.size() - 1))
	{
		counter  = bfs(dfa, visited, final, (x + 1), verify(states, dfa[x][y], states.size()), states, counter, alphabet);
	}
	else
	{
		visited[x][y] = 1;
		counter = bfs(dfa, visited, final, x, verify(states, dfa[x][y], states.size()), states, counter, alphabet);
	}

	return counter;

}

vector<int> complement(vector<int> final) 
{
	for (int i = 0; i < final.size(); i++)
	{
		if (final[i] == 1)
			final[i] = 0;
		else
			final[i] = 1;
	}

	return final;
}

vector< vector<string> >  setUnion(vector< vector <string> > dfa1, vector< vector <string> > dfa2)
{

	vector< vector<string> > ndfa;
	int i = 0, col,j,k;
	ndfa.resize(dfa1.size(), vector<string>((dfa1[0].size()*dfa2[0].size()), " "));
	while (i < ndfa.size())
	{
		col = 0;
		for (j = 0; j < dfa1[0].size(); j++)
		{
			for (k = 0; k < dfa2[0].size(); k++)
			{
				ndfa[i][col] = dfa1[i][j] + dfa2[i][k];
				col++;
			}
		}
		i++;
	}
	return ndfa;
}

vector<int> finalUnion(vector<int> final1, vector<int> final2) {
	int i = 0,j,k;
	vector<int> final;
	final.resize((final1.size()*final2.size()));
	for (j = 0; j < final1.size(); j++)
	{
		for (k = 0; k < final2.size(); k++)
		{
			if (final1[j] == 1 || final2[k] == 1)
				final[i] = 1;
			else
				final[i] = 0;
			i++;
		}
	}
	return final;
}
vector<string> statesUnion(vector<string> states1, vector<string> states2) {

	int i = 0,j,k;
	vector<string> states;
	states.resize((states1.size()*states2.size()));
	for (j = 0; j < states1.size(); j++)
	{
		for (k = 0; k < states2.size(); k++)
		{
			states[i] = states1[j] + states2[k];
			i++;
		}
	}
	return states;
}
int startUnion(int start1, int start2,int size) {
	if (start1 > 0)
		start1 = start1 + size;

		return start1 + start2;
}