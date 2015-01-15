# kolmogorov-stephens-test
Simple C code for applying Kolmogorov test and test Stephens result

# Compiling the code

## Compilation for Linux 
To compile the code just run make in the project dir.
```bash
make
```

After compilation 3 executables will be generated: `deviation_statistics`, `simulate_data` and `stephens`

## Compilation for Window
Download the MinGW compiler: 'http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download'
In MinGW terminal and run the command:
```bash
mingw32-make
```
After compilation 3 executables will be generated: `deviation_statistics.exe`, `simulate_data.exe` and `stephens.exe`

# Running the code
This project contains 3 executalbe programs each of them will be described in the next chapter

## Program `simulate_data`
This program is used to generate set of random variables.
### Usage:
```bash
./simulate_data <type> <file_name> <number_of_points>
     <type> - currently supported - are gauss or pseudo gauss.
     <file_name> - output file name
    <number_of_points> - the number of the generated points
```
### Example for Linux:
```bash
./simulate_data gauss test_1000.txt 1000
```

### Example for Windows:
```bash
simulate_data.exe gauss test_1000.txt 1000
```

This example will produce file `test_1000.txt` witl the following content:
```bash
$ head -n 5 test_1000.txt 
#1000
0.693775
2.050064
-0.022952
-0.934158
$
```


## Program `stephens`
M. A. Stephens,
"EDF Statistics for Goodness of Fit and Some Comparisons"
J. of Amer. Statist. Assoc., 69, 730-737 (1974).
http://www.math.utah.edu/~morris/Courses/6010/p1/writeup/ks.pdf
Empirical Distribution Function EDF  F[i].
Reject hypothesis at chosen level of significance.
(double) sqN= sqrt(n) - .01 + .85/sqrt(n);
Probability{max [sqN*fabs(F-F^*)]>D}=alpha
Table 1, 1.3 Modification, mu and sigma unknown.
D_alpha refers to upper tail critical value of the 
Kolmogorov-Smirnov statistics at level alpha,
giving a (1-alpha) confidential interval.
--------------
alpha    D
==============
15%     0.775
10%     0.819
5%      0.895
2.5%    0,995
1.0%    1.035
--------------

### Usage:
```bash
./stephens <file_name>
```
### Example for Linux:
```bash
./stephens test_1000.txt
```
### Example for Windows:
```bash
stephens.exe test_1000.txt
```

The first line of the input file should contain the number of random variables in the file. Like:
```bash
$ head -n 5 test_1000.txt 
#1000
0.693775
2.050064
-0.022952
-0.934158
$
```

## Program `deviation_statistics`
This program is used to calculate sqrt(N)*D_N as function of N

### Example for Linux
```bash
./deviation_statistics test 1000000
```
### Example for Windows
```bash
deviation_statistics.exe test 1000000
```
