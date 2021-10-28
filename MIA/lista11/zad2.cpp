#include <stdio.h>
#include <math.h>

#define SIZE 1000003

using namespace std;

bool prime[SIZE];//reversed values: true = not prime
int psum[SIZE];

void sieve(){
    prime[0]=true;
    prime[1]=true;

    int n = sqrt(SIZE);
    for (int p=2; p<=n; p++) { 
        if (prime[p] == false) { 
            for (int i=p*p; i<=SIZE; i += p) 
                prime[i] = true;
        } 
    } 
}

void sum(){
    for(int i=1; i<SIZE; i++)
        psum[i] = psum[i-1] + ((prime[i]==false) ? 1 : 0);
}


int main(){
    sieve();
    sum();

    int a, b, k;
    scanf("%d %d %d", &a, &b, &k);


	int l = 1, r = b - a + 1, mid, res = -1;
	while(l <= r) {
		mid = (l + r)/2;

        bool fine = true;
        for(int i=a; (i<=b-mid+1) && fine; i++)
            if(psum[i+mid-1]- psum[i-1] < k)
                fine = false;

		if(fine) {
			r = mid - 1;
			res = mid;
		}
        else
			l = mid + 1;
	}

	printf("%d\n", res);

    return 0;
}