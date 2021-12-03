#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


int findSide(int p1x,int p1y,int p2x,int p2y,int px,int py)
{
    int val = (py - p1y) * (p2x - p1x) -
            (p2y - p1y) * (px - p1x);

    if (val > 0)
        return 1;
    if (val < 0)
        return -1;
    return 0;
}


int lineDist(int px,int py,int qx,int qy,int rx,int ry)
{
    return abs ((ry - py) * (qx - px) -
            (qy - py) * (rx - px));
}


int setfunction(int hullx[],int hully[],int index,int convx[],int convy[]){
    int ind=0,temp;
    for(int i=0;i<index-1;i++){
        for(int j=0;j<index-i-1;j++){
            if (hullx[j]>hullx[j+1]){
                temp=hullx[j];
                hullx[j]=hullx[j+1];
                hullx[j+1]=temp;

                temp=hully[j];
                hully[j]=hully[j+1];
                hully[j+1]=temp;
            }
            else if(hullx[j]==hullx[j+1]){
                if(hully[j]>hully[j+1]){
                    temp=hully[j];
                    hully[j]=hully[j+1];
                    hully[j+1]=temp;
                }
            }
        }
    }
    int i=0;
    while (i<index){
        convx[ind]=hullx[i];
        convy[ind]=hully[i];
        ind++;
        if(hullx[i]==hullx[i+1] && hully[i]==hully[i+1])
            i=i+2;
        else
            i++;
    }
    return ind;
}


int quickHull(int x[],int y[], int n, int px,int py,int qx,int qy, int side,int hullx[],int hully[],int index)
{
    int ind = -1;
    int max_dist = 0;

    // finding the point with maximum distance
    // from L and also on the specified side of L.
    for (int i=0; i<n; i++)
    {
        int temp = lineDist(px,py, qx,qy, x[i],y[i]);
        if (findSide(px,py,qx,qy,x[i],y[i]) == side && temp > max_dist)
        {
            ind = i;
            max_dist = temp;
        }
    }

    // If no point is found, add the end points
    // of L to the convex hull.
    if (ind == -1)
    {
        hullx[index]=px;
        hully[index]=py;
        index=index+1;
        hullx[index]=qx;
        hully[index]=qy;
        index=index+1;
        return index;
    }

    // Recur for the two parts divided by a[ind]
    index=quickHull(x,y,n, x[ind],y[ind],px,py, -findSide(x[ind],y[ind],px,py, qx,qy),hullx,hully,index);
    index=quickHull(x,y, n, x[ind],y[ind],qx,qy, -findSide(x[ind],y[ind], qx,qy, px,py),hullx,hully,index);
    return index;
}

void findHull(int x[],int y[],int n,int *convx,int *convy,int *n_c)
{
    int hullx[100000],hully[100000],index=0;
    
    if (n < 3)
    {
        printf("Convex hull not possible\n");
        return;
    }

    // Finding the point with minimum and
    // maximum x-coordinate
    int min_x = 0, max_x = 0;
    for (int i=1; i<n; i++)
    {
        if (x[i] < x[min_x])
            min_x = i;
        if (x[i] > x[max_x])
            max_x = i;
    }

    // Recursively find convex hull points on
    // one side of line joining a[min_x] and
    // a[max_x]
    index=quickHull(x,y, n, x[min_x],y[min_x], x[max_x],y[max_x],1,hullx,hully,index);

    // Recursively find convex hull points on
    // other side of line joining a[min_x] and
    // a[max_x]
    index=quickHull(x,y, n, x[min_x],y[min_x], x[max_x],y[max_x], -1,hullx,hully,index);

    int ind = setfunction(hullx,hully,index,convx,convy);
    *n_c=ind;
}

void size_sweep()
{
    printf("\nSize Sweep for Serial implementation\n");
    for (int j = 10;j<1000000; j=j*10){
        printf("\n=======================================================================\n");
        double t1,t2;
        srand(time(0));
        t1=omp_get_wtime();
        printf("\nsize: %d\n\n", j);
        int n=j;

        int x[n],y[n],convx[n],convy[n];
        int n_c=0;
        srand(time(0));
        for(int i=0;i<n;i++){
            x[i]=rand()%10000;
            y[i]=rand()%10000;
        }
        findHull(x,y,n,convx,convy,&n_c);
        for(int i=0;i<n_c;i++){
            printf("(%d,%d)\n",convx[i],convy[i] );
        }
        t2=omp_get_wtime();
        printf("\nTime Taken:%lf\n", t2-t1);
        printf("\n");
        printf("\n=======================================================================\n");
    }
}


int main()
{
    size_sweep();
    return 0;
}
