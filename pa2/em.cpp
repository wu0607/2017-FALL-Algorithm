# include <iostream>
# include <fstream>
# include <vector>
# include <stdlib.h> 
# include <algorithm> //abs
//# include <time.h>

using namespace std;

struct Graph{
	int x=0,y=0;
	int wirelength=0;
	int flow=0;
	int capacity=0;
};
struct ResidualGraph{
	int wirelength=0;
	//int flow=0;
	int capacity=0;
};
struct edge 
{
    int a, b, cost, capacity;
};
int n, m, v;
vector<edge> e;
const int INF = 1000000000;

bool solve(Graph** G, int num_source, int num_sink, int &area) {
    vector < int > d ( n , INF) ;
    vector < int > p ( n, - 1 ) ;
	vector < int > order ( n ) ;
    int x ;
	d[v] = 0;
    for ( int i = 0 ; i < n ; ++ i ) {       
		x = - 1 ;
		for ( int j = 0 ; j < m ; ++ j ){
			if(d[v]!=0){
				break;
			}
			if ( d [ e [ j ] . b ] > d [ e [ j ] . a ] + e [ j ] . cost ) {
                d [ e [ j ] . b ] = max ( - INF, d [ e [ j ] . a ] + e [ j ] . cost ) ;
                p [ e [ j ] . b ] = e [ j ] . a ;
                order[ e[j].b] = j;
				x = e [ j ] . b ;
            }	
		}
		if(d[v]!=0){
			break;
		}
    }

    if ( x == - 1 ){
        //cout << "No negative cycle found." <<endl;
		return false;
	}
    else {
		int y = x ;
        for ( int i = 0 ; i < n ; ++ i ){
			y = p [ y ] ;
		}
        vector < int > path ;
		int bottleneck = INF;
        for ( int cur = y ; ; cur = p [ cur ] ) {
            path. push_back ( cur ) ;
			//cout<<"cur:"<<cur<<" "<<"p:"<<p[cur]<<" "<<"order:"<<order[cur]<<"  ";
            if(abs(e[order[cur]].capacity) < bottleneck){
				bottleneck = abs(e[order[cur]].capacity);
			}
			if ( cur == y && path. size ( ) > 1 )  break ;
        }
        reverse ( path. begin ( ) , path. end ( ) ) ;

        // cout <<endl<< "Negative cycle: " ;
		
		// for ( size_t i = 0 ; i < path. size ( ) ; ++ i ){
            // cout << path [ i ] <<"-"<< e[order[path[i]]].a<<"-"<<e[order[path[i]]].b<<" ";			
		// }
		
		// cout<<"bottleneck"<<bottleneck<<endl;
		
		/* Update Graph flow and Edge. */
		edge add;
		for(int i=0 ; i<path.size()-1 ; ++i){
			if(path[i] < num_source){
				G[path[i]+1][path[i+1]+1-num_source].flow += bottleneck;
				area += G[path[i]+1][path[i+1]+1-num_source].wirelength * bottleneck;
			}else if(path[i] >= num_source){
				G[path[i+1]+1][path[i]+1-num_source].flow -= bottleneck;
				area -= G[path[i+1]+1][path[i]+1-num_source].wirelength * bottleneck;				
			}	
			if(e[order[path[i]]].capacity > 0){
				if(e[order[path[i]]].capacity - bottleneck == 0){
					int temp = e[order[path[i]]].a;					
					e[order[path[i]]].a = e[order[path[i]]].b;
					e[order[path[i]]].b = temp;
					e[order[path[i]]].cost = (-e[order[path[i]]].cost);
					e[order[path[i]]].capacity = (-e[order[path[i]]].capacity);
				}else if(e[order[path[i]]].capacity - bottleneck > 0){
					e[order[path[i]]].capacity -= bottleneck;
					bool find = false;
					for(int j=0 ; j<e.size() ; ++j){
						if(e[order[path[i]]].b == e[j].a && e[order[path[i]]].a == e[j].b){
							e[j].capacity = e[j].capacity + (-bottleneck);
							find = true;
							break;
						}
					}
					if(!find){
						add.a = e[order[path[i]]].b;
						add.b = e[order[path[i]]].a;
						add.cost = (-e[order[path[i]]].cost);
						add.capacity = (-bottleneck);
						e.push_back(add);
					}
				}
			}
			else if(e[order[path[i]]].capacity < 0){
				if(e[order[path[i]]].capacity + bottleneck == 0){	
					int temp = e[order[path[i]]].a;					
					e[order[path[i]]].a = e[order[path[i]]].b;
					e[order[path[i]]].b = temp;
					e[order[path[i]]].cost = (-e[order[path[i]]].cost);
					e[order[path[i]]].capacity = (-e[order[path[i]]].capacity);
				}else if(e[order[path[i]]].capacity + bottleneck < 0){
					e[order[path[i]]].capacity += bottleneck;
					bool find = false;
					for(int j=0 ; j<e.size() ; ++j){
						if(e[order[path[i]]].b == e[j].a && e[order[path[i]]].a == e[j].b){
							e[j].capacity = e[j].capacity + bottleneck;
							find = true;
							break;
						}
					}
					if(!find){
						add.a = e[order[path[i]]].b;
						add.b = e[order[path[i]]].a;
						add.cost = (-e[order[path[i]]].cost);
						add.capacity = bottleneck;
						e.push_back(add);
					}
				}
			}
		}
		//cout<<"area:"<<area;
		// outFile<<"New edge"<<endl;
		
		// for(int i=0 ; i<e.size() ; ++i){
			// outFile << e[i].a <<" "<< e[i].b <<" "<<e[i].cost <<" "<<e[i].capacity<<"  "<<endl;
		// }
		// 
		// outFile<<"e.size:"<<e.size()<<endl;
		/* Update Edge vector already. Erase capacity==0 edges. */
		// for(int i=e.size()-1 ; i>=0 ; --i){
			// if(e[i].capacity == 0){
				// e.erase(e.begin()+i);
			// }
		// }
		m = e.size();
		// outFile<<"Clear edge"<<endl;
		// for(int i=0 ; i<e.size() ; ++i){
			// outFile << e[i].a <<" "<< e[i].b <<" "<<e[i].cost <<" "<<e[i].capacity<<"  "<<endl;
		// }
		return true;
    }
} 
bool acompare(Graph lhs, Graph rhs) { 
	if(lhs.x==rhs.x) 
		return lhs.y < rhs.y;
	return lhs.x < rhs.x; }
