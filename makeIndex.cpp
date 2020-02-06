//In the name of Allah

#include<iostream>
#include<vector>
#include<set>
#include<string>
#include<fstream>
#include<makeIndex>

using namespace std;


void makeIndex(){
	//read k-mers of genomes (output of jellyfish)
	ifstream fin("/home/mohammad/Desktop/kraken/genome/output_count_in_file.txt");
	

	string s;
	getline(fin, s);
	cout<<s<<endl;
	getline(fin, s);
	cout<<s<<endl;
}


int main(){
	//preprocess
	makeIndex();

	cout<<"The End!";
	return 0;
}