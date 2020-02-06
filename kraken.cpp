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

map<string,pair<string, int> > index;
const int kmerSize=31, minSize=7;


string getMinimizer (string seq){
	//from https://homolog.us/blogs/bioinfo/2017/10/25/intro-minimizer/
	// rev = seq;
	// reverse(rev.begin(),rev.end());

 //    rev=rev.replace("A","X")
 //    rev=rev.replace("T","A")
 //    rev=rev.replace("X","T")
 //    rev=rev.replace("C","X")
 //    rev=rev.replace("G","C")
 //    rev=rev.replace("X","G")

    int L=seq.length();
    string minimizer="ZZZZZZZZZZZZZ";
    for(int i=0 ; i < (L-kmerSize+1) ; i++){
        string subseq=seq.substr(i,kmerSize);
        //sub_r=rev[L-Kmer-i:L-i]
        for(int j=0; j<(kmerSize-minSize+1);j++){
        	string subseq2 = subseq.substr(j,minSize);
            if(subseq2<minimizer)
                minimizer=subseq2;         
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
	ifstream fin("/home/mohammad/Desktop/kraken/genome/output_count_in_file.txt");
	
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

	}

}


int main(){
	//preprocess
	buildTree();
	makeIndex();

	cout<<"The End!";
	return 0;
}