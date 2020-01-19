
#include "console.h"
#include <fstream>
#include <iostream>

#include <algorithm>
#include <iterator>

#include <functional>
#include <vector>
#include <map>
#include <sstream>
#include  <set>
#include <error.h>
#include <cmath>
#include <cstdio>
#include <unordered_map>

const int kROW =25;



const double kMAGIC_DISTANCE =1000000;

const int kMAGICBEAN =24;

using namespace std;


class myNode{
public:
   myNode(){

   }

   myNode(double x,double y){

       xPosi =x;

       yPosi =y;
   }

   double getX() const{

       return xPosi;
   }

   double getY() const{

       return yPosi;
   }

private:

   double xPosi;
   double yPosi;
};

void intializeAllCities(vector<myNode>& cities,const string&fileName){

    ifstream input(fileName.c_str());

  string line;

    while(getline(input,line)){

        double x, y;

        stringstream ss(line);

        ss>>x>>y;

        myNode node(x,y);

        cities.push_back(node);
    }


}

double getDistanceOfTwoPoints(const myNode& a,const myNode& b){

    double ax =a.getX();

    double ay =a.getY();

    double bx =b.getX();

    double by =b.getY();

    return sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by));
}

void intializeOriginalTable(const vector<myNode>& cities,double* originalTable){

    for(int i=0;i<cities.size();i++){

        for(int j=0;j<cities.size();j++){

            int positon = i*kROW+j;



            if(i==j){

                originalTable[positon] =0;
            }else{

                myNode a =cities[i];
                myNode b =cities[j];

                double dist = getDistanceOfTwoPoints(a,b);

                originalTable[positon] =dist;

            }
        }
    }





}


vector<set<short>>  getAllSubset(int cardi,const set<short>& original){

 if(cardi>original.size()){

     error("cardi greater than the set size\n");

     vector<set<short>> empty;
     return empty; //never reached

}else if(cardi==0){

    vector<set<short>> empty;

    set<short> emptySet;
    empty.push_back(emptySet);
    return empty;


   }else if(cardi==original.size()){

       vector<set<short>> result;

       result.push_back(original);

       return result;

    }else{

        vector<set<short>> smaller,anotherWithoutFirstString;

        set<short> temp =original;

        auto firstOne = temp.begin();

        short firstShort =*firstOne;

        temp.erase(firstOne);

        smaller =getAllSubset(cardi-1,temp);

        anotherWithoutFirstString= getAllSubset(cardi,temp);


        //now we insert the first string in the all the smaller sets

       for(int i=0;i<smaller.size();i++)
           smaller[i].insert(firstShort);


       //combine two sets

       for(int i=0;i<anotherWithoutFirstString.size();i++){

           smaller.push_back(anotherWithoutFirstString[i]);

       }


       return smaller;


    }
}



//helper function

string getName(const set<short>& subset){

    string result ="";

    auto head =subset.begin();
    auto tail =subset.end();

    while(head!=tail){

        if(*head<10){

            char c ='0'+(*head);
            result+=c;
        }else{

            char deci ='0'+(*head/10);
            char c ='0'+(*head%10);

            result+=deci;
            result+=c;
        }

        ++head;

    }

    return result;

}



//intialize the first step

void intializeTheTableAtFirstStep(double*&dynamicTable1,const double*table,const vector<set<short>>&firstSet){


    // the dyanamic table 1 is nullptr at first , so no need to clean up
    int colum =firstSet.size();

    dynamicTable1 =new double[kMAGICBEAN*colum];

    // now we take advantage of structure of the vector itself ,the index of the set in the vector
    //could also be used as the index in the two-dimension array

    /*first we start the road end with 02 for every sub set*/



    for(int i=0;i<firstSet.size();i++){

        set<short> temp =firstSet[i];

        auto head =temp.begin(); // the set is just size 1

        short endNode =*head;

        int posi = (endNode-2)*colum+i;

        double result = table[endNode-1];

        dynamicTable1[posi]=result;

    }




}

void  clearTheIntermediateTable(double* &dynamicTable,map<string,int>* &indexTable){

    delete []dynamicTable;
    dynamicTable =nullptr;

    delete indexTable;
    indexTable =nullptr;
}

//helper function

void  generateSubsetIndex(const vector<set<short>>&allSubset,map<string,int>*&indexTable2){

    if(indexTable2!=nullptr){

        error("in generating new map , the map pointer is not nullptr ,memory leak\n");
    }else{

        indexTable2 =new map<string,int>();

        for(int i=0;i<allSubset.size();i++){

            string roadName =getName(allSubset[i]);

            (*indexTable2)[roadName]=i;
        }
    }

}

//fill one talbe

