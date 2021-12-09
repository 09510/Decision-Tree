#include<iostream> 
#include <vector> 
#include<time.h>



class Node
{
    public:
        Node()
        {
            test_val=1;
        }

        Node * right_Node;
        Node * left_Node;
        int using_feature;
        int threshold;
        int impurity;
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
        double debug[6]={0,0,0,0,0,0};


        Node* split(std::vector<std::vector<double>>  data,int now_depth);
        void build_tree(std::vector<double> x,std::vector<double> y,int data_num);
        double classfy(std::vector<double> x,std::vector<double> y,int data_num);
        
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


