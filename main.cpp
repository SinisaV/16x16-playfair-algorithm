#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

typedef unsigned int uint;
bool BranjeStevil(string &A, const char *filename);
bool ZapisStevil(const string &A, const char *filename);
vector<unsigned char> MatrikaKljuca(string kljuc);
string PredobdelavaVhoda(const string& A);
string encrypt(const string& A, vector<unsigned char> matrika);
string decrypt(const string& A, vector<unsigned char> matrika);

int main(int argc, const char** argv) {
    if (argc != 5) {
        return 0;
    }
    string B;
    BranjeStevil(B, argv[4]);
    cout << B << endl;

    vector<unsigned char> matrika;
    matrika = MatrikaKljuca(argv[3]);

    string S;
    S = PredobdelavaVhoda(B);
    cout << "\n\nPreobdelava vhoda: " << S << "\n";

    if (argv[2][0] == 'e') {
        cout << "Enkripcija\n";

        string sifriranTekst;
        sifriranTekst = encrypt(S, matrika);
        ZapisStevil(sifriranTekst, "out_e.txt");

    }
    else if ((argv[2][0] == 'd')) {
        cout << "Dekripcija\n";

        string sifriranTekst;
        sifriranTekst = encrypt(S, matrika);

        string desifriranTekst;
        desifriranTekst = decrypt(sifriranTekst, matrika);
        ZapisStevil(desifriranTekst, "out_d.txt");
    }

    return 0;
}

bool BranjeStevil(string& A, const char* filename) {
    ifstream input(filename);
    string st, result;

    if (!input.is_open()) {
        return false;
    }

    while (input >> st) {
        result += st;
    }

    input.close();

    A = result;

    return true;
}

bool ZapisStevil(const string &A, const char *filename) {
    ofstream output(filename);

    if (!output.is_open()) {
        return false;
    }

    output << A;

    output.close();

    return true;
}

vector<unsigned char> MatrikaKljuca(string kljuc) {
    vector<unsigned char> matrika(256);

    string noviKljuc;
    bool f;

    for (int i = 0; i < kljuc.length(); i++) {
        f = false;
        for (int j = 0; j < noviKljuc.length(); j++) {
            if (kljuc[i] == noviKljuc[j]) {
                f = true;
                break;
            }
        }
        if (!f) {
            noviKljuc += kljuc[i];
        }
    }
    // cout << noviKljuc << endl;

    for (int i = 0; i < noviKljuc.size(); i++) {
        matrika[i] = noviKljuc[i];
    }

    int h = 0;
    for (int i = noviKljuc.length(); i < matrika.size(); i++) {
        f = false;
        for (int j = 0; j < noviKljuc.length(); j++) {
            if (noviKljuc[j] == static_cast<unsigned char>(h)) {
                f = true;
                i--;
                break;
            }
        }
        if (!f) {
            matrika[i] = static_cast<unsigned char>(h);
        }
        h++;
    }

    cout << endl;
    for (int i = 0; i < matrika.size(); i++) {
        cout << matrika[i] << " ";
    }

    return matrika;
}

string PredobdelavaVhoda(const string& A) {

    string B(A.length() + 1, ' ');

    for (int i = 0; i < A.length(); i += 2) {
        if (A[i] == A[i + 1]) {
            B[i] = A[i];
            // B[i+1] = 'X';
            B[i + 1] = '\0';
        } else if (i + 1 == A.length()) {
            B[i] = A[i];
            // B[i+1] = 'Y';
            B[i + 1] = static_cast<char>(3);
        } else {
            B[i] = A[i];
            B[i + 1] = A[i + 1];
        }
    }

    /*for (int i = 0; i < B.size(); i++) {
        cout << B[i];
    }*/

    return B;
}

string encrypt(const string& A, vector<unsigned char> matrika) {
    string sifrirantekst;

    for (int i = 0; i < A.length(); i += 2) {
        char znakA = A[i];
        char znakB = A[i+1];

        int i_poz = -1;
        int j_poz = -1;
        int k_poz = -1;
        int f_poz = -1;

        for (int row = 0; row < 16; row++) {
            for (int col = 0; col < 16; col++) {
                if (matrika[row * 16 + col] == znakA) {
                    i_poz = row;
                    j_poz = col;
                }
                if (matrika[row * 16 + col] == znakB) {
                    k_poz = row;
                    f_poz = col;
                }
            }
        }

        char sifriranoA = ' ', sifriranoB = ' ';

        if (i_poz != k_poz && j_poz != f_poz) {
            // (i != k) && (j != f)
            sifriranoA = matrika[i_poz * 16 + f_poz];
            sifriranoB = matrika[k_poz * 16 + j_poz];
        }
        else if (i_poz == k_poz) {
            // (i==k)
            sifriranoA = matrika[i_poz * 16 + (j_poz + 1) % 16];
            sifriranoB = matrika[k_poz * 16 + (f_poz + 1) % 16];
        }
        else if (j_poz == f_poz) {
            // (j==f)
            sifriranoA = matrika[((i_poz + 1) % 16) * 16 + j_poz];
            sifriranoB = matrika[((k_poz + 1) % 16) * 16 + f_poz];
        }

        sifrirantekst += sifriranoA;
        sifrirantekst += sifriranoB;
    }

    cout << "Sifriran text: " << sifrirantekst << endl;
    return sifrirantekst;
}

string decrypt(const string& A, vector<unsigned char> matrika) {
    string desifriranTekst;
    char prejsnji_znak = ' ';

    for (int i = 0; i < A.length(); i += 2) {
        char znakA = A[i];
        char znakB = A[i + 1];

        int i_poz = -1;
        int j_poz = -1;
        int k_poz = -1;
        int f_poz = -1;

        for (int row = 0; row < 16; row++) {
            for (int col = 0; col < 16; col++) {
                if (matrika[row * 16 + col] == znakA) {
                    i_poz = row;
                    j_poz = col;
                }
                if (matrika[row * 16 + col] == znakB) {
                    k_poz = row;
                    f_poz = col;
                }
            }
        }

        char desifriranoA = ' ';
        char desifriranoB = ' ';

        if (i_poz != k_poz && j_poz != f_poz) {
            // (i != k) && (j != f)
            desifriranoA = matrika[i_poz * 16 + f_poz];
            desifriranoB = matrika[k_poz * 16 + j_poz];
        } else if (i_poz == k_poz) {
            // (i==k)
            desifriranoA = matrika[i_poz * 16 + (j_poz - 1 + 16) % 16];
            desifriranoB = matrika[k_poz * 16 + (f_poz - 1 + 16) % 16];
        } else if (j_poz == f_poz) {
            // (j==f)
            desifriranoA = matrika[((i_poz - 1 + 16) % 16) * 16 + j_poz];
            desifriranoB = matrika[((k_poz - 1 + 16) % 16) * 16 + f_poz];
        }
        prejsnji_znak = desifriranoA;

        /*if (desifriranoA == '\0') {
            desifriranoA = prejsnji_znak;
        }*/
        if (desifriranoB == '\0') {
            desifriranoB = prejsnji_znak;
        }

        /*if (desifriranoA == static_cast<char>(3))
            desifriranoA = ' ';*/
        if (desifriranoB == static_cast<char>(3)) {
            desifriranoB = ' ';
        }

        desifriranTekst += desifriranoA;
        desifriranTekst += desifriranoB;
    }

    cout << "Desifriran text: " << desifriranTekst << endl;
    return desifriranTekst;
}