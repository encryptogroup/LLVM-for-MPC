#include<stdio.h>

int mpc_main(int INPUT_A, int INPUT_B){
	unsigned int r1=INPUT_A*INPUT_A;
	unsigned int r2= INPUT_B* INPUT_B;
	unsigned int r3=r1+r2;
	unsigned int result=1;
	for( int i=0;i<20;i++)
		result = (result + r3 / result) /2;
	return result;
}

int main(int argc, char* argv[]) {

	unsigned int a,b,result;
	scanf("%u",&a);
	scanf("%u",&b);
	result = mpc_main(a,b);
	printf("%d\n",result);
	return 0;
}

