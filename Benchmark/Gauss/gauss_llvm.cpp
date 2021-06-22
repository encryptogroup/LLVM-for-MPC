#include <cstdio>

int solve(int a0,int  a1,int  a2,int  a3,int  a4,int  a5,int  a6,int  a7,int  a8,int  a9,int  a10, int a11,int  a12,int  a13,int  a14,int  a15, int b0, int b1, int b2, int  b3){
	int a[16] ={a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15};

	int b[4] = {b0,b1,b2,b3};

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

	int a[16] ={1,2,1,-1,
			3,2,4,4,
		 	4,4,3,4,
			2,0,1,5};

	int b[4] = {5,16,22,15};
	int x[4];

	int a0=1, a1=2, a2=1, a3=-1, a4=3, a5=2, a6=4, a7=4, a8=4, a9=4, a10=3, a11=4, a12=2, a13=0, a14=1, a15=5, b1=5, b2=16, b3=22, b4=15;
	printf("%d\n",solve(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, b1, b2, b3, b4));

	 

    return 0; 
} 