int main(int argc, char**argv){
	//clock_t start= clock(), finish;
	int num_points, num_source=0, num_sink=0;
	int area = 0;
	ifstream inFile(argv[1]);
	ofstream outFile(argv[2]);
	vector<int> X, Y, init_flow;//???input X = int or double
	inFile >> num_points;
	
	while( (num_source+num_sink) < num_points){	
			int temp;
			inFile >> temp;		
			X.push_back(temp);
			inFile >> temp;
			Y.push_back(temp);
			inFile >> temp;
			if(temp>0){++num_source;}
			else{++num_sink;}			
			init_flow.push_back(temp);
			
	}
	//cout<<num_source<<" "<<num_sink<<endl;
	// for(int i=0;i<init_flow.size(); ++i){
		// cout<< X[i]<<" "<<Y[i]<<" "<<init_flow[i]<<endl;
	// }
	// cout<<"num_source:"num_source<<endl<<"num_sink:"<<num_sink<<endl;
	/* new 2D structure to build a graph / residual graph */
	Graph** G = new Graph*[num_source+1];
	for(int i=0 ; i<=num_source ; ++i){
		G[i] = new Graph[num_sink+1];
	}
	//Put sources and sinks into graph in the lexigrapgical order.
	int a=0,b=0;
	for(int i=0 ; i<num_points ; ++i){
		if(init_flow[i] > 0){
			G[a+1][0].x = X[i];
			G[a+1][0].y = Y[i];
			G[a+1][0].flow = init_flow[i];
			++a;
		}else{
			G[0][b+1].x = X[i];
			G[0][b+1].y = Y[i];
			G[0][b+1].flow = init_flow[i];
			++b;
		}
	}
	
	sort(*G, *G+(num_sink+1), acompare);
	for(int i=1 ; i<=num_source ; ++i){
		for(int j=i ; j<=num_source ; ++j){
			if(G[i][0].x > G[j][0].x){
				swap(G[i][0].x, G[j][0].x);
				swap(G[i][0].y, G[j][0].y);
				swap(G[i][0].flow, G[j][0].flow);
			}
			if(G[i][0].x == G[j][0].x && G[i][0].y >= G[j][0].y){
				swap(G[i][0].x, G[j][0].x);
				swap(G[i][0].y, G[j][0].y);
				swap(G[i][0].flow, G[j][0].flow);
			}
		}
	}
	
	/* Preprosessing is done! */	
	/* count distance(x-x + y-y)and capacity */
	for(int i=1;i<=num_source; ++i){
		for(int j=1;j<=num_sink;++j){
			G[i][j].wirelength = abs(G[i][0].x-G[0][j].x) + abs(G[i][0].y-G[0][j].y);
			if( abs(G[i][0].flow) < abs(G[0][j].flow) ){
				G[i][j].capacity = abs(G[i][0].flow);
			}else{
				G[i][j].capacity = abs(G[0][j].flow);
			}
		}
	}
	//print and check
	// for(int i=0;i<=num_source; ++i){
		// for(int j=0;j<=num_sink;++j){
			// cout<<G[i][j].wirelength<<" "<<G[i][j].capacity<<" "<<G[i][j].flow<<"  ";
		// }
		// cout <<endl;
	// }
	
	/* Start to assign flow! */
	int min_index = 1;
	//cout<<"Start to assign flow"<<endl;
	for(int i=1 ; i<=num_source ; ++i){
		while(G[i][0].flow > 0){			
			for(int j=2 ; j<=num_sink ; ++j){
				while(G[i][min_index].flow == G[i][min_index].capacity 
					|| G[0][min_index].flow==0){
					min_index = (min_index+1)%(num_sink+1) ;
				}
				if(G[i][min_index].wirelength > G[i][j].wirelength 
					&& G[i][j].flow!=G[i][j].capacity
					&& G[0][j].flow!=0){
					min_index = j;					
				}
				if(G[i][min_index].wirelength == G[i][j].wirelength 
					//&& G[i][min_index].capacity < G[i][j].capacity
					&& abs(G[0][min_index].flow) < abs(G[0][j].flow)
					&& G[i][j].flow!=G[i][j].capacity
					&& G[0][j].flow!=0){
					min_index = j;
				}
				
			}
			if(G[i][0].flow > abs(G[0][min_index].flow)){
				G[i][0].flow += G[0][min_index].flow;
				G[i][min_index].flow += abs(G[0][min_index].flow);
				G[0][min_index].flow = 0;
			}else{
				G[0][min_index].flow += G[i][0].flow;
				G[i][min_index].flow += G[i][0].flow;
				G[i][0].flow = 0;
			}
			
		}
		// cout<<endl<<i<<endl;
		// for(int i=0;i<=num_source; ++i){
		// for(int j=0;j<=num_sink;++j){
			// cout<<G[i][j].wirelength<<" "<<G[i][j].flow<<" "<<G[i][j].capacity<<"  ";
		// }
		// cout <<endl;
		// }
	}
	//cout<<"Done send flow"<<endl;
	
	/* We have initial graph(num_source+1)*(num_sink+1) now. Draw the residual grapgh. */	
	ResidualGraph** RG = new ResidualGraph*[num_source+num_sink+1];
	for(int i=0 ; i<=(num_source+num_sink) ; ++i){
		RG[i] = new ResidualGraph[num_source+num_sink+1];
	}
	int num_edge = 0;
	
	for(int i=1;i<=num_source; ++i){		
		for(int j=1;j<=num_sink;++j){
			area += G[i][j].wirelength * G[i][j].flow;
			if(G[i][j].flow==0){
				++ num_edge;
				RG[i][num_source+j].wirelength = G[i][j].wirelength;
				RG[i][num_source+j].capacity = G[i][j].capacity;				
			}else if(G[i][j].flow < G[i][j].capacity){
				++ num_edge;
				RG[i][num_source+j].wirelength = G[i][j].wirelength;
				RG[i][num_source+j].capacity = G[i][j].capacity -  G[i][j].flow;
				++ num_edge;
				RG[num_source+j][i].wirelength = (-G[i][j].wirelength);
				RG[num_source+j][i].capacity = (-G[i][j].flow);
			}else if(G[i][j].flow == G[i][j].capacity){
				++ num_edge;
				RG[num_source+j][i].wirelength = (-G[i][j].wirelength);
				RG[num_source+j][i].capacity = (-G[i][j].flow);
			}
		}		
	}
	// outFile<<"num_edge"<<num_edge<<endl;
	// for(int i=0;i<=num_source+num_sink; ++i){
		// for(int j=0;j<=num_source+num_sink;++j){
			// outFile<<RG[i][j].wirelength<<" "<<RG[i][j].capacity<<"  ";
		// }
		// outFile <<endl;
	// }
	
	/* We have residual graph(num_source+num_sink+1)^2 now. Run Bellman-Ford. */
	/* First pick out RG's edge. */
	int edgecount = 0;
	e.resize(num_edge);
	// for(int i=1;i<=num_source+num_sink; ++i){
		// for(int j=1;j<=num_source+num_sink;++j){			
			// if(RG[i][j].wirelength != 0){
				// e[edgecount].a = i-1;
				// e[edgecount].b = j-1;
				// e[edgecount].cost = RG[i][j].wirelength;
				// e[edgecount].capacity = RG[i][j].capacity;
				// ++edgecount;
			// }
		// }
	// }
	for(int i=1;i<=num_source; ++i){
		for(int j=num_source+1;j<=num_source+num_sink;++j){			
			if(RG[i][j].wirelength != 0){
				e[edgecount].a = i-1;
				e[edgecount].b = j-1;
				e[edgecount].cost = RG[i][j].wirelength;
				e[edgecount].capacity = RG[i][j].capacity;
				++edgecount;
			}
		}
	}
	for(int i=num_source+1;i<=num_source+num_sink; ++i){
		for(int j=1;j<=num_source;++j){			
			if(RG[i][j].wirelength != 0){
				e[edgecount].a = i-1;
				e[edgecount].b = j-1;
				e[edgecount].cost = RG[i][j].wirelength;
				e[edgecount].capacity = RG[i][j].capacity;
				++edgecount;
			}
		}
	}
	// cout<<endl<<"Flow Graph:"<<endl;
		// for(int i=0;i<=num_source; ++i){
		// for(int j=0;j<=num_sink;++j){
			// cout<<G[i][j].wirelength<<" "<<G[i][j].flow<<" "<<G[i][j].capacity<<"  ";
		// }
		// cout <<endl;
		// }
	// cout<<"edge"<<endl;
	// for(int i=0 ; i<e.size() ; ++i){
		// cout <<i<<": "<<e[i].a <<" "<< e[i].b <<" "<<e[i].cost <<" "<<e[i].capacity<<"  "<<endl;
	// }
	//cout << "num_edge:"<<num_edge<<" e.size():"<<e.size()<<endl;
	
	n = num_source + num_sink;
	m = num_edge;
	v=0;
	// cout << "n:"<<n<<endl<<"m:"<<m<<endl<<"v:"<<v<<endl;
	bool negative = 1;
	
	while(negative == true){
		negative = solve(G, num_source, num_sink, area);
	}
	
	// cout<<endl<<"Flow Graph:"<<endl;
	// for(int i=0;i<=num_source; ++i){
		// for(int j=0;j<=num_sink;++j){
			// cout<<G[i][j].wirelength<<" "<<G[i][j].flow<<" "<<G[i][j].capacity<<"  ";
		// }
		// cout <<endl;
	// }
	//cout<< "area:"<<area <<endl;;
	outFile << area;
	for(int i=1;i<=num_source; ++i){
		for(int j=1;j<=num_sink;++j){
			if(G[i][j].flow != 0){
				outFile<<endl<<G[i][0].x<<" "<<G[i][0].y<<" "<<G[0][j].x<<" "<<G[0][j].y<<" "<<G[i][j].flow;
			}
		}
	}
	
	
	/* Release memory. */	
	for(int i=0 ; i<=(num_source+num_sink) ; ++i){
		delete [] RG[i];
	}
	delete [] RG;
	for(int i=0 ; i<=num_source ; ++i){
		delete [] G[i];
	}
	delete [] G;
	inFile.close();
	outFile.close();
	//cout << "Time: "<<(double)(clock() - start) / CLOCKS_PER_SEC <<" s"<<endl; 
	return 0;
}