void  fillTheTable(double*tableToFill,const double* previousTable, const vector<set<short>>&allSubset, map<string,int>&indexTable,const double*table){

    int toFillTableCol =allSubset.size();

    for(int i=0;i<allSubset.size();i++){

        set<short> temp =allSubset[i];

        auto head =temp.begin();
        auto tail =temp.end();

        while(head!=tail){

            short endNode =*head;

            set<short> tempSet =temp;

            tempSet.erase(tempSet.find(endNode));

            string name =getName(tempSet);

            int colum =indexTable.size();
            int setPosi;

            if(indexTable.find(name)!=indexTable.end()){
            setPosi =indexTable[name];

            }else{

                error("the name should be in the set ,but it is not\n ");
            }


            auto head1 =tempSet.begin();
            auto tail1 =tempSet.end();

            double result =kMAGIC_DISTANCE;
            while(head1!=tail1){

                short onePossibleEndNode =*head1;

                int posi =(onePossibleEndNode-2)*colum+setPosi;// the dynamic table ,for the node , the index is starting from 2

                double previosuOptimalVal =previousTable[posi];

                double theLastHop =table[(onePossibleEndNode-1)*kROW+endNode-1];

                if(result>previosuOptimalVal+theLastHop)
                    result=previosuOptimalVal+theLastHop;

                //update iterator

                ++head1;
            }

            // the subset to fill is at position i , the outmost loop counter ,

            // now could the position to be filled from the endnode and subset posi ,i

            int posiToFill =(endNode-2)*toFillTableCol+i;

            tableToFill[posiToFill]=result ; //we fill one possible end node in  the set in the table

            // now contiune to fill other possible end nodes

            ++head; //!

        }

    }


}
//helper function ,clean the final mess

void finalClean(double* doubleTable1,double* doubleTable2,map<string,int>* map1,map<string,int>* map2){

    delete []doubleTable1;
    delete[] doubleTable2;

    delete map1;
    delete map2;

}

// the main function ,tsp

double tsp(const double* table,const set<short>& theNodes){

    double* dynamicTable1 =nullptr;
    double* dynamicTable2 =nullptr;
    map<string,int> * indexTable1 =nullptr;
    map<string,int>*indexTable2 =nullptr;

    //intialize the first step ,that the road is lenght 2 ,from starting node ,node 1 ,to node itself



    vector<set<short>> firstSet =getAllSubset(1,theNodes);

     generateSubsetIndex(firstSet,indexTable1);


    intializeTheTableAtFirstStep(dynamicTable1,table,firstSet);


    int indicator =1;// this numble is to show which table is the final table to use ,start same as i


    for(int i=2;i<=kMAGICBEAN;i++){

        indicator++;

        if(i%2==0){//even turn ,then use dynamic table 2 and index table 2


        // clear the useless table , then for the next turn use ,to save memory use
        clearTheIntermediateTable(dynamicTable2,indexTable2);//clear the table that will be used to store new values




        vector<set<short>> allSubset =getAllSubset(i,theNodes);


        generateSubsetIndex(allSubset,indexTable2);

         dynamicTable2 = new double[kMAGICBEAN*allSubset.size()];// this array is for two dimension use
                                                                          // a table , dynaimic programming


         // fill the table
        fillTheTable(dynamicTable2,dynamicTable1,allSubset,*indexTable1,table);




    }else{// the odd iteration

            clearTheIntermediateTable(dynamicTable1,indexTable1);

            vector<set<short>> allSubset =getAllSubset(i,theNodes);

            generateSubsetIndex(allSubset,indexTable1);

            dynamicTable1 =new double[kMAGICBEAN*allSubset.size()];

            fillTheTable(dynamicTable1,dynamicTable2,allSubset,*indexTable2,table);




        }



}

    // now decide which table has the final reuslt

    // and there is a fact about the final table ,there is just one set ,so the colum is 1

    double result =kMAGIC_DISTANCE;



    if(indicator%2==0){// it is table2

        for(int i=0;i<kMAGICBEAN;i++){

            double theMainPart =dynamicTable2[i]; // the main part of the road ,from starting 1 ,to the end node,crossing all cities

            double lastHop =table[i+2-1];

            if(result>theMainPart+lastHop)
                result =theMainPart+lastHop;

        }

    }else{// use table1 ,that has the final result


        for(int i=0;i<kMAGICBEAN;i++){

            double theMainPart =dynamicTable1[i]; // the main part of the road ,from starting 1 ,to the end node,crossing all cities

            double lastHop =table[i+2-1];

            if(result>theMainPart+lastHop)
                result =theMainPart+lastHop;

        }




    }


    finalClean(dynamicTable1,dynamicTable2,indexTable1,indexTable2);


    // return result

    return result;

}


int main(){


set<short> s;

for(short i=2;i<=25;i++)
    s.insert(i);



string fileName="readme.txt";

double table[kROW*kROW];

vector<myNode> source;

intializeAllCities(source,fileName);

intializeOriginalTable(source,table);

double result =tsp(table,s);

cout<<result;




return 0;
}
