#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

long long int p, q, n, e, fi, d, k;

int isPrime(long long n) {
    if (n == 2 || n == 3) {
        return 1;
    } else if (n%2 == 0) {
        return 0;
    } else {
        int i;

        for (i = 3; i <= sqrt(n); i+=2) {
            if (n%i == 0) {
                return 0;
            }
        }
        return 1;
    }
}

int calcGCD(long long a, long long b) {
    if (a%b == 0) {
        return b;
    } else {
        calcGCD(b, a%b);
    }
}

int euclidesExtended(long long int a,long long int b,long long int *x,long long int *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }

    long long int x1, y1;
    long long int gcd = euclidesExtended(b%a, a, &x1, &y1);


    *x = y1 - (b/a) * x1;
    *y = x1;
    
    
    if(*x < 0) {
        *x += b;
    }

    return gcd;
}

int modularPow(int base, int exponent, int modulus) {
    int c = 1;
    int i;

    for (i = 0; i < exponent; i++) {
        c = (c * base) % modulus;
    }
    return c;
}

void mapAlphabet(char *msg, int file_size_) {
    int i = 0;
    int j = 0;
    /*
    printf("\nMessage Buffer:");
    for (i = 0; i < strlen(msg); i++) {
        printf("%d ", msg[i]);
    }
    */
    for (i = 0; i < file_size_; i++) {
        msg[i] = tolower(msg[i]);
    }

    for (j = 0; j < i; j++) {
        if (msg[j] == 32) {
            msg[j] = 26;
        } else {
            msg[j] = msg[j] - 97;
        }
    }
    /*
    printf("\nMessage Buffer 2:");
    for (i = 0; i < strlen(msg); i++) {
        printf("[%d]", msg[i]);
    }
    */
}

