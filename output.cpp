#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#define CLOCKS_PER_SEC ((clock_t)1000)

using namespace std;

int number[12800000][20]={0};//store the number to convert
string **data;//to daclare the data
int spacing=0;// count_x/thread

int str_to_int(string tmp){
	stringstream ss;
	int b=0;
	ss<<tmp;
	ss>>b;
	return b;
}

string ToString(int in){
        stringstream stream;
        stream<<in;
        return stream.str();
}

struct thread_data{
	int thread_start;
	int thread_end;

};
void *arrangement(void *threadarg){
	struct thread_data *my_data;
	my_data=(struct thread_data *)threadarg;
	for(int x=(my_data->thread_start);x<=(my_data->thread_end);x++){
		 for(int i=0;i<20;i++){
                	data[x][i]="\"col_"+ToString(i+1)+"\":"+ToString(number[x][i]);
       		 }
	}
	pthread_exit(NULL);
}

int main(){
	char buffer[240];//store the line of csv		
	fstream file;
	int thread;//the number of thread
	int count_x=0;//count the now number_x
	int count_y=0;//count the now number_y
	clock_t start, finish;
	double  duration;
	string tmp="";
	file.open("input.csv",ios::in);
	if(!file){
		cout<<"file can not open"<<endl;
	}
	else{
		while(!file.eof()){
			count_y=0;
			for(int i=0;i<240;i++){
                        	buffer[i]='x';//to reset the buffer
	                }
			file.getline(buffer,240,'\n');//if 240 or \n,then end
			int a=0;//to count the left buffer
			int b=0;//to count the right buffer
			while(buffer[b]!='x'){

				if(buffer[b]=='|'){
					tmp="";
					for(int i=a;i<b;i++){//char to string
						
						tmp=tmp+buffer[i];
					}
					number[count_x][count_y]=str_to_int(tmp);
					a=b+1;
					b++;
					count_y++;
				}
				b++;
			}
			tmp="";
			for(int i=a;i<b;i++){
				tmp=tmp+buffer[i];
			}
			number[count_x][count_y]=str_to_int(tmp);
			count_x++;
		}
		
	}
	file.close();
	count_x--;//to represent the team of the number  
	cout<<"please input the number of thread"<<endl;
        cin>>thread;
	data=new string*[count_x];
	for(int i=0;i<count_x;i++){
		data[i]=new string[20];
	}
	
	pthread_attr_t attr;
	void *status;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	spacing=count_x/thread;
	int quotient=count_x%thread;
	pthread_t threads[thread];
	struct thread_data td[thread];
	int rc;
	start = clock();
	for(int i=0;i<thread;i++){
		//cout<<"main():creating thread,"<<i<<endl;
		td[i].thread_start=i*spacing;
		td[i].thread_end=i*spacing+(spacing-1);
		if((td[i].thread_end+quotient)==count_x-1){
			td[i].thread_end=count_x-1;
		}
		//cout<<"thread_start:"<<td[i].thread_start<<endl;
	        //cout<<"thread_end:"<<td[i].thread_end<<endl;
		rc=pthread_create(&threads[i],NULL,arrangement,(void*)&td[i]);
                if(rc){
                        cout<<"Error:unable to create thread,"<<rc<<endl;
                        exit(-1);
		}
	}
	
	pthread_attr_destroy(&attr);
	for(int i=0;i<thread;i++){
		rc=pthread_join(threads[i],&status);
		if(rc){
			cout<<"ERROR:unavle to join,"<<rc<<endl;
			exit(-1);
		}		
		//cout<<"Main: completed thread id:"<<i;
		//cout<<"exiting with status:"<<status<<endl;
	}
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout<<"Duration:"<<duration<<endl;
	for(int a=0;a<count_x;a++){
		for(int b=0;b<20;b++){
			//cout<<data[a][b]<<' ';
		}
		//cout<<endl;
	}
	fstream write;
	write.open("output.json",ios::out);
	if(!write){
		cout<<"Fail to open file:"<<"output.json"<<endl;
	}
	//cout<<"File Descriptor:"<<endl;
	write<<"["<<endl;
	for(int a=0;a<count_x;a++){
		write<<" "<<"{"<<endl;
		for(int b=0;b<20;b++){
			if(b==19){
				write<<"  "<<data[a][b]<<endl;
			}
			else{
				write<<"  "<<data[a][b]<<","<<endl;
			}
		}
		if(a==count_x-1){
			write<<" "<<"}"<<endl;
		}
		else{		
			write<<" "<<"},"<<endl;
		}
	}
	write<<"]"<<endl;
	write.close();
	
	//system("pause");
	//cout<<"pthread_exit"<<endl;
        pthread_exit(NULL);
        for(int i=0;i<count_x;i++){
                delete[] data[i];
        }
        delete [] data;
}
