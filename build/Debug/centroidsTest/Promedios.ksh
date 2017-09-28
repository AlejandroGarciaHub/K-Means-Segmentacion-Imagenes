

for var1 in 86016 42049 260058
do
		
			./average $var1/resultadosEstandar.txt $var1/totalEstandar.txt 30 3
			echo $var1" "10>>$var1/totalEstandar.txt
        
done

for var1 in 86016 42049 260058
do

./average $var1/resultadosMejora.txt $var1/totalMejora.txt 30 3
echo $var1" "10>>$var1/totalMejora.txt

done
