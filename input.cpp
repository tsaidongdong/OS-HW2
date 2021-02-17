#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

int main(){
	int n=0;//set the number of number
	int max=2147483647;
	int min=-2147483648;
	int output=0;
	int test=0;
	int count=1;//the number of the line
	cout<<"Please input the number to generate"<<endl;
	cin>>n;
	string filename="input.csv";
	fstream fp;
	fp.open(filename,ios::out);
	if(!fp){
		cout<<"Fail to open the file:"<<endl;
	}
	srand(time(NULL));
	for(int i=0;i<n;i++){
		output=rand()%(max-0+1)+0;
		test=rand()%(1-0+1)+0;
		if(test==0){
			output=output*1;
		}
		else{
			output=output*(-1);
		}
		if(count==20){
			count=1;
			fp<<output<<endl;
		}
		else{
			count++;
			fp<<output<<"|";
		}
		
	}
	fp.close();
	return 0;
}

