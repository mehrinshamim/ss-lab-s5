#include <stdio.h>
#include <stdlib.h>

void fcfs(int r, int st, int requests[r])
{
	int totalseek = 0;
	int currentseek = 0;
	int pos = st;
	
	printf("%d ",pos);
	for (int i=0;i<r;i++){
		currentseek = abs(pos - requests[i]);
		totalseek += currentseek;
		printf("-> %d ",requests[i]);
		 
		
	}
	printf("\nTotal Seek Time: %d",totalseek);
}

void main(){
	int r, req[r],st;
	printf("Enter no of req:");
	scanf("%d",&r);
	printf("Enter req:");
	for (int i=0;i<r;i++){
		scanf("%d",&req[i]);
	}
	printf("\nEnter head:");
	scanf("%d",&st);
	fcfs(r,st,req);
	return;

}
