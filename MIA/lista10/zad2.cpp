#include <stdio.h>

using namespace std;

int main(){
    double p;
    int n,t;
    scanf("%d %lf %d", &n, &p, &t);

    double **dp = new double*[n+1];

    for(int i = 0; i < n+1; i++)
        dp[i] = new double[t+1];


    for(int i=0; i<=n; i++)
        for(int j=0; j<=t; j++)
            dp[i][j]=0.0;

    dp[0][0] = 1.0;

    for(int j=0; j<t; j++){//time
        for(int i=0; i<n; i++){//people
            dp[i+1][j+1] += dp[i][j] * p;
            dp[i  ][j+1] += dp[i][j] * (1.0-p);
        }
        dp[n][j+1] += dp[n][j];
    }


    float sum = 0.0;
    for(int i=0; i<=n; i++)
        sum += i*dp[i][t];

    printf("%.10lf\n",sum);



    for(int i = 0; i < n+1; i++)
        delete[] dp[i];
    delete[] dp;
}

