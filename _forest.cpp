#include<iostream>
#include "_forest.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cmath>
#include <vector>

using namespace std;

 bool debug=true;


int Node::get_val()
{
    return test_val;
}


double log_2(double val)
{
    return log(val)/log(2);
}
void debug_v(std::vector<double> v)
        {
            for(int i=0;i<v.size();i++)
            {
                
                std::cout<<v[i]<<",";
            }
            std::cout<<std::endl;
        }

//use for 2 label
double gini(vector<double> v)
{
    
    int length=v.size();
    double num_one=0;


    for (int i=0;i<length;i++)
    {
        num_one+=v[i];
    }

    double purity=num_one/length;
    double g = pow(purity,2)+pow(1-purity,2);
    
    //std::cout<<g<<std::endl;
    g=1-g;
    return g;
}


double entropy(vector<double> v)
{
    int length=v.size();
    double num_one=0;

    for (int i=0;i<length;i++)
    {
        num_one+=v[i];
    }

    double purity=num_one/length;
    double e = purity * log_2(purity) + (1-purity)*log_2(1-purity);  
    e=-1 * e;
    return e;
}



double sort();

std::vector<int> DecisionTree::classfy(std::vector<double> x,std::vector<double> y,int data_num,bool for_forest=false)
{
    std::vector<std::vector<double>> data = set_data_type(x,y,x.size()/data_num);

    /*
    hight = self.max_depth
    data_count, data_f = np.shape(x)
    error = 0
    predict_label = np.zeros((data_count))
    */

    int hight=max_depth; 
    int data_count=data_num;
    int data_f=total_feature;
    double error=0;
    std::vector<int> predict_label(data_count);
    /*
    for i in range(data_count):
        r = self.root
        result = 10000
        # print("DATA",i)
        while True:
            f = r.using_feature
            if r.label is None:
                if x[i][f] < r.threshold:
                    r = r.left_node
                else:
                    r = r.right_node
            else:
                result = r.label
                break
        # 計算error
        if result != y[i]:
            error += 1
        predict_label[i] = result
    */

    for(int i=0;i<data_count;i++)
    {
        Node * r = root;
        int result=1000000;
        while(true)
        {
            int f=r->using_feature;
            if(r->label==-1)
            {
                if (data[i][f]<r->threshold)
                {
                    r=r->left_Node;
                }else
                {
                    r=r->right_Node;
                }
            }else
            {
                result=r->label;
                break;
            }
        }

        if(result!=y[i])
        {
            error+=1;
        }
        predict_label[i]=result;

    }

    /*
    error_rate = error / data_count
    if for_forest is not True:
        print("==============result==============")
        print("type : ", self.criterion)
        print("max_depth : ", self.max_depth)
        print("accuracy : ", 1 - error_rate)
    */

    double error_rate=error/data_count;
    if(for_forest==false)
    {
        std::cout<<"==============result==============\nmax depth : "<<max_depth<<"\nacuraccy : "<<1-error_rate<<std::endl;

    }

    return predict_label;

}

void DecisionTree::build_tree(std::vector<double> x,std::vector<double> y,int data_num)
{
    long t1=clock();
    std::vector<std::vector<double>> data = set_data_type(x,y,x.size()/data_num);
    double t= clock()-t1;
    t/=CLOCKS_PER_SEC;

    root = split(data,max_depth);
    //std::cout<<"preprocess time :"<<t<<"\nsort time :"<<debug[0]<<"\nsplit-feature:"<<debug[2]<<"\nsplit data:"<<debug[3]<<"\nget y:"<<debug[5]<<"\ngini:"<<debug[4]<<"\ntransform time:"<<debug[6]<<std::endl;
   
    //debug_tree_build(root);
}


