/*
 Author: <Qiaoling(Jolin) Chen>
 Class: ECE4122 or ECE6122
 Last Date Modified: <2019.8.26>
 Description:
 What is the purpose of this file?
 
 This code is for homework1_problem3, to find a solution for knight to sit in a circle following certain rules.
 
 This code shoule be compiled using command: g++ -o hw1_p3 hw1_p3.cpp -std=c++0x
 */
#include <iostream>
#include <map>
#include <vector>
#include <forward_list>
#include <stack>
using namespace std;

vector<unsigned long> longestPath; // This is to store the longest path found in the process.
vector<unsigned long> temp; // This is to store the points while searching inside a graph.

struct undirectedGraph
{
    /*
     The undirectGraph is a data structure to store information in the form of undirected graph.
     */
    size_t V; // V is the point of graph
    map<unsigned long, forward_list<unsigned long> > adj; // adj is the content of undirectedGraph
};

unsigned long fib(unsigned long n)
{
    /*
     fib is a function to list n Fibonacci numbers using recursive method.
     */
    if (n == 1 || n == 2)
    {
        return 1;
    }
    else return fib(n-1)+fib(n-2);
}


void dfs(undirectedGraph graph, vector<int> visited, unsigned long v)
{
    /*
     The function dfs is the implementation of depth first search. To search
     inside an undirected graph and renew longestway.
     */
    vector <unsigned long>().swap(temp);
    temp.push_back(v);
    while (temp.size() != 0)
    {
        /*
         Using iteration method to traverse the graph and find the longest path.
         */
        unsigned long i = temp[temp.size() - 1];
        forward_list<unsigned long>::const_iterator ite;
        for (ite = graph.adj.at(i).cbegin(); ite != graph.adj.at(i).cend(); ++ite)
        {
            if (!visited[*ite])
            {
                visited[*ite] = 1;
                temp.push_back(*ite);
                break;
            }
        }
        if (ite == graph.adj.at(i).cend())
        {
            if (longestPath.size() < temp.size())
            {
                vector <unsigned long>().swap(longestPath);
                longestPath = temp;
            }
            temp.pop_back();
        }
    }
    return ;
}



int main()
{
    unsigned long n;
    cout << "Please enter the number of knights: \n";
    cin >> n;
    if (n <= 0)
    {
        cout << "invalid number! Please input positive number of knights!" << endl;
        return 0;
    }
    // creat Fibonacci sequence and store in map fibo
    map<unsigned long, unsigned long>  fibo;
    for (unsigned long i = 1; fib(i) <= 2*n-1; i++)
    {
        unsigned long key = fib(i);
        fibo[key] = i;
    }
    // initiate undirectedGraph
    undirectedGraph graph;
    graph.V = n;
    for (unsigned long row = 1; row <= n; row++)
    {
        forward_list<unsigned long> temp_list;
        for (unsigned long col =1; col <= n; col++)
        {
            if (fibo.count(row + col) && row != col)
            {
                temp_list.push_front(col);
            }
        }
        graph.adj[row] = temp_list;
    }
    
    for (unsigned long i = 1; i <= n; i++)
    {
        /*
        using dfs searching method in a loop from the first point
        until find the path that allows all knights sitting around the table
         */
        vector<int> visited(n + 1,0);
        visited[i] = 1;
        dfs(graph, visited, i);
        if (longestPath.size() == n)
        {
            // already find the solution, print it out
            cout << 'K' << ' ';
            for (std::vector<unsigned long>::const_iterator jj = longestPath.begin(); jj != longestPath.end(); ++jj)
            {
                std::cout << *jj << ' ';
            }
            cout << endl;
            return 0;
        }
    }
    // no solution found in the loop search, print "No solution found!"
    cout << "No solution found!" << endl;
    
}
