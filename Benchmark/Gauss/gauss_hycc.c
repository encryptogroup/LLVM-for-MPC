#include <stdio.h>

struct I_A {
	int a0;
	int a1;
	int a2;
	int a3;
	int a4;
	int a5;
	int a6;
	int a7;
	int a8;
	int a9;
	int a10;
	int a11;
	int a12;
	int a13;
	int a14;
	int a15;
};

struct I_B {
	int b0;
	int b1;
	int b2;
	int b3;
};

int mpc_main(struct I_A INPUT_A, struct I_B INPUT_B){
	int a[16] ={INPUT_A.a0,
		INPUT_A.a1,
		INPUT_A.a2,
		INPUT_A.a3,
		INPUT_A.a4,
		INPUT_A.a5,
		INPUT_A.a6,
		INPUT_A.a7,
		INPUT_A.a8,
		INPUT_A.a9,
		INPUT_A.a10,
		INPUT_A.a11,
		INPUT_A.a12,
		INPUT_A.a13,
		INPUT_A.a14,
		INPUT_A.a15};

	int b[4] = {INPUT_B.b0,INPUT_B.b1,INPUT_B.b2,INPUT_B.b3};

	int x[4] ={0,0,0,0};

	/*for (int j=0;j<3;j++){
		for (int i=j+1;i<4;i++){
			a[i*4+j]=a[i*4+j]/a[j*4+j];
			for(int k=j+1;k<4;k++){
				a[i*4+k]=a[i*4+k]-a[i*4+j]*a[j*4+k];
			}
			b[i]=b[i]-a[i*4+j]*b[j];
		}
	}*/


	for (int i=0+1;i<4;i++){
		a[i*4+0]=a[i*4+0]/a[0*4+0];
		for(int k=0+1;k<4;k++){
			a[i*4+k]=a[i*4+k]-a[i*4+0]*a[0*4+k];
		}
		b[i]=b[i]-a[i*4+0]*b[0];
	}
	for (int i=1+1;i<4;i++){
		a[i*4+1]=a[i*4+1]/a[1*4+1];
		for(int k=1+1;k<4;k++){
			a[i*4+k]=a[i*4+k]-a[i*4+1]*a[1*4+k];
		}
		b[i]=b[i]-a[i*4+1]*b[1];
	}
	for (int i=2+1;i<4;i++){
		a[i*4+2]=a[i*4+2]/a[2*4+2];
		for(int k=2+1;k<4;k++){
			a[i*4+k]=a[i*4+k]-a[i*4+2]*a[2*4+k];
		}
		b[i]=b[i]-a[i*4+2]*b[2];
	}



	for (int i=3;i>=0;i--){
		x[i]=b[i];
		for(int j=i+1;j<4;j++){
			x[i] = x[i]-a[i*4+j]*x[j];
		}
		x[i]=x[i]/a[i*4+i];
	}
	return x[0]+x[1]+x[2]+x[3];
}

int main(){ 
    /* input matrix */

	int aa[16] ={1,2,1,-1,
			3,2,4,4,
		 	4,4,3,4,
			2,0,1,5};

	int bb[4] = {5,16,22,15};
	int xx[4];

	struct I_A a;

	a.a0=1;
	a.a1=2;
	a.a2=1;
	a.a3=-1;
	a.a4=3;
	a.a5=2; 
	a.a6=4; 
	a.a7=4; 
	a.a8=4; 
	a.a9=4;
	a.a10=3; 
	a.a11=4; 
	a.a12=2; 
	a.a13=0; 
	a.a14=1; 
	a.a15=5;

	struct I_B b;

	b.b0=5;
	b.b1=16;
	b.b2=22;
	b.b3=15;

	
	
	printf("%d\n",mpc_main(a,b));

	 

    return 0; 
} 

