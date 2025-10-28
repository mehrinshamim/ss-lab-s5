#include <stdio.h>
#include <stdlib.h>

void sort(int n, int arr[n]){
	for (int i=0;i<n;i++){
		for (int j=0;j<n-i-1;j++){
			int temp = arr[j];
			arr[j] = arr[j+1];
			arr[j+1] = temp;
		}
	}
}

void scan(int r,int req[r+1],int ds,int head){
int seekTime;
int pos;
	for (int i=0;i<r+1;i++){
		if (req[i] == head){
			pos = i;
			break;
		}
	}
	
	//dir - right
	for (int j= pos;j<r+1;j++)
	{
		printf("%d -> ",req[j]);
		seekTime += abs(head - req[j]);
		head = req[j];
	}
	
	if (req[r] != ds-1){
		printf("%d -> ",ds);
		seekTime += abs(head - ds-1);
		head = ds-1;
		
	for (int k=pos-1;k<=0;k--){
		printf("%d -> ",req[k]);
		seekTime += abs(head - req[k]);
		head = req[k];
	}

}
	void main(){}
	
	
}
