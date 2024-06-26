# atypik

A TYPe Inference Kit for B.

This project aims to infered a type for each expression in a bxml or pog file. 
It will generate a new file adding a RichTypesInfo tag.

Formats bxml and pog are used by Atelier B to represent B components
and their proof obligations.

The development of atypik has been partially funded by
[ANR](<https://anr.fr>) through project
[BLaSST](<https://anr.fr/Project-ANR-21-CE25-0010>) "Enhancing B Language Reasoners with SAT and SMT Techniques".

## Installation

1. Clone the repository with its submodules.

```bash
git clone --recurse-submodules https://github.com/CLEARSY/atypik.git
```

2. Install `cvc5` in the `smt-switch` folder.

```bash
cd atypik/atypik/3rdparty/smt-switch
./contrib/setup-cvc5.sh
./configure.sh --cvc5
cd build
make -j8
```

3. Compile 

```bash
cd ../../..
mkdir ../../build-atypik
cd ../../build-atypik
cmake ../atypik/atypik
cmake --build . --target all
```

A runnable file named `atipyk` will be generated in the `build-atypik` folder.

## Usage 

To use atypik the input of the file should be given in argument. It may be `--bxml` or `--pog`.

```bash
./atypik --format path/to/file.format --output path/to/output.format
```

Please note that the argument `--output` is optional and if it is not given, the result file will be displayed on `stdout`.

To run the tests in the build folder, run the following command:

```bash
ctest -j8 --output-on-failure
```

## Adding tests

To add a test, a folder should be created into the `test/data` directory. It should contain a folder `src` with all the bxml sources and a folder `expected` with a csv file containing all the types of the expression. This file can be created by the `test/template-generator` program wich takes the same arguments as atypik. Note that this file can be edited after being generated.

Finally, in the file `test/CTest.cmake`, the folder containing the new test should be added into the `BxmlTestFolders` variable as many time as it contains a file. The name of each file should then be added in the `BxmlToTest` variable. 




