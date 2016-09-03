// OPTIMAL CLASSROM ASSIGNMENT (classroom.cpp)
//  using maximum flow networks
//  Matthew Bennett, 4-4-06
//  Changed 04-04-2006 abandoned nicer runtime for simpler structure.

// Takes an argument filename from the command line for a file to read in.
// Outputs to filename.out the classroom matching, room then class

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <algorithm>

#include <vector>
#include <deque>
#include <map>

using namespace std;

struct link {int c,f; string d; }; //weight, destination

bool operator<(const link &a, const link &b) {return a.f < b.f;}
bool operator>(const link &a, const link &b) {return a.f > b.f;}

typedef map < string, vector<link> > graph;
typedef vector <string> path;

void load(string filename, graph & g, int &min);

path BFS (graph & in, string s, string t, bool (* comparator)(const link & a, const link & b) );
//path DFS (graph & in, string s, string t, bool (* comparator)(const link & a, const link & b) );

const int MAX = 1066; //NORMANS INVADE ENGLAND!!

int main(int argc, char* argv[])
{
 string filename( argc > 1 ? argv[1] : "room_pref_big");

 graph g; //the flow network
 int min;
 load(filename, g, min);
 graph gg = g;
 cout << filename << " loaded...\n";
 //beginning of the maximum flow (Edmunds-Karp) algorithm

 path p = BFS (g, "s", "t", operator>);
 //for (int i = 0; i < p.size(); i++) cout << p[i] << endl;

 while (!p.empty())
 {
  for (int i = 1; i < p.size(); i++) //for all in the path
  {
   for (int k = 0; k < g[p[i-1]].size(); k++)
   {
    if (g[p[i-1]][k].d == p[i])
		g[p[i-1]][k].f--;
   }
   for (int k = 0; k < g[p[i]].size(); k++)
   {
    if (g[p[i]][k].d == p[i-1])
		g[p[i]][k].f++; //residual flow increase
   }  
  }
  p  = BFS (g, "s", "t", operator>);
 }

 //done with edmunds-karp

 ofstream outfile;
 outfile.open((filename + ".out").c_str());

 const int MIN = min;
 int * stats = new int[MAX-MIN+1];
 for (int i = 0; i < MAX-MIN+1; i++) stats[i] = 0;

 graph::iterator k;
 for (k = g.begin(); k != g.end(); ++k)
 {
  for (int i = 0; i < k->second.size(); i++)
  if ( k->second[i].c == 0 && k->second[i].f > 0
	&& k->first != "t" && k->second[i].d != "s")
  {
   outfile << k->first << "\t" << k->second[i].d <<"\t";
   while(1)
   {
    if (gg[k->second[i].d].empty())
	{
	 outfile << "NONE";
	 stats[0]=stats[0]+1;
	}
    else if (gg[k->second[i].d].back().d == k->first)
	{
	 int q = (MAX-MIN) - (gg[k->second[i].d].back().f - MIN) + 1;
	 outfile << " (" << q << ")" << endl;
	 stats[q] = stats[q] + 1;
	 break;
	}
    else gg[k->second[i].d].pop_back();
   }
  }
 }
 if (outfile)
 {
  outfile.close();
  cout << filename << ".out written\n";
 }

 //Do I need to show how many got 1st, how many got 2nd, etc?
	// ::sigh:: ok
 for (int i = 0; i <= MAX-MIN; i++)
 {	
  cout << i 
  << (i==1||i==21||i==31||i==41||i==51||i==61||i==71?"st:\t":(i==2?"nd:\t":(i==3?"rd:\t":"th:\t"))) 
  << stats[i] << endl;
 }

 return 0;
}

void load(string filename, graph & g, int & min)
{
 ifstream infile;
 infile.open(filename.c_str());
 if ( !infile ) { cout << "Error: Can't open the file\n"; exit(1); } 

 string buffer, cs; //buffer, class/section
 string t1, t2;		//temporary
 
 while ( getline(infile, buffer) )
 {
  link newlie;	
  istringstream line(buffer); //for parsing
    
  line >> t1 >> t2;
  cs = t1 + t2;  //class id is class name plus section
  
  //hook up super-source
  newlie.c = newlie.f = 1;
  newlie.d = cs;
  g["s"].push_back(newlie);
  //with empty residual link
  newlie.c = newlie.f = 0;
  newlie.d = "s";
  g[cs].push_back(newlie);

  int priority = MAX;
  while (line >> t1) //room numbers!
  {
   //hook up a class-classroom link
   newlie.c = newlie.f = priority--;
   newlie.d = t1;
   g[cs].push_back(newlie);
   //with empty residual link
   newlie.c = newlie.f = 0;
   newlie.d = cs;
   g[t1].push_back(newlie);

   //hook up super-sink
   newlie.c = newlie.f = 1;
   newlie.d = "t";
   g[t1].push_back(newlie);
   //with empty residual link
   newlie.c = newlie.f = 0;
   newlie.d = t1;
   g["t"].push_back(newlie);
  }
  if (priority < min) min = priority;
 }
 
 //graph::iterator k;
 //for (k = g.begin(); k != g.end(); ++k)
 //{
 //  cout << k->first << "\t";
 //  sort(k->second.begin(), k->second.end(), operator>);
 //  for (int i = 0; i < k->second.size(); i++)
 //   cout << k->second[i].d << "(" << k->second[i].c << "/" << k->second[i].f <<  ")" << "\t";
 //  cout << endl;
 //}
}


path BFS (graph & in, string s, string t, bool (* comparator)(const link & a, const link & b) )
{
 path p;
 deque <string> q;
 
 map <string, bool>   marked;
 map <string, string> parent;

 p.push_back(s);
 q.push_back(s);
 marked[s] = true;

 while (! q.empty() )
 {
  string v = q.front();
  q.pop_front();

  if (v == t)
  {
   q.clear();
   while (v != s)
   {
    q.push_front(v);
    v = parent[v];
   }
   while (!q.empty())
   {
	p.push_back( q.front() );
	q.pop_front();
   }

   return p;
  }

  vector <link> nb = in[v];
  sort(nb.begin(), nb.end(), comparator);

  for (int i = 0; i < nb.size(); i++)
  if (marked[nb[i].d] == false && nb[i].f > 0)
  {
   marked[nb[i].d] = true;
   q.push_back(nb[i].d);
   parent[nb[i].d] = v;
  }
 }
 
 p.clear();
 return p;
}
