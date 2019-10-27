#include "hello.h"
#include <iostream>
#include <map>
#include <vector>
#include <tuple>
#include <type_traits>
#include <memory>

using namespace std;


class  Investment{

 public:
    virtual ~Investment() = default;
    Investment() =default;

    Investment(const Investment& other) = default;

    Investment& operator=(const Investment& other) = default;

    Investment(Investment&& other) = default;

    Investment& operator=(Investment&& other) = default;

};

class Stock:public Investment{

  public:

    Stock(int index,float val):index_(index),value_(val){}

    Stock(const Stock& other) =default;

    Stock& operator=(const Stock& other) = default;

    Stock(Stock&& other) = default;

    Stock& operator=(Stock&& other) = default;

   private:

    int index_;
    float value_;
};



class Bond:public Investment{


public:

};


class RealEaste:public Investment{

  public:


};


int global_investment_flag =0;



template<typename Ts...args>
auto MakeInvestment(Ts...args){

    auto deleter = [](Investment* invest_ptr){delete invest_ptr;};

   unique_ptr<Investment,decltype(deleter)> result(nullptr,deleter);


   if(global_investment_flag==0){


       result.reset(new Stock(forward<Ts>(args)...));
   }else if(global_investment_flag==1){

       result.reset(new Bond());
   }else{



   }

}

auto deleter = [](Investment* invest_ptr){delete invest_ptr;};




int main(){

 Derived d;

 Derived d2 = move(d);






    return 0;
}
