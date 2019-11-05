#!/bin/bash

echo -e

echo "Iniciando o processamento em Intel(R) Core(TM) i5-7500 CPU @ 3.40GHz"

echo -e


# ------------------------------ TRECHO DE EXPORT  --------------------------------
echo -e

echo "Exportando variáveis de ambiente."
export PATH=/home/soft/likwid/bin:/home/soft/likwid/sbin:$PATH
export LD_LIBRARY_PATH=/home/soft/likwid/lib:$LD_LIBRARY_PATH
sleep 2

echo -e


make


mkdir resultados
mkdir likwid

valores=(32 50 64 100 128 200 256 300 400 512 1000 1024 2000 2048 3000 4000 4096 5000 10000)


echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

echo "Começando os for"

# ------------------------------ TRECHO DE EXECUÇÃO  --------------------------------
for x in ${valores[@]};
do 
	echo "Executando teste para nx=ny=${x}...";
	likwid-perfctr -C 3 -g L3 -m ./pdeSolver -nx $x -ny $x -i 10 -o resultados/"saida${x}_L3.txt" | grep "L3 bandwidth" >> likwid/"saida_L3.txt";
	likwid-perfctr -C 3 -g L2CACHE -m ./pdeSolver -nx $x -ny $x -i 10 -o resultados/"saida${x}_L2CACHE.txt" | grep "L2 miss ratio" >> likwid/"saida_L2.txt";
	likwid-perfctr -C 3 -g FLOPS_AVX -m ./pdeSolver -nx $x -ny $x -i 10 -o resultados/"saida${x}_FLOPS.txt" | grep "Packed DP MFLOP/s" >> likwid/"saida_FLOPS.txt";
	echo -e;
done 

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor  