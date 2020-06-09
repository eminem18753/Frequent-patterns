#pragma GCC optimize("O3")  
#pragma G++ optimize("O3")  
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream> 
#include <cstdio>
#include <cstring>
#include <vector>
#include <deque>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <set>
#include <list>
#include <cmath>
using namespace std;
struct Trie
{
	bool isTheEnd;
	int key;
	int number;
	int real_number;
	vector<Trie*> prefix;
	Trie* parent;
	map<int,Trie*> maps;
};
struct storage
{
	vector<pair<int,int> > patterns;
	vector<int> the_sets;
};
map<int,int> header_table;
map<int,set<Trie*> > node_links;
map<set<int>,double> total_results;
map<set<int>,double> total_results_replace;

map<set<int>,storage*> conditional_trees;

map<set<int>,set<Trie*> > node_k_links;
map<set<int>,set<Trie*> > node_k_links_replace;
map<set<int>,storage*> conditional_k_trees;
map<set<int>,int> real_counter;

map<set<int>,map<Trie*,int> > final_previous_solution;
map<set<int>,map<Trie*,int> > final_solution;
double get_frequency(int key,int counters) __attribute__((optimize("-O3")));
double get_frequency(int key,int counters)
{
	double result=0;
	
    for (set<Trie*>::iterator its = node_links[key].begin(); its != node_links[key].end(); ++its)
	{
		result+=(*its)->number;
	}
	result=(double)result/(double)counters;
	return result;
} 
int get_k_counts(set<int> key) __attribute__((optimize("-O3")));
int get_k_counts(set<int> key)
{
	int result=0;
	
    for (set<Trie*>::iterator its = node_k_links[key].begin(); its != node_k_links[key].end(); ++its)
	{
		result+=(*its)->number;
	}
	return result;
} 

Trie* getNewTrieNode() __attribute__((optimize("-O3")));
Trie* getNewTrieNode()
{
	Trie* node=new Trie;
	node->number=0;
	node->key=-1;
	node->isTheEnd=false;
	return node;	
}
void insert(Trie*& root,vector<int> data) __attribute__((optimize("-O3")));
void insert(Trie*& root,vector<int> data)
{
	if(root==NULL)
	{
		root=getNewTrieNode(); 
		root->parent=NULL;
	}
	Trie* temp=root;
	for(int i=0;i<data.size();i++)
	{
		int x=data[i];
		if(temp->maps.find(x)==temp->maps.end())//not found
		{
			temp->maps[x]=getNewTrieNode();
			temp->maps[x]->parent=temp;
			temp->maps[x]->number+=1;
			temp->maps[x]->key=x;
			if(node_links.find(x)==node_links.end())
			{
				node_links[x].insert(temp->maps[x]);
			}
			else if(node_links[x].find(temp->maps[x])==node_links[x].end())
			{
				node_links[x].insert(temp->maps[x]);
			}
		}
		else
		{
			temp->maps[x]->number+=1;	
			temp->maps[x]->key=x;
		}
		temp=temp->maps[x];
	}
	temp->isTheEnd=true;
}

void subsetsUtil(vector<int>& A, vector<vector<int> >& res,vector<int>& subset, int index) __attribute__((optimize("-O3")));
void subsetsUtil(vector<int>& A, vector<vector<int> >& res,vector<int>& subset, int index)
{ 
    for(int i=index;i<A.size();i++) 
	{ 
        subset.push_back(A[i]); 
        res.push_back(subset); 
        subsetsUtil(A,res,subset,i+1); 
        subset.pop_back(); 
    } 
    return; 
} 
  
// below function returns the subsets of vector A. 
vector<vector<int> > subsets(vector<int>& A) __attribute__((optimize("-O3")));
vector<vector<int> > subsets(vector<int>& A)
{ 
    vector<int> subset; 
    vector<vector<int> > res; 
  
    // include the null element in the set. 
    res.push_back(subset); 
  
    // keeps track of current element in vector A; 
    int index=0; 
    subsetsUtil(A,res,subset,index); 
  
    return res; 
} 