Node* DecisionTree::split(std::vector<std::vector<double>> data , int now_depth)
{
    //std::cout<<"do split, now depth:"<<now_depth<<std::endl;


    Node * this_Node =new Node();
    
    std::vector<double> y;
    for(int i=0;i<data.size();i++)
    {
        y.push_back(data[i].back());
    }
    
    //如果只剩一種資料，判定label
    //如果已經到樹的最底層，判定label
    if(gini(y)==0)
    {
        this_Node->label=y[0];
    }
    else if(now_depth==0)
    {
        int total=y.size();
        int label=0;
        for(int i=0;i<total;i++){label+=y[i];}
        if(label>(total/2))
        {this_Node->label=1;}
        else{this_Node->label=0;}
    }    
    //find threshold
    //分群left_data,right_data
    else
    {
        
        int data_count = data.size();
        int feature_count = total_feature;

        float best_cut_threshold = 0;
        float best_cut_feature = 0;
        float best_cut_position = 0;
        float min_impurity = 100000000;
        
        double t2=clock();
        for (int f=0;f<feature_count;f++)
        {
            
            //after_sort = np.asarray(sorted(data, key=lambda t: t[f]))

            double t=clock();

            std::sort(data.begin(),
            data.end(),
            [f] (const std::vector<double> &a, const std::vector<double> &b)
            {
                return a[f] < b[f];
            });

            t=(clock()-t)/CLOCKS_PER_SEC;
            debug[0]+=t;


            double t6=clock();
            std::vector<std::vector<double>> t_data=transform(data);
            t6=(clock()-t6)/CLOCKS_PER_SEC;
            debug[6]+=t6;
            
            //for i in range(1, data_count):
            for(int i=1;i<data_count;i++)
            {
                //# treshold設為 data[i] 與 data[i-1] 的中間值
                //t = (after_sort[i][f] + after_sort[i - 1][f]) / 2
                double t=(data[i][f]+data[i-1][f])/2;
                std::vector<double> this_y = t_data[feature_count];

                //debug_print(t_data);
                //debug_v(this_y);
                
                double t3=clock();
                //left_data = after_sort[:i, :]
                //std::vector<std::vector<double>> left_data(data.begin(),data.begin()+i);
                std::vector<double> left_data (this_y.begin(),this_y.begin()+i);
                //right_data = after_sort[i:, :]
                std::vector<double> right_data(this_y.begin()+i,this_y.end());
                t3=(clock()-t3)/CLOCKS_PER_SEC;
                debug[3]+=t3;

//                debug_v(left_data);
                
                //float a=1/0;

                //left_impurity = self.count_pur(left_data[:, -1])
                double t5=clock();
                //vector<double> this_y=get_y_column(left_data);
                t5=(clock()-t5)/CLOCKS_PER_SEC;
                debug[5]+=t5;


                double t4=clock();
                double left_impurity =gini(left_data);
                t4=(clock()-t4)/CLOCKS_PER_SEC;
                debug[4]+=t4;
                //right_impurity = self.count_pur(right_data[:, -1])
                
                t5=clock();
                //this_y=get_y_column(right_data);
                t5=(clock()-t5)/CLOCKS_PER_SEC;
                debug[5]+=t5;

                
                t4=clock();
                double right_impurity=gini(right_data);
                t4=(clock()-t4)/CLOCKS_PER_SEC;
                debug[4]+=t4;


                /*
                total_impurity = i * left_impurity + \
                    (data_count - i) * right_impurity
                total_impurity /= data_count
                */

                double total_impurity = i*left_impurity + (data_count-i)*right_impurity;
                //std::cout<<"\tr:"<<right_impurity<<"\tl:"<<left_impurity<<"\tt:"<<total_impurity<<"\tmin:"<<min_impurity<<std::endl;
                if(right_impurity==0 && left_impurity==0)
                {
                    //std::cout<<"SSSSSSSSSSSS"<<std::endl;
                }

                total_impurity = total_impurity/data_count;


                //if total_impurity <= min_impurity:
                if(total_impurity<=min_impurity)
                {
                    
                    //min_impurity = total_impurity
                    min_impurity=total_impurity;
                    best_cut_feature = f;
                    best_cut_feature=f;
                    best_cut_threshold = t;
                    best_cut_position = i;

                }
            }


        }
        t2=(clock()-t2)/CLOCKS_PER_SEC;

        debug[2]+=t2;
            //debug_print(data);
            /*

        this_node.using_feature = best_cut_feature
        this_node.impurity = min_impurity
        this_node.threshold = best_cut_threshold
        */

        this_Node->using_feature=best_cut_feature;
        this_Node->impurity=min_impurity;
        this_Node->threshold=best_cut_threshold;

        //std::cout<<"impurity:"<<this_Node->impurity<<std::endl;
        /*

        after_sort = np.asarray(
            sorted(data, key=lambda t: t[best_cut_feature]))
        left_data = after_sort[:best_cut_position, :]
        right_data = after_sort[best_cut_position:, :]
        */
        std::sort(data.begin(),
        data.end(),
        [best_cut_feature] (const std::vector<double> &a, const std::vector<double> &b)
        {
            return a[best_cut_feature] < b[best_cut_feature];
        });

        std::vector<std::vector<double>> left_data(data.begin(),data.begin()+best_cut_position);
        std::vector<std::vector<double>> right_data(data.begin()+best_cut_position,data.end());
        

        /*
        if now_depth is None:
            this_node.right_node = self.split(right_data, None)
            this_node.left_node = self.split(left_data, None)
        else:
            this_node.right_node = self.split(right_data, now_depth - 1)
            this_node.left_node = self.split(left_data, now_depth - 1)
        */

        
        this_Node->right_Node=split(right_data,now_depth-1);
        this_Node->left_Node=split(left_data,now_depth-1);
        
        
    
    }
    return this_Node;

}




PYBIND11_MODULE(_forest, m) {
  
  m.doc() = "Random Forest";
  m.def("gini",&gini,"gini");
  m.def("entropy",&entropy,"entropy");
  pybind11::class_<RandomForest>(m, "RandomForest")
    .def(pybind11::init())
    .def("test", &RandomForest::test);

  pybind11::class_<DecisionTree>(m, "DecisionTree")
    .def(pybind11::init<std::string,int>())
    .def("build_tree",&DecisionTree::build_tree)
    .def("classfy",&DecisionTree::classfy,pybind11::arg("x") ,
    pybind11::arg("y") ,
    pybind11::arg("data_num") ,
    pybind11::arg("for_forest") = false);
}






















