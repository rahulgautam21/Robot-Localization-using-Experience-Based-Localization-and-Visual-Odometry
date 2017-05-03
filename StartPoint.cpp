#include <bits/stdc++.h>
#include <string>
using namespace std;

int main(){
	float x[6],y[6];
	float dis[6];
	string s="";
	ofstream xxx;
	ifstream myfile;
	myfile.open("Start.txt");
	
	xxx.open("StartPointRe.txt");
	s="";
	for(int i=0;i<6;i++){
		myfile>>x[i]>>y[i];
	}
	for(int i=0;i<6;i++){
		dis[i]=0;
		for(int j=0;j<6;j++){
			dis[i] += (x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]);
		}
	}
	float mi=10000;
	int ind=0;
	for(int i=0;i<6;i++){
		cout<<dis[i]<<" ";
		if(mi>dis[i]){
			mi=dis[i];
			ind=i;
		}
	}
	cout<<endl;
	cout<<x[ind]<<","<<y[ind]<<endl;
	//float d=(x[ind]-6.5)*(x[ind]-7.5)+(y[ind]-6.5)*(y[ind]-7.5);
	//xxx<<pow(d,0.5)<<endl;
	xxx.close();
	return 0;
}
