#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define size 32

struct macro;
typedef struct macro* pMacro;
typedef struct macro** ppMacro;

struct macro
{
    int num; //количество макропараметров
    char name[size]; // имя макроопределния
    char param[size][size]; //макропараметры
    char code[size*size]; // макроопределене
};

void macroAdds(char* file1, int* num_macro, ppMacro macro) //метод заполняет структуры макросов
{
    FILE* in = fopen(file1, "r");

    char temp[size*size];

    while(fgets(temp, size*size, in) != NULL)
    {
        char* str = strstr(temp, "macro");
        if(str != NULL)
        {
            macro[*num_macro] = (pMacro)malloc(sizeof(struct macro));
            macro[*num_macro] -> num = 0;
            strncpy(macro[*num_macro] -> name, temp, strlen(temp) - strlen(str) - 1); //присваиваем имя

            char* params = str + strlen("macro");
            if(strcmp("\n", params) != 0) //проверяем есть ли параметры
            {
                char* param = strtok(params," ,\n");
                for(int i = 0; param != NULL; i++) //записываем в структуру все параметры
                {
                    strcpy(macro[*num_macro] -> param[i], param);
                    macro[*num_macro] ->num++;
                    param = strtok(NULL, " ,\n");
                }
            }
            memset(macro[*num_macro] ->code, '\0', size*size);
            while(fgets(temp, size*size, in) != NULL) // заполняем поле с  макроопределением
            {
                if(strstr(temp, "endm") == NULL)
                    strcat(macro[*num_macro] -> code, temp);
                else
                {
                    *num_macro+=1;
                    break;
                }
            }
        }
    }
    fclose(in);
}

void replace(char* str, char* substr1, char* substr2)
{
    char res[size*size];
    memset(res, '\0', size*size);
    char* temp = strstr(str, substr1);
    while(temp!= NULL)
    {
        char words[size*size];
        memset(words, '\0', size*size);
        strncpy(words, str, strlen(str) - strlen(temp));
        strcat(res, words);
        strcat(res, substr2);
        strcat(res, temp + strlen(substr1));
        strcpy(str, res);
        temp = strstr(str, substr1);
    }
}

void generateCode(char* file1, char* file2, int num_macro, ppMacro macro)
{
    FILE* in = fopen(file1, "r");
    FILE* out = fopen(file2, "w");

    char temp[size*size];

    while(fgets(temp, size*size, in) != NULL)
    {
        int pr = 1;
        for(int i = 0; i < num_macro; i++)
        {
            char* name = strstr(temp, macro[i] -> name);
            if(name != NULL && strstr(temp, "macro") == NULL)
            {
                char* params = name + strlen(macro[i] -> name);
                char param_actual[size][size];

                if(strcmp("\n", params) != 0) //проверяем есть ли параметры
                {
                    char* param = strtok(params," ,\n");
                    for(int j = 0; param != NULL; j++) //записываем в структуру все параметры
                    {
                        strcpy(param_actual[j], param);
                        param = strtok(NULL, " ,\n");
                    }
                }

                char new_code[size*size];
                strcpy(new_code, macro[i] -> code);
                for(int j = 0; j < macro[i] -> num; j++) // замена формальных параметров на фактические
                    replace(new_code, macro[i]->param[j], param_actual[j]);

                fprintf(out, "%s", new_code);
                pr = 0;
            }
        }
        if(pr == 1)
            fprintf(out, "%s", temp);
    }

    fclose(in);
    fclose(out);
}

int main(int argc, char* argv[])
{
    int num_macro = 0;
    pMacro macro[size];
    for(int i = 0; i < size; i++)
        macro[i] = NULL;

    macroAdds(argv[1], &num_macro, macro);
    generateCode(argv[1], argv[2], num_macro, macro);

    return 0;
}
