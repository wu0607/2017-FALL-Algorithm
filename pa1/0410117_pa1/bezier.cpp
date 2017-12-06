# include <iostream>
# include <fstream>
# include <vector>
# include <iomanip> 
using namespace std;

int main(int argc, char**argv){
	int num_points;
	int num_sample;
	ifstream inFile(argv[1]);
	ofstream outFile(argv[2]);
	//outFile.open ("output.txt");
	vector<double> X, Y;
	bool initial = false;
	int count = 0;
	while(!inFile.eof()){
		if(!initial){
			inFile >> num_points;
			initial = true;
		}
		else if(count < num_points){
			int temp;
			inFile >> temp;
			X.push_back(temp);
			inFile >> temp;
			Y.push_back(temp);
			++ count;
		}
		else{
			inFile >> num_sample;
		}
				
	}
	//cout<<count<<endl;
	int node = (1+num_points)*num_points/2;
	double *data_x = new double[node];
	double *data_y = new double[node];
	
	for(int i=0 ; i<num_points ; ++i){
		data_x[i] = X[i];
		data_y[i] = Y[i];
	}
	
	for(int n=0 ; n<num_sample ; ++n){		
		double t = 1.0/ (double)(num_sample-1) *n;
				
		cout.setf(ios::fixed,ios::floatfield);
		cout.precision(2);
		outFile.setf(ios::fixed,ios::floatfield);
		outFile.precision(2);
		
		for(int i=num_points ; i>1 ; --i){
			for(int j=0 ; j<i-1 ; ++j){//j=0 have been initialized		
				int base = (i+num_points)*(num_points-i+1)/2;//(upper+lower)*hight/2
				data_x[base+j] = 
					(1-t) * data_x[(i+1+num_points)*(num_points-i)/2 +i+j - i] + 
					t * data_x[(i+1+num_points)*(num_points-i)/2 +i+j - i+1];
				data_y[base+j] = 
					(1-t) * data_y[base+j - i] + 
					t * data_y[base+j - i+1];
				//cout<<"i="<<i<<" j="<<j<<endl<<base+j<<" "
				//<<data_x[(i+num_points)*(num_points-i)/2 +i+j]<<" "
				//<<data_y[base+j]<<endl;
				
			}
		}
		
		outFile<<data_x[node-1]<<" "<<data_y[node-1]<<endl;
	
	}
	
	//cout<<"time: "<<(clock()-start) / CLOCKS_PER_SEC<<endl;
	
	
	delete[] data_x;
	delete[] data_y;
	
	
	inFile.close();
	outFile.close();
	return 0;
}
