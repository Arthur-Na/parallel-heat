# Heat diffusion

This project performs heat propagation in a 3-dimension room.
It uses different techniques and evaluates performances in each different methods.

## Getting started

### Prerequisites

In order to launch this project, you'll need:

* CMake version 3.6 or higher
* An nVidia graphic card
* nvcc version 8.1 or higher
* gcc version 8.1 or higher

### Installation

Do the following commands to generate the project's binaries

```
mkdir build && cd build
cmake ..
make
```

### Launching the projet

Three binaries are now under the **build** directory.

#### CPU project

To launch the project to see CPU performance in sequential and parallel time, use the following command:

```
cd build
./parallel-heat input_file output_file nb_iter
```

#### GPU project

To launch the project to see GPU performance, use the following command:
```
cd build
./parallel-heat-cuda input_file output_file nb_iter
```

#### Graphical project

To launch the project and see a visual output, use the following command:
```
cd build
./parallel-heat-view input_file output_file nb_iter
```
If you want to use an nVidia graphic card to run this program, you might need to use the following command:
```
cd build
sudo optirun ./parallel-heat-view input_file output_file nb_iter
```
See the [Bumblebee](https://github.com/Bumblebee-Project/Bumblebee) github page for more information.

### Generating more example files

To generate new input files, use the [file_generator.py](file_generator.py) file.

Use
```
./file_generator.py -h
```
for more informations

## Authors

* **Arthur NAEGELY** - [Arthur-NA](https://github.com/Arthur-NA)
* **Nicolas LUGASSY** - [Ringokilol](https://github.com/Ringokilol)
* **Hippolyte MAES** - [HippolyteMaes](https://github.com/HippolyteMaes)

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
