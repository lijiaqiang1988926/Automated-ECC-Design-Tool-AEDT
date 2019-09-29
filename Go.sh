# This is an example for 3-bit burst error code
#file_name: Go.sh
Length_D=12
Length_R=7
Length_Code=19
Pattern_Cor_Num=4
Pattern_Dec_Num=0
Stoping_time=100
Mode=0
Injection_Num=1

#ECC_Generate_Program
./ECC_Search_Gen $Length_D $Length_R $Pattern_Cor_Num $Pattern_Dec_Num $Stoping_time $Mode
./ECC_Search

#Check_Program
./Check_Gen $Length_D $Length_R $Pattern_Cor_Num $Pattern_Dec_Num $Stoping_time $Mode
./Check

#File Preparation
head -n $(($Pattern_Cor_Num+1)) ./Set.txt > EP_in.txt
tail -n 2 ./1.txt > Matrix.txt

#Error_Pattern_Generation
./Error_Pattern_Gen $Length_Code $Pattern_Cor_Num

#Fault_Injection_Generation
./Fault_Injection_Gen $Length_D $Length_R $Pattern_Cor_Num $Pattern_Dec_Num $Stoping_time
./Fault_Inject $Injection_Num
