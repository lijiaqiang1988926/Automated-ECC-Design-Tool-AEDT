#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
        
#define Length_Code 18          //Code length
#define Length_R 6              //Redundant bit
#define Length_D 12             //Data bit
#define Length_Range 64    //Size of array of probability of adding array
        
        
int main(int argc, char *argv[]){
        
int Injection_Num = atoi(argv[1]); // Injection Number
        
ifstream fin, fin_b;;
ofstream fout;
fin.open("Matrix.txt");
fin_b.open("EP_out.txt");
        
int ar_in[Length_Code]={0};
int ar_out[Length_Code][Length_R];

unsigned char tmp[30];
int cnt = 100; 
int counter = 0;
        
while(cnt){
// Read "******************" Row
  fin>>tmp;
// Indentify the end of the document
  if (!fin.good()) break;
//Identify to read a new line
  if(tmp[0]=='*')
  cout<<"****** Read Matrix Successfully ******"<<endl;
  else
  {cout<<"****** Read Failure ******"<<endl; break;}
//Process Number
counter++;
cout<<"Process No. "<<counter<<" is doing !"<<endl;
  
//Begining Reading the Matrix
  for (int i=0;i<Length_Code;i++){
    fin>>ar_in[i];
  }
//Decimal to Binary Conversion
  for(int j=0; j<Length_Code; j++){
    for(int i=0;i<Length_R;i++){
      if ((ar_in[j]&(1<<i))) ar_out[j][i] = 1;
      else ar_out[j][i] = 0;
    }
  }
//Output the Matrix
  cout<<"******** Matrix **********"<<endl;
  for(int k=Length_R-1;k>=0;k--){         //先输出每一列的第一个元素
    for (int i=0;i<Length_Code;i++){
      cout<<ar_out[i][k]<<" ";
    }
      cout<<"\n";
  }
  cout<<"*************************"<<endl;
//---------------------------------------------------------------
int counter_pattern = 0;
int ep[Length_Code] = {0};
float total_ok = 0;
float total_dok = 0;
int cnt_pattern = 1;
int Num_pattern = 0;
        
while(cnt_pattern){
  fin_b>>tmp;  // read ****************** line
  if (!fin_b.good()) break;
// read data
  if(tmp[0]=='*')
  cout<<"-------------Read_Error_Pattern_Successful----------"<<endl;
  else
  {cout<<"------------Read_Error_Pattern_Failure-------------"<<endl; break;}
  cout<<"********************************************"<<endl;
//Process Number
  counter_pattern = counter_pattern + 1;
  cout<<"Error_Pattern Process No. "<<counter_pattern<<" is doing !"<<endl;
// start to read Error_Pattern
  for(int i=0;i<Length_Code;i++){
    fin_b>> ep[i];
  }
  Num_pattern++;
//---------------------------------------------------------------
int counter_injection = 0;
int p_ok = 0; // ok event for one pattern
int d_ok = 0; // ok for dectection
while(counter_injection<Injection_Num){
counter_injection = counter_injection + 1;
//---------------------------------------------------------------
int input_deci;
int in_put[Length_D]={0};
srand((unsigned)time(NULL));
input_deci = rand() % Length_Range;
//Input Decimal to Binary Conversion
for(int i=0; i<Length_D; i++){
  if (input_deci&(1<<i)) in_put[i] = 1;
  else in_put[i] = 0;
}

int e_p1[1] = {1};
int e_p2[2] = {1,1};
int e_p3[3] = {1,0,1};
int e_p4[3] = {1,1,1};
        
int p1_en[18] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int p2_en[18] = {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int p3_en[18] = {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int p4_en[18] = {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
        
int pb_position[6] = {1,2,3,4,5,6};
        
int Syndrome_P1 (int A[Length_Code], int column_i, int row_number, int e_p1[1]);
int Syndrome_P2 (int A[Length_Code], int column_i, int row_number, int e_p2[2]);
int Syndrome_P3 (int A[Length_Code], int column_i, int row_number, int e_p3[3]);
int Syndrome_P4 (int A[Length_Code], int column_i, int row_number, int e_p4[3]);
        
int* Encoder(int Data_in[Length_D], int ar_out[Length_Code][Length_R], int *pb_position);
int* Decoder(int *code_word, int ar_in[Length_Code], int ar_out[Length_Code][Length_R], int p1_en[Length_Code], int p2_en[Length_Code], int p3_en[Length_Code], int p4_en[Length_Code], int e_p1[1], int e_p2[2], int e_p3[3], int e_p4[3], int *pb_position); 

int *code_word;
int *out_put;
int A[Length_Code] = {0};
int *code_word_e = A;

code_word = Encoder(in_put, ar_out, pb_position);
for(int i=0;i<Length_Code;i++){
  *(code_word_e+i) = ep[i]^*(code_word+i);
}
out_put = Decoder(code_word_e, ar_in, ar_out, p1_en, p2_en, p3_en, p4_en, e_p1, e_p2, e_p3, e_p4, pb_position); 
//Comparison
int c_flg;
for(int i=0;i<Length_D;i++){
  if(in_put[i] != *(out_put+i)) {c_flg=1; break;}
  else c_flg = 0;
}
if (c_flg==0)  p_ok++;
if (*(out_put+Length_D)==1) d_ok++;
} //while{}end Injection_Num
total_ok = total_ok + p_ok;
total_dok = total_dok + d_ok;
cout<<"Correction_Rate/Pattern is: "<<(p_ok/Injection_Num)*100<<"%"<<endl;
cout<<"Detection_Rate/Pattern is: "<<(d_ok/Injection_Num)*100<<"%"<<endl;
} //while{}end error_pattern
float c_r = (total_ok/(Num_pattern*Injection_Num))*100;
float d_r = (total_dok/(Num_pattern*Injection_Num))*100;
cout<<"**************************************"<<endl;
cout<<" --------------------------------------------"<<endl;
cout<<" --------------------------------------------"<<endl;
cout<<"|   Total Pattern is "<<Num_pattern<<"                      |"<<endl;
cout<<"|   Total Correction_Rate is: "<<c_r<<"%"<<"           |"<<endl;
cout<<"|   Total Detection_Rate is: "<<d_r<<"%"<<"            |"<<endl;
cout<<"|   Injection Number/(each pattern) is "<<Injection_Num<<"    |"<<endl;
cout<<" --------------------------------------------"<<endl;

} //while_end() matrix
fout.close();
}
        
int* Encoder (int Data_in[Length_D], int ar_out[Length_Code][Length_R], int *pb_position){
int *code_word = new int[Length_Code];
for(int i=0;i<Length_Code;i++){
  *(code_word+i) = 0;
}
        
int cp = 0;
int P[Length_R] = {0};
//Assign Data Bit
for(int i=0;i<Length_Code;i++){
  int flag_p = 0;
  for(int j=0;j<Length_R;j++){
    int pb_pos_ar = *(pb_position+j)-1;
    if(i==(pb_pos_ar)) {flag_p = 1; break;}
  }
  if (flag_p == 0) {
    *(code_word+i) = Data_in[cp];
    cp = cp+1;
  }
}
//Assign Parity Bit
for(int i=0;i<Length_R;i++){
  int pb_pos_ar = *(pb_position+i)-1;
  for(int j=0;j<Length_R;j++){
     if (ar_out[pb_pos_ar][j]==1){
       for(int k=0;k<Length_Code;k++){
         if(k==pb_pos_ar) continue;
         P[i] = (ar_out[k][j]&(*(code_word+k)))^P[i];
       }
     }
  }
}
//Data+Parity
for(int i=0;i<Length_R;i++){
  *(code_word+*(pb_position+i)-1) = P[i];
}
return code_word;
}
        
int Syndrome_P1 (int A[Length_Code], int column_i, int row_number, int e_p1[1]){
         
int a_1 = A[column_i-0];
     
int A_1[row_number];
         
int A_S[row_number];
int syn = 0;
         
for(int i=0;i<row_number;i++){
  if ((a_1&(1<<i))) A_1[i] = 1;
  else A_1[i] = 0;
}
         
for(int i=0;i<row_number;i++){
  A_S[i] = (e_p1[0] && A_1[i]);
}
for(int i=0;i<row_number;i++){
  syn += pow(2,i)*A_S[i];
}
return syn;
}
        
int Syndrome_P2 (int A[Length_Code], int column_i, int row_number, int e_p2[2]){
         
int a_1 = A[column_i-1];
int a_2 = A[column_i-0];
     
int A_1[row_number];
int A_2[row_number];
         
int A_S[row_number];
int syn = 0;
         
for(int i=0;i<row_number;i++){
  if ((a_1&(1<<i))) A_1[i] = 1;
  else A_1[i] = 0;
}
for(int i=0;i<row_number;i++){
  if ((a_2&(1<<i))) A_2[i] = 1;
  else A_2[i] = 0;
}
         
for(int i=0;i<row_number;i++){
  A_S[i] = (e_p2[0]&&A_1[i])^(e_p2[1]&&A_2[i]);
}
for(int i=0;i<row_number;i++){
  syn += pow(2,i)*A_S[i];
}
return syn;
}
        
int Syndrome_P3 (int A[Length_Code], int column_i, int row_number, int e_p3[3]){
         
int a_1 = A[column_i-2];
int a_2 = A[column_i-1];
int a_3 = A[column_i-0];
     
int A_1[row_number];
int A_2[row_number];
int A_3[row_number];
         
int A_S[row_number];
int syn = 0;
         
for(int i=0;i<row_number;i++){
  if ((a_1&(1<<i))) A_1[i] = 1;
  else A_1[i] = 0;
}
for(int i=0;i<row_number;i++){
  if ((a_2&(1<<i))) A_2[i] = 1;
  else A_2[i] = 0;
}
for(int i=0;i<row_number;i++){
  if ((a_3&(1<<i))) A_3[i] = 1;
  else A_3[i] = 0;
}
         
for(int i=0;i<row_number;i++){
  A_S[i] = (e_p3[0]&&A_1[i])^(e_p3[1]&&A_2[i])^(e_p3[2]&&A_3[i]);
}
for(int i=0;i<row_number;i++){
  syn += pow(2,i)*A_S[i];
}
return syn;
}
        
int Syndrome_P4 (int A[Length_Code], int column_i, int row_number, int e_p4[3]){
         
int a_1 = A[column_i-2];
int a_2 = A[column_i-1];
int a_3 = A[column_i-0];
     
int A_1[row_number];
int A_2[row_number];
int A_3[row_number];
         
int A_S[row_number];
int syn = 0;
         
for(int i=0;i<row_number;i++){
  if ((a_1&(1<<i))) A_1[i] = 1;
  else A_1[i] = 0;
}
for(int i=0;i<row_number;i++){
  if ((a_2&(1<<i))) A_2[i] = 1;
  else A_2[i] = 0;
}
for(int i=0;i<row_number;i++){
  if ((a_3&(1<<i))) A_3[i] = 1;
  else A_3[i] = 0;
}
         
for(int i=0;i<row_number;i++){
  A_S[i] = (e_p4[0]&&A_1[i])^(e_p4[1]&&A_2[i])^(e_p4[2]&&A_3[i]);
}
for(int i=0;i<row_number;i++){
  syn += pow(2,i)*A_S[i];
}
return syn;
}
int* Decoder(int *code_word, int ar_in[Length_Code], int ar_out[Length_Code][Length_R], int p1_en[Length_Code], int p2_en[Length_Code], int p3_en[Length_Code], int p4_en[Length_Code], int e_p1[1], int e_p2[2], int e_p3[3], int e_p4[3], int *pb_position){ 
int *out_put = new int[Length_D+1];
int e[Length_Code] = {0};
int cw_tmp[Length_Code] = {0};
//********************
int cp = 0;
int P[Length_R] = {0};
int data_out[Length_D] = {0};
int data_e[Length_D] = {0};
//Assign Syndrome
int Syn[Length_R] = {0};
for(int i=0;i<Length_R;i++){
   for(int j=0;j<Length_Code;j++){
     Syn[i] = (ar_out[j][i]&(*(code_word+j)))^Syn[i];
   }
}
int syn = 0;
for(int i=0;i<Length_R;i++){
  syn += pow(2,i)*Syn[i];
}
for (int i=0;i<Length_Code;i++){
  if (p1_en[i]==1) {
    int Syn_tmp_1 = Syndrome_P1(ar_in,i,Length_R,e_p1);
    if(syn==Syn_tmp_1) {
      e[i-0]=e_p1[0];
      break;
    }
  }
  else continue;
}
for (int i=0;i<Length_Code;i++){
  if (p2_en[i]==1) {
    int Syn_tmp_2 = Syndrome_P2(ar_in,i,Length_R,e_p2);
    if(syn==Syn_tmp_2) {
      e[i-0]=e_p2[1];
      e[i-1]=e_p2[0];
      break;
    }
  }
  else continue;
}
for (int i=0;i<Length_Code;i++){
  if (p3_en[i]==1) {
    int Syn_tmp_3 = Syndrome_P3(ar_in,i,Length_R,e_p3);
    if(syn==Syn_tmp_3) {
      e[i-0]=e_p3[2];
      e[i-1]=e_p3[1];
      e[i-2]=e_p3[0];
      break;
    }
  }
  else continue;
}
for (int i=0;i<Length_Code;i++){
  if (p4_en[i]==1) {
    int Syn_tmp_4 = Syndrome_P4(ar_in,i,Length_R,e_p4);
    if(syn==Syn_tmp_4) {
      e[i-0]=e_p4[2];
      e[i-1]=e_p4[1];
      e[i-2]=e_p4[0];
      break;
    }
  }
  else continue;
}
//*********************
//Assign error_pattern
for(int i=0;i<Length_Code;i++){
  cw_tmp[i] = *(code_word+i)^e[i];
}
//Assign data_out
for(int i=0;i<Length_Code;i++){
  int flag_p = 0;
  for(int j=0;j<Length_R;j++){
    int pb_pos_ar = *(pb_position+j)-1;
    if(i==(pb_pos_ar)) {flag_p = 1; break;}
  }
  if(flag_p == 0){
    data_out[cp] = cw_tmp[i];
    data_e[cp] = e[i];
    cp = cp+1;
  }
}
//Detection_function
int flg_e=0;
int flg_dec=0;
for(int i=0;i<Length_D;i++){
  flg_e = data_e[i] || flg_e;
}
if((flg_e==0)&&(syn!=0)) flg_dec=1;
//Assign out_put
for(int i=0;i<Length_D;i++){
  *(out_put+i) = data_out[i];
}
*(out_put+Length_D) = flg_dec;
return out_put;
}