void encrypt() {

    printf("\tType the public keys N and E.\n\t");

    scanf("%lld", &n);
    printf("\t");
    scanf("%lld", &e);

    FILE *untouched_message = fopen("2_1_untouched_message.txt", "rt");
    FILE *encrypted_message = fopen("2_2_encrypted_message.txt", "w");

    char *buffer;
    fseek(untouched_message , 0 , SEEK_END); // Ajusta a posição do indicador com base em uma referência. SEEK_END (final do arquivo) é a referência e 0 é posição à partir da referência. Isso significa que o indicador vai permanecer na referência. O resultado dessa função é que o indicador está ajustado para indicar o final do arquivo.
    int file_size = ftell(untouched_message); // A função "ftell" retorna a posição do indicador. Nesse caso, ele retorna a posição ajustada pela "fseek", ou seja, o valor da posição do último caractere do arquivo. Essa posição é interpretada como o tamanho do arquivo em bytes (1 caractere = 1 byte).
    rewind(untouched_message); // Essa função ajusta o indicador para o começo do arquivo, como era antes da função "fseek".
    buffer = malloc(sizeof(char) *file_size); // Faz uma atribuição ao "buffer" reservando um espaço na memória igual ao tamanho do arquivo.
    //mapped_buffer = malloc(sizeof(char) *file_size);
    fread(buffer, 1, file_size, untouched_message); // Copia os caracteres do arquivo para o "buffer".

    //printf("file_size: %d strlen(buffer): %d\n", file_size, strlen(buffer));
    int i, current;

    printf("\n\t==========\n\n\tMessage from 2_1_untouched_message.txt:\n\t");
    for (i = 0; i < file_size; i++) {
        printf("%d ", buffer[i]);
    }

    mapAlphabet(buffer, file_size);

    printf("\n\tMapped Message:\n\t");
    for (i = 0; i < file_size; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n\tEncrypted Message:\n\t");
    for (i = 0; i < file_size; i++) {
        current = modularPow(buffer[i], e, n);
        fprintf(encrypted_message, "%d ", current);
        printf("%d ", current);
    }

    printf("\n\n\t==========\n\n");

    free(buffer);
    fclose(untouched_message);
    fclose(encrypted_message);
}

void decrypt() {
    int i, file_size = 0;
    // Getting numbers from user: P, Q, E
    //printf("\nP: [%lld] Q: [%lld] E: [%lld]\n", p, q, e);
    printf("\tType one pair of prime numbers P and Q and an exponent E relativaly prime to (p - 1)(q- 1).\n\t");

    scanf("%lld", &p);
    printf("\t");
    scanf("%lld", &q);

    while (!isPrime(p) || !isPrime(q)) {
        printf("\n\n\tP or Q is not prime. Please, type them again.\n\t");
        scanf("%lld", &p);
        printf("\t");
        scanf("%lld", &q);
    }
    printf("\tType the exponent E:\n\t");
    scanf("%lld", &e);
    //printf("\nP: [%lld] Q: [%lld] E: [%lld]\n", p, q, e);

    n = p*q;
    fi = (p - 1)*(q - 1);
    euclidesExtended(e, fi, &d, &k);

    char aux[100];
    long long int number;

    FILE *encrypted_message = fopen("2_2_encrypted_message.txt", "rt");
    FILE *decrypted_message = fopen("3_decrypted_message.txt", "w");

    while(fscanf(encrypted_message, "%s", aux) != EOF) {
        file_size++;
    }
    fclose(encrypted_message);
    encrypted_message = fopen("2_2_encrypted_message.txt", "rt");

    char data;
    printf("\n\t==========\n\n\tDecrypted message:\n\t\"");
    for(i = 0;i < file_size; i++) {
        fscanf(encrypted_message, "%lld", &number);

        number = modularPow(number, d, n) + 97;
        //printf("Number char: %c Number int: %lld\n", number, number);
        if (number == 26+97) {
            number = 32;
            //printf("Entrei aqui\n");
            fprintf(decrypted_message, "%c", number);
            printf("%c", number);
        }
        else {
            fprintf(decrypted_message, "%c", number-32);
            printf("%c", number-32);
        }
    }
    printf("\"\n\n\tIt is also saved in the file: 3_decrypted_message.txt\n\n\t==========\n\n");

    fclose(encrypted_message);
    fclose(decrypted_message);
}

void generate_keys() {

    // Getting numbers from user P, Q, E
    //printf("\nP: [%lld] Q: [%lld] E: [%lld]\n", p, q, e);
    printf("\tType one pair of prime numbers P and Q and an exponent E relativaly prime to (p - 1)(q- 1).\n\t");

    scanf("%lld", &p);
    printf("\t");
    scanf("%lld", &q);

    while (!isPrime(p) || !isPrime(q)) {
        printf("=====\n\n\tP or Q is not prime. Please, type them again.\n\t");
        scanf("%lld", &p);
        getchar();
        printf("\t");
        scanf("%lld", &q);
        getchar();
    }
    printf("\tType the exponent E:\n\t");
    scanf("%lld", &e);
    //getchar();

    while (calcGCD(e, ((p-1)*(q-1))) != 1) {
        printf("\n\tE is not coprime in relation to (p - 1)(q- 1). Please, type again.\n\t");
        scanf("%lld", &e);
    }

    // Saving Public keys
    FILE *publicKeyFile;
    publicKeyFile = fopen("1_1_public_key.txt","w");
    fprintf(publicKeyFile, "%lld\n%lld\n", p*q, e);
    fclose(publicKeyFile);

    fi = (p - 1)*(q - 1);
    euclidesExtended(e, fi, &d, &k);

    // Saving Private keys
    FILE *privateKeyFile = fopen("1_2_private_key.txt", "w");
    fprintf(privateKeyFile, "%lld\n%lld\n%lld", p, q, d);
    fclose(privateKeyFile);

    printf("\n\n\t==========\n\n\tPublic key:\tn: [%lld] e: [%lld]\n", p*q, e);
    printf("\tPrivate key:\tp: [%lld] q: [%lld] d: [%lld]\n", p, q, d);
    printf("\n\tThe keys are also stored in the following files:\n\n\t\t1_1_public_key.txt\n\t\t1_2_private_key.txt\n\n\t==========\n\n");
}

void main() {

    int choice;
    /*
    euclidesExtended(e, fi, &d, &k);
    printf("%s %lld\n", "Chave privada: ", d);

    printf("Mensagem:              ");
    for (i = 0; i < strlen(msg); i++) {
        printf("%c ", msg[i]);
    }
    encrypt(msg, n, e, codifiedMsg);

    printf("\nMensagem codificada:   ");
    for (i = 0; i < strlen(msg); i++) {
        printf("%d ", codifiedMsg[i]);
    }

    printf("\nMensagem decodificada: ");
    desencrypt(codifiedMsg, strlen(msg), p*q, d, decodefiedMsg);

    printf("\nMensagem remapeada:    ");
    for (i = 0; i < strlen(msg); i++) {
        printf("%d ", decodefiedMsg[i] - 97);
    }

    printf("\nMensagem original:     ");
    for (i = 0; i < strlen(msg); i++) {
        printf("%c ", decodefiedMsg[i]);
    }
    */
    printf("\n\tWelcome to Alien Crypt! Please choose:\n\n");
    do {
        printf("\t1 Public Key Generator\n");
        printf("\t2 Encrypt (from 2_1_untouched_message.txt to 2_2_encrypted_message.txt)\n");
        printf("\t3 Decrypt (from 2_2_encrypted_message.txt to 3_decrypted_message.txt)\n");
        printf("\t4 Exit\n\t");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
            {
                generate_keys();
                break;
            }
            case 2:
            {
                encrypt();
                break;
            }
            case 3:
            {
                decrypt();
                break;
            }
            case 4:
            {
                printf("\n\t==========\n\n\tBye!\n\n\t==========");
                break;
            }
        }
    } while (choice != 4);

}
