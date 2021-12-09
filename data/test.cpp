#include<iostream>
#include<vector>

using namespace std;


void de(vector<int> v)
{
    for(int i=0;i<v.size();i++)
    {
        cout<<v[i];
    }
    cout<<endl;
}

int main()
{

    vector<int> v1={0,1,2,3,4,5,6,7,8,9,10};

    vector<int> v2(v1.begin(),v1.begin()+5);
    vector<int> v3(v1.begin()+5,v1.end());
    

    v1[1]=10;
    de(v1);
    de(v2);
    de(v3);

    return 0;
}


