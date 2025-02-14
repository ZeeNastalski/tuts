#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <list>
#include <queue>

using namespace std;

/*
 * Problem: https://www.hackerrank.com/challenges/bfsshortreach
 */

class UndirectedGraph
{
private:
	int V;
	vector<list<int>> adj; //vector of adjacencies (lists)

public:
	UndirectedGraph(int V)
	{
		this->V = V;
		adj = vector<list<int>>(V, list<int>());
	}

	void addEdge(int v, int w)
	{
		adj[v].push_back(w);
		adj[w].push_back(v);
	}

	list<int> adjacency(int v)
	{
		return adj[v];
	}

	vector<int> bfs_distances(int start, int cost)
	{
		queue<int> bfs_queue;
		vector<bool> marked(V, false);
		vector<int> distances(V, -1);

		bfs_queue.push(start);
		distances[start] = 0;
		marked[start] = true;

		while (!bfs_queue.empty())
		{
			int currentNode = bfs_queue.front();
			bfs_queue.pop();
			marked[currentNode] = true;

			for (int v : adjacency(currentNode))
			{
				if (!marked[v])
				{					
					bfs_queue.push(v);
					distances[v] = distances[currentNode] + cost;
					marked[v] = true;
				}
			}

		}

		return distances;

	}

};

int main() {
	
	int T = 0;
	cin >> T;
	
	for (int i = 0; i < T; i++) {
		
		int N = 0;
		int M = 0;
		cin >> N;
		cin >> M;

		UndirectedGraph graph(N);
		for (int j = 0; j < M; j++)
		{
			int a, b;
			cin >> a;
			cin >> b;
			
			// graph supports vertices starting from 0 
			// but the problem statement they start from 1 			
			graph.addEdge(a-1, b-1);
		}

		int start;
		cin >> start;

		vector<int> distances = graph.bfs_distances(start-1, 6);
		for (int v : distances)
		{
			if (v != 0)
			{
				cout << v << " ";
			}
		}
		cout << endl;

	}




	return 0;
}