int items[1000];
vector<vector<int> > transactions;
bool comp_by_key(pair<int,int> &p1, pair<int,int> &p2) __attribute__((optimize("-O3")));
bool comp_by_key(pair<int,int> &p1, pair<int,int> &p2)
{	
    return p1.first > p2.first;
}

bool comp_by_value(pair<int,int> &p1, pair<int,int> &p2) __attribute__((optimize("-O3")));
bool comp_by_value(pair<int,int> &p1, pair<int,int> &p2)
{	
    return p1.second > p2.second;
}
bool comp_condition(pair<int,int> &p1, pair<int,int> &p2) __attribute__((optimize("-O3")));
bool comp_condition(pair<int,int> &p1, pair<int,int> &p2)
{	
    return p1.second < p2.second;
}
int main(int argc,char* argv[]) __attribute__((optimize("-O3")));
int main(int argc,char* argv[])
{
	clock_t time_start;
	clock_t time_end;
	time_start=clock();
	if(argc!=4)
	{
		cout<<"not enough input arguments"<<endl;
		return -1;
	}
	
	double support=atof(argv[1]);
	
	string filename=argv[2];

	int counter=0;
	freopen(filename.c_str(),"r",stdin);
	string line;
	map<int,int> header_table;

	string outputFileName=argv[3];
	ofstream file_output;
	file_output.open(outputFileName.c_str());
	streamsize c_precision = cout.precision();

	while(!getline(cin,line).eof())
	{
		vector<int> arr;
		istringstream ssline(line);
		string number;
		while(getline(ssline,number,','))
		{
			arr.push_back(atoi(number.c_str()));
			items[atoi(number.c_str())]+=1;
		}
		transactions.push_back(arr);
		counter+=1;
	}

	for(int i=0;i<1000;i++)
	{
		if(items[i]>=support*counter)
		{
			header_table[i]=items[i];
			set<int> current_item_set;
			current_item_set.insert(i);
			file_output<<i;
			file_output<<fixed<<setprecision(4)<<":"<<floor((double)(items[i])/(double)counter*10000+0.5+0.001)/10000<<endl;
			file_output<<scientific<<setprecision(c_precision);			
		}
	}

	vector<pair<int,int> > vector_map (header_table.begin(), header_table.end());
    sort(vector_map.begin(),vector_map.end(),comp_by_value);
	
	vector<vector<int> > ordered_frequent_items;
	for(int i=0;i<transactions.size();i++)
	{
		vector<int> current_item;
		for(int j=0;j<vector_map.size();j++)
		{
			if(binary_search(transactions[i].begin(),transactions[i].end(),vector_map[j].first))
			{
				current_item.push_back(vector_map[j].first);
			}
		}
		ordered_frequent_items.push_back(current_item);
	} 
	
	transactions.clear();
	vector<vector<int> >().swap(transactions);
	//trie node
	Trie* root=NULL;
	for(int i=0;i<ordered_frequent_items.size();i++)
	{
		insert(root,ordered_frequent_items[i]);
	}
	
	Trie* real_root=root;
	
	deque<Trie*> overallNode;
	while(root->maps.size()!=0||overallNode.size()!=0)
	{
		Trie* current;
		if(root->maps.size()!=0)
		{
			current=root->maps.begin()->second;
			for(map<int, Trie*>::const_iterator it = root->maps.begin();it != root->maps.end();it++)
			{
				if(it!=root->maps.begin())
				{
					overallNode.push_back(it->second);
				}
				vector<Trie*> current_prefix;
				Trie* temp=it->second;
				while(temp->parent)
				{
					current_prefix.push_back(temp->parent);
					temp=temp->parent;
				}
				it->second->prefix=current_prefix;

			}
			root=current;
		}
		else
		{
			root=overallNode.front();
			overallNode.pop_front();
		}
	}
	overallNode.clear();
	deque<Trie*>().swap(overallNode);
	//construct fp tree for the pattern database
    //the first tree
	for(int i=vector_map.size()-1;i>-1;i--)
	{
		set<int> indices;
		indices.insert(vector_map[i].first);
		map<int,int> current_map;
		vector<pair<int,int> > current_patterns;
		vector<int> single_pattern;
	    for (set<Trie*>::iterator its = node_links[vector_map[i].first].begin(); its != node_links[vector_map[i].first].end(); ++its)
		{			
			for(int j=0;j<(*its)->prefix.size()-1;j++)
			{
				//new add
				set<int> for_condition;
				for_condition.insert(vector_map[i].first);//original set
				for_condition.insert((*its)->prefix[j]->key);//one prefix

				final_previous_solution[for_condition][(*its)->prefix[j]]+=(*its)->number;
				node_k_links[for_condition].insert((*its)->prefix[j]);//all ma tries*
				current_map[(*its)->prefix[j]->key]+=min((*its)->number,(*its)->prefix[j]->number);
			}
		}
		for(map<int, int>::const_iterator it = current_map.begin(); it != current_map.end(); ++it)
		{
			if(it->second>=support*counter)
			{
				
				pair<int,int> current_item=make_pair(it->first,it->second);
				current_patterns.push_back(current_item);
				single_pattern.push_back(it->first);
			}
		}
		
		conditional_trees[indices]=new storage;
		conditional_trees[indices]->patterns=current_patterns;
		conditional_trees[indices]->the_sets=single_pattern;
		
		indices.clear();
		set<int>().swap(indices);
		current_map.clear();
		map<int,int>().swap(current_map);
		current_patterns.clear();
		vector<pair<int,int> >().swap(current_patterns);
		single_pattern.clear();
		vector<int>().swap(single_pattern);
	}
	
	vector_map.clear();
	vector<pair<int,int> >().swap(vector_map);

	for(map<set<int>,storage*>::const_iterator it = conditional_trees.begin(); it != conditional_trees.end(); ++it)
	{
		int root_number;
		for (set<int>::iterator its = it->first.begin(); its != it->first.end(); ++its)
		{
			root_number=(*its);
		}

		for(int i=0;i<it->second->patterns.size();i++)
		{
			set<int> current_thing=it->first;
			current_thing.insert(it->second->patterns[i].first);
			int real_count=min(header_table[root_number],it->second->patterns[i].second);
			total_results[current_thing]=(double)real_count/(double)counter;
		}
	}
	
	for(map<set<int>,storage*>::const_iterator it = conditional_trees.begin(); it != conditional_trees.end(); ++it)
	{
		it->second->patterns.clear();
		vector<pair<int,int> >().swap(it->second->patterns);
		it->second->the_sets.clear();
		vector<int>().swap(it->second->the_sets);
		delete it->second;	
	}
	conditional_trees.clear();
	map<set<int>,storage*>().swap(conditional_trees);
	
	for(map<set<int>,double>::const_iterator it = total_results.begin();    it != total_results.end(); ++it)
	{
		string commas="";
		if(it->second>=support)
		{ 
		    for (set<int>::iterator its = it->first.begin(); its != it->first.end(); ++its)
			{
				file_output<<commas<<(*its);
				commas=",";
			}
			//file_output<<":"<<it->second<<endl;
			file_output<<fixed<<setprecision(4)<<":"<<floor(it->second*10000+0.5+0.001)/10000<<endl;
			file_output<<scientific<<setprecision(c_precision);
		}
	}

	//k tree
	total_results_replace=total_results;
	int end=0;
	while(total_results_replace.size()!=0)
	{
		int counts=0;

		total_results.clear();
		map<set<int>,double>().swap(total_results);
		
		for(map<set<int>,double>::const_iterator it = total_results_replace.begin();    it != total_results_replace.end(); ++it)
		{
			set<int> k_indices;
			k_indices=it->first;//copy

			map<int,int> k_current_map;
			vector<pair<int,int> > k_current_patterns;
			vector<int> k_single_pattern;
	
		    for (set<Trie*>::iterator its = node_k_links[it->first].begin(); its != node_k_links[it->first].end(); ++its)
			{

				for(int j=0;j<(*its)->prefix.size()-1;j++)
				{
					set<int> k_for_condition;
					k_for_condition=it->first;//original set
					k_for_condition.insert((*its)->prefix[j]->key);//one prefix added

					final_solution[k_for_condition][(*its)->prefix[j]]+=final_previous_solution[it->first][(*its)]; 
					
					node_k_links_replace[k_for_condition].insert((*its)->prefix[j]);//all ma tries*

					k_current_map[(*its)->prefix[j]->key]+=min(final_previous_solution[it->first][(*its)],(*its)->prefix[j]->number);
				}
			}

			for(map<int, int>::const_iterator iter = k_current_map.begin(); iter != k_current_map.end(); ++iter)
			{
				if(iter->second>=support*counter)
				{
					pair<int,int> current_item=make_pair(iter->first,iter->second);
					k_current_patterns.push_back(current_item);
					k_single_pattern.push_back(iter->first);
					
				}
				conditional_k_trees[k_indices]=new storage;
				conditional_k_trees[k_indices]->patterns=k_current_patterns;
				conditional_k_trees[k_indices]->the_sets=k_single_pattern;
			}
			
			k_indices.clear();
			set<int>().swap(k_indices);
			k_current_map.clear();
			map<int,int>().swap(k_current_map);
			k_current_patterns.clear();
			vector<pair<int,int> >().swap(k_current_patterns);
			k_single_pattern.clear();
			vector<int>().swap(k_single_pattern);
		}

		final_previous_solution=final_solution;
		final_solution.clear();

		for(map<set<int>,storage*>::const_iterator it = conditional_k_trees.begin(); it != conditional_k_trees.end(); ++it)
		{
			for(int i=0;i<it->second->patterns.size();i++)
			{
				set<int> k_current_thing=it->first;

				k_current_thing.insert(it->second->patterns[i].first);
				int k_real_count=it->second->patterns[i].second;
				total_results[k_current_thing]=(double)k_real_count/(double)counter;
				
				k_current_thing.clear();
				set<int>().swap(k_current_thing);
			}
		}

		for(map<set<int>,double>::const_iterator it = total_results.begin();    it != total_results.end(); ++it)
		{
			string commas="";
			if(it->second>=support)
			{ 
			    for (set<int>::iterator its = it->first.begin(); its != it->first.end(); ++its)
				{
					file_output<<commas<<(*its);
					commas=",";
				}
				//file_output<<":"<<it->second<<endl;
				file_output<<fixed<<setprecision(4)<<":"<<floor(it->second*10000+0.5+0.001)/10000<<endl;
				file_output<<scientific<<setprecision(c_precision);
			}
		}

		node_k_links=node_k_links_replace;
		node_k_links_replace.clear();
		map<set<int>,set<Trie*> >().swap(node_k_links_replace);

		for(map<set<int>,storage*>::const_iterator it = conditional_k_trees.begin(); it != conditional_k_trees.end(); ++it)
		{
			it->second->patterns.clear();
			vector<pair<int,int> >().swap(it->second->patterns);
			it->second->the_sets.clear();
			vector<int>().swap(it->second->the_sets);
			delete it->second;
		}

		conditional_k_trees.clear();
		map<set<int>,storage*>().swap(conditional_k_trees);		

		total_results_replace=total_results;
		end+=1;
	}
	
	time_end=clock();
	cout<<(time_end-time_start)/(double)(CLOCKS_PER_SEC)<<" seconds"<<endl;

	return 0;	
}
