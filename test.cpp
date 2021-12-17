#include <vector>
#include<iostream>
using namespace std;
#include <iostream>
#include <immintrin.h>
#include <x86intrin.h>
#include<time.h>
using namespace std;




int SumArray(int *buf,int N)
{
    

    int i;
    __m128i *vec128 = (__m128i *)buf;
    __m128i sum;
    

    //cout<<N/4<<endl;

    //cout<<vec128[0]<<endl;

    sum = _mm_sub_epi32(sum,sum);
    for(i=0;i<N/4;i++ ){

        sum = _mm_add_epi32(sum,vec128[i]);
    }
        sum = _mm_add_epi32(sum,_mm_srli_si128(sum,8));
        sum = _mm_add_epi32(sum,_mm_srli_si128(sum,4));
    


    return _mm_cvtsi128_si32(sum);
}
int _SumArray(int *buf,int N)
{
    

    
    __m128i vec128 = _mm_load_si128((const __m128i*)(buf));
    __m128i sum;
    

    //cout<<N/4<<endl;

    //cout<<vec128[0]<<endl;

    sum = _mm_setzero_si128();
    sum = _mm_add_epi32(sum,vec128);

    sum = _mm_add_epi32(sum,_mm_srli_si128(sum,8));
    sum = _mm_add_epi32(sum,_mm_srli_si128(sum,4));
    

    return _mm_cvtsi128_si32(sum);
}

// 32位整數數組求和_SSE版.
int32_t sumint_sse(const int32_t* pbuf, size_t cntbuf)
{
	int32_t s = 0;	// 求和變量.
	size_t i;
	size_t nBlockWidth = 4;	// 塊寬. SSE寄存器能一次處理4個int32_t.
	size_t cntBlock = cntbuf / nBlockWidth;	// 塊數.
	size_t cntRem = cntbuf % nBlockWidth;	// 剩餘數量.
	__m128i xidSum = _mm_setzero_si128();	// 求和變量。[SSE2] PXOR. 賦初值0.
	__m128i xidLoad;	// 加載.
	const __m128i* p = (const __m128i*)pbuf;	// SSE批量處理時所用的指針.
	const int32_t* q;	// 單個數據處理時所用指針.

	// SSE批量處理.
	for(i=0; i<cntBlock; ++i)
	{
        //cout<<i<<endl;
		xidLoad = _mm_load_si128(p);	// [SSE2] MOVDQA. 加載.
		xidSum = _mm_add_epi32(xidSum, xidLoad);	// [SSE2] PADDD. 32位整數緊縮環繞加法.
		p ++;
	}
	// 合併.
	q = (const int32_t*)&xidSum;
	s = q[0] + q[1] + q[2] + q[3];

	// 處理剩下的.
	q = (const int32_t*)p;
	for(i=0; i<cntRem; ++i)
	{
		s += q[i];
	}

	return s;
}

__m256 vectorAdd(__m256 a, __m256 b, __m256 c) {
    return _mm256_add_ps(a,b); //一个方法可以同时操作整个数组进行运算 - 一步到位
}

int main(){


    int size=521;
    int a[size];

    for(int i=0;i<size;i++)
    {
        a[i]=1;
    }

    int ans=0;

    double t1=clock();

    for(int l=0;l<1250000;l++){
        ans=sumint_sse(a,size);
        //ans=SumArray(a,size);
    }
    t1= clock()-t1;
    cout<<"time:"<<t1<<"\t"<<ans<<endl;

    t1=clock();
    for(int l=0;l<1250000;l++){
    ans=0; 
    for(int i=0;i<size;i++)
    {
        ans+=a[i];
    }
    }
    
    t1= clock()-t1;
    cout<<"time:"<<t1<<"\t"<<ans<<endl;



    /*
    double a[N][N],b[N][N],c[N][N];
  	int i,j;

  	for(i=0;i<N;i++)
    		for(j=0;j<N;j++)
			{
     			 a[i][j]=10;
     			 b[i][j]=6;
    		}

	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			c[i][j] = a[i][j] + b[i][j];
	}

	int block = N / 4;
	int reserve = N % 4;
	__m256d ymm0, ymm1;
	__m256d avx_sum0 = _mm256_setzero_pd();
	for(i = 0; i < N; i++){
		for(j = 0; j < block; j ++){
			ymm0 = _mm256_loadu_pd(&a[i][j*4]);
			ymm1 = _mm256_loadu_pd(&b[i][j*4]);
			avx_sum0 = _mm256_add_pd(ymm0, ymm1);
			
			_mm256_storeu_pd(*(c+i)+j*4, avx_sum0);
		}
	}
 	if(c[4][6]==16)
		printf("\n結果正確，測試完成!\n\n");
	else
    		
		printf("\n結果不正確，測試完成!\n\n");



    double a1[]={1,2,3,4};
    double a2[]={6,7,8,9};

*/

  	return 0;
}