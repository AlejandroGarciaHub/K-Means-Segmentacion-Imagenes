for var1 in 42049
do
	for var2 in 10
	do
		for var3 in 3
		do
			./K-Means-Segmentacion-Mejorado images/test/$var1.jpg $var2 centroidsTest/$var1/$var3.xml imagesTest/Estandar/$var1/$var3.jpg centroidsTest/$var1/resultadosEstandar.txt 0 100
		done
	done	
done

