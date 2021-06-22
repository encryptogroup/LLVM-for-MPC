#include <stdio.h>

int dummy(int a, int b, int c) {
    int array[8];
    for(int i=0;i<8;i++){
        array[i]=a+b*i;
    }
    int ret=0;
    if(c<array[c]){
        ret = array[2]+array[3];
    }
    else{
        ret = array[0]*array[1];
    }
    return ret;
}


int main(){
	int a;
	int b;
        int c;
	scanf("%d",&a);
	scanf("%d",&b);
        scanf("%d",&c);
printf("dummy: %d\n",dummy(a,b,c));
}


