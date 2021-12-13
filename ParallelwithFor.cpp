#include    <bits/stdc++.h> 
#include    <omp.h>
#include    <stdlib.h>
#include    <iostream>
#include    <chrono>

using namespace std::chrono;

using namespace std; 

#define iPair pair<int, int> 
  

set<iPair> hull; 
  
int findSide(iPair p1, iPair p2, iPair p) 
{ 
    int val = (p.second - p1.second) * (p2.first - p1.first) - 
              (p2.second - p1.second) * (p.first - p1.first); 
  
    if (val > 0) 
        return 1; 
    if (val < 0) 
        return -1; 
    return 0; 
} 
  
 
int lineDist(iPair p1, iPair p2, iPair p) 
{ 
    return abs ((p.second - p1.second) * (p2.first - p1.first) - 
               (p2.second - p1.second) * (p.first - p1.first)); 
} 
  

void quickHull(iPair a[], int n, iPair p1, iPair p2, int side) 
{ 
    
    int ind = -1; 
    int max_dist = 0; 
    
    //Este for presenta el mayor impacto al paralelizar
    #pragma omp parallel for shared(p1,p2,a,max_dist,ind)
    for (int i=0; i<n; i++) 
    { 
        int temp = lineDist(p1, p2, a[i]); 
        if (findSide(p1, p2, a[i]) == side && temp > max_dist) 
        { 
            ind = i; 
            max_dist = temp; 
        } 
    } 
  
    if (ind == -1) 
    { 
        hull.insert(p1); 
        hull.insert(p2); 
        return;
    } 
  
    #pragma omp task 
    {
    quickHull(a, n, a[ind], p1, -findSide(a[ind], p1, p2)); 
    }
    #pragma omp task 
    {
    quickHull(a, n, a[ind], p2, -findSide(a[ind], p2, p1)); 
    }

} 
  
void printHull(iPair a[], int n) 
{ 
    // a[i].second -> y-coordinate of the ith point 
    if (n < 3) 
    { 
        cout << "Convex hull not possible\n"; 
        return; 
    } 
  
    // Finding the point with minimum and 
    // maximum x-coordinate 

    int min_x = 0, max_x = 0; 
    //No vale la pena paralelizar
    //#pragma omp parallel for
    for (int i=1; i<n; i++) 
    { 
        if (a[i].first < a[min_x].first) 
            min_x = i; 
        if (a[i].first > a[max_x].first) 
            max_x = i; 
    } 
  
    // Recursively find convex hull points on 
    // one side of line joining a[min_x] and 
    // a[max_x] 
    #pragma omp task 
    quickHull(a, n, a[min_x], a[max_x], 1); 
  
    // Recursively find convex hull points on 
    // other side of line joining a[min_x] and 
    // a[max_x] 
    #pragma omp task
    quickHull(a, n, a[min_x], a[max_x], -1); 
  
    cout << "The points in Convex Hull are:\n"; 
    while (!hull.empty()) 
    { 
        cout << "(" <<( *hull.begin()).first << ", "
             << (*hull.begin()).second << ") "; 
        hull.erase(hull.begin()); 
    } 
} 



void sizeSweep(int num_threads){

    int max_numb_threats = omp_get_max_threads();
	printf("Maximum number of threads = %d\n", max_numb_threats);

    omp_set_num_threads(num_threads);
    printf("Number of threads used = %d\n",num_threads); 

    for (int j = 1000000; j >= 10; j = j/10){
    printf("\nsize: %d\n",j);
    iPair a[j] = {};
    int i=0;

    if (j >= 10000){
        while(i < j){
            a[i].first = rand()%10000;
            a[i].second = rand()%10000;
            i++;
        }
    }
    else if(j == 1000){
        while(i < j){
            a[i].first = rand()%100;
            a[i].second = rand()%100;
            i++;
        }
    }
    else if(j == 100 || j==10){
        while(i < j){
            a[i].first = rand()%10;
            a[i].second = rand()%10;
            i++;
        }
    }

    int n = sizeof(a)/sizeof(a[0]); 
    auto start = high_resolution_clock::now();
    printHull(a, n); 
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    printf("\n\nTiempo: %ld\n", duration.count());

    }
}

void threadSweep(){
    

    for (int j = 1000000; j >= 10; j = j/10){
        int max_numb_threats = omp_get_max_threads();
	    printf("Maximum number of threads = %d\n", max_numb_threats);
        printf ("\n ================================================ \n");
        printf("\nsize: %d\n",j);
        iPair a[j] = {};
        int i=0;

        if (j >= 10000){
            while(i < j){
                a[i].first = rand()%10000;
                a[i].second = rand()%10000;
                i++;
            }
        }
        else if(j == 1000){
            while(i < j){
                a[i].first = rand()%100;
                a[i].second = rand()%100;
                i++;
            }
        }
        else if(j == 100 || j==10){
            while(i < j){
                a[i].first = rand()%10;
                a[i].second = rand()%10;
                i++;
            }
        }

    for (int k = 1; k <= 8; k++){

        omp_set_num_threads(k);
        printf("Number of threads used = %d\n",k); 


        int n = sizeof(a)/sizeof(a[0]); 
        auto start = high_resolution_clock::now();
        printHull(a, n); 
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);

        printf("\n\nTiempo: %ld\n\n\n", duration.count());

    }
     printf ("\n ================================================ \n");
    }
}
  
int main() 
{ 
    printf ("\nInitializing Size Sweep for Quick Hull\n\n");
   
    printf ("\n ================================================ \n");
    sizeSweep(1);
    printf ("\n ================================================ \n");
    sizeSweep(2);
    printf ("\n ================================================ \n");
    sizeSweep(4);
    printf ("\n ================================================ \n");
    sizeSweep(6);
    printf ("\n ================================================ \n");
    sizeSweep(8);
    printf ("\n ================================================ \n");
    
    printf ("\nFinalizing Size Sweep for Quick Hull\n\n");

    printf ("\nInitializing Thread Sweep for Quick Hull\n\n");

    threadSweep();

    printf ("\n Finalizing Thread Sweep for Quick Hull\n\n");
    return 0; 
} 