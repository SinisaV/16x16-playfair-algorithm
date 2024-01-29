# Encryption and Decryption Program

This C++ program performs encryption and decryption using a key matrix. It takes an input text file, a key, and the operation (encryption or decryption) as command-line arguments.

## Usage

```
./program [e/d] "key string" input.txt
```
- ***[e/d]:*** Operation to be performed. Use e for encryption and d for decryption.
- ***key string:*** File containing the key used for encryption or decryption.
- ***input.txt:*** Input text file to be encrypted or decrypted.

### Compilation
Compile the program using a C++ compiler. For example:
```
g++ main.cpp -o program
```

### Execution
Run the compiled program with appropriate command-line arguments:

```
./program e "key string" input.txt
or
./program d "key string" input.txt
```

**Example:**
```
input.txt:
aaaba

key:
playfair

out_e.txt:
y[y

out_d.txt:
aaaba
```

### Key Format
The key should be a string containing unique characters used for constructing the key matrix. The program generates a 16x16 key matrix based on this key.
