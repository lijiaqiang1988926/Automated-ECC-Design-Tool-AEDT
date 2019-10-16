#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <stdlib.h>

#define P_in_length 10      // the size of the error pattern vector
#define Pran_num 5          // the number of range for each error pattern
//#define Stop_time 1*5*60    // hour * miniute * second

using namespace std;

int main(int argc, char *argv[]){

  int p_num = atoi(argv[1]);    // number of the error pattern correction
  int pd_num = atoi(argv[2]);    // number of the error pattern detection
  float Stop_time = atof(argv[3]);    // stopping time
  int Mode = atoi(argv[4]);    // stopping time 0 is correction ; 1 is correction+detection

  void swap(int *a, int *b);
  int Counter(int a, int row_number);

  ofstream fout;
  ifstream fin,fin_1;

  fout.open("ECC_Search.C");
  fin.open("Set.txt");
  fin_1.open("Column_Weight.txt");

  unsigned char tmp[30];
  int cnt=100;

  if(Mode == 0){

    int Length_D = 0; // number of data bits
    int Length_R = 0; // number of parity bits

    int *p_size = new int[p_num]; // error_pattern correction size
    for(int i=0;i<p_num;i++){
      *(p_size+i) = 0;
    }

    int **p = new int*[p_num];  // error_pattern
    for(int i=0;i<p_num;i++) {p[i] = new int[P_in_length];}

    int **p_range = new int*[p_num];   // error pattern range
    for(int i=0;i<p_num;i++) p_range[i] = new int[2*Pran_num];

    // read Data Bit and Parity Bit
      fin>>tmp;  // read ****************** line
      fin>>Length_D;
      fin>>tmp;  // read ****************** line
      fin>>Length_R;
    // definition
    int Length_Code = Length_D + Length_R;

    int *Column_Weight_upper = new int[Length_Code]; //Column_Weight_upper
    for(int i=0;i<Length_Code;i++){
      *(Column_Weight_upper+i) = 0;
    }
    int *Column_Weight_bottom = new int[Length_Code]; //Column_Weight_bottom
    for(int i=0;i<Length_Code;i++){
      *(Column_Weight_bottom+i) = 0;
    }

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
    // start to read Error_Pattern
    fin>>tmp;  // read ****************** line
      for (int i=0;i<p_num;i++){
        for(int j=0;j<P_in_length;j++){
          fin>> *(*(p+i)+j);
        }
      }
    // start to read Error_Pattern_Range
    fin>>tmp;  // read ****************** line
      for (int i=0;i<p_num;i++){
        for(int j=0;j<2*Pran_num;j++){
          fin>> *(*(p_range+i)+j);
        }
      }
    // start to read Parity_Bit_Position
    fin>>tmp;  // read ****************** line
      for (int i=0;i<Length_R;i++){
        fin>> *(pb_position+i);
      }


    while(cnt){
      fin_1>>tmp;  // read ****************** line
      if (!fin_1.good()) break;
      // read column_weight upper limit
      for (int i=0;i<Length_Code;i++){
        fin_1>> *(Column_Weight_upper+i);
      }
      fin_1>>tmp;  // read ****************** line
      // read column_weight bottom limit
      for (int i=0;i<Length_Code;i++){
        fin_1>> *(Column_Weight_bottom+i);
      }
    }
  //---------------------------------
  cout<<"                                            "<<endl;
  cout<<"********************************************"<<endl;
  cout<<"***** Welcom to use the Auto_ECC Tool ******"<<endl;
  cout<<"********************************************"<<endl;
  cout<<">>> Data Bit is "<<Length_D<<endl;
  cout<<"********************************************"<<endl;
  cout<<">>> Parity Bit is "<<Length_R<<endl;
  cout<<"********************************************"<<endl;
  cout<<">>> Error Pattern Number is "<<p_num<<endl;
  cout<<"********************************************"<<endl;
  cout<<">>> Stop Time is "<<(Stop_time/60)<<" min "<<endl;
  cout<<"********************************************"<<endl;
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
    cout<<"********************************************"<<endl;
    cout<<">>> Parity Bit : ";
    for(int i=0;i<Length_R;i++){
      *(parity+i) = pow(2,(Length_R-1-i));
      cout<< *(parity+i) <<" ";
    }
    cout<<"\n";
    cout<<"********************************************"<<endl;
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
    cout<<"********************************************"<<endl;
    cout<<">>> Parity Bit Position : ";
    for (int i=0;i<Length_R;i++){
      cout<< *(pb_position+i) <<" ";
    }
    cout<<"\n";
    cout<<"********************************************"<<endl;

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
    fout<<"#define Stop_time "<<Stop_time<<endl;
    fout<<"        "<<endl;
    fout<<"        "<<endl;
    // main body of the function
    fout<<"int main(){"<<endl;
    fout<<"        "<<endl;
    fout<<"ofstream fout;"<<endl;
    fout<<"fout.open(\"1.txt\");"<<endl;

    fout<<"        "<<endl;
    fout<<"clock_t start, finish;"<<endl;
    fout<<"        "<<endl;
    fout<<"int HR_Temp=1000; "<<endl;
    fout<<"int TS_Temp=1000; "<<endl;
    fout<<"        "<<endl;
    fout<<"int col[Length_Code]={0};"<<endl;
    fout<<"        "<<endl;
    fout<<"int Correction_Set["<<Length_Code<<"]["<<p_num<<"]={0};"<<endl;
    fout<<"        "<<endl;
    fout<<"int identity["<<Length_Code<<"]={0};"<<endl;
    fout<<"        "<<endl;
    fout<<"int array[Length_Range-1] = {"<<endl;
    for (int i = 0; i < array_size-1; i++){
        fout<<array[i]<<",";
    }
    fout<<array[array_size-1]<<endl;
    fout<<"};"<<endl;
    fout<<"        "<<endl;
    for(int i=0;i<p_num;i++){
      fout<<"int e_p"<<(i+1)<<"["<<*(p_size+i)<<"] = {";
      for(int j=0;j<*(p_size+i)-1;j++){
        fout<< *(*(p+i)+j) <<",";
      }
      fout<<*(*(p+i) + *(p_size+i)-1)<<"};"<<endl;
    }
    fout<<"        "<<endl;
    for(int i=0;i<p_num;i++){
      fout<<"int p"<<(i+1)<<"["<<Length_Code<<"] = {0};"<<endl;
    }
    fout<<"        "<<endl;

    for(int l=0;l<p_num;l++){
      fout<<"int Function_P"<<(l+1)<<" (int A[Length_Code], int column_i, int row_number, int Correction_Set[]["<<p_num<<"], int e_p"<<(l+1)<<"["<<*(p_size+l)<<"]);"<<endl;
    }
    fout<<"        "<<endl;
    fout<<"int Heavy_Row_inline (int col[Length_Code]); "<<endl;
    fout<<"int Total_One_inline (int col[Length_Code]); "<<endl;
    fout<<"int Counter (int a, int row_number);"<<endl;
    fout<<"        "<<endl;
    fout<<"start = clock();"<<endl;
    fout<<"        "<<endl;


    // Search part of the Generation function
    int pb_num = 0;

    for(int i=0;i<(Length_D+Length_R);i++){
      int flag_dp = 0;
      for (int js=0;js<Length_R;js++){
        if (*(pb_position+js) == i+1) {flag_dp = 1; break;}
        else flag_dp = 0;
      }
      // cout<<"flag_dp is "<<flag_dp<<endl;
      if(flag_dp==0){
        fout<<"// Level_"<<i+1<<"[D]"<<endl;

        fout<<"for (int i"<<i<<" = 0; i"<<i<<" < Length_Range-1; i"<<i<<"++){"<<"\n";
        fout<<"if(Counter(array[i"<<i<<"],Length_R) > "<<*(Column_Weight_upper+i)<<") continue;"<<endl;
        fout<<"if(Counter(array[i"<<i<<"],Length_R) < "<<*(Column_Weight_bottom+i)<<") continue;"<<endl;
        fout<<"col["<<i<<"] = array[i"<<i<<"];"<<"\n";

      //******************
        for(int j=0;j<p_num;j++){
          if(*(*(en_p+j)+i)==1){
            fout<<"p"<<(j+1)<<"["<<i<<"] = Function_P"<<(j+1)<<"(col, "<<i<<", Length_R, Correction_Set, e_p"<<(j+1)<<");"<<endl;
          }
        }
      //******************
        fout<<"identity["<<i<<"] = ";
        int cnt_temp = 0;

        int *p_flag = new int[p_num];
        for(int i=0;i<p_num;i++){
          *(p_flag+i) = 0;
        }

        for(int j=0;j<p_num;j++){
          cnt_temp = cnt_temp + *(*(en_p+j)+i);
          if (*(*(en_p+j)+i)==1) *(p_flag+j) = cnt_temp;
        }
        if (cnt_temp == 0) fout<<"1;"<<"\n";
        if (cnt_temp == 1) {
          for(int j=0;j<p_num;j++){
            if (*(p_flag+j)==1) fout<<"p"<<(j+1)<<"["<<i<<"]";
          }
          fout<<";"<<"\n";
        }
        if (cnt_temp > 1){
          for(int j=0;j<p_num;j++){
            if (*(p_flag+j)==1) fout<<"p"<<(j+1)<<"["<<i<<"]";
            if (*(p_flag+j)>1) fout<<" && p"<<(j+1)<<"["<<i<<"]";
          }
          fout<<";"<<"\n";
        }
        fout<<"if(!identity["<<i<<"]) continue;"<<"\n";

        fout<<"else {"<<endl;
        fout<<"clock_t finish_"<<i<<" = clock();"<<endl;
        fout<<"double duration_"<<i<<" = (double)(finish_"<<i<<" - start) / CLOCKS_PER_SEC;"<<endl;
        fout<<"if(duration_"<<i<<" > Stop_time) return 0;"<<endl;
        // delete the dynamic array
        delete[] p_flag;
      }

      if(flag_dp==1){
        fout<<"// Level_"<<i+1<<"[P]"<<endl;

        fout<<"for (int i"<<i<<"="<<*(parity+pb_num)<<";i"<<i<<"<"<<(*(parity+pb_num)+1)<<";i"<<i<<"++){"<<"\n";
        fout<<"if(Counter(i"<<i<<",Length_R) > "<<*(Column_Weight_upper+i)<<") continue;"<<endl;
        fout<<"if(Counter(i"<<i<<",Length_R) < "<<*(Column_Weight_bottom+i)<<") continue;"<<endl;

        pb_num++;

        fout<<"col["<<i<<"]=i"<<i<<";"<<"\n";

        //******************
        for(int l=0;l<p_num;l++){
          if(*(*(en_p+l)+i)==1){
            fout<<"p"<<(l+1)<<"["<<i<<"] = Function_P"<<(l+1)<<"(col,"<<i<<",Length_R,Correction_Set,e_p"<<(l+1)<<");"<<endl;
          }
        }
        //******************
        fout<<"identity["<<i<<"] = ";
        int cnt_temp = 0;

        int *p_flag = new int[p_num];
        for(int j=0;j<p_num;j++){
          *(p_flag+j) = 0;
        }

        for(int j=0;j<p_num;j++){
          cnt_temp = cnt_temp+ *(*(en_p+j)+i);
          if (*(*(en_p+j)+i)==1) *(p_flag+j) = cnt_temp;
        }

        if (cnt_temp == 0) fout<<"1;"<<"\n";
        if (cnt_temp == 1) {
          for(int j=0;j<p_num;j++){
            if (*(p_flag+j)==1) fout<<"p"<<(j+1)<<"["<<i<<"]";
          }
          fout<<";"<<"\n";
        }
        if (cnt_temp > 1){
          for(int j=0;j<p_num;j++){
            if (*(p_flag+j)==1) fout<<"p"<<(j+1)<<"["<<i<<"]";
            if (*(p_flag+j)>1) fout<<" && p"<<(j+1)<<"["<<i<<"]";
          }
          fout<<";"<<"\n";
        }
        //******************

        fout<<"if(!identity["<<i<<"]) continue;"<<"\n";

        fout<<"else {"<<endl;
        fout<<"clock_t finish_"<<i<<" = clock();"<<endl;
        fout<<"double duration_"<<i<<" = (double)(finish_"<<i<<" - start) / CLOCKS_PER_SEC;"<<endl;
        fout<<"if(duration_"<<i<<" > Stop_time) return 0;"<<endl;

        // delete the dynamic array
        delete[] p_flag;
      }
    }
    fout<<"     "<<endl;
    fout<<"int HR = 0;"<<endl;
    fout<<"HR = Heavy_Row_inline(col);"<<endl;
    fout<<"if(HR < HR_Temp){"<<endl;
    fout<<"HR_Temp = HR;"<<endl;
    fout<<"cout<<\"Smallest HR is : \"<<HR<<endl;"<<endl;
    fout<<"cout<<\"Find it !\"<<endl;"<<endl;
    fout<<"// Output the Target Array"<<endl;
    fout<<"fout<<\"******************\"<<endl;"<<endl;
    fout<<"  for (int i=0;i<Length_Code;i++){"<<endl;
    fout<<"    fout<<col[i]<<\" \";"<<endl;
    fout<<"  }"<<endl;
    fout<<"  fout<<\"\\n\";"<<endl;
    fout<<"}"<<endl;

    fout<<"     "<<endl;
    fout<<"int TS = 0;"<<endl;
    fout<<"TS = Total_One_inline(col);"<<endl;
    fout<<"if(TS < TS_Temp){"<<endl;
    fout<<"TS_Temp = TS;"<<endl;
    fout<<"cout<<\"Smallest TS is : \"<<TS<<endl;"<<endl;
    fout<<"cout<<\"Find it !\"<<endl;"<<endl;
    fout<<"// Output the Target Array"<<endl;
    fout<<"fout<<\"******************\"<<endl;"<<endl;
    fout<<"  for (int i=0;i<Length_Code;i++){"<<endl;
    fout<<"    fout<<col[i]<<\" \";"<<endl;
    fout<<"  }"<<endl;
    fout<<"  fout<<\"\\n\";"<<endl;
    fout<<"}"<<endl;

    fout<<"     "<<endl;
    fout<<"     "<<endl;
    fout<<"     "<<endl;
    //************************
    for(int i=0;i<(Length_D+Length_R);i++){
      fout<<" } "<<endl;
      fout<<"  } "<<endl;
    }
    fout<<"fout.close();"<<endl;
    fout<<"}"<<endl; //main {}
    //************************
    // Error_Pattern Indentify Function
    for(int l=0;l<p_num;l++){
      fout<<"int Function_P"<<(l+1)<<" (int A[Length_Code], int column_i, int row_number, int Correction_Set[]["<<p_num<<"], int e_p"<<(l+1)<<"["<<*(p_size+l)<<"]){"<<endl;
      fout<<"         "<<endl;
      fout<<"int Correction;"<<endl;
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
      fout<<"int c = 0;"<<endl;
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
      fout<<"  c += pow(2,i)*A_S[i];"<<endl;
      fout<<"}"<<endl;
      fout<<"Correction = c;"<<endl;

      fout<<"       "<<endl;
      fout<<"//!=0"<<endl;
      fout<<" if(Correction == 0) return 0;"<<endl;
      fout<<"       "<<endl;
      if(l>0){
        fout<<"//!= previous P in column_i"<<endl;
        fout<<"for(int i=0;i<"<<l<<";i++){"<<endl;
        fout<<" if(Correction == Correction_Set[column_i][i]) return 0;"<<endl;
        fout<<"}"<<endl;
        fout<<"       "<<endl;
      }
      fout<<"//Correction是否在Correction_Set集合内"<<endl;
      fout<<"  for (int j=0;j<column_i;j++){"<<endl;
      fout<<"    for (int k=0;k<"<<p_num<<";k++){"<<endl;
      fout<<"      if(Correction == Correction_Set[j][k])"<<endl;
      fout<<"      return 0;"<<endl;
      fout<<"    }"<<endl;
      fout<<"  }"<<endl;

      fout<<"Correction_Set[column_i]["<<l<<"] = Correction;"<<endl;

      fout<<"  return 1;"<<endl;
      fout<<"}"<<endl;
    }
    //Total_One_inline
    fout<<"   "<<endl;
    fout<<"int Total_One(int A[Length_Code][Length_R], int column_number, int row_number){"<<endl;
    fout<<"  int counter = 0;"<<endl;
    fout<<"  for(int i=0;i<Length_R;i++){"<<endl;
    fout<<"    for (int k=0;k<Length_Code;k++){"<<endl;
    fout<<"      if(A[k][i]==1) counter++;"<<endl;
    fout<<"      else continue;"<<endl;
    fout<<"    }"<<endl;
    fout<<"  }"<<endl;
    fout<<"  return counter;"<<endl;
    fout<<"}"<<endl;
    fout<<"   "<<endl;
    fout<<"int Total_One_inline (int col[Length_Code]){"<<endl;
    fout<<"  int ar_in[Length_Code];"<<endl;
    fout<<"  int ar_out[Length_Code][Length_R];"<<endl;
    fout<<"    "<<endl;
    fout<<"  for(int i=0;i<Length_Code;i++){"<<endl;
    fout<<"    ar_in[i] = col[i];"<<endl;
    fout<<"  }"<<endl;
    fout<<"    "<<endl;
    fout<<"//10进制格式转换到2进制格式"<<endl;
    fout<<"  for(int j=0; j<Length_Code; j++){"<<endl;
    fout<<"    for(int i=0;i<Length_R;i++){"<<endl;
    fout<<"      if ((ar_in[j]&(1<<i))) ar_out[j][i] = 1;"<<endl;
    fout<<"      else ar_out[j][i] = 0;"<<endl;
    fout<<"    }"<<endl;
    fout<<"  }"<<endl;
    fout<<"      "<<endl;
    fout<<"  int total_one_num;"<<endl;
    fout<<"  total_one_num = Total_One(ar_out, Length_Code, Length_R);"<<endl;
    fout<<"  return total_one_num;"<<endl;
    fout<<"}"<<endl;
    //Heavy_Row_inline
    fout<<"   "<<endl;
    fout<<"int Heavy_Row(int A[Length_Code][Length_R], int column_number, int row_number){"<<endl;
    fout<<"  int Num_row[Length_R];"<<endl;
    fout<<"  int counter = 0;"<<endl;
    fout<<"  for(int i=0;i<Length_R;i++){"<<endl;
    fout<<"    counter = 0;"<<endl;
    fout<<"    for (int k=0;k<Length_Code;k++){"<<endl;
    fout<<"      if(A[k][i]==1) counter++;"<<endl;
    fout<<"      else continue;"<<endl;
    fout<<"    }"<<endl;
    fout<<"    Num_row[i]=counter;"<<endl;
    fout<<"   "<<endl;
    fout<<"  }"<<endl;
    fout<<"  int Heavy_Num = Num_row[0];"<<endl;
    fout<<"  for (int i=1;i<Length_R;i++){"<<endl;
    fout<<"    if(Num_row[i]>Heavy_Num){"<<endl;
    fout<<"      Heavy_Num = Num_row[i];"<<endl;
    fout<<"    }"<<endl;
    fout<<"  }"<<endl;
    fout<<"  return Heavy_Num;"<<endl;
    fout<<"}"<<endl;
    fout<<"   "<<endl;
    fout<<"int Heavy_Row_inline (int col[Length_Code]){"<<endl;
    fout<<"  int ar_in[Length_Code];"<<endl;
    fout<<"  int ar_out[Length_Code][Length_R];"<<endl;
    fout<<"   "<<endl;
    fout<<"  for(int i=0;i<Length_Code;i++){"<<endl;
    fout<<"    ar_in[i] = col[i];"<<endl;
    fout<<"  }"<<endl;
    fout<<"   "<<endl;
    fout<<"//10进制格式转换到2进制格式"<<endl;
    fout<<"  for(int j=0; j<Length_Code; j++){"<<endl;
    fout<<"    for(int i=0;i<Length_R;i++){"<<endl;
    fout<<"      if ((ar_in[j]&(1<<i))) ar_out[j][i] = 1;"<<endl;
    fout<<"      else ar_out[j][i] = 0;"<<endl;
    fout<<"    }"<<endl;
    fout<<"  }"<<endl;
    fout<<"   "<<endl;
    fout<<"  int heavy_row_num;"<<endl;
    fout<<"  heavy_row_num = Heavy_Row(ar_out, Length_Code, Length_R);"<<endl;
    fout<<"  return heavy_row_num;"<<endl;
    fout<<"}"<<endl;
    fout<<"   "<<endl;
    fout<<"int Counter(int a, int row_number){"<<endl;

    fout<<"int A[row_number];"<<endl;
    fout<<"int counter = 0;"<<endl;

    fout<<"for(int i=0;i<row_number;i++){"<<endl;
    fout<<"  if ((a&(1<<i))) A[i] = 1;"<<endl;
    fout<<"  else A[i] = 0;"<<endl;
    fout<<"}"<<endl;
    fout<<"for(int i=0;i<row_number;i++){"<<endl;
    fout<<"    if(A[i]==1) counter++;"<<endl;
    fout<<"    else continue;"<<endl;
    fout<<"}"<<endl;
    fout<<"return counter;"<<endl;
    fout<<"}"<<endl;

    // delete the dynamic array
    delete[] p_size;
    delete[] parity;
    delete[] pb_position;
    delete[] Column_Weight_upper;
    delete[] Column_Weight_bottom;

    for(int i=0;i<p_num;i++){delete[] p[i];}
    delete[] p;

    for(int i=0;i<p_num;i++){delete[] p_range[i];}
    delete[] p_range;

    for(int i=0;i<p_num;i++){delete[] en_p[i];}
    delete[] en_p;

    cout<<"----------------------------------------------"<<endl;
    cout<<">>> Auto_ECC Generation Program Successful ***"<<endl;
    cout<<"----------------------------------------------"<<endl;
    cout<<"##############################################"<<endl;
    cout<<"##############################################"<<endl;
    cout<<"                                              "<<endl;
  }



if(Mode == 1){
  int Length_D = 0; // number of data bits
  int Length_R = 0; // number of parity bits

  int *p_size = new int[p_num];  // error_pattern correction size
  for(int i=0;i<p_num;i++){
    *(p_size+i) = 0;
  }

  int *pd_size = new int[pd_num];  // error_pattern detection size
  for(int i=0;i<pd_num;i++){
    *(pd_size+i) = 0;
  }

  int **p = new int*[p_num];  // error_pattern correction
  for(int i=0;i<p_num;i++) {p[i] = new int[P_in_length];}

  int **pd = new int*[pd_num];  // error_pattern detection
  for(int i=0;i<pd_num;i++) {pd[i] = new int[P_in_length];}

  int **p_range = new int*[p_num];   // error pattern range correction
  for(int i=0;i<p_num;i++) p_range[i] = new int[2*Pran_num];

  int **pd_range = new int*[pd_num];   // error pattern range detection
  for(int i=0;i<pd_num;i++) pd_range[i] = new int[2*Pran_num];

  // read Data Bit and Parity Bit
    fin>>tmp;  // read ****************** line
    fin>>Length_D;
    fin>>tmp;  // read ****************** line
    fin>>Length_R;
  // definition
  int Length_Code = Length_D + Length_R;

  int *Column_Weight_upper = new int[Length_Code]; //Column_Weight_upper
  for(int i=0;i<Length_Code;i++){
    *(Column_Weight_upper+i) = 0;
  }
  int *Column_Weight_bottom = new int[Length_Code]; //Column_Weight_bottom
  for(int i=0;i<Length_Code;i++){
    *(Column_Weight_bottom+i) = 0;
  }


  int **en_p = new int*[p_num];    // error_pattern correction enable
  for(int i=0;i<p_num;i++) en_p[i] = new int[Length_D+Length_R];

  int **en_pd = new int*[pd_num];    // error_pattern detection enable
  for(int i=0;i<pd_num;i++) en_pd[i] = new int[Length_D+Length_R];

  for(int i=0;i<p_num;i++){    // error_pattern correction enable initialization
    for(int j=0;j<Length_D+Length_R;j++){
      *(*(en_p+i)+j) = 0;
    }
  }

  for(int i=0;i<pd_num;i++){   // error_pattern detection enable initialization
    for(int j=0;j<Length_D+Length_R;j++){
      *(*(en_pd+i)+j) = 0;
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

  // start to read Error_Pattern Correction
  fin>>tmp;  // read ****************** line
    for (int i=0;i<p_num;i++){
      for(int j=0;j<P_in_length;j++){
        fin>> *(*(p+i)+j);
      }
    }
  // start to read Error_Pattern_Range Correction
  fin>>tmp;  // read ****************** line
    for (int i=0;i<p_num;i++){
      for(int j=0;j<2*Pran_num;j++){
        fin>> *(*(p_range+i)+j);
      }
    }
  // start to read Error_Pattern Detection
  fin>>tmp;  // read ****************** line
    for (int i=0;i<pd_num;i++){
      for(int j=0;j<P_in_length;j++){
        fin>> *(*(pd+i)+j);
      }
    }
  // start to read Error_Pattern_Range Detection
  fin>>tmp;  // read ****************** line
    for (int i=0;i<pd_num;i++){
      for(int j=0;j<2*Pran_num;j++){
        fin>> *(*(pd_range+i)+j);
      }
    }
  // start to read Parity_Bit_Position
  fin>>tmp;  // read ****************** line
    for (int i=0;i<Length_R;i++){
      fin>> *(pb_position+i);
    }

  while(cnt){
    fin_1>>tmp;  // read ****************** line
    if (!fin_1.good()) break;
    // read Column_Weight_upper
    for (int i=0;i<Length_Code;i++){
      fin_1>> *(Column_Weight_upper+i);
    }
    fin_1>>tmp;  // read ****************** line
    // read column_weight bottom limit
    for (int i=0;i<Length_Code;i++){
      fin_1>> *(Column_Weight_bottom+i);
    }
  }
  // --------------------------------
  cout<<"                                            "<<endl;
  cout<<"********************************************"<<endl;
  cout<<"***** Welcom to use the Auto_ECC Tool ******"<<endl;
  cout<<"********************************************"<<endl;
  cout<<">>> Data Bit is "<<Length_D<<endl;
  cout<<"********************************************"<<endl;
  cout<<">>> Parity Bit Value is "<<Length_R<<endl;
  cout<<"********************************************"<<endl;
  cout<<">>> Error Pattern (Correction) Number is "<<p_num<<endl;
  cout<<"********************************************"<<endl;
  cout<<">>> Error Pattern (Detection) Number is "<<pd_num<<endl;
  cout<<"********************************************"<<endl;
  cout<<">>> Stop Time is "<<(Stop_time/60)<<" min "<<endl;
  cout<<"********************************************"<<endl;
// --------------------------------
// Obtain the Correction Error_Pattern size by using the error_pattern
for (int i=0;i<p_num;i++){
  for(int j=(P_in_length-1);j>=0;j--){
    if (*(*(p+i)+j) == 1){
      *(p_size+i) = j+1;
      break;
    }
  }
}
// Obtain the Detection Error_Pattern size by using the error_pattern
for (int i=0;i<pd_num;i++){
  for(int j=(P_in_length-1);j>=0;j--){
    if (*(*(pd+i)+j) == 1){
      *(pd_size+i) = j+1;
      break;
    }
  }
}
cout<<"********************************************"<<endl;
cout<<">>> Parity Bit : ";
for(int i=0;i<Length_R;i++){
  *(parity+i) = pow(2,(Length_R-1-i));
  cout<< *(parity+i) <<" ";
}
cout<<"\n";
cout<<"********************************************"<<endl;
// --------------------------------
// 5 groups of the range to the correction error pattern enable
for(int i=0;i<p_num;i++){
  for(int j=0;j<Pran_num;j++){
    if(*(*(p_range+i)+2*j) != 0){
      for(int k=(*(*(p_range+i)+2*j)-1 + *(p_size+i)-1); k<*(*(p_range+i)+2*j+1); k++)
        *(*(en_p+i)+k) = 1;
      }
    if(*(*(p_range+i)+2*j) == 0) continue;
   }
  }
// 5 groups of the range to the detection error pattern enable
for(int i=0;i<pd_num;i++){
  for(int j=0;j<Pran_num;j++){
    if(*(*(pd_range+i)+2*j) != 0){
      for(int k=(*(*(pd_range+i)+2*j)-1 + *(pd_size+i)-1); k<*(*(pd_range+i)+2*j+1); k++)
        *(*(en_pd+i)+k) = 1;
      }
    if(*(*(pd_range+i)+2*j) == 0) continue;
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
  cout<<">>> Correction Error Pattern Range : "<<endl;
  for(int i=0;i<p_num;i++){
    for(int j=0;j<2*Pran_num;j++){
      cout<< *(*(p_range+i)+j) <<" ";
    }
    cout<<"\n";
  }
  cout<<"********************************************"<<endl;
  cout<<">>> Detection Error Pattern Range : "<<endl;
  for(int i=0;i<pd_num;i++){
    for(int j=0;j<2*Pran_num;j++){
      cout<< *(*(pd_range+i)+j) <<" ";
    }
    cout<<"\n";
  }
  cout<<"********************************************"<<endl;
  cout<<">>> Correction Error Pattern Enable : "<<endl;
  for(int i=0;i<p_num;i++){
    for(int j=0;j<Length_D+Length_R;j++){
      cout<< *(*(en_p+i)+j) <<" ";
    }
    cout<<"\n";
  }
  cout<<"********************************************"<<endl;
  cout<<">>> Detection Error Pattern Enable : "<<endl;
  for(int i=0;i<pd_num;i++){
    for(int j=0;j<Length_D+Length_R;j++){
      cout<< *(*(en_pd+i)+j) <<" ";
    }
    cout<<"\n";
  }
  cout<<"********************************************"<<endl;
  cout<<">>> Parity Bit Position : ";
  for (int i=0;i<Length_R;i++){
    cout<< *(pb_position+i) <<" ";
  }
  cout<<"\n";
  cout<<"********************************************"<<endl;

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
  fout<<"#define Stop_time "<<Stop_time<<endl;
  fout<<"        "<<endl;
  fout<<"        "<<endl;
  // main body of the function
  fout<<"int main(){"<<endl;
  fout<<"        "<<endl;
  fout<<"ofstream fout;"<<endl;
  fout<<"fout.open(\"1.txt\");"<<endl;

  fout<<"        "<<endl;
  fout<<"clock_t start, finish;"<<endl;
  fout<<"        "<<endl;
  fout<<"int HR_Temp=1000; "<<endl;
  fout<<"int TS_Temp=1000; "<<endl;
  fout<<"        "<<endl;
  fout<<"int col[Length_Code]={0};"<<endl;
  fout<<"        "<<endl;
  fout<<"int Correction_Set["<<Length_Code<<"]["<<p_num<<"]={0};"<<endl;
  fout<<"        "<<endl;
  fout<<"int Detection_Set["<<Length_Code<<"]["<<pd_num<<"]={0};"<<endl;
  fout<<"        "<<endl;
  fout<<"int identity_p["<<Length_Code<<"]={0};"<<endl;
  fout<<"int identity_pd["<<Length_Code<<"]={0};"<<endl;
  fout<<"        "<<endl;
  fout<<"int array[Length_Range-1] = {"<<endl;
  for (int i = 0; i < array_size-1; i++){
      fout<<array[i]<<",";
  }
  fout<<array[array_size-1]<<endl;
  fout<<"};"<<endl;
  fout<<"        "<<endl;
  for(int i=0;i<p_num;i++){
    fout<<"int e_p"<<(i+1)<<"["<<*(p_size+i)<<"] = {";
    for(int j=0;j<*(p_size+i)-1;j++){
      fout<< *(*(p+i)+j) <<",";
    }
    fout<<*(*(p+i) + *(p_size+i)-1)<<"};"<<endl;
  }
  fout<<"        "<<endl;
  for(int i=0;i<pd_num;i++){
    fout<<"int e_pd"<<(i+1)<<"["<<*(pd_size+i)<<"] = {";
    for(int j=0;j<*(pd_size+i)-1;j++){
      fout<< *(*(pd+i)+j) <<",";
    }
    fout<<*(*(pd+i) + *(pd_size+i)-1)<<"};"<<endl;
  }
  fout<<"        "<<endl;
  for(int i=0;i<p_num;i++){
    fout<<"int p"<<(i+1)<<"["<<Length_Code<<"] = {0};"<<endl;
  }
  fout<<"        "<<endl;
  for(int i=0;i<pd_num;i++){
    fout<<"int pd"<<(i+1)<<"["<<Length_Code<<"] = {0};"<<endl;
  }
  fout<<"        "<<endl;

  for(int l=0;l<p_num;l++){
    fout<<"int Function_P"<<(l+1)<<" (int A[Length_Code], int column_i, int row_number, int Correction_Set[]["<<p_num<<"], int Detection_Set[]["<<pd_num<<"], int e_p"<<(l+1)<<"["<<*(p_size+l)<<"]);"<<endl;
  }
  fout<<"        "<<endl;
  for(int l=0;l<pd_num;l++){
    fout<<"int Function_PD"<<(l+1)<<" (int A[Length_Code], int column_i, int row_number, int Correction_Set[]["<<p_num<<"], int Detection_Set[]["<<pd_num<<"], int e_pd"<<(l+1)<<"["<<*(pd_size+l)<<"]);"<<endl;
  }
  fout<<"        "<<endl;
  fout<<"int Heavy_Row_inline (int col[Length_Code]); "<<endl;
  fout<<"int Total_One_inline (int col[Length_Code]); "<<endl;
  fout<<"int Counter (int a, int row_number);"<<endl;
  fout<<"        "<<endl;
  fout<<"start = clock();"<<endl;
  fout<<"        "<<endl;

  // Search part of the Generation function
  int pb_num = 0;

  for(int i=0;i<(Length_D+Length_R);i++){
    int flag_dp = 0;
    for (int js=0;js<Length_R;js++){
      if (*(pb_position+js) == i+1) {flag_dp = 1; break;}
      else flag_dp = 0;
    }

    // cout<<"flag_dp is "<<flag_dp<<endl;

    if(flag_dp==0){
      fout<<"// Level_"<<i+1<<"[D]"<<endl;

      fout<<"for (int i"<<i<<" = 0; i"<<i<<" < Length_Range-1; i"<<i<<"++){"<<"\n";
      fout<<"if(Counter(array[i"<<i<<"],Length_R) > "<<*(Column_Weight_upper+i)<<") continue;"<<endl;
      fout<<"if(Counter(array[i"<<i<<"],Length_R) < "<<*(Column_Weight_bottom+i)<<") continue;"<<endl;
      fout<<"col["<<i<<"] = array[i"<<i<<"];"<<"\n";

    //******************
      for(int j=0;j<p_num;j++){
        if(*(*(en_p+j)+i)==1){
          fout<<"p"<<(j+1)<<"["<<i<<"] = Function_P"<<(j+1)<<"(col, "<<i<<", Length_R, Correction_Set, Detection_Set, e_p"<<(j+1)<<");"<<endl;
        }
      }
      for(int j=0;j<pd_num;j++){
        if(*(*(en_pd+j)+i)==1){
          fout<<"pd"<<(j+1)<<"["<<i<<"] = Function_PD"<<(j+1)<<"(col, "<<i<<", Length_R, Correction_Set, Detection_Set, e_pd"<<(j+1)<<");"<<endl;
        }
      }
    //******************
      fout<<"identity_p["<<i<<"] = ";
      int cnt_temp = 0;

      int *p_flag = new int[p_num];
      for(int i=0;i<p_num;i++){
        *(p_flag+i) = 0;
      }

      for(int j=0;j<p_num;j++){
        cnt_temp = cnt_temp + *(*(en_p+j)+i);
        if (*(*(en_p+j)+i)==1) *(p_flag+j) = cnt_temp;
      }

      if (cnt_temp == 0) fout<<"1;"<<"\n";
      if (cnt_temp == 1) {
        for(int j=0;j<p_num;j++){
          if (*(p_flag+j)==1) fout<<"p"<<(j+1)<<"["<<i<<"]";
        }
        fout<<";"<<"\n";
      }
      if (cnt_temp > 1){
        for(int j=0;j<p_num;j++){
          if (*(p_flag+j)==1) fout<<"p"<<(j+1)<<"["<<i<<"]";
          if (*(p_flag+j)>1) fout<<" && p"<<(j+1)<<"["<<i<<"]";
        }
        fout<<";"<<"\n";
      }
      //******************
        fout<<"identity_pd["<<i<<"] = ";
        int cnt_tempd = 0;

        int *pd_flag = new int[pd_num];
        for(int i=0;i<pd_num;i++){
          *(pd_flag+i) = 0;
        }

        for(int j=0;j<pd_num;j++){
          cnt_tempd = cnt_tempd + *(*(en_pd+j)+i);
          if (*(*(en_pd+j)+i)==1) *(pd_flag+j) = cnt_tempd;
        }

        if (cnt_tempd == 0) fout<<"1;"<<"\n";
        if (cnt_tempd == 1) {
          for(int j=0;j<pd_num;j++){
            if (*(pd_flag+j)==1) fout<<"pd"<<(j+1)<<"["<<i<<"]";
          }
          fout<<";"<<"\n";
        }
        if (cnt_tempd > 1){
          for(int j=0;j<pd_num;j++){
            if (*(pd_flag+j)==1) fout<<"pd"<<(j+1)<<"["<<i<<"]";
            if (*(pd_flag+j)>1) fout<<" && pd"<<(j+1)<<"["<<i<<"]";
          }
          fout<<";"<<"\n";
        }

      fout<<"if(!identity_p["<<i<<"]) continue;"<<"\n";
      fout<<"if(!identity_pd["<<i<<"]) continue;"<<"\n";

      fout<<"else {"<<endl;
      fout<<"clock_t finish_"<<i<<" = clock();"<<endl;
      fout<<"double duration_"<<i<<" = (double)(finish_"<<i<<" - start) / CLOCKS_PER_SEC;"<<endl;
      fout<<"if(duration_"<<i<<" > Stop_time) return 0;"<<endl;
      // delete the dynamic array
      delete[] p_flag;
      delete[] pd_flag;
    }
    if(flag_dp==1){
      fout<<"// Level_"<<i+1<<"[P]"<<endl;

      fout<<"for (int i"<<i<<"="<<*(parity+pb_num)<<";i"<<i<<"<"<<(*(parity+pb_num)+1)<<";i"<<i<<"++){"<<"\n";
      fout<<"if(Counter(i"<<i<<",Length_R) > "<<*(Column_Weight_upper+i)<<") continue;"<<endl;
      fout<<"if(Counter(i"<<i<<",Length_R) < "<<*(Column_Weight_bottom+i)<<") continue;"<<endl;

      pb_num++;

      fout<<"col["<<i<<"]=i"<<i<<";"<<"\n";

      //******************
        for(int l=0;l<p_num;l++){
          if(*(*(en_p+l)+i)==1){
            fout<<"p"<<(l+1)<<"["<<i<<"] = Function_P"<<(l+1)<<"(col,"<<i<<", Length_R, Correction_Set, Detection_Set, e_p"<<(l+1)<<");"<<endl;
          }
        }
        for(int l=0;l<pd_num;l++){
          if(*(*(en_pd+l)+i)==1){
            fout<<"pd"<<(l+1)<<"["<<i<<"] = Function_PD"<<(l+1)<<"(col,"<<i<<", Length_R, Correction_Set, Detection_Set, e_pd"<<(l+1)<<");"<<endl;
          }
        }
      //******************
        fout<<"identity_p["<<i<<"] = ";
        int cnt_temp = 0;

        int *p_flag = new int[p_num];
        for(int j=0;j<p_num;j++){
          *(p_flag+j) = 0;
        }

        for(int j=0;j<p_num;j++){
          cnt_temp = cnt_temp+ *(*(en_p+j)+i);
          if (*(*(en_p+j)+i)==1) *(p_flag+j) = cnt_temp;
        }

        if (cnt_temp == 0) fout<<"1;"<<"\n";
        if (cnt_temp == 1) {
          for(int j=0;j<p_num;j++){
            if (*(p_flag+j)==1) fout<<"p"<<(j+1)<<"["<<i<<"]";
          }
          fout<<";"<<"\n";
        }
        if (cnt_temp > 1){
          for(int j=0;j<p_num;j++){
            if (*(p_flag+j)==1) fout<<"p"<<(j+1)<<"["<<i<<"]";
            if (*(p_flag+j)>1) fout<<" && p"<<(j+1)<<"["<<i<<"]";
          }
          fout<<";"<<"\n";
        }
      //******************
        fout<<"identity_pd["<<i<<"] = ";
        int cnt_tempd = 0;

        int *pd_flag = new int[pd_num];
        for(int i=0;i<pd_num;i++){
          *(pd_flag+i) = 0;
        }

        for(int j=0;j<pd_num;j++){
          cnt_tempd = cnt_tempd + *(*(en_pd+j)+i);
          if (*(*(en_pd+j)+i)==1) *(pd_flag+j) = cnt_tempd;
        }

        if (cnt_tempd == 0) fout<<"1;"<<"\n";
        if (cnt_tempd == 1) {
          for(int j=0;j<pd_num;j++){
            if (*(pd_flag+j)==1) fout<<"pd"<<(j+1)<<"["<<i<<"]";
          }
          fout<<";"<<"\n";
        }
        if (cnt_tempd > 1){
          for(int j=0;j<pd_num;j++){
            if (*(pd_flag+j)==1) fout<<"pd"<<(j+1)<<"["<<i<<"]";
            if (*(pd_flag+j)>1) fout<<" && pd"<<(j+1)<<"["<<i<<"]";
          }
          fout<<";"<<"\n";
        }
      //******************

      fout<<"if(!identity_p["<<i<<"]) continue;"<<"\n";
      fout<<"if(!identity_pd["<<i<<"]) continue;"<<"\n";

        fout<<"else {"<<endl;
        fout<<"clock_t finish_"<<i<<" = clock();"<<endl;
        fout<<"double duration_"<<i<<" = (double)(finish_"<<i<<" - start) / CLOCKS_PER_SEC;"<<endl;
        fout<<"if(duration_"<<i<<" > Stop_time) return 0;"<<endl;

        // delete the dynamic array
        delete[] p_flag;
        delete[] pd_flag;
    }
  }
  fout<<"     "<<endl;
  fout<<"     "<<endl;
  fout<<"int HR = 0;"<<endl;
  fout<<"HR = Heavy_Row_inline(col);"<<endl;
  fout<<"if(HR < HR_Temp){"<<endl;
  fout<<"HR_Temp = HR;"<<endl;
  fout<<"cout<<\"Smallest HR is : \"<<HR<<endl;"<<endl;
  fout<<"cout<<\"Find it !\"<<endl;"<<endl;
  fout<<"// Output the Target Array"<<endl;
  fout<<"fout<<\"******************\"<<endl;"<<endl;
  fout<<"  for (int i=0;i<Length_Code;i++){"<<endl;
  fout<<"    fout<<col[i]<<\" \";"<<endl;
  fout<<"  }"<<endl;
  fout<<"  fout<<\"\\n\";"<<endl;
  fout<<"}"<<endl;

  fout<<"     "<<endl;
  fout<<"int TS = 0;"<<endl;
  fout<<"TS = Total_One_inline(col);"<<endl;
  fout<<"if(TS < TS_Temp){"<<endl;
  fout<<"TS_Temp = TS;"<<endl;
  fout<<"cout<<\"Smallest TS is : \"<<TS<<endl;"<<endl;
  fout<<"cout<<\"Find it !\"<<endl;"<<endl;
  fout<<"// Output the Target Array"<<endl;
  fout<<"fout<<\"******************\"<<endl;"<<endl;
  fout<<"  for (int i=0;i<Length_Code;i++){"<<endl;
  fout<<"    fout<<col[i]<<\" \";"<<endl;
  fout<<"  }"<<endl;
  fout<<"  fout<<\"\\n\";"<<endl;
  fout<<"}"<<endl;

  fout<<"     "<<endl;
  fout<<"     "<<endl;

//  fout<<"   return 0;  "<<endl;
  //************************
  for(int i=0;i<(Length_D+Length_R);i++){
    fout<<" } "<<endl;
    fout<<"  } "<<endl;
  }
  fout<<"fout.close();"<<endl;
  fout<<"}"<<endl; //main {}
  //************************
  // Error_Pattern Indentify Function
  // Correction_function
  for(int l=0;l<p_num;l++){
    fout<<"int Function_P"<<(l+1)<<" (int A[Length_Code], int column_i, int row_number, int Correction_Set[]["<<p_num<<"], int Detection_Set[]["<<pd_num<<"], int e_p"<<(l+1)<<"["<<*(p_size+l)<<"]){"<<endl;
    fout<<"         "<<endl;
    fout<<"int Correction;"<<endl;
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
    fout<<"int c = 0;"<<endl;
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
    fout<<"  c += pow(2,i)*A_S[i];"<<endl;
    fout<<"}"<<endl;
    fout<<"Correction = c;"<<endl;

    fout<<"       "<<endl;
    fout<<"//!=0"<<endl;
    fout<<"if(Correction == 0) return 0;"<<endl;
    fout<<"       "<<endl;

    if(l>0){
      fout<<"//!= previous P in column_i"<<endl;
      fout<<"for(int i=0;i<"<<l<<";i++){"<<endl;
      fout<<" if(Correction == Correction_Set[column_i][i]) return 0;"<<endl;
      fout<<"}"<<endl;
      fout<<"       "<<endl;
    }

    fout<<"//Correction是否在Correction_Set集合内"<<endl;
    fout<<"  for (int j=0;j<column_i;j++){"<<endl;
    fout<<"    for (int k=0;k<"<<p_num<<";k++){"<<endl;
    fout<<"      if(Correction == Correction_Set[j][k])"<<endl;
    fout<<"      return 0;"<<endl;
    fout<<"    }"<<endl;
    fout<<"  }"<<endl;
    fout<<"       "<<endl;

    fout<<"//Correction是否在Detection_Set集合内"<<endl;
    fout<<"  for (int j=0;j<column_i;j++){"<<endl;
    fout<<"    for (int k=0;k<"<<pd_num<<";k++){"<<endl;
    fout<<"      if(Correction == Detection_Set[j][k])"<<endl;
    fout<<"      return 0;"<<endl;
    fout<<"    }"<<endl;
    fout<<"  }"<<endl;

    fout<<"Correction_Set[column_i]["<<l<<"] = Correction;"<<endl;

    fout<<"  return 1;"<<endl;
    fout<<"}"<<endl;
  }

  // Detection_function
  for(int l=0;l<pd_num;l++){
    fout<<"int Function_PD"<<(l+1)<<" (int A[Length_Code], int column_i, int row_number, int Correction_Set[]["<<p_num<<"], int Detection_Set[]["<<pd_num<<"], int e_pd"<<(l+1)<<"["<<*(pd_size+l)<<"]){"<<endl;
    fout<<"         "<<endl;
    fout<<"int Detection;"<<endl;
    fout<<"         "<<endl;
    for(int i=0;i<*(pd_size+l);i++){
      fout<<"int a_"<<(i+1)<<" = A[column_i-"<<(*(pd_size+l)-1-i)<<"];"<<endl;
    }
    fout<<"     "<<endl;
    for(int i=0;i<*(pd_size+l);i++){
      fout<<"int A_"<<(i+1)<<"[row_number];"<<endl;
    }
    fout<<"         "<<endl;
    fout<<"int A_S[row_number];"<<endl;
    fout<<"int c = 0;"<<endl;
    fout<<"         "<<endl;
    for(int i=0;i<*(pd_size+l);i++){
      fout<<"for(int i=0;i<row_number;i++){"<<endl;
      fout<<"  if ((a_"<<(i+1)<<"&(1<<i))) A_"<<(i+1)<<"[i] = 1;"<<endl;
      fout<<"  else A_"<<(i+1)<<"[i] = 0;"<<endl;
      fout<<"}"<<endl;
    }
    fout<<"         "<<endl;
    fout<<"for(int i=0;i<row_number;i++){"<<endl;
    fout<<"  A_S[i] = ";
    if(*(pd_size+l) == 1) fout<<"(e_pd"<<(l+1)<<"[0] && A_"<<*(pd_size+l)<<"[i]);"<<endl;
    if(*(pd_size+l) > 1){
      fout<<"(e_pd"<<(l+1)<<"[0]&&A_"<<1<<"[i])";
      for(int i=1;i<*(pd_size+l);i++){
          fout<<"^(e_pd"<<(l+1)<<"["<<i<<"]&&A_"<<(i+1)<<"[i])";
      }
    fout<<";"<<endl;
    }
    fout<<"}"<<endl;

    fout<<"for(int i=0;i<row_number;i++){"<<endl;
    fout<<"  c += pow(2,i)*A_S[i];"<<endl;
    fout<<"}"<<endl;
    fout<<"Detection = c;"<<endl;

    fout<<"       "<<endl;
    fout<<"//!=0"<<endl;
      fout<<"if(Detection == 0) return 0;"<<endl;

    fout<<"       "<<endl;
    fout<<"//Detection是否在Correction_Set集合内"<<endl;
    fout<<"  for (int j=0;j<=column_i;j++){  // Different from the Correction_Set including now column_i"<<endl;
    fout<<"    for (int k=0;k<"<<p_num<<";k++){"<<endl;
    fout<<"      if(Detection == Correction_Set[j][k])"<<endl;
    fout<<"      return 0;"<<endl;
    fout<<"    }"<<endl;
    fout<<"  }"<<endl;

    fout<<"Detection_Set[column_i]["<<l<<"] = Detection;"<<endl;

    fout<<"  return 1;"<<endl;
    fout<<"}"<<endl;
  }
  //Total_One_inline
  fout<<"   "<<endl;
  fout<<"int Total_One(int A[Length_Code][Length_R], int column_number, int row_number){"<<endl;
  fout<<"  int counter = 0;"<<endl;
  fout<<"  for(int i=0;i<Length_R;i++){"<<endl;
  fout<<"    for (int k=0;k<Length_Code;k++){"<<endl;
  fout<<"      if(A[k][i]==1) counter++;"<<endl;
  fout<<"      else continue;"<<endl;
  fout<<"    }"<<endl;
  fout<<"  }"<<endl;
  fout<<"  return counter;"<<endl;
  fout<<"}"<<endl;
  fout<<"   "<<endl;
  fout<<"int Total_One_inline (int col[Length_Code]){"<<endl;
  fout<<"  int ar_in[Length_Code];"<<endl;
  fout<<"  int ar_out[Length_Code][Length_R];"<<endl;
  fout<<"    "<<endl;
  fout<<"  for(int i=0;i<Length_Code;i++){"<<endl;
  fout<<"    ar_in[i] = col[i];"<<endl;
  fout<<"  }"<<endl;
  fout<<"    "<<endl;
  fout<<"//10进制格式转换到2进制格式"<<endl;
  fout<<"  for(int j=0; j<Length_Code; j++){"<<endl;
  fout<<"    for(int i=0;i<Length_R;i++){"<<endl;
  fout<<"      if ((ar_in[j]&(1<<i))) ar_out[j][i] = 1;"<<endl;
  fout<<"      else ar_out[j][i] = 0;"<<endl;
  fout<<"    }"<<endl;
  fout<<"  }"<<endl;
  fout<<"      "<<endl;
  fout<<"  int total_one_num;"<<endl;
  fout<<"  total_one_num = Total_One(ar_out, Length_Code, Length_R);"<<endl;
  fout<<"  return total_one_num;"<<endl;
  fout<<"}"<<endl;
  //Heavy_Row_inline
  fout<<"   "<<endl;
  fout<<"int Heavy_Row(int A[Length_Code][Length_R], int column_number, int row_number){"<<endl;
  fout<<"  int Num_row[Length_R];"<<endl;
  fout<<"  int counter = 0;"<<endl;
  fout<<"  for(int i=0;i<Length_R;i++){"<<endl;
  fout<<"    counter = 0;"<<endl;
  fout<<"    for (int k=0;k<Length_Code;k++){"<<endl;
  fout<<"      if(A[k][i]==1) counter++;"<<endl;
  fout<<"      else continue;"<<endl;
  fout<<"    }"<<endl;
  fout<<"    Num_row[i]=counter;"<<endl;
  fout<<"   "<<endl;
  fout<<"  }"<<endl;
  fout<<"  int Heavy_Num = Num_row[0];"<<endl;
  fout<<"  for (int i=1;i<Length_R;i++){"<<endl;
  fout<<"    if(Num_row[i]>Heavy_Num){"<<endl;
  fout<<"      Heavy_Num = Num_row[i];"<<endl;
  fout<<"    }"<<endl;
  fout<<"  }"<<endl;
  fout<<"  return Heavy_Num;"<<endl;
  fout<<"}"<<endl;
  fout<<"   "<<endl;
  fout<<"int Heavy_Row_inline (int col[Length_Code]){"<<endl;
  fout<<"  int ar_in[Length_Code];"<<endl;
  fout<<"  int ar_out[Length_Code][Length_R];"<<endl;
  fout<<"   "<<endl;
  fout<<"  for(int i=0;i<Length_Code;i++){"<<endl;
  fout<<"    ar_in[i] = col[i];"<<endl;
  fout<<"  }"<<endl;
  fout<<"   "<<endl;
  fout<<"//10进制格式转换到2进制格式"<<endl;
  fout<<"  for(int j=0; j<Length_Code; j++){"<<endl;
  fout<<"    for(int i=0;i<Length_R;i++){"<<endl;
  fout<<"      if ((ar_in[j]&(1<<i))) ar_out[j][i] = 1;"<<endl;
  fout<<"      else ar_out[j][i] = 0;"<<endl;
  fout<<"    }"<<endl;
  fout<<"  }"<<endl;
  fout<<"   "<<endl;
  fout<<"  int heavy_row_num;"<<endl;
  fout<<"  heavy_row_num = Heavy_Row(ar_out, Length_Code, Length_R);"<<endl;
  fout<<"  return heavy_row_num;"<<endl;
  fout<<"}"<<endl;
  fout<<"   "<<endl;
  fout<<"int Counter(int a, int row_number){"<<endl;

  fout<<"int A[row_number];"<<endl;
  fout<<"int counter = 0;"<<endl;

  fout<<"for(int i=0;i<row_number;i++){"<<endl;
  fout<<"  if ((a&(1<<i))) A[i] = 1;"<<endl;
  fout<<"  else A[i] = 0;"<<endl;
  fout<<"}"<<endl;
  fout<<"for(int i=0;i<row_number;i++){"<<endl;
  fout<<"    if(A[i]==1) counter++;"<<endl;
  fout<<"    else continue;"<<endl;
  fout<<"}"<<endl;
  fout<<"return counter;"<<endl;
  fout<<"}"<<endl;
  // delete the dynamic array
  delete[] p_size;
  delete[] pd_size;
  delete[] parity;
  delete[] pb_position;

  delete[] Column_Weight_upper;
  delete[] Column_Weight_bottom;

  for(int i=0;i<p_num;i++)
    delete[] p[i];
  for(int i=0;i<pd_num;i++)
    delete[] pd[i];

  delete[] p;
  delete[] pd;

  for(int i=0;i<p_num;i++)
    delete[] p_range[i];
  for(int i=0;i<pd_num;i++)
    delete[] pd_range[i];

  delete[] p_range;
  delete[] pd_range;

  for(int i=0;i<p_num;i++)
    delete[] en_p[i];
  for(int i=0;i<pd_num;i++)
    delete[] en_pd[i];
  delete[] en_p;
  delete[] en_pd;

  cout<<"----------------------------------------------"<<endl;
  cout<<">>> Auto_ECC Generation Program Successful ***"<<endl;
  cout<<"----------------------------------------------"<<endl;
  cout<<"##############################################"<<endl;
  cout<<"##############################################"<<endl;
  cout<<"                                              "<<endl;
}

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
