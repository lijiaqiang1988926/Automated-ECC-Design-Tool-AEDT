#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>

#define P_in_length 10      // the size of the error pattern vector
#define Pran_num 5          // the number of range for each error pattern
//#define Stop_time 1*5*60    // hour * miniute * second

using namespace std;

int main(int argc, char *argv[]){

  int Length_D = atoi(argv[1]); // number of data bits
  int Length_R = atoi(argv[2]); // number of parity bits
  int p_num = atoi(argv[3]);    // number of the error pattern correction
  int pd_num = atoi(argv[4]);    // number of the error pattern detection
  int Stop_time = atoi(argv[5]);    // stopping time

  void swap(int *a, int *b);
  int Counter(int a, int row_number);

  ofstream fout;
  ifstream fin;

  fout.open("Fault_Inject.C");
  fin.open("PSet.txt");

  unsigned char tmp[30];
  int cnt=100;

    int Length_Code = Length_D + Length_R;

    int *p_size = new int[p_num];

    int **p = new int*[p_num];  // error_pattern
    for(int i=0;i<p_num;i++) {p[i] = new int[P_in_length];}

    int **p_range = new int*[p_num];   // error pattern range
    for(int i=0;i<p_num;i++) p_range[i] = new int[2*Pran_num];

    int **en_p = new int*[p_num];    // error_pattern enable
    for(int i=0;i<p_num;i++) en_p[i] = new int[Length_D+Length_R];

    for(int i=0;i<p_num;i++){
      for(int j=0;j<Length_D+Length_R;j++){
        *(*(en_p+i)+j) = 0;
      }
    }

    int *parity = new int[Length_R];  // parity_bit value
    for(int i=0;i<Length_R;i++){
      *(parity+i) = 0;
    }

    int *pb_position = new int[Length_R];  //parity_bit position
    for(int i=0;i<Length_R;i++){
      *(pb_position+i) = 0;
    }

    cout<<"********************************************"<<endl;
    cout<<"               Fault_Injection_File_Gen     "<<endl;
    cout<<"********************************************"<<endl;

    while(cnt){
      fin>>tmp;  // read ****************** line
      if (!fin.good()) break;
    // read data
      if(tmp[0]=='*'){
        cout<<"             Read_Data_Successful           "<<endl;
      }
      else{
        cout<<"                 Read_Data_Failure             "<<endl;
        break;
      }
      cout<<"--------------------------------------------"<<endl;
    // start to read Error_Pattern
      for (int i=0;i<p_num;i++){
        for(int j=0;j<P_in_length;j++){
          fin>> *(*(p+i)+j);
        }
      }
      fin>>tmp;  // read ****************** line
    // start to read Error_Pattern_Range
      for (int i=0;i<p_num;i++){
        for(int j=0;j<2*Pran_num;j++){
          fin>> *(*(p_range+i)+j);
        }
      }
      fin>>tmp;  // read ****************** line
    // start to read Parity_Bit_Position
      for (int i=0;i<Length_R;i++){
        fin>> *(pb_position+i);
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
    // 5 groups of the range to the error pattern enable
    for(int i=0;i<p_num;i++){
      for(int j=0;j<Pran_num;j++){
        if(*(*(p_range+i)+2*j) != 0){
          for(int k=(*(*(p_range+i)+2*j)-1 + *(p_size+i)-1); k<*(*(p_range+i)+2*j+1); k++)
            *(*(en_p+i)+k) = 1;
          }
        if(*(*(p_range+i)+2*j) == 0) continue;
       }
      }
    // --------------------------------
    // set the range of the circle ( the element ordered by the weight from low to high)
    int array_size = pow(2,Length_R)-1;
    int *array = new int[array_size];
    for(int i=0;i<array_size;i++){
      array[i]=i+1;
    }
    int key;
    for (int i = 0; i < array_size; i++){
        key=0;
        for (int j = 0; j+1<array_size-i; j++){
            if (Counter(*(array+j),Length_R) > Counter(*(array+j+1),Length_R)){
                key=1;
                swap((array+j), (array+j+1));
            }
        }
        if (key==0) {
            break;
        }
    }
  // --------------------------------
    cout<<">>> Error Pattern Range : "<<endl;
    for(int i=0;i<p_num;i++){
      for(int j=0;j<2*Pran_num;j++){
        cout<< *(*(p_range+i)+j) <<" ";
      }
      cout<<"\n";
    }
    cout<<"********************************************"<<endl;
    cout<<">>> Error Pattern Enable : "<<endl;
    for(int i=0;i<p_num;i++){
      for(int j=0;j<Length_D+Length_R;j++){
        cout<< *(*(en_p+i)+j) <<" ";
      }
      cout<<"\n";
    }

    // definition
    fout<<"#include <iostream>"<<endl;
    fout<<"#include <stdio.h>"<<endl;
    fout<<"#include <fstream>"<<endl;
    fout<<"#include <math.h>"<<endl;
    fout<<"#include <stdlib.h>"<<endl;
    fout<<"#include <time.h>"<<endl;
    fout<<"using namespace std;"<<endl;
    fout<<"        "<<endl;
    fout<<"#define Length_Code "<<Length_Code<<"          //Code length"<<endl;
    fout<<"#define Length_R "<<Length_R<<"              //Redundant bit"<<endl;
    fout<<"#define Length_D "<<Length_D<<"             //Data bit"<<endl;
    fout<<"#define Length_Range "<<pow(2,Length_R)<<"    //Size of array of probability of adding array"<<endl;

    fout<<"        "<<endl;
    fout<<"        "<<endl;
    // main body of the function
    fout<<"int main(int argc, char *argv[]){"<<endl;
    fout<<"        "<<endl;
    fout<<"int Injection_Num = atoi(argv[1]); // Injection Number"<<endl;
    fout<<"        "<<endl;
    fout<<"ifstream fin, fin_b;;"<<endl;
    fout<<"ofstream fout;"<<endl;
    fout<<"fin.open(\"Matrix.txt\");"<<endl;
    fout<<"fin_b.open(\"EP_out.txt\");"<<endl;
    fout<<"        "<<endl;
    fout<<"int ar_in[Length_Code]={0};"<<endl;
    fout<<"int ar_out[Length_Code][Length_R];"<<endl;
    fout<<"\n";
    fout<<"unsigned char tmp["<<30<<"];"<<endl;
    fout<<"int cnt = 100; "<<endl;
    fout<<"int counter = 0;"<<endl;
    fout<<"        "<<endl;

    fout<<"while(cnt){"<<endl;
    fout<<"// Read \"******************\" Row"<<endl;
    fout<<"  fin>>tmp;"<<endl;
    fout<<"// Indentify the end of the document"<<endl;
    fout<<"  if (!fin.good()) break;"<<endl;
    fout<<"//Identify to read a new line"<<endl;
    fout<<"  if(tmp[0]=='*')"<<endl;
    fout<<"  cout<<\"****** Read Matrix Successfully ******\"<<endl;"<<endl;
    fout<<"  else"<<endl;
    fout<<"  {cout<<\"****** Read Failure ******\"<<endl; break;}"<<endl;
    fout<<"//Process Number"<<endl;
    fout<<"counter++;"<<endl;
    fout<<"cout<<\"Process No. \"<<counter<<\" is doing !\"<<endl;"<<endl;
    fout<<"  "<<endl;
    fout<<"//Begining Reading the Matrix"<<endl;
    fout<<"  for (int i=0;i<Length_Code;i++){"<<endl;
    fout<<"    fin>>ar_in[i];"<<endl;
    fout<<"  }"<<endl;
    fout<<"//Decimal to Binary Conversion"<<endl;
    fout<<"  for(int j=0; j<Length_Code; j++){"<<endl;
    fout<<"    for(int i=0;i<Length_R;i++){"<<endl;
    fout<<"      if ((ar_in[j]&(1<<i))) ar_out[j][i] = 1;"<<endl;
    fout<<"      else ar_out[j][i] = 0;"<<endl;
    fout<<"    }"<<endl;
    fout<<"  }"<<endl;
    fout<<"//Output the Matrix"<<endl;
    fout<<"  cout<<\"******** Matrix **********\"<<endl;"<<endl;
    fout<<"  for(int k=Length_R-1;k>=0;k--){         //先输出每一列的第一个元素"<<endl;
    fout<<"    for (int i=0;i<Length_Code;i++){"<<endl;
    fout<<"      cout<<ar_out[i][k]<<\" \";"<<endl;
    fout<<"    }"<<endl;
    fout<<"      cout<<\"\\n\";"<<endl;
    fout<<"  }"<<endl;
    fout<<"  cout<<\"*************************\"<<endl;"<<endl;
    fout<<"//---------------------------------------------------------------"<<endl;
    fout<<"int counter_pattern = 0;"<<endl;
    fout<<"int ep[Length_Code] = {0};"<<endl;
    fout<<"float total_ok = 0;"<<endl;
    fout<<"float total_dok = 0;"<<endl;
    fout<<"int cnt_pattern = 1;"<<endl;
    fout<<"int Num_pattern = 0;"<<endl;
    fout<<"        "<<endl;
    fout<<"while(cnt_pattern){"<<endl;
    fout<<"  fin_b>>tmp;  // read ****************** line"<<endl;
    fout<<"  if (!fin_b.good()) break;"<<endl;
    fout<<"// read data"<<endl;
    fout<<"  if(tmp[0]==\'*\')"<<endl;
    fout<<"  cout<<\"-------------Read_Error_Pattern_Successful----------\"<<endl;"<<endl;
    fout<<"  else"<<endl;
    fout<<"  {cout<<\"------------Read_Error_Pattern_Failure-------------\"<<endl; break;}"<<endl;
    fout<<"  cout<<\"********************************************\"<<endl;"<<endl;
    fout<<"//Process Number"<<endl;
    fout<<"  counter_pattern = counter_pattern + 1;"<<endl;
    fout<<"  cout<<\"Error_Pattern Process No. \"<<counter_pattern<<\" is doing !\"<<endl;"<<endl;
    fout<<"// start to read Error_Pattern"<<endl;
    fout<<"  for(int i=0;i<Length_Code;i++){"<<endl;
    fout<<"    fin_b>> ep[i];"<<endl;
    fout<<"  }"<<endl;
    fout<<"  Num_pattern++;"<<endl;
    fout<<"//---------------------------------------------------------------"<<endl;
    fout<<"int counter_injection = 0;"<<endl;
    fout<<"int p_ok = 0; // ok event for one pattern"<<endl;
    fout<<"int d_ok = 0; // ok for dectection"<<endl;
    fout<<"while(counter_injection<Injection_Num){"<<endl;
    fout<<"counter_injection = counter_injection + 1;"<<endl;
    fout<<"//---------------------------------------------------------------"<<endl;
    fout<<"int input_deci;"<<endl;
    fout<<"int in_put[Length_D]={0};"<<endl;
    fout<<"srand((unsigned)time(NULL));"<<endl;
    fout<<"input_deci = rand() % Length_Range;"<<endl;
    fout<<"//Input Decimal to Binary Conversion"<<endl;
    fout<<"for(int i=0; i<Length_D; i++){"<<endl;
    fout<<"  if (input_deci&(1<<i)) in_put[i] = 1;"<<endl;
    fout<<"  else in_put[i] = 0;"<<endl;
    fout<<"}"<<endl;
    fout<<"\n";
    for(int i=0;i<p_num;i++){
      fout<<"int e_p"<<(i+1)<<"["<<*(p_size+i)<<"] = {";
      for(int j=0;j<*(p_size+i)-1;j++){
        fout<< *(*(p+i)+j) <<",";
      }
      fout<<*(*(p+i) + *(p_size+i)-1)<<"};"<<endl;
    }
    fout<<"        "<<endl;
    for(int i=0;i<p_num;i++){
      fout<<"int p"<<(i+1)<<"_en["<<Length_Code<<"] = {";
      for(int j=0;j<Length_D+Length_R-1;j++){
        fout<<*(*(en_p+i)+j)<<",";
      }
      fout<<*(*(en_p+i)+Length_D+Length_R-2)<<"};";
      fout<<"\n";
    }
    fout<<"        "<<endl;
    fout<<"int pb_position["<<Length_R<<"] = {";
    for (int i=0;i<Length_R-1;i++){
      fout<<*(pb_position+i)<<",";
    }
    fout<<*(pb_position+Length_R-1)<<"};"<<endl;
    fout<<"        "<<endl;
    for(int l=0;l<p_num;l++){
      fout<<"int Syndrome_P"<<(l+1)<<" (int A[Length_Code], int column_i, int row_number, int e_p"<<(l+1)<<"["<<*(p_size+l)<<"]);"<<endl;
    }
    fout<<"        "<<endl;
    fout<<"int* Encoder(int Data_in[Length_D], int ar_out[Length_Code][Length_R], int *pb_position);"<<endl;
    fout<<"int* Decoder(int *code_word, int ar_in[Length_Code], int ar_out[Length_Code][Length_R],";
    for(int l=0;l<p_num;l++){
      fout<<" int p"<<(l+1)<<"_en[Length_Code],";
    }
    for(int l=0;l<p_num;l++){
      fout<<" int e_p"<<(l+1)<<"["<<*(p_size+l)<<"],";
    }
    fout<<" int *pb_position); "<<endl;
    fout<<"\n";
    fout<<"int *code_word;"<<endl;
    fout<<"int *out_put;"<<endl;
    fout<<"int A[Length_Code] = {0};"<<endl;
    fout<<"int *code_word_e = A;"<<endl;
    fout<<"\n";
    fout<<"code_word = Encoder(in_put, ar_out, pb_position);"<<endl;
    fout<<"for(int i=0;i<Length_Code;i++){"<<endl;
    fout<<"  *(code_word_e+i) = ep[i]^*(code_word+i);"<<endl;
    fout<<"}"<<endl;
    fout<<"out_put = Decoder(code_word_e, ar_in, ar_out,";
    for(int l=0;l<p_num;l++){
      fout<<" p"<<(l+1)<<"_en,";
    }
    for(int l=0;l<p_num;l++){
      fout<<" e_p"<<(l+1)<<",";
    }
    fout<<" pb_position); "<<endl;
    fout<<"//Comparison"<<endl;
    fout<<"int c_flg;"<<endl;
    fout<<"for(int i=0;i<Length_D;i++){"<<endl;
    fout<<"  if(in_put[i] != *(out_put+i)) {c_flg=1; break;}"<<endl;
    fout<<"  else c_flg = 0;"<<endl;
    fout<<"}"<<endl;
    fout<<"if (c_flg==0)  p_ok++;"<<endl;
    fout<<"if (*(out_put+Length_D)==1) d_ok++;"<<endl;
    fout<<"} //while{}end Injection_Num"<<endl;
    fout<<"total_ok = total_ok + p_ok;"<<endl;
    fout<<"total_dok = total_dok + d_ok;"<<endl;
    fout<<"cout<<\"Correction_Rate/Pattern is: \"<<(p_ok/Injection_Num)*100<<\"\%\"<<endl;"<<endl;
    fout<<"cout<<\"Detection_Rate/Pattern is: \"<<(d_ok/Injection_Num)*100<<\"%\"<<endl;"<<endl;
    fout<<"} //while{}end error_pattern"<<endl;
    fout<<"float c_r = (total_ok/(Num_pattern*Injection_Num))*100;"<<endl;
    fout<<"float d_r = (total_dok/(Num_pattern*Injection_Num))*100;"<<endl;
    fout<<"cout<<\"**************************************\"<<endl;"<<endl;
    fout<<"cout<<\" --------------------------------------------\"<<endl;"<<endl;
    fout<<"cout<<\" --------------------------------------------\"<<endl;"<<endl;
    fout<<"cout<<\"|   Total Pattern is \"<<Num_pattern<<\"                      |\"<<endl;"<<endl;
    fout<<"cout<<\"|   Total Correction_Rate is: \"<<c_r<<\"%\"<<\"           |\"<<endl;"<<endl;
    fout<<"cout<<\"|   Total Detection_Rate is: \"<<d_r<<\"%\"<<\"            |\"<<endl;"<<endl;
    fout<<"cout<<\"|   Injection Number/(each pattern) is \"<<Injection_Num<<\"    |\"<<endl;"<<endl;
    fout<<"cout<<\" --------------------------------------------\"<<endl;"<<endl;
    fout<<"\n";
    fout<<"} //while_end() matrix"<<endl;
    fout<<"fout.close();"<<endl;
    fout<<"}"<<endl; //main {}
    fout<<"        "<<endl;

    //************************
    fout<<"int* Encoder (int Data_in[Length_D], int ar_out[Length_Code][Length_R], int *pb_position){"<<endl;

    fout<<"int *code_word = new int[Length_Code];"<<endl;
    fout<<"for(int i=0;i<Length_Code;i++){"<<endl;
    fout<<"  *(code_word+i) = 0;"<<endl;
    fout<<"}"<<endl;
    fout<<"        "<<endl;
    fout<<"int cp = 0;"<<endl;
    fout<<"int P[Length_R] = {0};"<<endl;
    fout<<"//Assign Data Bit"<<endl;
    fout<<"for(int i=0;i<Length_Code;i++){"<<endl;
    fout<<"  int flag_p = 0;"<<endl;
    fout<<"  for(int j=0;j<Length_R;j++){"<<endl;
    fout<<"    int pb_pos_ar = *(pb_position+j)-1;"<<endl;
    fout<<"    if(i==(pb_pos_ar)) {flag_p = 1; break;}"<<endl;
    fout<<"  }"<<endl;
    fout<<"  if (flag_p == 0) {"<<endl;
    fout<<"    *(code_word+i) = Data_in[cp];"<<endl;
    fout<<"    cp = cp+1;"<<endl;
    fout<<"  }"<<endl;
    fout<<"}"<<endl;
    fout<<"//Assign Parity Bit"<<endl;
    fout<<"for(int i=0;i<Length_R;i++){"<<endl;
    fout<<"  int pb_pos_ar = *(pb_position+i)-1;"<<endl;
    fout<<"  for(int j=0;j<Length_R;j++){"<<endl;
    fout<<"     if (ar_out[pb_pos_ar][j]==1){"<<endl;
    fout<<"       for(int k=0;k<Length_Code;k++){"<<endl;
    fout<<"         if(k==pb_pos_ar) continue;"<<endl;
    fout<<"         P[i] = (ar_out[k][j]&(*(code_word+k)))^P[i];"<<endl;
    fout<<"       }"<<endl;
    fout<<"     }"<<endl;
    fout<<"  }"<<endl;
    fout<<"}"<<endl;
    fout<<"//Data+Parity"<<endl;
    fout<<"for(int i=0;i<Length_R;i++){"<<endl;
    fout<<"  *(code_word+*(pb_position+i)-1) = P[i];"<<endl;
    fout<<"}"<<endl;
    fout<<"return code_word;"<<endl;
    fout<<"}"<<endl;

    // Syndrome_Calculation Function
    for(int l=0;l<p_num;l++){
      fout<<"        "<<endl;
      fout<<"int Syndrome_P"<<(l+1)<<" (int A[Length_Code], int column_i, int row_number, int e_p"<<(l+1)<<"["<<*(p_size+l)<<"]){"<<endl;
      fout<<"         "<<endl;
      for(int i=0;i<*(p_size+l);i++){
        fout<<"int a_"<<(i+1)<<" = A[column_i-"<<(*(p_size+l)-1-i)<<"];"<<endl;
      }
      fout<<"     "<<endl;
      for(int i=0;i<*(p_size+l);i++){
        fout<<"int A_"<<(i+1)<<"[row_number];"<<endl;
      }
      fout<<"         "<<endl;
      fout<<"int A_S[row_number];"<<endl;
      fout<<"int syn = 0;"<<endl;
      fout<<"         "<<endl;
      for(int i=0;i<*(p_size+l);i++){
        fout<<"for(int i=0;i<row_number;i++){"<<endl;
        fout<<"  if ((a_"<<(i+1)<<"&(1<<i))) A_"<<(i+1)<<"[i] = 1;"<<endl;
        fout<<"  else A_"<<(i+1)<<"[i] = 0;"<<endl;
        fout<<"}"<<endl;
      }
      fout<<"         "<<endl;
      fout<<"for(int i=0;i<row_number;i++){"<<endl;
      fout<<"  A_S[i] = ";
      if(*(p_size+l) == 1) fout<<"(e_p"<<(l+1)<<"[0] && A_"<<*(p_size+l)<<"[i]);"<<endl;
      if(*(p_size+l) > 1){
        fout<<"(e_p"<<(l+1)<<"[0]&&A_"<<1<<"[i])";
        for(int i=1;i<*(p_size+l);i++){
            fout<<"^(e_p"<<(l+1)<<"["<<i<<"]&&A_"<<(i+1)<<"[i])";
        }
      fout<<";"<<endl;
      }
      fout<<"}"<<endl;

      fout<<"for(int i=0;i<row_number;i++){"<<endl;
      fout<<"  syn += pow(2,i)*A_S[i];"<<endl;
      fout<<"}"<<endl;
      fout<<"return syn;"<<endl;
      fout<<"}"<<endl;
    }

    fout<<"int* Decoder(int *code_word, int ar_in[Length_Code], int ar_out[Length_Code][Length_R],";
    for(int l=0;l<p_num;l++){
      fout<<" int p"<<(l+1)<<"_en[Length_Code],";
    }
    for(int l=0;l<p_num;l++){
      fout<<" int e_p"<<(l+1)<<"["<<*(p_size+l)<<"],";
    }
    fout<<" int *pb_position){ "<<endl;

      fout<<"int *out_put = new int[Length_D+1];"<<endl;
      fout<<"int e[Length_Code] = {0};"<<endl;
      fout<<"int cw_tmp[Length_Code] = {0};"<<endl;
      fout<<"//********************"<<endl;
      fout<<"int cp = 0;"<<endl;
      fout<<"int P[Length_R] = {0};"<<endl;
      fout<<"int data_out[Length_D] = {0};"<<endl;
      fout<<"int data_e[Length_D] = {0};"<<endl;
      fout<<"//Assign Syndrome"<<endl;
      fout<<"int Syn[Length_R] = {0};"<<endl;
      fout<<"for(int i=0;i<Length_R;i++){"<<endl;
      fout<<"   for(int j=0;j<Length_Code;j++){"<<endl;
      fout<<"     Syn[i] = (ar_out[j][i]&(*(code_word+j)))^Syn[i];"<<endl;
      fout<<"   }"<<endl;
      fout<<"}"<<endl;
      fout<<"int syn = 0;"<<endl;
      fout<<"for(int i=0;i<Length_R;i++){"<<endl;
      fout<<"  syn += pow(2,i)*Syn[i];"<<endl;
      fout<<"}"<<endl;
      //标准校正子比对
      for(int l=0;l<p_num;l++){
      fout<<"for (int i=0;i<Length_Code;i++){"<<endl;
      fout<<"  if (p"<<(l+1)<<"_en[i]==1) {"<<endl;
      fout<<"    int Syn_tmp_"<<(l+1)<<" = Syndrome_P"<<(l+1)<<"(ar_in,i,Length_R,e_p"<<(l+1)<<");"<<endl;
      fout<<"    if(syn==Syn_tmp_"<<(l+1)<<") {"<<endl;
      for(int j=0;j<*(p_size+l);j++){
      fout<<"      e[i-"<<j<<"]=e_p"<<(l+1)<<"["<<(*(p_size+l)-1-j)<<"];"<<endl;
      }
      fout<<"      break;"<<endl;
      fout<<"    }"<<endl;
      fout<<"  }"<<endl;
      fout<<"  else continue;"<<endl;
      fout<<"}"<<endl;
      }
      fout<<"//*********************"<<endl;

      fout<<"//Assign error_pattern"<<endl;
      fout<<"for(int i=0;i<Length_Code;i++){"<<endl;
      fout<<"  cw_tmp[i] = *(code_word+i)^e[i];"<<endl;
      fout<<"}"<<endl;
      fout<<"//Assign data_out"<<endl;
      fout<<"for(int i=0;i<Length_Code;i++){"<<endl;
      fout<<"  int flag_p = 0;"<<endl;
      fout<<"  for(int j=0;j<Length_R;j++){"<<endl;
      fout<<"    int pb_pos_ar = *(pb_position+j)-1;"<<endl;
      fout<<"    if(i==(pb_pos_ar)) {flag_p = 1; break;}"<<endl;
      fout<<"  }"<<endl;
      fout<<"  if(flag_p == 0){"<<endl;
      fout<<"    data_out[cp] = cw_tmp[i];"<<endl;
      fout<<"    data_e[cp] = e[i];"<<endl;
      fout<<"    cp = cp+1;"<<endl;
      fout<<"  }"<<endl;
      fout<<"}"<<endl;
      fout<<"//Detection_function"<<endl;
      fout<<"int flg_e=0;"<<endl;
      fout<<"int flg_dec=0;"<<endl;
      fout<<"for(int i=0;i<Length_D;i++){"<<endl;
      fout<<"  flg_e = data_e[i] || flg_e;"<<endl;
      fout<<"}"<<endl;
      fout<<"if((flg_e==0)&&(syn!=0)) flg_dec=1;"<<endl;
      fout<<"//Assign out_put"<<endl;
      fout<<"for(int i=0;i<Length_D;i++){"<<endl;
      fout<<"  *(out_put+i) = data_out[i];"<<endl;
      fout<<"}"<<endl;
      fout<<"*(out_put+Length_D) = flg_dec;"<<endl;
      fout<<"return out_put;"<<endl;
      fout<<"}"<<endl;


    // delete the dynamic array
    delete[] p_size;
    delete[] parity;
    delete[] pb_position;

    for(int i=0;i<p_num;i++){delete[] p[i];}
    delete[] p;

    for(int i=0;i<p_num;i++){delete[] p_range[i];}
    delete[] p_range;

    for(int i=0;i<p_num;i++){delete[] en_p[i];}
    delete[] en_p;

    cout<<"***--------------------------------------------***"<<endl;
    cout<<"*** Fault_Injection_File_Generation Successful ***"<<endl;
    cout<<"***--------------------------------------------***"<<endl;
    cout<<"                                            "<<endl;



fout.close();
}
// other function
//---------------------
int Counter(int a, int row_number){
  int A[row_number];
  for(int i=0;i<row_number;i++){
    if ((a&(1<<i))) A[i] = 1;
    else A[i] = 0;
  }
  int counter = 0;
  for(int i=0;i<row_number;i++){
      if(A[i]==1) counter++;
      else continue;
  }
  return counter;
}

void swap(int *a, int *b){
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
