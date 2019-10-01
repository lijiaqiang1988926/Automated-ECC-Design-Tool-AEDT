#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>

#define P_in_length 10      // the size of the error pattern vector
#define Pran_num 5          // the number of range for each error pattern
//#define Stop_time 1*5*60    // hour * miniute * second

using namespace std;

int main(int argc, char *argv[]){

  int Length_Code = atoi(argv[1]); // number of total bits
  int p_num = atoi(argv[2]);    // number of the error pattern correction

  ofstream fout;
  ifstream fin;

  fout.open("EP_out.txt");
  fin.open("EP_in.txt");

  unsigned char tmp[30];
  int cnt=100;

  int *p_size = new int[p_num];

  int **p = new int*[p_num];  // error_pattern
  for(int i=0;i<p_num;i++) {p[i] = new int[P_in_length];}

  void buffer_right_move(int *buffer , int buf_len);

  cout<<"********************************************"<<endl;
  cout<<"               Error_Pattern_Gen            "<<endl;
  cout<<"********************************************"<<endl;

  while(cnt){
    fin>>tmp;  // read ****************** line
    if (!fin.good()) break;
  // read data
    if(tmp[0]=='*'){
      cout<<"             Read_Data_Successful         "<<endl;
    }
    else{
      cout<<"             Read_Data_Failure             "<<endl;
      break;
    }
    cout<<"--------------------------------------------"<<endl;
  // start to read Error_Pattern
    for (int i=0;i<p_num;i++){
      for(int j=0;j<P_in_length;j++){
        fin>> *(*(p+i)+j);
      }
    }

  }// while{}end

  // --------------------------------
  // Obtain the Error_Pattern size by using the  error_pattern
  for (int i=0;i<p_num;i++){
    for(int j=(P_in_length-1);j>=0;j--){
      if (*(*(p+i)+j) == 1){
        *(p_size+i) = j+1;
        break;
      }
    }
  }
// --------------------------------
int *buffer = new int[Length_Code];

for(int i=0;i<p_num;i++){
  cout<<"Error_Pattern"<<(i+1)<<"["<<*(p_size+i)<<"] = {";
  for(int j=0;j<*(p_size+i)-1;j++){
    cout<< *(*(p+i)+j) <<",";
  }
  cout<<*(*(p+i) + *(p_size+i)-1)<<"};"<<endl;
}

 for(int i=0;i<p_num;i++){
   for(int j=0;j<Length_Code;j++){
     *(buffer+j) = 0;
   }
   for(int j=0;j<*(p_size+i);j++){
     buffer[j] = *(*(p+i)+j);
   }
   fout<<"**************"<<endl;
   for(int l=0;l<Length_Code;l++){
     fout<<buffer[l]<<" ";
   }
   fout<<"\n";
   for(int k=0;k<Length_Code-(*(p_size+i));k++){
     buffer_right_move(buffer, Length_Code);
     fout<<"**************"<<endl;
     for(int l=0;l<Length_Code;l++){
       fout<<buffer[l]<<" ";
     }
     fout<<"\n";
   }
 }

  // delete the dynamic array
  delete[] p_size;
  delete[] buffer;

  for(int i=0;i<p_num;i++){delete[] p[i];}
  delete[] p;

  cout<<"--------------------------------------------"<<endl;
  cout<<">>> Error_Pattern_Generation Successful ***"<<endl;
  cout<<"--------------------------------------------"<<endl;
  cout<<"                                            "<<endl;

fout.close();
}

//数组右移
void buffer_right_move(int *buffer , int buf_len){
	int i ;
	char tmp = buffer[buf_len - 1];
	for(i = buf_len ; i > 0 ; i--)
	{
		buffer[i] = buffer[i-1] ;
	}
	buffer[0] = tmp ;
}
