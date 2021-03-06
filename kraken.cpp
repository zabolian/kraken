//In the name of Allah

#include<iostream>
#include<vector>
#include<set>
#include<string>
#include<fstream>
#include<set>
#include<map>
#include<algorithm>

using namespace std;


struct node{
	int data,parent,cnt;
};

node tree[20];
int root;

map<string, map<string, int> > index;
const int kmerSize=31, minSize=7, readSize= 100;
map<string, int> readclass;
double precent[11];
int readcnt[20];
int genomecnt[11];

string revcomp(string s){
	reverse(s.begin(),s.end());
	for(int i=0;i<s.length();i++){
		if(s[i]=='A')
			s[i]='X';
	}
	for(int i=0;i<s.length();i++){
		if(s[i]=='T')
			s[i]='A';
	}
	for(int i=0;i<s.length();i++){
		if(s[i]=='X')
			s[i]='T';
	}
	for(int i=0;i<s.length();i++){
		if(s[i]=='C')
			s[i]='X';
	}
	for(int i=0;i<s.length();i++){
		if(s[i]=='G')
			s[i]='C';
	}
	for(int i=0;i<s.length();i++){
		if(s[i]=='X')
			s[i]='G';
	}
	return s;
}

string getMinimizer (string seq){
	//from https://homolog.us/blogs/bioinfo/2017/10/25/intro-minimizer/
	//we can xor here
	string rev = revcomp(seq);

    int L=seq.length();
    string minimizer="ZZZZZZZZZZZZZ";
    for(int i=0 ; i < (L-kmerSize+1) ; i++){
        string subseq=seq.substr(i,kmerSize);
        string subseqrev=rev.substr(i,kmerSize);
        //sub_r=rev[L-Kmer-i:L-i]
        for(int j=0; j<(kmerSize-minSize+1);j++){
        	string subseq2 = subseq.substr(j,minSize);
        	string subseq3 = rev.substr(j,minSize);
            if(subseq2<minimizer)
                minimizer=subseq2;
            if(subseq3<minimizer)
            	minimizer=subseq3;         
        }
 	}
    return minimizer;
}

void buildTree(){
	//leaves = 1 to 10
	for(int i=1;i<20;i++){
		tree[i].data=tree[i].parent=tree[i].cnt=0;
	}
	root = 19;
	tree[18].parent = 19;
	tree[12].parent = 19;
	tree[1].parent = 12;
	tree[11].parent = 12;
	tree[2].parent = 11;
	tree[3].parent = 11;
	tree[4].parent = 18;
	tree[17].parent = 18;
	tree[5].parent = 17;
	tree[16].parent = 17;
	tree[6].parent = 16;
	tree[15].parent = 16;
	tree[7].parent = 15;
	tree[14].parent = 15;
	tree[8].parent = 14;
	tree[13].parent = 14;
	tree[9].parent = 13;
	tree[10].parent = 13;
}

int LCA(int node1, int node2){
	set<int> path;
    int temp1 = node1;
    int temp2 = node2;
    path.insert(temp1);
    while (temp1 != root){
    	temp1 = tree[temp1].parent;
    	path.insert(temp1);
    }
    while(path.find(temp2) == path.end()){
    	temp2 = tree[temp2].parent;
    }
    return temp2;
}

void makeIndex(){
	//read k-mers of genomes (output of jellyfish)
	ifstream fin("count.txt");
	
	for(int i=1;true;i++){
		if(i % 1000000 == 0){
			cout<<"reading "<<i<<"'s input line"<<endl;
		}
		string kmer;
		if(!(fin>>kmer))
			break;
		int node_number=-1;
		for(int j=1;j<=10;j++){
			int tmp;
			fin>>tmp;
			if(tmp){
				if(node_number==-1)
					node_number = j;
				else
					node_number = LCA(node_number,j);
			}
		}
		string tempmin = getMinimizer(kmer);
		index[tempmin][kmer]= node_number;
	}

}

void saveIndex(){
	ofstream fout("index.txt");
	map<string, map<string, int>  >::iterator it = index.begin();
	while(it!=index.end()){
		fout<<it->first<<endl;
		map<string, int> ::iterator it2 = it->second.begin();
		while(it2!=it->second.end()){
			fout<<it2->first<<" "<<it2->second<<endl;
			it2++;
		}
		it++;
	}
}

void readClassification(){
	ifstream fin("fastq.txt");
	for(int i=1;true;i++){
		if(i % 1000000 == 0){
			cout<<"reading reads "<<i<<"'s input line"<<endl;
		}
		string read;
		if(!(fin>>read))
			break;

		for(int i=1;i<20;i++){
			tree[i].cnt=0;
		}

		for(int j=0;j<readSize-kmerSize+1;j++){
			string kmer=read.substr(j,kmerSize);
			string minim = getMinimizer(kmer);
			int nodeNumber = index[minim][kmer];
			if(nodeNumber != 0){
				tree[nodeNumber].cnt++;
			}
		}

		//path to root
		for(int j=18;j>=1;j--){
			tree[j].cnt+=tree[tree[j].parent].cnt;
		}

		//find maximum
		int maxim=-1;
		for(int j=1;j<=10;j++){
			if(tree[j].cnt>maxim)
				maxim=tree[j].cnt;
		}

		int finalnode=-1;
		for(int j=1;j<=10;j++){
			if(tree[j].cnt==maxim){
				if(finalnode==-1)
					finalnode=j;
				else
					finalnode=LCA(finalnode,j);
			}
		}
		readclass[read]=finalnode;
	}
}

void calculateGenomePrecent(){
	map<string, int >::iterator it = readclass.begin();
	int sum = 0;
	while(it!=readclass.end()){
		readcnt[it->second]++;
		it++;
	}
	for(int j=18;j>=1;j--){
		readcnt[j]+=readcnt[tree[j].parent];
	}
	for(int j=1;j<=10;j++){
		sum+=readcnt[j];
	}
	for(int j=1;j<=10;j++){
		if(sum!=0)
			precent[j]=(double)readcnt[j]/(double)sum;
		else
			precent[j]=0;
		//cerr<<sum<< " "<<readcnt[j]<<endl;
	}
}

int main(){
	//preprocess
	buildTree();
	makeIndex();
	// saveIndex();
	readClassification();
	calculateGenomePrecent();
	for(int i=1;i<=10;i++){
		cout<<"genome "<<i<<" precent is: "<<precent[i]<<endl;
	}
	cout<<"The End!"<<endl;
	return 0;
}