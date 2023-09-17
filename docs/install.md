## Installation Guide
Follow these steps to compile, run, and optionally install passmx:

1. Clone the Repository:
   Clone this repository to your local machine using Git.

```bash
git clone https://github.com/tory1103/passmx.git
```

2. Navigate to the Repository Directory:
   Open a terminal and navigate to the root directory of the cloned repository.

```bash
cd passmx
```

3. Compile the Program:
   You first need to create a build dir and run cmake:

```bash
# Create build directory and change to it
# Prepare compilation with cmake
mkdir build
cd build
cmake ..

# After preparing compilation, run make to compile code
make
```

4. Run the Program:
   After compilation, run the program by providing the required arguments. Use the following command-line format:

```bash
# Replace 'secretkey' and 'passphrase' with your desired secret and passphrase.
# Include optional options like -l to specify the password length
# and -v for verbose output.
# For detailed information use -h option.
./passmx -s 'secretkey' -p 'passphrase' ... [OPTIONS]
```