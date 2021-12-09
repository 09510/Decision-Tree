#include<iostream> 
#include <vector> 
#include<time.h>



class Node
{
    public:
        Node()
        {
            label=-1;
        }

        Node * right_Node;
        Node * left_Node;
        int using_feature;
        double threshold;
        double impurity;
        int label;

        int test_val;
        int get_val();

};

class DecisionTree
{
    public:
        DecisionTree(std::string c,int  d )
        {
            max_depth=d;
            criterion=c;
        }
        std::string criterion;
        int max_depth;
        int total_feature;
        Node * root;
        double debug[7]={0,0,0,0,0,0,0};


        Node* split(std::vector<std::vector<double>>  data,int now_depth);
        void build_tree(std::vector<double> x,std::vector<double> y,int data_num);
        std::vector<int>  classfy(std::vector<double> x,std::vector<double> y,int data_num,bool for_forest);
        
        void get_one_node_feature();
        void fit();
        std::vector<std::vector<double>> set_data_type(std::vector<double>x,std::vector<double> y ,int feature_num)
        {
            total_feature = feature_num;
            int data_num = x.size()/total_feature;
            std::vector<std::vector<double>> after_shape;

            for(int i=0;i<data_num;i++)
            {
                std::vector<double> this_v;
                for(int f=0;f<total_feature;f++)
                {
                    this_v.push_back(x[f + i * total_feature]);
                }
                this_v.push_back(y[i]);
                after_shape.push_back(this_v);
            }

            return after_shape;
        }
        void debug_print(std::vector<std::vector<double>> t)
        {
            int row=t.size();
            int column=t[1].size();

            for(int i =0;i<row;i++)
            {
                std::cout<<i<<'\t';
                for(int j = 0;j<column;j++)
                {
                    std::cout<<t[i][j]<<" \t";
                }

                std::cout<<std::endl;
            }
        }

        void debug_v(std::vector<double> v)
        {
            for(int i=0;i<v.size();i++)
            {
                
                std::cout<<v[i]<<",";
            }
            std::cout<<std::endl;
        }
        void debug_tree_build(Node*n)
        {
            
            if(root->right_Node!=nullptr || root->left_Node!=nullptr)
            {
                
                std::cout<<"impurity:"<<n->impurity
                    <<"\tthreshold:"<<n->threshold
                    <<"\t"<<n->using_feature<<std::endl;
                
                if(n->right_Node!=nullptr){
                debug_tree_build(n->right_Node);
                }
                else{
                debug_tree_build(n->left_Node);
                }
            }
        }
        
        std::vector<std::vector<double>> transform(std::vector<std::vector<double>>x)
        {
            std::vector<std::vector<double>> ans(x[0].size(),std::vector<double>(x.size()));
            for(int i=0;i<x.size();i++)
            {
                for(int j=0;j<x[0].size();j++)
                {
                   ans[j][i]=x[i][j];
                }
            }
            /*
            for(int i=0;i<x.size();i++)
            {
                for(int j=0;j<x[1].size();j++)
                {
                    if(ans[j][i]!=x[i][j])
                    {
                        std::cout<<"FUCK"<<std::endl;
                    }
                }
            }

            std::cout<<"gi bai"<<std::endl;
            */
            return ans;
        }

        std::vector<double> get_y_column(std::vector<std::vector<double>> data)
        {
            std::vector<double> ans;
            for(int i=0;i<data.size();i++)
            {
                ans.push_back(data[i].back());
            }
            return ans;
        }

        

};

class RandomForest
{
    public:
        RandomForest()
        {
            DecisionTree * t;
            forest.push_back(t);
            max_depth=3;
        }
        std::vector<DecisionTree*> forest;
        int max_depth;
        int max_feature;
        std::string criterion;




        void build_forest();
        int test()
        {
            return max_depth;
        }

        
};


