#include <iostream>
#include <bitset>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<string.h>
using namespace std;
struct code {
    char c;
    string code;
};

struct node {
    char c;
    int freq;
    struct node* next;
    struct node* left;
    struct node* right;
    string code = "";
};
node* head;
node* root;
int inqueue(node* n) {
    if (head == NULL) {
        head = n;
        return 1;
    }
    else if (head->freq > n->freq) {
        n->next = head;
        head = n;
        return 1;
    }
    else {
        node* temp = head;
        while (temp->next != NULL && temp->next->freq < n->freq) {
            temp = temp->next;
        }
        n->next = temp->next;
        temp->next = n;
        return 1;
    }
}
int dequeue(node** n) {
    if (head == NULL) {
        return 0;
    }
    else {
        *n = head;
        head = head->next;
        (*n)->next = NULL;
        return 1;
    }
}

char filename[100] = { 0 };
int x;
int func1()
{
    int frequency[256] = { 0 };

    printf("Enter Filename (full path\\name.txt) :");
    scanf_s("%s", filename, 100);
    FILE* f;
    if (fopen_s(&f, filename, "r") != 0)
    {
        printf("File not found\n");
        return 0;
    }
    char c;
    while ((c = getc(f)) != EOF)
    {
        frequency[(int)c]++;
    }
    fclose(f);

    x = strlen(filename);

    int i;
    for (i = 0; i < 256; i++)
    {
        if (frequency[i] > 0)
        {
            node* n;
            n = (struct node*) new (struct node);
            n->c = (char)i;
            n->freq = frequency[i];
            n->next = NULL;
            n->left = NULL;
            n->right = NULL;
            inqueue(n);
        }
    }
    node* n1;
    node* n2;
    while (1)
    {
        dequeue(&n1);
        if (dequeue(&n2) == 0)
            break;

        node* n3;
        n3 = (struct node*) new (struct node);
        n3->left = n1;
        n3->right = n2;
        n3->next = NULL;
        n3->c = 0;
        n3->freq = n1->freq + n2->freq;
        inqueue(n3);
    }
    root = n1;
    return 1;
}

FILE* f1;
char codfile[100] = { 0 };
int getcoderecursion(struct node* node, string path, string code)
{

    if (node == NULL)
        return 0;

    path = path + code;

    if (node->left != NULL && node->right != NULL)
    {
        getcoderecursion(node->left, path, "0");
        getcoderecursion(node->right, path, "1");
    }

    else
    {
        node->code = path.c_str();
        printf("%c = %s\n", node->c, node->code.c_str());
        fprintf(f1, "%c = %s\n", node->c, node->code.c_str());
    }
}
int getcode(struct node* node)
{
    string path = "";

    strcpy(codfile, filename);
    codfile[x - 3] = 'c';
    codfile[x - 2] = 'o';
    codfile[x - 1] = 'd';
    codfile[x] = '\0';

    f1 = fopen(codfile, "w");
    if (!f1)
    {
        printf("File cannot be created");
        return -1;
    }

    getcoderecursion(node, path, "");
    fclose(f1);
}
int binaryToDecimal(string num)
{
    int value = 0;
    int base = 1;
    for (int i = num.length() - 1; i >= 0; i--)
    {
        if (num[i] == '1')
            value += base;
        base = base * 2;
    }
    return value;
}

int padding = 0;       //padding counter
char comfile[100] = { 0 };
void compression()
{
    code codes[127];
    FILE* f;
    char str[CHAR_MAX];
    f = fopen(codfile, "r");
    int count = 0;
    while (fgets(str, CHAR_MAX, f) != NULL)
    {
        if (strlen(str) == 1)
            continue;
        code c;
        c.c = str[0];

        if (str[1] == '=')
            c.c = '\n';
        string tok = strtok(str, "=");
        tok = strtok(NULL, "=");
        memmove(&tok[strlen(tok.c_str()) - 1], &tok[(strlen(tok.c_str()) - 1) + 1], strlen(tok.c_str()) - (strlen(tok.c_str()) - 1));
        memmove(&tok[0], &tok[0 + 1], strlen(tok.c_str()) - 0);
        c.code = tok;

        codes[count] = c;
        count++;
    }
    fclose(f);

    string encoded_text = "";
    f = fopen(filename, "r");

    while (fgets(str, CHAR_MAX, f) != NULL)
    {
        for (int i = 0; i < strlen(str); i++)
        {
            char enc[10] = "";
            for (int j = 0; j < count; j++)
            {
                if (str[i] == codes[j].c)
                {
                    strcat(enc, codes[j].code.c_str());
                    encoded_text += enc;
                }

            }

        }
    }
    fclose(f);
    while (encoded_text.length() % 8 != 0)
    {
        encoded_text = '0' + encoded_text;           //padding
        padding++;
    }

    char h = ' ';
    string bitcode = "";

    strcpy(comfile, filename);
    comfile[x - 3] = 'c';
    comfile[x - 2] = 'o';
    comfile[x - 1] = 'm';
    comfile[x] = '\0';
    f = fopen(comfile, "w");
    for (int i = 0; i < encoded_text.length(); i++)
    {
        bitcode += encoded_text[i];
        if (bitcode.length() == 8)
        {
            h = binaryToDecimal(bitcode);
            fprintf(f, "%c", h);
            //printf("\ndecimal = %d\t ", h);       //for testing
            bitcode = "";
        }
    }
    fclose(f);
    int sizeAFter = encoded_text.length();
    sizeAFter /= 8;
    printf("\n\nFile %s  is successfully compressed to %s with size = %d bytes\n__________\n\n", filename, comfile, sizeAFter);
    //printf("\n%s \nsize=%d \npadding=%d\n", encoded_text.c_str(), encoded_text.length(), counter);    //for padding testing
}

int decompression(struct node* node)
{
    
    int arr[] = { 1,0,1,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,1,0 };  
    size_t size = sizeof(arr) / sizeof(arr[0]);

    filename[x - 4] = 'n';
    filename[x - 3] = 'e';
    filename[x - 2] = 'w';
    filename[x - 1] = '.';
    filename[x] = 't';
    filename[x + 1] = 'x';
    filename[x + 2] = 't';
    filename[x + 3] = '\0';

    FILE* f5;
    f5 = fopen(filename, "w");
    if (!f5)
    {
        printf("File cannot be created");
        return -1;
    }
    for (int i = 0; i < size; )
    {
        node = root;
        while (node->left != NULL && node->right != NULL)
        {
            if (arr[i] == 0)
            {
                node = node->left;
                i++;
            }
            else if (arr[i] == 1)
            {
                node = node->right;
                i++;
            }
        }
        //printf("%c", node->c);         //for testing
        fprintf(f5, "%c", node->c);

    }
    fclose(f5);
}

int main()
{
    int choice = 0;
    while (choice != 3)
    {
        printf("Welcome to our Compress/Decompress program\n\n");
        printf("Please enter your choice:\n");
        printf("1.file compression:\n");
        printf("2.file decompression:\n3.Exit\n\n");
        scanf("%d", &choice);

        switch (choice)
        {

        case 1:
            func1();
            getcode(root);
            compression();
            printf("Check the file with extention (.com)\n\n");
            break;
        case 2:
            func1();
            getcode(root);
            decompression(root);
            printf("\nCheck the file with name (.....new.txt)\n\n");
            break;
        case 3:
            break;
        default:
            printf("Invalid choice , pleaese try again.\n");
            break;
        }
    }
}
