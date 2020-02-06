//In the name of Allah

#include<iostream>
#include<vector>
#include<set>
#include<string>
#include<fstream>

using namespace std;

void readClassification(){

}

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
	readClassification();

	cout<<"hello";
	return 0;
}