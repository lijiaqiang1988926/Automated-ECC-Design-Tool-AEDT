#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
        
#define Length_Code 23          //Code length
#define Length_R 7              //Redundant bit
#define Length_D 16             //Data bit
#define Length_Range 128    //Size of array of probability of adding array
#define Stop_time 1e+10
        
        
int main(){
        
ofstream fout;
fout.open("1.txt");
        
clock_t start, finish;
        
int HR_Temp=1000; 
int TS_Temp=1000; 
        
int col[Length_Code]={0};
        
int Correction_Set[23][4]={0};
        
int identity[23]={0};
        
int array[Length_Range-1] = {
1,2,4,8,16,32,64,3,5,6,9,10,12,17,18,20,24,33,34,36,40,48,65,66,68,72,80,96,7,11,13,14,19,21,22,25,26,28,35,37,38,41,42,44,49,50,52,56,67,69,70,73,74,76,81,82,84,88,97,98,100,104,112,15,23,27,29,30,39,43,45,46,51,53,54,57,58,60,71,75,77,78,83,85,86,89,90,92,99,101,102,105,106,108,113,114,116,120,31,47,55,59,61,62,79,87,91,93,94,103,107,109,110,115,117,118,121,122,124,63,95,111,119,123,125,126,127
};
        
int e_p1[1] = {1};
int e_p2[2] = {1,1};
int e_p3[3] = {1,0,1};
int e_p4[3] = {1,1,1};
        
int p1[23] = {0};
int p2[23] = {0};
int p3[23] = {0};
int p4[23] = {0};
        
int Function_P1 (int A[Length_Code], int column_i, int row_number, int Correction_Set[][4], int e_p1[1]);
int Function_P2 (int A[Length_Code], int column_i, int row_number, int Correction_Set[][4], int e_p2[2]);
int Function_P3 (int A[Length_Code], int column_i, int row_number, int Correction_Set[][4], int e_p3[3]);
int Function_P4 (int A[Length_Code], int column_i, int row_number, int Correction_Set[][4], int e_p4[3]);
        
int Heavy_Row_inline (int col[Length_Code]); 
int Total_One_inline (int col[Length_Code]); 
int Counter (int a, int row_number);
        
start = clock();
        
// Level_1[P]
for (int i0=64;i0<65;i0++){
if(Counter(i0,Length_R) > 1) continue;
col[0]=i0;
p1[0] = Function_P1(col,0,Length_R,Correction_Set,e_p1);
identity[0] = p1[0];
if(!identity[0]) continue;
else {
clock_t finish_0 = clock();
double duration_0 = (double)(finish_0 - start) / CLOCKS_PER_SEC;
if(duration_0 > Stop_time) return 0;
// Level_2[P]
for (int i1=32;i1<33;i1++){
if(Counter(i1,Length_R) > 1) continue;
col[1]=i1;
p1[1] = Function_P1(col,1,Length_R,Correction_Set,e_p1);
p2[1] = Function_P2(col,1,Length_R,Correction_Set,e_p2);
identity[1] = p1[1] && p2[1];
if(!identity[1]) continue;
else {
clock_t finish_1 = clock();
double duration_1 = (double)(finish_1 - start) / CLOCKS_PER_SEC;
if(duration_1 > Stop_time) return 0;
// Level_3[P]
for (int i2=16;i2<17;i2++){
if(Counter(i2,Length_R) > 1) continue;
col[2]=i2;
p1[2] = Function_P1(col,2,Length_R,Correction_Set,e_p1);
p2[2] = Function_P2(col,2,Length_R,Correction_Set,e_p2);
p3[2] = Function_P3(col,2,Length_R,Correction_Set,e_p3);
p4[2] = Function_P4(col,2,Length_R,Correction_Set,e_p4);
identity[2] = p1[2] && p2[2] && p3[2] && p4[2];
if(!identity[2]) continue;
else {
clock_t finish_2 = clock();
double duration_2 = (double)(finish_2 - start) / CLOCKS_PER_SEC;
if(duration_2 > Stop_time) return 0;
// Level_4[P]
for (int i3=8;i3<9;i3++){
if(Counter(i3,Length_R) > 1) continue;
col[3]=i3;
p1[3] = Function_P1(col,3,Length_R,Correction_Set,e_p1);
p2[3] = Function_P2(col,3,Length_R,Correction_Set,e_p2);
p3[3] = Function_P3(col,3,Length_R,Correction_Set,e_p3);
p4[3] = Function_P4(col,3,Length_R,Correction_Set,e_p4);
identity[3] = p1[3] && p2[3] && p3[3] && p4[3];
if(!identity[3]) continue;
else {
clock_t finish_3 = clock();
double duration_3 = (double)(finish_3 - start) / CLOCKS_PER_SEC;
if(duration_3 > Stop_time) return 0;
// Level_5[P]
for (int i4=4;i4<5;i4++){
if(Counter(i4,Length_R) > 1) continue;
col[4]=i4;
p1[4] = Function_P1(col,4,Length_R,Correction_Set,e_p1);
p2[4] = Function_P2(col,4,Length_R,Correction_Set,e_p2);
p3[4] = Function_P3(col,4,Length_R,Correction_Set,e_p3);
p4[4] = Function_P4(col,4,Length_R,Correction_Set,e_p4);
identity[4] = p1[4] && p2[4] && p3[4] && p4[4];
if(!identity[4]) continue;
else {
clock_t finish_4 = clock();
double duration_4 = (double)(finish_4 - start) / CLOCKS_PER_SEC;
if(duration_4 > Stop_time) return 0;
// Level_6[P]
for (int i5=2;i5<3;i5++){
if(Counter(i5,Length_R) > 1) continue;
col[5]=i5;
p1[5] = Function_P1(col,5,Length_R,Correction_Set,e_p1);
p2[5] = Function_P2(col,5,Length_R,Correction_Set,e_p2);
p3[5] = Function_P3(col,5,Length_R,Correction_Set,e_p3);
p4[5] = Function_P4(col,5,Length_R,Correction_Set,e_p4);
identity[5] = p1[5] && p2[5] && p3[5] && p4[5];
if(!identity[5]) continue;
else {
clock_t finish_5 = clock();
double duration_5 = (double)(finish_5 - start) / CLOCKS_PER_SEC;
if(duration_5 > Stop_time) return 0;
// Level_7[D]
for (int i6 = 0; i6 < Length_Range-1; i6++){
if(Counter(array[i6],Length_R) > 1) continue;
col[6] = array[i6];
p1[6] = Function_P1(col, 6, Length_R, Correction_Set, e_p1);
p2[6] = Function_P2(col, 6, Length_R, Correction_Set, e_p2);
p3[6] = Function_P3(col, 6, Length_R, Correction_Set, e_p3);
p4[6] = Function_P4(col, 6, Length_R, Correction_Set, e_p4);
identity[6] = p1[6] && p2[6] && p3[6] && p4[6];
if(!identity[6]) continue;
else {
clock_t finish_6 = clock();
double duration_6 = (double)(finish_6 - start) / CLOCKS_PER_SEC;
if(duration_6 > Stop_time) return 0;
// Level_8[D]
for (int i7 = 0; i7 < Length_Range-1; i7++){
if(Counter(array[i7],Length_R) > 3) continue;
col[7] = array[i7];
p1[7] = Function_P1(col, 7, Length_R, Correction_Set, e_p1);
p2[7] = Function_P2(col, 7, Length_R, Correction_Set, e_p2);
p3[7] = Function_P3(col, 7, Length_R, Correction_Set, e_p3);
p4[7] = Function_P4(col, 7, Length_R, Correction_Set, e_p4);
identity[7] = p1[7] && p2[7] && p3[7] && p4[7];
if(!identity[7]) continue;
else {
clock_t finish_7 = clock();
double duration_7 = (double)(finish_7 - start) / CLOCKS_PER_SEC;
if(duration_7 > Stop_time) return 0;
// Level_9[D]
for (int i8 = 0; i8 < Length_Range-1; i8++){
if(Counter(array[i8],Length_R) > 3) continue;
col[8] = array[i8];
p1[8] = Function_P1(col, 8, Length_R, Correction_Set, e_p1);
p2[8] = Function_P2(col, 8, Length_R, Correction_Set, e_p2);
p3[8] = Function_P3(col, 8, Length_R, Correction_Set, e_p3);
p4[8] = Function_P4(col, 8, Length_R, Correction_Set, e_p4);
identity[8] = p1[8] && p2[8] && p3[8] && p4[8];
if(!identity[8]) continue;
else {
clock_t finish_8 = clock();
double duration_8 = (double)(finish_8 - start) / CLOCKS_PER_SEC;
if(duration_8 > Stop_time) return 0;
// Level_10[D]
for (int i9 = 0; i9 < Length_Range-1; i9++){
if(Counter(array[i9],Length_R) > 3) continue;
col[9] = array[i9];
p1[9] = Function_P1(col, 9, Length_R, Correction_Set, e_p1);
p2[9] = Function_P2(col, 9, Length_R, Correction_Set, e_p2);
p3[9] = Function_P3(col, 9, Length_R, Correction_Set, e_p3);
p4[9] = Function_P4(col, 9, Length_R, Correction_Set, e_p4);
identity[9] = p1[9] && p2[9] && p3[9] && p4[9];
if(!identity[9]) continue;
else {
clock_t finish_9 = clock();
double duration_9 = (double)(finish_9 - start) / CLOCKS_PER_SEC;
if(duration_9 > Stop_time) return 0;
// Level_11[D]
for (int i10 = 0; i10 < Length_Range-1; i10++){
if(Counter(array[i10],Length_R) > 3) continue;
col[10] = array[i10];
p1[10] = Function_P1(col, 10, Length_R, Correction_Set, e_p1);
p2[10] = Function_P2(col, 10, Length_R, Correction_Set, e_p2);
p3[10] = Function_P3(col, 10, Length_R, Correction_Set, e_p3);
p4[10] = Function_P4(col, 10, Length_R, Correction_Set, e_p4);
identity[10] = p1[10] && p2[10] && p3[10] && p4[10];
if(!identity[10]) continue;
else {
clock_t finish_10 = clock();
double duration_10 = (double)(finish_10 - start) / CLOCKS_PER_SEC;
if(duration_10 > Stop_time) return 0;
// Level_12[D]
for (int i11 = 0; i11 < Length_Range-1; i11++){
if(Counter(array[i11],Length_R) > 3) continue;
col[11] = array[i11];
p1[11] = Function_P1(col, 11, Length_R, Correction_Set, e_p1);
p2[11] = Function_P2(col, 11, Length_R, Correction_Set, e_p2);
p3[11] = Function_P3(col, 11, Length_R, Correction_Set, e_p3);
p4[11] = Function_P4(col, 11, Length_R, Correction_Set, e_p4);
identity[11] = p1[11] && p2[11] && p3[11] && p4[11];
if(!identity[11]) continue;
else {
clock_t finish_11 = clock();
double duration_11 = (double)(finish_11 - start) / CLOCKS_PER_SEC;
if(duration_11 > Stop_time) return 0;
// Level_13[D]
for (int i12 = 0; i12 < Length_Range-1; i12++){
if(Counter(array[i12],Length_R) > 3) continue;
col[12] = array[i12];
p1[12] = Function_P1(col, 12, Length_R, Correction_Set, e_p1);
p2[12] = Function_P2(col, 12, Length_R, Correction_Set, e_p2);
p3[12] = Function_P3(col, 12, Length_R, Correction_Set, e_p3);
p4[12] = Function_P4(col, 12, Length_R, Correction_Set, e_p4);
identity[12] = p1[12] && p2[12] && p3[12] && p4[12];
if(!identity[12]) continue;
else {
clock_t finish_12 = clock();
double duration_12 = (double)(finish_12 - start) / CLOCKS_PER_SEC;
if(duration_12 > Stop_time) return 0;
// Level_14[D]
for (int i13 = 0; i13 < Length_Range-1; i13++){
if(Counter(array[i13],Length_R) > 3) continue;
col[13] = array[i13];
p1[13] = Function_P1(col, 13, Length_R, Correction_Set, e_p1);
p2[13] = Function_P2(col, 13, Length_R, Correction_Set, e_p2);
p3[13] = Function_P3(col, 13, Length_R, Correction_Set, e_p3);
p4[13] = Function_P4(col, 13, Length_R, Correction_Set, e_p4);
identity[13] = p1[13] && p2[13] && p3[13] && p4[13];
if(!identity[13]) continue;
else {
clock_t finish_13 = clock();
double duration_13 = (double)(finish_13 - start) / CLOCKS_PER_SEC;
if(duration_13 > Stop_time) return 0;
// Level_15[D]
for (int i14 = 0; i14 < Length_Range-1; i14++){
if(Counter(array[i14],Length_R) > 3) continue;
col[14] = array[i14];
p1[14] = Function_P1(col, 14, Length_R, Correction_Set, e_p1);
p2[14] = Function_P2(col, 14, Length_R, Correction_Set, e_p2);
p3[14] = Function_P3(col, 14, Length_R, Correction_Set, e_p3);
p4[14] = Function_P4(col, 14, Length_R, Correction_Set, e_p4);
identity[14] = p1[14] && p2[14] && p3[14] && p4[14];
if(!identity[14]) continue;
else {
clock_t finish_14 = clock();
double duration_14 = (double)(finish_14 - start) / CLOCKS_PER_SEC;
if(duration_14 > Stop_time) return 0;
// Level_16[D]
for (int i15 = 0; i15 < Length_Range-1; i15++){
if(Counter(array[i15],Length_R) > 3) continue;
col[15] = array[i15];
p1[15] = Function_P1(col, 15, Length_R, Correction_Set, e_p1);
p2[15] = Function_P2(col, 15, Length_R, Correction_Set, e_p2);
p3[15] = Function_P3(col, 15, Length_R, Correction_Set, e_p3);
p4[15] = Function_P4(col, 15, Length_R, Correction_Set, e_p4);
identity[15] = p1[15] && p2[15] && p3[15] && p4[15];
if(!identity[15]) continue;
else {
clock_t finish_15 = clock();
double duration_15 = (double)(finish_15 - start) / CLOCKS_PER_SEC;
if(duration_15 > Stop_time) return 0;
// Level_17[D]
for (int i16 = 0; i16 < Length_Range-1; i16++){
if(Counter(array[i16],Length_R) > 3) continue;
col[16] = array[i16];
p1[16] = Function_P1(col, 16, Length_R, Correction_Set, e_p1);
p2[16] = Function_P2(col, 16, Length_R, Correction_Set, e_p2);
p3[16] = Function_P3(col, 16, Length_R, Correction_Set, e_p3);
p4[16] = Function_P4(col, 16, Length_R, Correction_Set, e_p4);
identity[16] = p1[16] && p2[16] && p3[16] && p4[16];
if(!identity[16]) continue;
else {
clock_t finish_16 = clock();
double duration_16 = (double)(finish_16 - start) / CLOCKS_PER_SEC;
if(duration_16 > Stop_time) return 0;
// Level_18[D]
for (int i17 = 0; i17 < Length_Range-1; i17++){
if(Counter(array[i17],Length_R) > 3) continue;
col[17] = array[i17];
p1[17] = Function_P1(col, 17, Length_R, Correction_Set, e_p1);
p2[17] = Function_P2(col, 17, Length_R, Correction_Set, e_p2);
p3[17] = Function_P3(col, 17, Length_R, Correction_Set, e_p3);
p4[17] = Function_P4(col, 17, Length_R, Correction_Set, e_p4);
identity[17] = p1[17] && p2[17] && p3[17] && p4[17];
if(!identity[17]) continue;
else {
clock_t finish_17 = clock();
double duration_17 = (double)(finish_17 - start) / CLOCKS_PER_SEC;
if(duration_17 > Stop_time) return 0;
// Level_19[D]
for (int i18 = 0; i18 < Length_Range-1; i18++){
if(Counter(array[i18],Length_R) > 3) continue;
col[18] = array[i18];
identity[18] = 1;
if(!identity[18]) continue;
else {
clock_t finish_18 = clock();
double duration_18 = (double)(finish_18 - start) / CLOCKS_PER_SEC;
if(duration_18 > Stop_time) return 0;
// Level_20[D]
for (int i19 = 0; i19 < Length_Range-1; i19++){
if(Counter(array[i19],Length_R) > 3) continue;
col[19] = array[i19];
identity[19] = 1;
if(!identity[19]) continue;
else {
clock_t finish_19 = clock();
double duration_19 = (double)(finish_19 - start) / CLOCKS_PER_SEC;
if(duration_19 > Stop_time) return 0;
// Level_21[D]
for (int i20 = 0; i20 < Length_Range-1; i20++){
if(Counter(array[i20],Length_R) > 3) continue;
col[20] = array[i20];
identity[20] = 1;
if(!identity[20]) continue;
else {
clock_t finish_20 = clock();
double duration_20 = (double)(finish_20 - start) / CLOCKS_PER_SEC;
if(duration_20 > Stop_time) return 0;
// Level_22[D]
for (int i21 = 0; i21 < Length_Range-1; i21++){
if(Counter(array[i21],Length_R) > 3) continue;
col[21] = array[i21];
identity[21] = 1;
if(!identity[21]) continue;
else {
clock_t finish_21 = clock();
double duration_21 = (double)(finish_21 - start) / CLOCKS_PER_SEC;
if(duration_21 > Stop_time) return 0;
// Level_23[D]
for (int i22 = 0; i22 < Length_Range-1; i22++){
if(Counter(array[i22],Length_R) > 3) continue;
col[22] = array[i22];
identity[22] = 1;
if(!identity[22]) continue;
else {
clock_t finish_22 = clock();
double duration_22 = (double)(finish_22 - start) / CLOCKS_PER_SEC;
if(duration_22 > Stop_time) return 0;
     
int HR = 0;
HR = Heavy_Row_inline(col);
if(HR < HR_Temp){
HR_Temp = HR;
cout<<"Smallest HR is : "<<HR<<endl;
cout<<"Find it !"<<endl;
// Output the Target Array
fout<<"******************"<<endl;
  for (int i=0;i<Length_Code;i++){
    fout<<col[i]<<" ";
  }
  fout<<"\n";
}
     
int TS = 0;
TS = Total_One_inline(col);
if(TS < TS_Temp){
TS_Temp = TS;
cout<<"Smallest TS is : "<<TS<<endl;
cout<<"Find it !"<<endl;
// Output the Target Array
fout<<"******************"<<endl;
  for (int i=0;i<Length_Code;i++){
    fout<<col[i]<<" ";
  }
  fout<<"\n";
}
     
     
     
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
 } 
  } 
fout.close();
}
int Function_P1 (int A[Length_Code], int column_i, int row_number, int Correction_Set[][4], int e_p1[1]){
         
int Correction;
         
int a_1 = A[column_i-0];
     
int A_1[row_number];
         
int A_S[row_number];
int c = 0;
         
for(int i=0;i<row_number;i++){
  if ((a_1&(1<<i))) A_1[i] = 1;
  else A_1[i] = 0;
}
         
for(int i=0;i<row_number;i++){
  A_S[i] = (e_p1[0] && A_1[i]);
}
for(int i=0;i<row_number;i++){
  c += pow(2,i)*A_S[i];
}
Correction = c;
       
//!=0
 if(Correction == 0) return 0;
       
//Correction是否在Correction_Set集合内
  for (int j=0;j<column_i;j++){
    for (int k=0;k<4;k++){
      if(Correction == Correction_Set[j][k])
      return 0;
    }
  }
Correction_Set[column_i][0] = Correction;
  return 1;
}
int Function_P2 (int A[Length_Code], int column_i, int row_number, int Correction_Set[][4], int e_p2[2]){
         
int Correction;
         
int a_1 = A[column_i-1];
int a_2 = A[column_i-0];
     
int A_1[row_number];
int A_2[row_number];
         
int A_S[row_number];
int c = 0;
         
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
  c += pow(2,i)*A_S[i];
}
Correction = c;
       
//!=0
 if(Correction == 0) return 0;
       
//!= previous P in column_i
for(int i=0;i<1;i++){
 if(Correction == Correction_Set[column_i][i]) return 0;
}
       
//Correction是否在Correction_Set集合内
  for (int j=0;j<column_i;j++){
    for (int k=0;k<4;k++){
      if(Correction == Correction_Set[j][k])
      return 0;
    }
  }
Correction_Set[column_i][1] = Correction;
  return 1;
}
int Function_P3 (int A[Length_Code], int column_i, int row_number, int Correction_Set[][4], int e_p3[3]){
         
int Correction;
         
int a_1 = A[column_i-2];
int a_2 = A[column_i-1];
int a_3 = A[column_i-0];
     
int A_1[row_number];
int A_2[row_number];
int A_3[row_number];
         
int A_S[row_number];
int c = 0;
         
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
  c += pow(2,i)*A_S[i];
}
Correction = c;
       
//!=0
 if(Correction == 0) return 0;
       
//!= previous P in column_i
for(int i=0;i<2;i++){
 if(Correction == Correction_Set[column_i][i]) return 0;
}
       
//Correction是否在Correction_Set集合内
  for (int j=0;j<column_i;j++){
    for (int k=0;k<4;k++){
      if(Correction == Correction_Set[j][k])
      return 0;
    }
  }
Correction_Set[column_i][2] = Correction;
  return 1;
}
int Function_P4 (int A[Length_Code], int column_i, int row_number, int Correction_Set[][4], int e_p4[3]){
         
int Correction;
         
int a_1 = A[column_i-2];
int a_2 = A[column_i-1];
int a_3 = A[column_i-0];
     
int A_1[row_number];
int A_2[row_number];
int A_3[row_number];
         
int A_S[row_number];
int c = 0;
         
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
  c += pow(2,i)*A_S[i];
}
Correction = c;
       
//!=0
 if(Correction == 0) return 0;
       
//!= previous P in column_i
for(int i=0;i<3;i++){
 if(Correction == Correction_Set[column_i][i]) return 0;
}
       
//Correction是否在Correction_Set集合内
  for (int j=0;j<column_i;j++){
    for (int k=0;k<4;k++){
      if(Correction == Correction_Set[j][k])
      return 0;
    }
  }
Correction_Set[column_i][3] = Correction;
  return 1;
}
   
int Total_One(int A[Length_Code][Length_R], int column_number, int row_number){
  int counter = 0;
  for(int i=0;i<Length_R;i++){
    for (int k=0;k<Length_Code;k++){
      if(A[k][i]==1) counter++;
      else continue;
    }
  }
  return counter;
}
   
int Total_One_inline (int col[Length_Code]){
  int ar_in[Length_Code];
  int ar_out[Length_Code][Length_R];
    
  for(int i=0;i<Length_Code;i++){
    ar_in[i] = col[i];
  }
    
//10进制格式转换到2进制格式
  for(int j=0; j<Length_Code; j++){
    for(int i=0;i<Length_R;i++){
      if ((ar_in[j]&(1<<i))) ar_out[j][i] = 1;
      else ar_out[j][i] = 0;
    }
  }
      
  int total_one_num;
  total_one_num = Total_One(ar_out, Length_Code, Length_R);
  return total_one_num;
}
   
int Heavy_Row(int A[Length_Code][Length_R], int column_number, int row_number){
  int Num_row[Length_R];
  int counter = 0;
  for(int i=0;i<Length_R;i++){
    counter = 0;
    for (int k=0;k<Length_Code;k++){
      if(A[k][i]==1) counter++;
      else continue;
    }
    Num_row[i]=counter;
   
  }
  int Heavy_Num = Num_row[0];
  for (int i=1;i<Length_R;i++){
    if(Num_row[i]>Heavy_Num){
      Heavy_Num = Num_row[i];
    }
  }
  return Heavy_Num;
}
   
int Heavy_Row_inline (int col[Length_Code]){
  int ar_in[Length_Code];
  int ar_out[Length_Code][Length_R];
   
  for(int i=0;i<Length_Code;i++){
    ar_in[i] = col[i];
  }
   
//10进制格式转换到2进制格式
  for(int j=0; j<Length_Code; j++){
    for(int i=0;i<Length_R;i++){
      if ((ar_in[j]&(1<<i))) ar_out[j][i] = 1;
      else ar_out[j][i] = 0;
    }
  }
   
  int heavy_row_num;
  heavy_row_num = Heavy_Row(ar_out, Length_Code, Length_R);
  return heavy_row_num;
}
   
int Counter(int a, int row_number){
int A[row_number];
int counter = 0;
for(int i=0;i<row_number;i++){
  if ((a&(1<<i))) A[i] = 1;
  else A[i] = 0;
}
for(int i=0;i<row_number;i++){
    if(A[i]==1) counter++;
    else continue;
}
return counter;
}
